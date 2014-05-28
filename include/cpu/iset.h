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

#ifndef ISET_H
#define ISET_H

#include <inttypes.h>
#include <stdbool.h>

typedef void (*opfun)();

struct opdef {
	uint16_t opcode;			// basic/extended opcode
	uint16_t opmask;			// opcode mask to match
	_Bool norm_arg;				// normal argument?
	_Bool short_arg;			// short argument?
	_Bool user_illegal;			// illegal in user mode?
	opfun fun;					// instruction execution function
};

#include "cpu/cpu.h"

// basic opcodes jump table
extern struct opdef iset[];

// macros to access sub-opcodes
#define EXT_OP_37(x) _A(x)
#define EXT_OP_70(x) _A(x)
#define EXT_OP_71(x) ((x & 0b0000001100000000) >> 8)
#define EXT_OP_72(x) (((x & 0b0000001000000000) >> 3) | (x & 0b0000000000111111))
#define EXT_OP_73(x) (((x & 0b0000001111000000) >> 3) | (x & 0b0000000000000111))
#define EXT_OP_74(x) _A(x)
#define EXT_OP_75(x) _A(x)
#define EXT_OP_76(x) _A(x)
#define EXT_OP_77(x) _A(x)

// sub-opcodes (2nd level) jump tables
extern struct opdef iset_73[];

#endif

// vim: tabstop=4 shiftwidth=4 autoindent
