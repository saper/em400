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

#ifndef REGISTERS_H
#define REGISTERS_H

#include <inttypes.h>

// -----------------------------------------------------------------------
// registers and "registers"
// -----------------------------------------------------------------------

enum _registers {
	R_R0	= 0,
	R_R1	= 1,
	R_R2	= 2,
	R_R3	= 3,
	R_R4	= 4,
	R_R5	= 5,
	R_R6	= 6,
	R_R7	= 7,
	R_IC,
	R_SR,
	R_IR,
	R_KB,
	R_MOD,
	R_MODc,
	R_ALARM,
	R_MAX
};

extern uint16_t regs[];

#endif

// vim: tabstop=4 shiftwidth=4 autoindent
