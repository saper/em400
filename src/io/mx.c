//  Copyright (c) 2012-2015 Jakub Filipowicz <jakubf@gmail.com>
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc.,
//  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

#define _XOPEN_SOURCE 500

#include <stdlib.h>
#include <inttypes.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#include "errors.h"
#include "log.h"
#include "mem/mem.h"
#include "io/chan.h"
#include "io/mx.h"
#include "io/mx_cmds.h"
#include "io/mx_ev.h"
#include "io/mx_irq.h"

// Real multix boots up in probably just under a second
// ~500ms (for ROM/RAM check) + ~185ms (for RAM cleanup).
// Here we need just a reasonable delay - big enough for
// OS scheduler to switch threads between MULTIX and CPU,
// so we don't finish MULTIX' job before switching back to CPU thread.
#define RESET_WAIT_MSEC 150

enum mx_state {
	MX_STATE_RUN, MX_STATE_QUIT, MX_STATE_RESET
};

const char *mx_state_names[] = {
	"RUN", "QUIT", "RESET"
};

// -----------------------------------------------------------------------
// ---- channel driver interface -----------------------------------------
// -----------------------------------------------------------------------

void * mx_create(int num, struct cfg_unit *units);
void mx_shutdown(void *ch);
void mx_reset(void *ch);
int mx_cmd(void *ch, int dir, uint16_t n_arg, uint16_t *r_arg);

struct chan_drv mx_chan_driver = {
	.name = "multix",
	.create = mx_create,
	.shutdown = mx_shutdown,
	.reset = mx_reset,
	.cmd = mx_cmd
};

static void * mx_main(void *ptr);

// -----------------------------------------------------------------------
void * mx_create(int num, struct cfg_unit *units)
{
	struct mx *multix = calloc(1, sizeof(struct mx));
	if (!multix) {
		gerr = E_ALLOC;
		goto cleanup;
	}

	multix->num = num;

	LOG_SET_ID(multix, "MX %i", multix->num);

	LOGID(L_MX, 1, multix, "Creating new MULTIX");

	multix->state = MX_STATE_RESET;
	if (pthread_mutex_init(&multix->state_mutex, NULL)) {
		goto cleanup;
	}
	if (pthread_cond_init(&multix->state_cond, NULL)) {
		goto cleanup;
	}

	multix->reset_ack = 0;
	if (pthread_mutex_init(&multix->reset_ack_mutex, NULL)) {
		goto cleanup;
	}
	if (pthread_cond_init(&multix->reset_ack_cond, NULL)) {
		goto cleanup;
	}

	// create event queue
	multix->evq = mx_evq_create(-1);
	if (!multix->evq) {
		gerr = E_EVQ;
		goto cleanup;
	}
	LOG_SET_ID(multix->evq, "%s EV", LOG_GET_ID(multix));

	// create interrupt system
	multix->irqq = mx_irqq_create(multix->num, MX_INTRQ_LEN);
	LOG_SET_ID(multix->irqq, "%s IRQ", LOG_GET_ID(multix));

	// initialize main thread
	if (pthread_create(&multix->main_th, NULL, mx_main, multix)) {
		gerr = E_THREAD;
		goto cleanup;
	}

	return multix;

cleanup:
	mx_shutdown(multix);
	return NULL;
}

// -----------------------------------------------------------------------
void mx_shutdown(void *ch)
{
	if (!ch) return;

	struct mx *multix = (struct mx *) ch;

	LOGID(L_MX, 1, multix, "Shutting down");

	// notify main thread to quit
	pthread_mutex_lock(&multix->state_mutex);
	multix->state = MX_STATE_QUIT;
	pthread_cond_signal(&multix->state_cond);
	pthread_mutex_unlock(&multix->state_mutex);

	mx_evq_disable(multix->evq);

	LOGID(L_MX, 3, multix, "Waiting for main thread to join");
	pthread_join(multix->main_th, NULL);

	// destroy interrupt system
	mx_irqq_destroy(multix->irqq);

	// destroy event queue
	mx_evq_destroy(multix->evq);

	pthread_mutex_destroy(&multix->state_mutex);
	pthread_cond_destroy(&multix->state_cond);
	pthread_mutex_destroy(&multix->reset_ack_mutex);
	pthread_cond_destroy(&multix->reset_ack_cond);

	LOGID(L_MX, 3, multix, "Shutdown complete");

	free(multix);
}

// -----------------------------------------------------------------------
void mx_reset(void *ch)
{
	struct mx *multix = (struct mx *) ch;

	LOGID(L_MX, 2, multix, "Initiating reset");

	// set multix state to "reset"
	pthread_mutex_lock(&multix->state_mutex);
	if (multix->state != MX_STATE_QUIT) {
		multix->state = MX_STATE_RESET;
		pthread_cond_signal(&multix->state_cond);
	}
	pthread_mutex_unlock(&multix->state_mutex);

	// disable event queue
	// waiting evq_dequeue()s will return and further commands will fail
	mx_evq_disable(multix->evq);

	// wait for reset to really kick in
	pthread_mutex_lock(&multix->reset_ack_mutex);
	while (!multix->reset_ack) {
		LOGID(L_MX, 3, multix, "Waiting for reset to kick in");
		pthread_cond_wait(&multix->reset_ack_cond, &multix->reset_ack_mutex);
	}
	pthread_mutex_unlock(&multix->reset_ack_mutex);
	LOGID(L_MX, 2, multix, "Reset initiated");
}

// -----------------------------------------------------------------------
int mx_cmd(void *ch, int dir, uint16_t n_arg, uint16_t *r_arg)
{
	struct mx *multix = (struct mx *) ch;
	struct mx_ev *ev;

	unsigned cmd		= ((n_arg & 0b1110000000000000) >> 13) | ((dir&1) << 3);
	unsigned chan_cmd	=  (n_arg & 0b0001100000000000) >> 11;
	unsigned log_n		=  (n_arg & 0b0001111111100000) >> 5;

	// channel commands
	if (cmd == MX_CMD_CHAN) {
		LOGID(L_MX, 2, multix, "Incomming channel command %i: %s", chan_cmd, mx_chan_cmd_names[chan_cmd]);

		switch (chan_cmd) {
			case MX_CMD_INTSPEC:
				// get intspec (always there, even if invalid)
				*r_arg = mx_irqq_get_intspec(multix->irqq);
				// notify main thread that IRQ has been received byu CPU
				ev = mx_ev_simple(MX_EV_INT_RECVD);
				if (mx_evq_enqueue(multix->evq, ev, MX_EVQ_F_WAIT) <= 0) {
					mx_ev_delete(ev);
				}
				return IO_OK;
			case MX_CMD_EXISTS:
				// 'exists' does nothing, just returns OK
				return IO_OK;
			case MX_CMD_RESET:
				// initiate reset and return OK (actual reset is done in main thread)
				mx_reset(multix);
				return IO_OK;
			default:
				// handle other commands (only MX_CMD_INVALID in fact) as illegal in main thread
				ev = mx_ev_cmd(cmd, 0, 0);
				if (mx_evq_enqueue(multix->evq, ev, MX_EVQ_F_TRY) <= 0) {
					LOGID(L_MX, 2, multix, "ENGAGED");
					mx_ev_delete(ev);
					return IO_EN;
				} else {
					return IO_OK;
				}
		}
	// handle general and line commands in main thread
	} else {
		LOGID(L_MX, 2, multix, "Incomming general/line command %i for line %i: %s", cmd, log_n, mx_cmd_names[cmd]);
		ev = mx_ev_cmd(cmd, log_n, *r_arg);
		if (mx_evq_enqueue(multix->evq, ev, MX_EVQ_F_TRY) <= 0) {
			LOGID(L_MX, 2, multix, "ENGAGED");
			mx_ev_delete(ev);
			return IO_EN;
		} else {
			return IO_OK;
		}
	}

}

// -----------------------------------------------------------------------
// ---- MULTIX -----------------------------------------------------------
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
static void mx_deconfigure(struct mx *multix)
{
	multix->conf_set = 0;

	for (int i=0 ; i<MX_LINE_MAX ; i++) {
		struct mx_line *line = multix->lines + i;
		mx_line_deconfigure(line);
		multix->log_lines[i] = NULL;
		LOG_SET_ID(line, "%s:? (p?)", LOG_GET_ID(multix));
	}
}

// -----------------------------------------------------------------------
static int mx_init(struct mx *multix)
{
	LOGID(L_MX, 2, multix, "Entering initialization loop");

	int ret;

	mx_deconfigure(multix);

	pthread_mutex_lock(&multix->state_mutex);

	// notify CPU thread that reset has been initiated
	// (i.e. MULTIX won't send anything to CPU from now on)
	pthread_mutex_lock(&multix->reset_ack_mutex);
	multix->reset_ack = 1;
	pthread_cond_signal(&multix->reset_ack_cond);
	pthread_mutex_unlock(&multix->reset_ack_mutex);

	// loop while state is RESET
	while (multix->state == MX_STATE_RESET) {
		// we set state to RUN here - if another reset is initiated in the cpu thread,
		// and state changes to RESET during wait on state_cond and we'll loop again.
		// If state changes to QUIT, we exit the loop and handle QUIT
		multix->state = MX_STATE_RUN;

		// set abstime to RESET_WAIT_MSEC in the future, wall clock
		struct timespec abstime;
		clock_gettime(CLOCK_REALTIME, &abstime);
		long new_nsec = abstime.tv_nsec + RESET_WAIT_MSEC * 1000000L;
		abstime.tv_sec += new_nsec / 1000000000L;
		abstime.tv_nsec = new_nsec % 1000000000L;

		LOGID(L_MX, 3, multix, "Starting initialization delay: %i ms", RESET_WAIT_MSEC);

		// wait on condition for RESET_WAIT_MSEC ms
		int res = pthread_cond_timedwait(&multix->state_cond, &multix->state_mutex, &abstime);
		LOGID(L_MX, 3, multix, "Initialization wait %s (\"%s\"), MULTIX state is now: %s",
			(res == 0) ? "interrupted" : "finished",
			strerror(res),
			mx_state_names[multix->state]
		);
	}

	if (multix->state == MX_STATE_QUIT) {
		LOGID(L_MX, 3, multix, "Quit received during initialization");
		ret = -1;
	} else {
		mx_evq_clear(multix->evq);
		mx_irqq_clear(multix->irqq);
		mx_evq_enable(multix->evq);
		mx_irqq_enqueue(multix->irqq, MX_IRQ_IWYZE, 0);

		// clear reset ack flag
		pthread_mutex_lock(&multix->reset_ack_mutex);
		multix->reset_ack = 0;
		pthread_cond_signal(&multix->reset_ack_cond);
		pthread_mutex_unlock(&multix->reset_ack_mutex);

		LOGID(L_MX, 2, multix, "Initialization done");
		ret = 0;
	}

	pthread_mutex_unlock(&multix->state_mutex);

	return ret;
}

// -----------------------------------------------------------------------
static void mx_test(struct mx *multix)
{
	// As we're not able to run any real 8085 code, TEST command
	// won't work. Best we can do is to pretend that test is done
	// and let the test wrapper on CPU side worry about the (non-)results.
	mx_evq_disable(multix->evq);
	mx_deconfigure(multix);
	mx_evq_clear(multix->evq);
	mx_irqq_clear(multix->irqq);
	mx_evq_enable(multix->evq);
	mx_irqq_enqueue(multix->irqq, MX_IRQ_IWYTE, 0);
}

// -----------------------------------------------------------------------
static void mx_setcfg_fin(struct mx *multix, int irq, uint16_t addr, unsigned result, unsigned line)
{
	// configuration was bad
	if (irq == MX_IRQ_INKON) {
		if (result >= MX_SC_E_PROTO_MISSING) {
			LOGID(L_MX, 1, multix, "Configuration not set. Error for logical line %i: %s", line, mx_line_sc_err_name(result));
		} else if (result >= MX_SC_E_DEVTYPE) {
			LOGID(L_MX, 1, multix, "Configuration not set. Error for physical line %i: %s", line, mx_line_sc_err_name(result));
		} else {
			LOGID(L_MX, 1, multix, "Configuration not set: %s", mx_line_sc_err_name(result));
		}
		// store command result
		if (!mem_put(0, addr, (result<<8) | line)) {
			irq = MX_IRQ_INKOT;
		}
	// configuration was OK
	} else if (irq == MX_IRQ_IUKON) {
		LOGID(L_MX, 1, multix, "Configuration successfully set");
	}

	// couldn't store the result
	if (irq == MX_IRQ_INKOT) {
		LOGID(L_MX, 1, multix, "Configuration not set: memory access error");
	}

	mx_irqq_enqueue(multix->irqq, irq, 0);
}

// -----------------------------------------------------------------------
static void mx_setcfg(struct mx *multix, uint16_t addr)
{
	int res;

	uint16_t data[MX_LINE_MAX+4*MX_LINE_MAX];
	uint16_t retf_addr = addr + 1;

	// check if configuration is already set
	if (multix->conf_set) {
		mx_setcfg_fin(multix, MX_IRQ_INKON, retf_addr, MX_SC_E_CONFSET, 0);
		return;
	}

	mx_deconfigure(multix);

	// read configuration header
	if (!mem_get(0, addr, data)) {
		mx_setcfg_fin(multix, MX_IRQ_INKOT, retf_addr, 0, 0);
		return;
	}

	unsigned phy_desc_count	= (*data & 0b1111111100000000) >> 8;
	unsigned log_count		= (*data & 0b0000000011111111);

	LOGID(L_MX, 3, multix, "Configuring: %i physical line descriptors, %i logical lines", multix->num, phy_desc_count, log_count);

	// read line descriptions
	if (!mem_mget(0, addr+2, data, phy_desc_count + 4*log_count)) {
		mx_setcfg_fin(multix, MX_IRQ_INKOT, retf_addr, 0, 0);
		return;
	}

	// check if number of phy line descriptors and log line counts are OK
	if ((phy_desc_count <= 0) || (phy_desc_count > MX_LINE_MAX) || (log_count <= 0) || (log_count > MX_LINE_MAX)) {
		mx_setcfg_fin(multix, MX_IRQ_INKON, retf_addr, MX_SC_E_NUMLINES, 0);
		return;
	}

	// configure physical lines
	int cur_line = 0;
	for (int i=0 ; i<phy_desc_count ; i++) {
		unsigned count		= (data[i] & 0b0000000000011111) + 1;
		LOGID(L_MX, 3, multix, "  %i Physical line(-s) %i..%i:", count, cur_line, cur_line+count-1);
		for (int j=0 ; j<count ; j++, cur_line++) {
			LOG_SET_ID(multix->lines+cur_line, "%s:? (p%i)", LOG_GET_ID(multix), cur_line);
			if (cur_line >= MX_LINE_MAX) {
				mx_setcfg_fin(multix, MX_IRQ_INKON, retf_addr, MX_SC_E_NUMLINES, 0);
				return;
			}
			res = mx_line_conf_phy(multix->lines+cur_line, data[i]);
			if (res != MX_SC_E_OK) {
				mx_setcfg_fin(multix, MX_IRQ_INKON, retf_addr, res, cur_line);
				return;
			}
		}
	}

	// check the completness of physical lines configuration
	int tape_formatters = 0;
	for (int i=0 ; i<MX_LINE_MAX ; i+=4) {
		// there can be only one tape formatter (4 lines)
		if ((multix->lines[i].type == MX_PHY_MTAPE) && (++tape_formatters > 1)) {
			mx_setcfg_fin(multix, MX_IRQ_INKON, retf_addr, MX_SC_E_PHY_INCOMPLETE, i);
			return;
		}
		// MULTIX lines are physically organized in 4-line groups and configuration needs to reflect this
		for (int j=1 ; j<=3 ; j++) {
			if (multix->lines[i+j].type != multix->lines[i].type) {
				mx_setcfg_fin(multix, MX_IRQ_INKON, retf_addr, MX_SC_E_PHY_INCOMPLETE, i+j);
				return;
			}
		}
	}

	// configure logical lines
	for (int i=0 ; i<log_count ; i++) {
		uint16_t *log_data = data+phy_desc_count+(i*4);
		unsigned phy_num = log_data[0] & 0b0000000000011111;
		struct mx_line *line = multix->lines + phy_num;

		LOG_SET_ID(line, "%s:%i (p%i)", LOG_GET_ID(multix), i, phy_num);

		res = mx_line_conf_log(line, log_data);
		if (res != MX_SC_E_OK) {
			mx_setcfg_fin(multix, MX_IRQ_INKON, retf_addr, res, i);
			return;
		}

		multix->log_lines[i] = line;
	}

	multix->conf_set = 1;
	mx_setcfg_fin(multix, MX_IRQ_IUKON, retf_addr, 0, 0);
}

// -----------------------------------------------------------------------
static void mx_ev_handle_cmd(struct mx *multix, struct mx_ev *ev)
{
	switch (ev->cmd) {
		case MX_CMD_ERR_0:
			mx_irqq_enqueue(multix->irqq, MX_IRQ_IEPS0, 0);
			break;
		case MX_CMD_TEST:
			mx_test(multix);
			break;
		case MX_CMD_ATTACH:
			break;
		case MX_CMD_STATUS:
			break;
		case MX_CMD_TRANSMIT:
			break;
		case MX_CMD_SETCFG:
			mx_setcfg(multix, ev->arg);
			break;
		case MX_CMD_ERR_6:
			mx_irqq_enqueue(multix->irqq, MX_IRQ_IEPS6, 0);
			break;
		case MX_CMD_ERR_7:
			mx_irqq_enqueue(multix->irqq, MX_IRQ_IEPS7, 0);
			break;
		case MX_CMD_ERR_8:
			mx_irqq_enqueue(multix->irqq, MX_IRQ_IEPS8, 0);
			break;
		case MX_CMD_REQUEUE:
			mx_irqq_irq_requeue(multix->irqq);
			break;
		case MX_CMD_DETACH:
			break;
		case MX_CMD_ABORT:
			break;
		case MX_CMD_ERR_C:
			mx_irqq_enqueue(multix->irqq, MX_IRQ_IEPSC, 0);
			break;
		case MX_CMD_ERR_D:
			mx_irqq_enqueue(multix->irqq, MX_IRQ_IEPSD, 0);
			break;
		case MX_CMD_ERR_E:
			mx_irqq_enqueue(multix->irqq, MX_IRQ_IEPSE, 0);
			break;
		case MX_CMD_ERR_F:
			mx_irqq_enqueue(multix->irqq, MX_IRQ_IEPSF, 0);
			break;
		default:
			LOGID(L_MX, 1, multix, "Unknown general/line command: %i - ignored", ev->cmd);
			break;
	}
}

// -----------------------------------------------------------------------
static void mx_ev_handle(struct mx *multix, struct mx_ev *ev)
{
	if (ev->type == MX_EV_INT_RECVD) {
		mx_irqq_advance(multix->irqq);
	} else if (ev->type == MX_EV_CMD) {
		mx_ev_handle_cmd(multix, ev);
	} else {
		LOGID(L_MX, 1, multix, "Unknown event type: %i", ev->type);
	}
}

/*
	MULTIX firmware loop. The main f/w job is to process line tasks.
	Tasks are created/updated in H/W interrupt handler routines.
	Here we simulate hardware interrupts by events in the event queue
	(events are reported by other threads, as CPU od device threads),
	and do interrupt routines by processing events from the queue in the
	main loop, just before starting Task Manager.

	From the Task Manager point of view we don't really care when
	those interrupts are received, so we may as well do the processing as follows:
		* While TM is processing data, events from other threads are queued.
		* TM finishes, and we start processing all events from the queue
		  or wait for a new event if queue is empty.
		* TM wakes up and processes all tasks requiring attention (in order
		  determined by the task and condition significance)
*/

// -----------------------------------------------------------------------
static void * mx_main(void *ptr)
{
	struct mx *multix = ptr;
	struct mx_ev *ev;

	LOGID(L_MX, 3, multix, "Starting main loop thread");

	// initialize MX if 'reset' or, break the loop if 'quit'
	while (!mx_init(multix)) {
		// wait for non-empty event
		// if event is empty, that means state is 'quit' or 'reset'
		while ((ev = mx_evq_dequeue(multix->evq, MX_EVQ_F_WAIT))) {
			mx_ev_handle(multix, ev);
			mx_ev_delete(ev);
			// TODO: run task manager
		}
		LOGID(L_MX, 3, multix, "Leaving event loop");
	}
	LOGID(L_MX, 3, multix, "Exiting main loop thread");

	pthread_exit(NULL);
}

// vim: tabstop=4 shiftwidth=4 autoindent
