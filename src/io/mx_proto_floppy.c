//  Copyright (c) 2013-2015 Jakub Filipowicz <jakubf@gmail.com>
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
#include <inttypes.h>

#include "log.h"
#include "io/mx_irq.h"
#include "io/mx_line.h"
#include "io/mx_proto.h"
#include "io/mx_proto_common.h"

enum mx_floppy_types {
	MX_FLOPPY_SD,
	MX_FLOPPY_DD,
	MX_FLOPPY_HD,
	MX_FLOPPY_MAX,
};

const char *mx_floppy_type_names[] = {
	"SD",
	"DD",
	"DD/HD",
	"[unknown-floppy-drv-type]",
};

struct proto_floppy_data {
	int type;
	int fprotect;
};

// -----------------------------------------------------------------------
const char * mx_proto_floppy_get_type_name(unsigned i)
{
	if (i >= MX_FLOPPY_MAX) {
		i = MX_FLOPPY_MAX;
	}
	return mx_floppy_type_names[i];
}

// -----------------------------------------------------------------------
int mx_proto_floppy_conf(struct mx_line *line, uint16_t *data)
{
	struct proto_floppy_data *proto_data = malloc(sizeof(struct proto_floppy_data));
	if (!proto_data) {
		return MX_SC_E_NOMEM;
	}

	proto_data->type		= (data[0] & 0b1111111100000000) >> 8;
	proto_data->fprotect	= (data[0] & 0b0000000011111111);

	LOGID(L_FLOP, 3, line, "    %s floppy drive%s",
		mx_proto_floppy_get_type_name(proto_data->type),
		proto_data->fprotect ? ", format-protected" : ""
	);

	if (proto_data->type > 2) {
		free(proto_data);
		return MX_SC_E_PROTO_PARAMS;
	}

	line->proto_data = proto_data;

	return MX_SC_E_OK;
}

// -----------------------------------------------------------------------
void mx_proto_floppy_free(struct mx_line *line)
{
	free(line->proto_data);
	line->proto_data = NULL;
}

// -----------------------------------------------------------------------
uint8_t mx_proto_floppy_transmit_start(struct mx_line *line, int *irq, uint16_t *data)
{
	// TODO: temporary, so mega bootloader works
	data[4] = 0;
	data[5] = 0x8000;
	*irq = MX_IRQ_ITRER;
	return MX_COND_NONE;
	// line is not attached
	if (!(line->status & MX_LSTATE_ATTACHED)) {
		*irq = MX_IRQ_INTRA;
		return MX_COND_NONE;
	}

	// check if there is a device connected
	if (!line->device || !line->dev_obj) {
		data[5] = 0xffff;
		*irq = MX_IRQ_INTRA;
		return MX_COND_NONE;
	}

	*irq = MX_IRQ_ITRER;
	return MX_COND_NONE;
}

// -----------------------------------------------------------------------
int mx_proto_floppy_phy_types[] = { MX_PHY_FLOPPY, -1 };

struct mx_proto mx_proto_floppy = {
	.enabled = 1,
	.name = "floppy 5.25\"",
	.dir = MX_DIR_NONE,
	.phy_types = mx_proto_floppy_phy_types,
	.conf = mx_proto_floppy_conf,
	.free = mx_proto_floppy_free,
	.task = {
		{ 0, 0, 1, { mx_proto_status_start, NULL, NULL, NULL, NULL, NULL, NULL, NULL } },
		{ 0, 0, 0, { mx_proto_detach_start, NULL, NULL, NULL, NULL, NULL, NULL, NULL } },
		{ 0, 0, 0, { mx_proto_oprq_start, NULL, NULL, NULL, NULL, NULL, NULL, NULL } },
		{ 4, 4, 3, { mx_proto_floppy_transmit_start, NULL, NULL, NULL, NULL, NULL, NULL, NULL } },
		{ 0, 0, 0, { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL } },
		{ 3, 0, 0, { mx_proto_attach_start, NULL, NULL, NULL, NULL, NULL, NULL, NULL } },
	}
};

// vim: tabstop=4 shiftwidth=4 autoindent
