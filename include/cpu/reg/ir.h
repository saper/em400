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

#ifndef REG_IR_H
#define REG_IR_H

// -----------------------------------------------------------------------
// IR access macros
// -----------------------------------------------------------------------

#define IR_OPMASK 0176000 /* 1 111 110 000 000 000 */
#define IR_DMASK  0001000 /* 0 000 001 000 000 000 */
#define IR_AMASK  0000700 /* 0 000 000 111 000 000 */
#define IR_BMASK  0000070 /* 0 000 000 000 111 000 */
#define IR_CMASK  0000007 /* 0 000 000 000 000 111 */

#define OP(x)   ((x) << 10)
#define _OP(x)	(((x) & IR_OPMASK) >> 10)
#define _D(x)	(((x) & IR_DMASK) >> 9)
#define _A(x)	(((x) & IR_AMASK) >> 6)
#define _B(x)	(((x) & IR_BMASK) >> 3)
#define _C(x)	(((x) & IR_CMASK) >> 0)
#define _T(x)	(int8_t) (((x) & 0b0000000000111111) * (((x) & 0b0000001000000000) ? -1 : 1))
#define _t(x)	(uint8_t) (((x) & 0b0000000000000111) | (((x) & 0b0000001000000000) >> 6)) // only SHC uses it
#define _b(x)	((x) & 0b0000000011111111)
#define IR_OP	_OP(regs[R_IR])
#define IR_D	_D(regs[R_IR])
#define IR_A	_A(regs[R_IR])
#define IR_B	_B(regs[R_IR])
#define IR_C	_C(regs[R_IR])
#define IR_T	_T(regs[R_IR])
#define IR_t	_t(regs[R_IR])
#define IR_b	_b(regs[R_IR])

#endif

// vim: tabstop=4 shiftwidth=4 autoindent
