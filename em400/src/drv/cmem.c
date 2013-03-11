//  Copyright (c) 2012-2013 Jakub Filipowicz <jakubf@gmail.com>
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

#include <inttypes.h>
#include <pthread.h>
#include <unistd.h>

#include "cfg.h"
#include "errors.h"
#include "io.h"
#include "drv/cmem.h"
#include "drv/lib.h"

#include "debugger/log.h"

// -----------------------------------------------------------------------
void * drv_cmem_thread(void *self)
{
	struct chan_t *ch = self;

	struct timespec ts;
/**/	ts.tv_sec = 0;
/**/	ts.tv_nsec = 1000000;

	while (!ch->finish) {
/**/		nanosleep(&ts, &ts);
	}
	pthread_exit(NULL);
}

// -----------------------------------------------------------------------
int drv_cmem_init(void *self, struct cfg_arg_t *arg)
{
	struct chan_t *ch = self;
	drv_cmem_reset(ch);
	pthread_create(&ch->thread, NULL, drv_cmem_thread, ch);
	return E_OK;
}

// -----------------------------------------------------------------------
void drv_cmem_shutdown(void *self)
{
	struct chan_t *ch = self;
	ch->finish = 1;
	pthread_join(ch->thread, NULL);
}

// -----------------------------------------------------------------------
void drv_cmem_reset(void *self)
{
	struct chan_t *ch = self;
	ch->int_spec = 0;
	ch->int_mask = 0;
	ch->untransmitted = 0;
}

// -----------------------------------------------------------------------
int drv_cmem_cmd(void *self, int u_num, int dir, int cmd, uint16_t *r)
{
	struct chan_t *ch = self;
	struct unit_t *unit = ch->unit[u_num];
	ch->int_mask = 0;

	// command for channel
	if ((cmd & 0b11100000) == 0) {
		if (dir == IO_OU) {
			switch (cmd & 0b00011000) {
			case CHAN_CMD_EXISTS:
				LOG(D_IO, 1, "%i:%i (%s:%s) command (chan): CHAN_CMD_EXISTS", ch->num, unit->num, ch->name, unit->name);
				break;
			case CHAN_CMD_INTSPEC:
				chan_get_int_spec(ch, r);
				LOG(D_IO, 1, "%i:%i (%s:%s) command (chan): CHAN_CMD_INTSPEC -> %i", ch->num, unit->num, ch->name, unit->name, *r);
				break;
			case CHAN_CMD_STATUS:
				*r = ch->untransmitted;
				LOG(D_IO, 1, "%i:%i (%s:%s) command (chan): CHAN_CMD_STATUS -> %i", ch->num, unit->num, ch->name, unit->name, ch->untransmitted);
				break;
			case CHAN_CMD_ALLOC:
				// all units always working with CPU 0
				*r = 0;
				LOG(D_IO, 1, "%i:%i (%s:%s) command (chan): CHAN_CMD_ALLOC -> %i", ch->num, unit->num, ch->name, unit->name, *r);
				break;
			default:
				// shouldn't happen, but as channel always reports OK...
				break;
			}
		} else {
			switch (cmd & 0b00011000) {
			case CHAN_CMD_EXISTS:
				LOG(D_IO, 1, "%i:%i (%s:%s) command (chan): CHAN_CMD_EXISTS", ch->num, unit->num, ch->name, unit->name);
				break;
			case CHAN_CMD_MASK_PN:
				ch->int_mask = 1;
				LOG(D_IO, 1, "%i:%i (%s:%s) command (chan): CHAN_CMD_MASK_PN -> %i", ch->num, unit->num, ch->name, unit->name, ch->int_mask);
				break;
			case CHAN_CMD_MASK_NPN:
				LOG(D_IO, 1, "%i:%i (%s:%s) command (chan): CHAN_CMD_MASK_NPN -> ignored", ch->num, unit->num, ch->name, unit->name);
				// ignore 2nd CPU
				break;
			case CHAN_CMD_ASSIGN:
				LOG(D_IO, 1, "%i:%i (%s:%s) command (chan): CHAN_CMD_ASSIGN -> ignored", ch->num, unit->num, ch->name, unit->name);
				// always for CPU 0
				break;
			default:
				LOG(D_IO, 1, "%i:%i (%s:%s) command (chan): unknow command", ch->num, unit->num, ch->name, unit->name);
				// shouldn't happen, but as channel always reports OK...
				break;
			}
		}
		return IO_OK;
	// command for unit
	} else {
		return unit->f_cmd(unit, -1, dir, cmd, r);
	}
}

// vim: tabstop=4
