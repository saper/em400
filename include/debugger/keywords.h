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

#ifndef KEYWORDS_H
#define KEYWORDS_H

struct kw_t {
	char *name;
	int value;
};

enum dbg_registers {
	DBG_R_IC = 100,
	DBG_R_KB,
	DBG_R_ALARM,
	DBG_R_MOD,
	DBG_R_MODc,
	DBG_R_IR,
	DBG_R_SR,
};

extern struct kw_t kw_regs[];
extern struct kw_t kw_bases[];

int find_token(struct kw_t *tok_table, char *text);

#endif

// vim: tabstop=4 shiftwidth=4 autoindent
