//  Copyright (c) 2013 Jakub Filipowicz <jakubf@gmail.com>
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

#include <stdlib.h>

#include "debugger/log.h"
#include "debugger/decode.h"
#include "errors.h"
#include "cpu/memory.h"

#include "io/multix_term.h"
#include "io/term.h"

#define UNIT ((struct mx_unit_terminal_t *)(unit))

// -----------------------------------------------------------------------
struct mx_unit_proto_t * mx_term_create(struct cfg_arg_t *args)
{
	int port;
	int res;

	struct mx_unit_proto_t *unit = mx_term_create_nodev();
	if (!unit) {
		gerr = E_ALLOC;
		goto fail;
	}

	res = cfg_args_decode(args, "i", &port);
	if (res != E_OK) {
		gerr = res;
		goto fail;
	}

	UNIT->term = term_open_tcp(port, 100);
	if (!UNIT->term) {
		gerr = E_TERM;
		goto fail;
	}

	eprint("      TCP terminal, port: %i\n", port);

	mx_term_connect(UNIT);
	return unit;

fail:
	return NULL;
}

// -----------------------------------------------------------------------
struct mx_unit_proto_t * mx_term_create_nodev()
{
	struct mx_unit_terminal_t *unit = calloc(1, sizeof(struct mx_unit_terminal_t));
	return (struct mx_unit_proto_t *) unit;
}

// -----------------------------------------------------------------------
void mx_term_connect(struct mx_unit_terminal_t *unit)
{
}

// -----------------------------------------------------------------------
void mx_term_disconnect(struct mx_unit_terminal_t *unit)
{
}

// -----------------------------------------------------------------------
void mx_term_shutdown(struct mx_unit_proto_t *unit)
{
	term_close(UNIT->term);
	mx_term_disconnect(UNIT);
	free(UNIT);
}

// -----------------------------------------------------------------------
void mx_term_reset(struct mx_unit_proto_t *unit)
{

}

// -----------------------------------------------------------------------
int mx_term_cfg_phy(struct mx_unit_proto_t *unit, struct mx_cf_sc_pl *cfg_phy)
{
	LOG(L_TERM, 10, "MULTIX/terminal (log:%i, phy:%i): configure physical line", unit->log_num, unit->phy_num);
	if (unit && cfg_phy) {
		unit->dir = cfg_phy->dir;
		unit->used = 1;
		unit->type = cfg_phy->type;
	} else {
		return E_MX_DECODE;
	}
	return E_OK;
}

// -----------------------------------------------------------------------
int mx_term_cfg_log(struct mx_unit_proto_t *unit, struct mx_cf_sc_ll *cfg_log)
{
	LOG(L_TERM, 10, "MULTIX/terminal (log:%i, phy:%i): configure logical line", unit->log_num, unit->phy_num);
	return E_OK;
}

// -----------------------------------------------------------------------
void mx_term_cmd_attach(struct mx_unit_proto_t *unit, uint16_t addr)
{
	LOG(L_TERM, 10, "MULTIX/terminal (log:%i, phy:%i): attach", unit->log_num, unit->phy_num);
	unit->attached = 1;
	mx_status_set(unit, MX_STATUS_ATTACHED);
	mx_int(unit->chan, unit->log_num, MX_INT_IDOLI);
}

// -----------------------------------------------------------------------
void mx_term_cmd_detach(struct mx_unit_proto_t *unit, uint16_t addr)
{
	LOG(L_TERM, 10, "MULTIX/terminal (log:%i, phy:%i): detach", unit->log_num, unit->phy_num);
	mx_status_clear(unit, MX_STATUS_ATTACHED);
	unit->attached = 0;
	mx_int(unit->chan, unit->log_num, MX_INT_IODLI);
}

// -----------------------------------------------------------------------
uint16_t mx_term_get_status(struct mx_unit_proto_t *unit)
{
	LOG(L_TERM, 10, "MULTIX/terminal (log:%i, phy:%i): status", unit->log_num, unit->phy_num);
	return 0;
}

// -----------------------------------------------------------------------
int mx_term_send(struct mx_unit_proto_t *unit, struct mx_term_cf_transmit_t *cf)
{
	if (cf->send_len <= 0) {
		return 0;
	}

	if ((cf->opts & (MX_TERM_TX_SEND_BY_SIZE | MX_TERM_TX_SEND_BY_EOT_EXCL | MX_TERM_TX_SEND_BY_EOT_INCL)) == 0) {
		return 0;
	}

	mx_status_set(unit, MX_STATUS_SEND);
	mx_status_set(unit, MX_STATUS_SEND_STARTED);

	char tmp[1024];

	int bytes_sent = 0;
	int words = 0;

	while (bytes_sent < cf->send_len) {
		uint16_t data = nMEMB(cf->send_nb, cf->send_buf_addr+words);
		char ch1 = data >> 8;
		char ch2 = data & 255;
		char lf = 10;

		if ((cf->opts & MX_TERM_TX_SEND_BY_EOT_EXCL) && (ch1 == cf->send_eot_char)) {
			break;
		}
		term_write(UNIT->term, &ch1, 1);
		//if (ch2 == 13) term_write(UNIT->term, &lf, 1);
		tmp[bytes_sent] = ch1;
		bytes_sent++;
		if ((cf->opts & MX_TERM_TX_SEND_BY_EOT_INCL) && (ch1 == cf->send_eot_char)) {
			break;
		}

		if ((cf->opts & MX_TERM_TX_SEND_BY_EOT_EXCL) && (ch2 == cf->send_eot_char)) {
			break;
		}
		term_write(UNIT->term, &ch2, 1);
		//if (ch2 == 13) term_write(UNIT->term, &lf, 1);
		tmp[bytes_sent] = ch2;
		bytes_sent++;
		if ((cf->opts & MX_TERM_TX_SEND_BY_EOT_INCL) && (ch2 == cf->send_eot_char)) {
			break;
		}

		words++;
	}

	tmp[bytes_sent] = 0;
	LOG(L_TERM, 50, "Terminal sent: \"%s\"\n", tmp);

	mx_status_clear(unit, MX_STATUS_SEND);

	return bytes_sent;
}

// -----------------------------------------------------------------------
int mx_term_recv(struct mx_unit_proto_t *unit, struct mx_term_cf_transmit_t *cf)
{
	if (cf->recv_len <= 0) {
		return 0;
	}

	if ((cf->opts & (MX_TERM_TX_RECV_BY_SIZE | MX_TERM_TX_RECV_BY_EOT_EXCL | MX_TERM_TX_RECV_BY_EOT_INCL)) == 0) {
		return 0;
	}

	mx_status_set(unit, MX_STATUS_RECV);
	mx_status_set(unit, MX_STATUS_RECV_STARTED);

	int res;
	int bytes_recv = 0;
	int real_bytes_recv = 0;

	// send prompt
	if (cf->opts & MX_TERM_TX_PROMPT) {
		char *prompt = cf->prompt_text;
		while (prompt && *prompt) {
			term_write(UNIT->term, prompt, 1);
			prompt++;
		}
	}

	char buf[1024];
	while (bytes_recv < cf->recv_len) {
		char data;
		res = term_read(UNIT->term, &data, 1);
		if (res <= 0) continue;
		//if (data == 10) continue;
		if ((cf->opts & MX_TERM_TX_RECV_BY_EOT_EXCL) && (data == cf->recv_eot_char)) {
			mx_status_set(unit, MX_STATUS_RECV_EOT);
			break;
		}
		buf[bytes_recv] = data;
		bytes_recv++;
		if ((cf->opts & MX_TERM_TX_RECV_BY_EOT_INCL) && (data == cf->recv_eot_char)) {
			mx_status_set(unit, MX_STATUS_RECV_EOT);
			break;
		}
	}

	real_bytes_recv = bytes_recv;

	if (bytes_recv % 2) {
		buf[bytes_recv] = 0;
		bytes_recv++;
	}

	buf[bytes_recv] = 0;

	for (int i=0 ; i<bytes_recv/2 ; i++) {
		char d = buf[i*2];
		buf[i*2] = buf[i*2+1];
		buf[i*2+1] = d;
		uint16_t data = *((uint16_t*) (buf+(i*2)));
		nMEMBw(cf->recv_nb, cf->recv_buf_addr+i, data);
	}

	mx_status_clear(unit, MX_STATUS_RECV);

	return real_bytes_recv;
}

// -----------------------------------------------------------------------
void mx_term_cmd_transmit(struct mx_unit_proto_t *unit, uint16_t addr)
{
	int ret = E_OK;

	mx_status_clear(unit, MX_STATUS_NOTRANS);

	// we're transmitting
	pthread_mutex_trylock(&unit->transmit_mutex);

	LOG(L_TERM, 1, "MULTIX/terminal (log:%i, phy:%i): transmit", unit->log_num, unit->phy_num);

	// decode control field
	struct mx_term_cf_transmit_t *cf = mx_term_cf_transmit_decode(addr);
	if (!cf) {
		mx_int(unit->chan, unit->log_num, MX_INT_INTRA);
		return;
	}

#ifdef WITH_DEBUGGER
	char *details = decode_mxpst_term(addr, 0);
	LOG(L_TERM, 50, "Transmission details:\n%s", details);
	free(details);
#endif

	int bytes_sent = 0;
	int bytes_recv = 0;

	bytes_sent = mx_term_send(unit, cf);
	bytes_recv = mx_term_recv(unit, cf);

	MEMBw(0, addr+10, bytes_sent);
	MEMBw(0, addr+11, 0);
	MEMBw(0, addr+12, bytes_recv);

	if (ret == E_OK) {
		mx_int(unit->chan, unit->log_num, MX_INT_IETRA);
	} else if (ret == E_MX_CANCEL) {
		mx_int(unit->chan, unit->log_num, MX_INT_ITRAB);
	} else {
		//MEMBw(0, addr+6, cf->ret_status);
		mx_int(unit->chan, unit->log_num, MX_INT_ITRER);
	}

	// done transmitting
	pthread_mutex_unlock(&unit->transmit_mutex);
}

// -----------------------------------------------------------------------
struct mx_term_cf_attach_t * mx_term_cf_attach_decode(int addr)
{
	uint16_t data;
	struct mx_term_cf_attach_t *cf = calloc(1, sizeof(struct mx_term_cf_attach_t));
	if (!cf) {
		return NULL;
	}

	data = MEMB(0, addr+0);
	cf->opts = data >> 8;
	cf->eot_mark = data & 255;

	data = MEMB(0, addr+1);
	cf->call_mark = data >> 8;
	cf->text_proc = data & 255;

	data = MEMB(0, addr+2);
	cf->text_proc_params = data;

	return cf;
}

// -----------------------------------------------------------------------
void mx_term_cf_attach_free(struct mx_term_cf_attach_t *cf)
{
	free(cf);
}

// -----------------------------------------------------------------------
struct mx_term_cf_transmit_t * mx_term_cf_transmit_decode(int addr)
{
	uint16_t data;
	struct mx_term_cf_transmit_t *cf = calloc(1, sizeof(struct mx_term_cf_transmit_t));
	if (!cf) {
		return NULL;
	}

	data = MEMB(0, addr+0);
	cf->opts = data >> 8;
	cf->timeout = data & 255;

	cf->send_len = MEMB(0, addr+1);
	cf->send_buf_addr = MEMB(0, addr+2);

	data = MEMB(0, addr+3);
	cf->send_eot_char = data >> 8;
	cf->send_start_byte = (data >> 9) & 1;
	cf->send_nb = data & 0b1111;

	cf->recv_len = MEMB(0, addr+4);
	cf->recv_buf_addr = MEMB(0, addr+5);

	data = MEMB(0, addr+6);
	cf->recv_start_byte = (data >> 9) &1;
	cf->recv_nb = data & 0b1111;

	data = MEMB(0, addr+7);
	cf->recv_eot_char = data >> 8;
	cf->recv_eot_char2 = data & 255;

	data = MEMB(0, addr+8);
	cf->prompt_text[0] = data >> 8;
	cf->prompt_text[1] = data & 255;

	data = MEMB(0, addr+9);
	cf->prompt_text[2] = data >> 8;
	cf->prompt_text[3] = data & 255;
	cf->prompt_text[4] = '\0';

	cf->bytes_sent = 0;
	cf->bytes_awaiting = 0;
	cf->bytes_transmitted = 0;
	cf->status = 0;

	return cf;
}

// -----------------------------------------------------------------------
void mx_term_cf_transmit_free(struct mx_term_cf_transmit_t *cf)
{
	free(cf);
}


// vim: tabstop=4 shiftwidth=4 autoindent