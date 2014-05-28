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

#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>

#include "cpu/iset.h"
#include "cpu/reg/ir.h"
#include "cpu/instructions.h"

struct opdef iset[] = {
	{ OP(001), IR_OPMASK, false, false, false, NULL},
	{ OP(002), IR_OPMASK, false, false, false, NULL},
	{ OP(003), IR_OPMASK, false, false, false, NULL},
	{ OP(004), IR_OPMASK, false, false, false, NULL},
	{ OP(005), IR_OPMASK, false, false, false, NULL},
	{ OP(006), IR_OPMASK, false, false, false, NULL},
	{ OP(007), IR_OPMASK, false, false, false, NULL},
	{ OP(010), IR_OPMASK, false, false, false, NULL},
	{ OP(011), IR_OPMASK, false, false, false, NULL},
	{ OP(012), IR_OPMASK, false, false, false, NULL},
	{ OP(013), IR_OPMASK, false, false, false, NULL},
	{ OP(014), IR_OPMASK, false, false, false, NULL},
	{ OP(015), IR_OPMASK, false, false, false, NULL},
	{ OP(016), IR_OPMASK, false, false, false, NULL},
	{ OP(017), IR_OPMASK, false, false, false, NULL},
	
	{ OP(020), IR_OPMASK, true, false, false, op_lw},
	{ OP(021), IR_OPMASK, true, false, false, op_tw},
	{ OP(022), IR_OPMASK, true, false, false, op_ls},
	{ OP(023), IR_OPMASK, true, false, false, op_ri},
	{ OP(024), IR_OPMASK, true, false, false, op_rw},
	{ OP(025), IR_OPMASK, true, false, false, op_pw},
	{ OP(026), IR_OPMASK, true, false, false, op_rj},
	{ OP(027), IR_OPMASK, true, false, false, op_is},
	{ OP(030), IR_OPMASK, true, false, false, op_bb},
	{ OP(031), IR_OPMASK, true, false, false, op_bm},
	{ OP(032), IR_OPMASK, true, false, false, op_bs},
	{ OP(033), IR_OPMASK, true, false, false, op_bc},
	{ OP(034), IR_OPMASK, true, false, false, op_bn},
	{ OP(035), IR_OPMASK, true, false, true, op_ou},
	{ OP(036), IR_OPMASK, true, false, true, op_in},

	{ OP(037), IR_OPMASK, true, false, false, NULL /* goaway op_37 */ },

	{ OP(040), IR_OPMASK, true, false, false, op_aw},
	{ OP(041), IR_OPMASK, true, false, false, op_ac},
	{ OP(042), IR_OPMASK, true, false, false, op_sw},
	{ OP(043), IR_OPMASK, true, false, false, op_cw},
	{ OP(044), IR_OPMASK, true, false, false, op_or},
	{ OP(045), IR_OPMASK, true, false, false, op_om},
	{ OP(046), IR_OPMASK, true, false, false, op_nr},
	{ OP(047), IR_OPMASK, true, false, false, op_nm},
	{ OP(050), IR_OPMASK, true, false, false, op_er},
	{ OP(051), IR_OPMASK, true, false, false, op_em},
	{ OP(052), IR_OPMASK, true, false, false, op_xr},
	{ OP(053), IR_OPMASK, true, false, false, op_xm},
	{ OP(054), IR_OPMASK, true, false, false, op_cl},
	{ OP(055), IR_OPMASK, true, false, false, op_lb},
	{ OP(056), IR_OPMASK, true, false, false, op_rb},
	{ OP(057), IR_OPMASK, true, false, false, op_cb},

	{ OP(060), IR_OPMASK, false, true, false, op_awt},
	{ OP(061), IR_OPMASK, false, true, false, op_trb},
	{ OP(062), IR_OPMASK, false, true, false, op_irb},
	{ OP(063), IR_OPMASK, false, true, false, op_drb},
	{ OP(064), IR_OPMASK, false, true, false, op_cwt},
	{ OP(065), IR_OPMASK, false, true, false, op_lwt},
	{ OP(066), IR_OPMASK, false, true, false, op_lws},
	{ OP(067), IR_OPMASK, false, true, false, op_rws},

	{ OP(070), IR_OPMASK, false, true,  false, NULL /* goaway op_70 */ },
	{ OP(071), IR_OPMASK, false, false, false, NULL /* goaway op_71 */ },
	{ OP(072), IR_OPMASK, false, false, false, NULL /* goaway op_72 */ },
	{ OP(073), IR_OPMASK, false, false, true,  NULL /* goaway op_73 */ },
	{ OP(074), IR_OPMASK, true,  false, false, NULL /* goaway op_74 */ },
	{ OP(075), IR_OPMASK, true,  false, false, NULL /* goaway op_75 */ },
	{ OP(076), IR_OPMASK, true,  false, false, NULL /* goaway op_76 */ },
	{ OP(077), IR_OPMASK, true,  false, false, NULL /* goaway op_77 */ },

	/* End of table */
	{ 0, 0, false, false, false, NULL },
};

// [A]
struct opdef iset_37[] = {
	{ 0, IR_OPMASK, true, false, false, op_37_ad},
	{ 1, IR_OPMASK, true, false, false, op_37_sd},
	{ 2, IR_OPMASK, true, false, false, op_37_mw},
	{ 3, IR_OPMASK, true, false, false, op_37_dw},
	{ 4, IR_OPMASK, true, false, false, op_37_af},
	{ 5, IR_OPMASK, true, false, false, op_37_sf},
	{ 6, IR_OPMASK, true, false, false, op_37_mf},
	{ 7, IR_OPMASK, true, false, false, op_37_df}
};

// [A]
struct opdef iset_70[] = {
	{ 0, IR_OPMASK, false, true, false, op_70_ujs},
	{ 1, IR_OPMASK, false, true, false, op_70_jls},
	{ 2, IR_OPMASK, false, true, false, op_70_jes},
	{ 3, IR_OPMASK, false, true, false, op_70_jgs},
	{ 4, IR_OPMASK, false, true, false, op_70_jvs},
	{ 5, IR_OPMASK, false, true, false, op_70_jxs},
	{ 6, IR_OPMASK, false, true, false, op_70_jys},
	{ 7, IR_OPMASK, false, true, false, op_70_jcs}
};

// [D,A0]
struct opdef iset_71[] = {
	{ 0, IR_OPMASK, false, false, false, op_71_blc},
	{ 1, IR_OPMASK, false, false, false, op_71_exl},
	{ 2, IR_OPMASK, false, false, false, op_71_brc},
	{ 3, IR_OPMASK, false, false, false, op_71_nrf}
};

// [D,B,C]
struct opdef iset_72[] = {
	{ 0b0000000, IR_OPMASK, false, false, false, op_72_ric},
	{ 0b0000001, IR_OPMASK, false, false, false, op_72_zlb},
	{ 0b0000010, IR_OPMASK, false, false, false, op_72_sxu},
	{ 0b0000011, IR_OPMASK, false, false, false, op_72_nga},
	{ 0b0000100, IR_OPMASK, false, false, false, op_72_slz},
	{ 0b0000101, IR_OPMASK, false, false, false, op_72_sly},
	{ 0b0000110, IR_OPMASK, false, false, false, op_72_slx},
	{ 0b0000111, IR_OPMASK, false, false, false, op_72_sry},
	{ 0b0001000, IR_OPMASK, false, false, false, op_72_ngl},
	{ 0b0001001, IR_OPMASK, false, false, false, op_72_rpc},
	{ 0b0001010, IR_OPMASK, false, false, false, NULL},
	{ 0b0001011, IR_OPMASK, false, false, false, NULL},
	{ 0b0001100, IR_OPMASK, false, false, false, NULL},
	{ 0b0001101, IR_OPMASK, false, false, false, NULL},
	{ 0b0001110, IR_OPMASK, false, false, false, NULL},
	{ 0b0001111, IR_OPMASK, false, false, false, NULL},
	{ 0b0010000, IR_OPMASK, false, false, false, op_72_shc},
	{ 0b0010001, IR_OPMASK, false, false, false, op_72_shc},
	{ 0b0010010, IR_OPMASK, false, false, false, op_72_shc},
	{ 0b0010011, IR_OPMASK, false, false, false, op_72_shc},
	{ 0b0010100, IR_OPMASK, false, false, false, op_72_shc},
	{ 0b0010101, IR_OPMASK, false, false, false, op_72_shc},
	{ 0b0010110, IR_OPMASK, false, false, false, op_72_shc},
	{ 0b0010111, IR_OPMASK, false, false, false, op_72_shc},
	{ 0b0011000, IR_OPMASK, false, false, false, NULL},
	{ 0b0011001, IR_OPMASK, false, false, false, NULL},
	{ 0b0011010, IR_OPMASK, false, false, false, NULL},
	{ 0b0011011, IR_OPMASK, false, false, false, NULL},
	{ 0b0011100, IR_OPMASK, false, false, false, NULL},
	{ 0b0011101, IR_OPMASK, false, false, false, NULL},
	{ 0b0011110, IR_OPMASK, false, false, false, NULL},
	{ 0b0011111, IR_OPMASK, false, false, false, NULL},
	{ 0b0100000, IR_OPMASK, false, false, false, NULL},
	{ 0b0100001, IR_OPMASK, false, false, false, NULL},
	{ 0b0100010, IR_OPMASK, false, false, false, NULL},
	{ 0b0100011, IR_OPMASK, false, false, false, NULL},
	{ 0b0100100, IR_OPMASK, false, false, false, NULL},
	{ 0b0100101, IR_OPMASK, false, false, false, NULL},
	{ 0b0100110, IR_OPMASK, false, false, false, NULL},
	{ 0b0100111, IR_OPMASK, false, false, false, NULL},
	{ 0b0101000, IR_OPMASK, false, false, false, NULL},
	{ 0b0101001, IR_OPMASK, false, false, false, NULL},
	{ 0b0101010, IR_OPMASK, false, false, false, NULL},
	{ 0b0101011, IR_OPMASK, false, false, false, NULL},
	{ 0b0101100, IR_OPMASK, false, false, false, NULL},
	{ 0b0101101, IR_OPMASK, false, false, false, NULL},
	{ 0b0101110, IR_OPMASK, false, false, false, NULL},
	{ 0b0101111, IR_OPMASK, false, false, false, NULL},
	{ 0b0110000, IR_OPMASK, false, false, false, NULL},
	{ 0b0110001, IR_OPMASK, false, false, false, NULL},
	{ 0b0110010, IR_OPMASK, false, false, false, NULL},
	{ 0b0110011, IR_OPMASK, false, false, false, NULL},
	{ 0b0110100, IR_OPMASK, false, false, false, NULL},
	{ 0b0110101, IR_OPMASK, false, false, false, NULL},
	{ 0b0110110, IR_OPMASK, false, false, false, NULL},
	{ 0b0110111, IR_OPMASK, false, false, false, NULL},
	{ 0b0111000, IR_OPMASK, false, false, false, NULL},
	{ 0b0111001, IR_OPMASK, false, false, false, NULL},
	{ 0b0111010, IR_OPMASK, false, false, false, NULL},
	{ 0b0111011, IR_OPMASK, false, false, false, NULL},
	{ 0b0111100, IR_OPMASK, false, false, false, NULL},
	{ 0b0111101, IR_OPMASK, false, false, false, NULL},
	{ 0b0111110, IR_OPMASK, false, false, false, NULL},
	{ 0b0111111, IR_OPMASK, false, false, false, NULL},
	{ 0b1000000, IR_OPMASK, false, false, false, op_72_rky},
	{ 0b1000001, IR_OPMASK, false, false, false, op_72_zrb},
	{ 0b1000010, IR_OPMASK, false, false, false, op_72_sxl},
	{ 0b1000011, IR_OPMASK, false, false, false, op_72_ngc},
	{ 0b1000100, IR_OPMASK, false, false, false, op_72_svz},
	{ 0b1000101, IR_OPMASK, false, false, false, op_72_svy},
	{ 0b1000110, IR_OPMASK, false, false, false, op_72_svx},
	{ 0b1000111, IR_OPMASK, false, false, false, op_72_srx},
	{ 0b1001000, IR_OPMASK, false, false, false, op_72_srz},
	{ 0b1001001, IR_OPMASK, false, false, false, op_72_lpc},
	{ 0b1001010, IR_OPMASK, false, false, false, NULL},
	{ 0b1001011, IR_OPMASK, false, false, false, NULL},
	{ 0b1001100, IR_OPMASK, false, false, false, NULL},
	{ 0b1001101, IR_OPMASK, false, false, false, NULL},
	{ 0b1001110, IR_OPMASK, false, false, false, NULL},
	{ 0b1001111, IR_OPMASK, false, false, false, NULL},
	{ 0b1010001, IR_OPMASK, false, false, false, op_72_shc},
	{ 0b1010000, IR_OPMASK, false, false, false, op_72_shc},
	{ 0b1010010, IR_OPMASK, false, false, false, op_72_shc},
	{ 0b1010011, IR_OPMASK, false, false, false, op_72_shc},
	{ 0b1010100, IR_OPMASK, false, false, false, op_72_shc},
	{ 0b1010101, IR_OPMASK, false, false, false, op_72_shc},
	{ 0b1010110, IR_OPMASK, false, false, false, op_72_shc},
	{ 0b1010111, IR_OPMASK, false, false, false, op_72_shc},
	{ 0b1011000, IR_OPMASK, false, false, false, NULL},
	{ 0b1011001, IR_OPMASK, false, false, false, NULL},
	{ 0b1011010, IR_OPMASK, false, false, false, NULL},
	{ 0b1011011, IR_OPMASK, false, false, false, NULL},
	{ 0b1011100, IR_OPMASK, false, false, false, NULL},
	{ 0b1011101, IR_OPMASK, false, false, false, NULL},
	{ 0b1011110, IR_OPMASK, false, false, false, NULL},
	{ 0b1011111, IR_OPMASK, false, false, false, NULL},
	{ 0b1100000, IR_OPMASK, false, false, false, NULL},
	{ 0b1100001, IR_OPMASK, false, false, false, NULL},
	{ 0b1100010, IR_OPMASK, false, false, false, NULL},
	{ 0b1100011, IR_OPMASK, false, false, false, NULL},
	{ 0b1100100, IR_OPMASK, false, false, false, NULL},
	{ 0b1100101, IR_OPMASK, false, false, false, NULL},
	{ 0b1100110, IR_OPMASK, false, false, false, NULL},
	{ 0b1100111, IR_OPMASK, false, false, false, NULL},
	{ 0b1101000, IR_OPMASK, false, false, false, NULL},
	{ 0b1101001, IR_OPMASK, false, false, false, NULL},
	{ 0b1101010, IR_OPMASK, false, false, false, NULL},
	{ 0b1101011, IR_OPMASK, false, false, false, NULL},
	{ 0b1101100, IR_OPMASK, false, false, false, NULL},
	{ 0b1101101, IR_OPMASK, false, false, false, NULL},
	{ 0b1101110, IR_OPMASK, false, false, false, NULL},
	{ 0b1101111, IR_OPMASK, false, false, false, NULL},
	{ 0b1110001, IR_OPMASK, false, false, false, NULL},
	{ 0b1110000, IR_OPMASK, false, false, false, NULL},
	{ 0b1110010, IR_OPMASK, false, false, false, NULL},
	{ 0b1110011, IR_OPMASK, false, false, false, NULL},
	{ 0b1110100, IR_OPMASK, false, false, false, NULL},
	{ 0b1110101, IR_OPMASK, false, false, false, NULL},
	{ 0b1110110, IR_OPMASK, false, false, false, NULL},
	{ 0b1110111, IR_OPMASK, false, false, false, NULL},
	{ 0b1111000, IR_OPMASK, false, false, false, NULL},
	{ 0b1111001, IR_OPMASK, false, false, false, NULL},
	{ 0b1111010, IR_OPMASK, false, false, false, NULL},
	{ 0b1111011, IR_OPMASK, false, false, false, NULL},
	{ 0b1111100, IR_OPMASK, false, false, false, NULL},
	{ 0b1111101, IR_OPMASK, false, false, false, NULL},
	{ 0b1111110, IR_OPMASK, false, false, false, NULL},
	{ 0b1111111, IR_OPMASK, false, false, false, NULL}

};

// [D,A,C] (but not all used in each instruction)
struct opdef iset_73[] = {
	{ 0b0000000, IR_OPMASK, false, true, true, op_73_hlt},
	{ 0b0000001, IR_OPMASK, false, true, true, op_73_hlt},
	{ 0b0000010, IR_OPMASK, false, true, true, op_73_hlt},
	{ 0b0000011, IR_OPMASK, false, true, true, op_73_hlt},
	{ 0b0000100, IR_OPMASK, false, true, true, op_73_hlt},
	{ 0b0000101, IR_OPMASK, false, true, true, op_73_hlt},
	{ 0b0000110, IR_OPMASK, false, true, true, op_73_hlt},
	{ 0b0000111, IR_OPMASK, false, true, true, op_73_hlt},

	{ 0b0001000, IR_OPMASK, false, false, true, op_73_mcl},
	{ 0b0001001, IR_OPMASK, false, false, true, op_73_mcl},
	{ 0b0001010, IR_OPMASK, false, false, true, op_73_mcl},
	{ 0b0001011, IR_OPMASK, false, false, true, op_73_mcl},
	{ 0b0001100, IR_OPMASK, false, false, true, op_73_mcl},
	{ 0b0001101, IR_OPMASK, false, false, true, op_73_mcl},
	{ 0b0001110, IR_OPMASK, false, false, true, op_73_mcl},
	{ 0b0001111, IR_OPMASK, false, false, true, op_73_mcl},

	{ 0b0010000, IR_OPMASK, false, false, true, op_73_cit},
	{ 0b0010001, IR_OPMASK, false, false, true, op_73_sil},
	{ 0b0010010, IR_OPMASK, false, false, true, op_73_siu},
	{ 0b0010011, IR_OPMASK, false, false, true, op_73_sit},
	{ 0b0010100, IR_OPMASK, false, false, true, op_73_sint},
	{ 0b0010101, IR_OPMASK, false, false, false, NULL},
	{ 0b0010110, IR_OPMASK, false, false, false, NULL},
	{ 0b0010111, IR_OPMASK, false, false, false, NULL},

	{ 0b0011000, IR_OPMASK, false, false, true, op_73_giu},
	{ 0b0011001, IR_OPMASK, false, false, true, op_73_giu},
	{ 0b0011010, IR_OPMASK, false, false, true, op_73_giu},
	{ 0b0011011, IR_OPMASK, false, false, true, op_73_giu},
	{ 0b0011100, IR_OPMASK, false, false, true, op_73_giu},
	{ 0b0011101, IR_OPMASK, false, false, true, op_73_giu},
	{ 0b0011110, IR_OPMASK, false, false, true, op_73_giu},
	{ 0b0011111, IR_OPMASK, false, false, true, op_73_giu},

	{ 0b0100000, IR_OPMASK, false, false, true, op_73_lip},
	{ 0b0100001, IR_OPMASK, false, false, true, op_73_lip},
	{ 0b0100010, IR_OPMASK, false, false, true, op_73_lip},
	{ 0b0100011, IR_OPMASK, false, false, true, op_73_lip},
	{ 0b0100100, IR_OPMASK, false, false, true, op_73_lip},
	{ 0b0100101, IR_OPMASK, false, false, true, op_73_lip},
	{ 0b0100110, IR_OPMASK, false, false, true, op_73_lip},
	{ 0b0100111, IR_OPMASK, false, false, true, op_73_lip},

	{ 0b0101000, IR_OPMASK, false, false, true,  NULL},
	{ 0b0101001, IR_OPMASK, false, false, false, NULL},
	{ 0b0101010, IR_OPMASK, false, false, false, NULL},
	{ 0b0101011, IR_OPMASK, false, false, false, NULL},
	{ 0b0101100, IR_OPMASK, false, false, false, NULL},
	{ 0b0101101, IR_OPMASK, false, false, false, NULL},
	{ 0b0101110, IR_OPMASK, false, false, false, NULL},
	{ 0b0101111, IR_OPMASK, false, false, false, NULL},

	{ 0b0110000, IR_OPMASK, false, false, false, NULL},
	{ 0b0110001, IR_OPMASK, false, false, false, NULL},
	{ 0b0110010, IR_OPMASK, false, false, false, NULL},
	{ 0b0110011, IR_OPMASK, false, false, false, NULL},
	{ 0b0110100, IR_OPMASK, false, false, false, NULL},
	{ 0b0110101, IR_OPMASK, false, false, false, NULL},
	{ 0b0110110, IR_OPMASK, false, false, false, NULL},
	{ 0b0110111, IR_OPMASK, false, false, false, NULL},

	{ 0b0111000, IR_OPMASK, false, false, false, NULL},
	{ 0b0111001, IR_OPMASK, false, false, false, NULL},
	{ 0b0111010, IR_OPMASK, false, false, false, NULL},
	{ 0b0111011, IR_OPMASK, false, false, false, NULL},
	{ 0b0111100, IR_OPMASK, false, false, false, NULL},
	{ 0b0111101, IR_OPMASK, false, false, false, NULL},
	{ 0b0111110, IR_OPMASK, false, false, false, NULL},
	{ 0b0111111, IR_OPMASK, false, false, false, NULL},

	{ 0b1000000, IR_OPMASK, false, true, true, op_73_hlt},
	{ 0b1000001, IR_OPMASK, false, true, true, op_73_hlt},
	{ 0b1000010, IR_OPMASK, false, true, true, op_73_hlt},
	{ 0b1000011, IR_OPMASK, false, true, true, op_73_hlt},
	{ 0b1000100, IR_OPMASK, false, true, true, op_73_hlt},
	{ 0b1000101, IR_OPMASK, false, true, true, op_73_hlt},
	{ 0b1000110, IR_OPMASK, false, true, true, op_73_hlt},
	{ 0b1000111, IR_OPMASK, false, true, true, op_73_hlt},

	{ 0b1001000, IR_OPMASK, false, false, true, op_73_mcl},
	{ 0b1001001, IR_OPMASK, false, false, true, op_73_mcl},
	{ 0b1001010, IR_OPMASK, false, false, true, op_73_mcl},
	{ 0b1001011, IR_OPMASK, false, false, true, op_73_mcl},
	{ 0b1001100, IR_OPMASK, false, false, true, op_73_mcl},
	{ 0b1001101, IR_OPMASK, false, false, true, op_73_mcl},
	{ 0b1001110, IR_OPMASK, false, false, true, op_73_mcl},
	{ 0b1001111, IR_OPMASK, false, false, true, op_73_mcl},

	{ 0b1010000, IR_OPMASK, false, false, true, op_73_cit},
	{ 0b1010001, IR_OPMASK, false, false, true, op_73_sil},
	{ 0b1010010, IR_OPMASK, false, false, true, op_73_siu},
	{ 0b1010011, IR_OPMASK, false, false, true, op_73_sit},
	{ 0b1010100, IR_OPMASK, false, false, true, op_73_sind},
	{ 0b1010101, IR_OPMASK, false, false, false, NULL},
	{ 0b1010110, IR_OPMASK, false, false, false, NULL},
	{ 0b1010111, IR_OPMASK, false, false, false, NULL},

	{ 0b1011000, IR_OPMASK, false, false, true, op_73_gil},
	{ 0b1011001, IR_OPMASK, false, false, true, op_73_gil},
	{ 0b1011010, IR_OPMASK, false, false, true, op_73_gil},
	{ 0b1011011, IR_OPMASK, false, false, true, op_73_gil},
	{ 0b1011100, IR_OPMASK, false, false, true, op_73_gil},
	{ 0b1011101, IR_OPMASK, false, false, true, op_73_gil},
	{ 0b1011110, IR_OPMASK, false, false, true, op_73_gil},
	{ 0b1011111, IR_OPMASK, false, false, true, op_73_gil},

	{ 0b1100001, IR_OPMASK, false, false, true, op_73_lip},
	{ 0b1100000, IR_OPMASK, false, false, true, op_73_lip},
	{ 0b1100010, IR_OPMASK, false, false, true, op_73_lip},
	{ 0b1100011, IR_OPMASK, false, false, true, op_73_lip},
	{ 0b1100101, IR_OPMASK, false, false, true, op_73_lip},
	{ 0b1100100, IR_OPMASK, false, false, true, op_73_lip},
	{ 0b1100110, IR_OPMASK, false, false, true, op_73_lip},
	{ 0b1100111, IR_OPMASK, false, false, true, op_73_lip},

	{ 0b1101000, IR_OPMASK, false, false, false, NULL},
	{ 0b1101001, IR_OPMASK, false, false, false, NULL},
	{ 0b1101010, IR_OPMASK, false, false, false, NULL},
	{ 0b1101011, IR_OPMASK, false, false, false, NULL},
	{ 0b1101100, IR_OPMASK, false, false, false, NULL},
	{ 0b1101101, IR_OPMASK, false, false, false, NULL},
	{ 0b1101110, IR_OPMASK, false, false, false, NULL},
	{ 0b1101111, IR_OPMASK, false, false, false, NULL},

	{ 0b1110000, IR_OPMASK, false, false, false, NULL},
	{ 0b1110001, IR_OPMASK, false, false, false, NULL},
	{ 0b1110010, IR_OPMASK, false, false, false, NULL},
	{ 0b1110011, IR_OPMASK, false, false, false, NULL},
	{ 0b1110100, IR_OPMASK, false, false, false, NULL},
	{ 0b1110101, IR_OPMASK, false, false, false, NULL},
	{ 0b1110110, IR_OPMASK, false, false, false, NULL},
	{ 0b1110111, IR_OPMASK, false, false, false, NULL},

	{ 0b1111000, IR_OPMASK, false, false, false, NULL},
	{ 0b1111001, IR_OPMASK, false, false, false, NULL},
	{ 0b1111010, IR_OPMASK, false, false, false, NULL},
	{ 0b1111011, IR_OPMASK, false, false, false, NULL},
	{ 0b1111100, IR_OPMASK, false, false, false, NULL},
	{ 0b1111101, IR_OPMASK, false, false, false, NULL},
	{ 0b1111110, IR_OPMASK, false, false, false, NULL},
	{ 0b1111111, IR_OPMASK, false, false, false, NULL}

};

struct opdef iset_74[] = {
	{ 0, IR_OPMASK, true, false, false, op_74_uj},
	{ 1, IR_OPMASK, true, false, false, op_74_jl},
	{ 2, IR_OPMASK, true, false, false, op_74_je},
	{ 3, IR_OPMASK, true, false, false, op_74_jg},
	{ 4, IR_OPMASK, true, false, false, op_74_jz},
	{ 5, IR_OPMASK, true, false, false, op_74_jm},
	{ 6, IR_OPMASK, true, false, false, op_74_jn},
	{ 7, IR_OPMASK, true, false, false, op_74_lj}
};

struct opdef iset_75[] = {
	{ 0, IR_OPMASK, true, false, false, op_75_ld},
	{ 1, IR_OPMASK, true, false, false, op_75_lf},
	{ 2, IR_OPMASK, true, false, false, op_75_la},
	{ 3, IR_OPMASK, true, false, false, op_75_ll},
	{ 4, IR_OPMASK, true, false, false, op_75_td},
	{ 5, IR_OPMASK, true, false, false, op_75_tf},
	{ 6, IR_OPMASK, true, false, false, op_75_ta},
	{ 7, IR_OPMASK, true, false, false, op_75_tl}
};

struct opdef iset_76[] = {
	{ 0, IR_OPMASK, true, false, false, op_76_rd},
	{ 1, IR_OPMASK, true, false, false, op_76_rf},
	{ 2, IR_OPMASK, true, false, false, op_76_ra},
	{ 3, IR_OPMASK, true, false, false, op_76_rl},
	{ 4, IR_OPMASK, true, false, false, op_76_pd},
	{ 5, IR_OPMASK, true, false, false, op_76_pf},
	{ 6, IR_OPMASK, true, false, false, op_76_pa},
	{ 7, IR_OPMASK, true, false, false, op_76_pl}
};

struct opdef iset_77[] = {
	{ 0, IR_OPMASK, true, false, true,  op_77_mb},
	{ 1, IR_OPMASK, true, false, true,  op_77_im},
	{ 2, IR_OPMASK, true, false, true,  op_77_ki},
	{ 3, IR_OPMASK, true, false, true,  op_77_fi},
	{ 4, IR_OPMASK, true, false, true,  op_77_sp},
	{ 5, IR_OPMASK, true, false, false, op_77_md},
	{ 6, IR_OPMASK, true, false, false, op_77_rz},
	{ 7, IR_OPMASK, true, false, false, op_77_ib}
};

// vim: tabstop=4 shiftwidth=4 autoindent
