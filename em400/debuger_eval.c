//  Copyright (c) 2012 Jakub Filipowicz <jakubf@gmail.com>
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

#include "registers.h"
#include "memory.h"
#include "debuger_eval.h"
#include "debuger_parser.h"

// -----------------------------------------------------------------------
struct node_t * n_val(int16_t v)
{
	struct node_t *n = malloc(sizeof(struct node_t));
	n->type = N_VAL;
	n->val = v;
	return n;
}

// -----------------------------------------------------------------------
struct node_t * n_oper(int oper, struct node_t *n1, struct node_t *n2)
{
	struct node_t *n = malloc(sizeof(struct node_t));
	n->type = N_OPER;
	n->val = oper;
	n->n1 = n1;
	n->n2 = n2;
	return n;
}

// -----------------------------------------------------------------------
struct node_t * n_reg(int r)
{
	struct node_t *n = malloc(sizeof(struct node_t));
	n->type = N_REG;
	n->val = r;
	return n;
}
// -----------------------------------------------------------------------
uint16_t n_eval(struct node_t * n)
{
	if (!n) return 0;
	uint16_t v1, v2;
	switch (n->type) {
		case N_VAL:
			return n->val;
		case N_REG:
			return R(n->val);
		case N_OPER:
			v1 = n_eval(n->n1);
			v2 = n_eval(n->n2);
			switch (n->val) {
				case '-':
					return v1 - v2;
				case '+':
					return v1 + v2;
				case '*':
					return v1 * v2;
				case '|':
					return v1 | v2;
				case '&':
					return v1 & v2;
				case '^':
					return v1 ^ v2;
				case SHR:
					return v1 >> v2;
				case SHL:
					return v1 << v2;
				case '~':
					return ~v1;
				case '!':
					return !v1;
				case EQ:
					return v1 == v2;
				case NEQ:
					return v1 != v2;
				case '>':
					return v1 > v2;
				case '<':
					return v1 < v2;
				case GE:
					return v1 >= v2;
				case LE:
					return v1 <= v2;
				case '[':
					if (n->n2) {
						return MEMB(v1, v2);
					} else {
						return MEM(v1);
					}
				case UMINUS:	
					return -v1;
				case AND:
					if (v1 && v2) return 1;
					else return 0;
				case OR:
					if (v1 || v2) return 1;
					else return 0;
				default:
					return 0;
			}
		default:
			return 0;
	}
	return 0;
}


// vim: tabstop=4
