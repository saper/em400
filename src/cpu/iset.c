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
#include "cpu/instructions.h"

struct opdef iset[] = {
	{ 000, false, false, false, NULL},
	{ 001, false, false, false, NULL},
	{ 002, false, false, false, NULL},
	{ 003, false, false, false, NULL},
	{ 004, false, false, false, NULL},
	{ 005, false, false, false, NULL},
	{ 006, false, false, false, NULL},
	{ 007, false, false, false, NULL},
	{ 010, false, false, false, NULL},
	{ 011, false, false, false, NULL},
	{ 012, false, false, false, NULL},
	{ 013, false, false, false, NULL},
	{ 014, false, false, false, NULL},
	{ 015, false, false, false, NULL},
	{ 016, false, false, false, NULL},
	{ 017, false, false, false, NULL},
	
	{ 020, true, false, false, op_lw},
	{ 021, true, false, false, op_tw},
	{ 022, true, false, false, op_ls},
	{ 023, true, false, false, op_ri},
	{ 024, true, false, false, op_rw},
	{ 025, true, false, false, op_pw},
	{ 026, true, false, false, op_rj},
	{ 027, true, false, false, op_is},
	{ 030, true, false, false, op_bb},
	{ 031, true, false, false, op_bm},
	{ 032, true, false, false, op_bs},
	{ 033, true, false, false, op_bc},
	{ 034, true, false, false, op_bn},
	{ 035, true, false, true, op_ou},
	{ 036, true, false, true, op_in},

	{ 037, true, false, false, NULL /* goaway op_37 */ },

	{ 040, true, false, false, op_aw},
	{ 041, true, false, false, op_ac},
	{ 042, true, false, false, op_sw},
	{ 043, true, false, false, op_cw},
	{ 044, true, false, false, op_or},
	{ 045, true, false, false, op_om},
	{ 046, true, false, false, op_nr},
	{ 047, true, false, false, op_nm},
	{ 050, true, false, false, op_er},
	{ 051, true, false, false, op_em},
	{ 052, true, false, false, op_xr},
	{ 053, true, false, false, op_xm},
	{ 054, true, false, false, op_cl},
	{ 055, true, false, false, op_lb},
	{ 056, true, false, false, op_rb},
	{ 057, true, false, false, op_cb},

	{ 060, false, true, false, op_awt},
	{ 061, false, true, false, op_trb},
	{ 062, false, true, false, op_irb},
	{ 063, false, true, false, op_drb},
	{ 064, false, true, false, op_cwt},
	{ 065, false, true, false, op_lwt},
	{ 066, false, true, false, op_lws},
	{ 067, false, true, false, op_rws},

	{ 070, false, true,  false, NULL /* goaway op_70 */ },
	{ 071, false, false, false, NULL /* goaway op_71 */ },
	{ 072, false, false, false, NULL /* goaway op_72 */ },
	{ 073, false, false, true,  NULL /* goaway op_73 */ },
	{ 074, true,  false, false, NULL /* goaway op_74 */ },
	{ 075, true,  false, false, NULL /* goaway op_75 */ },
	{ 076, true,  false, false, NULL /* goaway op_76 */ },
	{ 077, true,  false, false, NULL /* goaway op_77 */ }
};

// [A]
struct opdef iset_37[] = {
	{ 0, true, false, false, op_37_ad},
	{ 1, true, false, false, op_37_sd},
	{ 2, true, false, false, op_37_mw},
	{ 3, true, false, false, op_37_dw},
	{ 4, true, false, false, op_37_af},
	{ 5, true, false, false, op_37_sf},
	{ 6, true, false, false, op_37_mf},
	{ 7, true, false, false, op_37_df}
};

// [A]
struct opdef iset_70[] = {
	{ 0, false, true, false, op_70_ujs},
	{ 1, false, true, false, op_70_jls},
	{ 2, false, true, false, op_70_jes},
	{ 3, false, true, false, op_70_jgs},
	{ 4, false, true, false, op_70_jvs},
	{ 5, false, true, false, op_70_jxs},
	{ 6, false, true, false, op_70_jys},
	{ 7, false, true, false, op_70_jcs}
};

// [D,A0]
struct opdef iset_71[] = {
	{ 0, false, false, false, op_71_blc},
	{ 1, false, false, false, op_71_exl},
	{ 2, false, false, false, op_71_brc},
	{ 3, false, false, false, op_71_nrf}
};

// [D,B,C]
struct opdef iset_72[] = {
	{ 0b0000000, false, false, false, op_72_ric},
	{ 0b0000001, false, false, false, op_72_zlb},
	{ 0b0000010, false, false, false, op_72_sxu},
	{ 0b0000011, false, false, false, op_72_nga},
	{ 0b0000100, false, false, false, op_72_slz},
	{ 0b0000101, false, false, false, op_72_sly},
	{ 0b0000110, false, false, false, op_72_slx},
	{ 0b0000111, false, false, false, op_72_sry},
	{ 0b0001000, false, false, false, op_72_ngl},
	{ 0b0001001, false, false, false, op_72_rpc},
	{ 0b0001010, false, false, false, NULL},
	{ 0b0001011, false, false, false, NULL},
	{ 0b0001100, false, false, false, NULL},
	{ 0b0001101, false, false, false, NULL},
	{ 0b0001110, false, false, false, NULL},
	{ 0b0001111, false, false, false, NULL},
	{ 0b0010000, false, false, false, op_72_shc},
	{ 0b0010001, false, false, false, op_72_shc},
	{ 0b0010010, false, false, false, op_72_shc},
	{ 0b0010011, false, false, false, op_72_shc},
	{ 0b0010100, false, false, false, op_72_shc},
	{ 0b0010101, false, false, false, op_72_shc},
	{ 0b0010110, false, false, false, op_72_shc},
	{ 0b0010111, false, false, false, op_72_shc},
	{ 0b0011000, false, false, false, NULL},
	{ 0b0011001, false, false, false, NULL},
	{ 0b0011010, false, false, false, NULL},
	{ 0b0011011, false, false, false, NULL},
	{ 0b0011100, false, false, false, NULL},
	{ 0b0011101, false, false, false, NULL},
	{ 0b0011110, false, false, false, NULL},
	{ 0b0011111, false, false, false, NULL},
	{ 0b0100000, false, false, false, NULL},
	{ 0b0100001, false, false, false, NULL},
	{ 0b0100010, false, false, false, NULL},
	{ 0b0100011, false, false, false, NULL},
	{ 0b0100100, false, false, false, NULL},
	{ 0b0100101, false, false, false, NULL},
	{ 0b0100110, false, false, false, NULL},
	{ 0b0100111, false, false, false, NULL},
	{ 0b0101000, false, false, false, NULL},
	{ 0b0101001, false, false, false, NULL},
	{ 0b0101010, false, false, false, NULL},
	{ 0b0101011, false, false, false, NULL},
	{ 0b0101100, false, false, false, NULL},
	{ 0b0101101, false, false, false, NULL},
	{ 0b0101110, false, false, false, NULL},
	{ 0b0101111, false, false, false, NULL},
	{ 0b0110000, false, false, false, NULL},
	{ 0b0110001, false, false, false, NULL},
	{ 0b0110010, false, false, false, NULL},
	{ 0b0110011, false, false, false, NULL},
	{ 0b0110100, false, false, false, NULL},
	{ 0b0110101, false, false, false, NULL},
	{ 0b0110110, false, false, false, NULL},
	{ 0b0110111, false, false, false, NULL},
	{ 0b0111000, false, false, false, NULL},
	{ 0b0111001, false, false, false, NULL},
	{ 0b0111010, false, false, false, NULL},
	{ 0b0111011, false, false, false, NULL},
	{ 0b0111100, false, false, false, NULL},
	{ 0b0111101, false, false, false, NULL},
	{ 0b0111110, false, false, false, NULL},
	{ 0b0111111, false, false, false, NULL},
	{ 0b1000000, false, false, false, op_72_rky},
	{ 0b1000001, false, false, false, op_72_zrb},
	{ 0b1000010, false, false, false, op_72_sxl},
	{ 0b1000011, false, false, false, op_72_ngc},
	{ 0b1000100, false, false, false, op_72_svz},
	{ 0b1000101, false, false, false, op_72_svy},
	{ 0b1000110, false, false, false, op_72_svx},
	{ 0b1000111, false, false, false, op_72_srx},
	{ 0b1001000, false, false, false, op_72_srz},
	{ 0b1001001, false, false, false, op_72_lpc},
	{ 0b1001010, false, false, false, NULL},
	{ 0b1001011, false, false, false, NULL},
	{ 0b1001100, false, false, false, NULL},
	{ 0b1001101, false, false, false, NULL},
	{ 0b1001110, false, false, false, NULL},
	{ 0b1001111, false, false, false, NULL},
	{ 0b1010001, false, false, false, op_72_shc},
	{ 0b1010000, false, false, false, op_72_shc},
	{ 0b1010010, false, false, false, op_72_shc},
	{ 0b1010011, false, false, false, op_72_shc},
	{ 0b1010100, false, false, false, op_72_shc},
	{ 0b1010101, false, false, false, op_72_shc},
	{ 0b1010110, false, false, false, op_72_shc},
	{ 0b1010111, false, false, false, op_72_shc},
	{ 0b1011000, false, false, false, NULL},
	{ 0b1011001, false, false, false, NULL},
	{ 0b1011010, false, false, false, NULL},
	{ 0b1011011, false, false, false, NULL},
	{ 0b1011100, false, false, false, NULL},
	{ 0b1011101, false, false, false, NULL},
	{ 0b1011110, false, false, false, NULL},
	{ 0b1011111, false, false, false, NULL},
	{ 0b1100000, false, false, false, NULL},
	{ 0b1100001, false, false, false, NULL},
	{ 0b1100010, false, false, false, NULL},
	{ 0b1100011, false, false, false, NULL},
	{ 0b1100100, false, false, false, NULL},
	{ 0b1100101, false, false, false, NULL},
	{ 0b1100110, false, false, false, NULL},
	{ 0b1100111, false, false, false, NULL},
	{ 0b1101000, false, false, false, NULL},
	{ 0b1101001, false, false, false, NULL},
	{ 0b1101010, false, false, false, NULL},
	{ 0b1101011, false, false, false, NULL},
	{ 0b1101100, false, false, false, NULL},
	{ 0b1101101, false, false, false, NULL},
	{ 0b1101110, false, false, false, NULL},
	{ 0b1101111, false, false, false, NULL},
	{ 0b1110001, false, false, false, NULL},
	{ 0b1110000, false, false, false, NULL},
	{ 0b1110010, false, false, false, NULL},
	{ 0b1110011, false, false, false, NULL},
	{ 0b1110100, false, false, false, NULL},
	{ 0b1110101, false, false, false, NULL},
	{ 0b1110110, false, false, false, NULL},
	{ 0b1110111, false, false, false, NULL},
	{ 0b1111000, false, false, false, NULL},
	{ 0b1111001, false, false, false, NULL},
	{ 0b1111010, false, false, false, NULL},
	{ 0b1111011, false, false, false, NULL},
	{ 0b1111100, false, false, false, NULL},
	{ 0b1111101, false, false, false, NULL},
	{ 0b1111110, false, false, false, NULL},
	{ 0b1111111, false, false, false, NULL}

};

// [D,A,C] (but not all used in each instruction)
struct opdef iset_73[] = {
	{ 0b0000000, false, true, true, op_73_hlt},
	{ 0b0000001, false, true, true, op_73_hlt},
	{ 0b0000010, false, true, true, op_73_hlt},
	{ 0b0000011, false, true, true, op_73_hlt},
	{ 0b0000100, false, true, true, op_73_hlt},
	{ 0b0000101, false, true, true, op_73_hlt},
	{ 0b0000110, false, true, true, op_73_hlt},
	{ 0b0000111, false, true, true, op_73_hlt},

	{ 0b0001000, false, false, true, op_73_mcl},
	{ 0b0001001, false, false, true, op_73_mcl},
	{ 0b0001010, false, false, true, op_73_mcl},
	{ 0b0001011, false, false, true, op_73_mcl},
	{ 0b0001100, false, false, true, op_73_mcl},
	{ 0b0001101, false, false, true, op_73_mcl},
	{ 0b0001110, false, false, true, op_73_mcl},
	{ 0b0001111, false, false, true, op_73_mcl},

	{ 0b0010000, false, false, true, op_73_cit},
	{ 0b0010001, false, false, true, op_73_sil},
	{ 0b0010010, false, false, true, op_73_siu},
	{ 0b0010011, false, false, true, op_73_sit},
	{ 0b0010100, false, false, true, op_73_sint},
	{ 0b0010101, false, false, false, NULL},
	{ 0b0010110, false, false, false, NULL},
	{ 0b0010111, false, false, false, NULL},

	{ 0b0011000, false, false, true, op_73_giu},
	{ 0b0011001, false, false, true, op_73_giu},
	{ 0b0011010, false, false, true, op_73_giu},
	{ 0b0011011, false, false, true, op_73_giu},
	{ 0b0011100, false, false, true, op_73_giu},
	{ 0b0011101, false, false, true, op_73_giu},
	{ 0b0011110, false, false, true, op_73_giu},
	{ 0b0011111, false, false, true, op_73_giu},

	{ 0b0100000, false, false, true, op_73_lip},
	{ 0b0100001, false, false, true, op_73_lip},
	{ 0b0100010, false, false, true, op_73_lip},
	{ 0b0100011, false, false, true, op_73_lip},
	{ 0b0100100, false, false, true, op_73_lip},
	{ 0b0100101, false, false, true, op_73_lip},
	{ 0b0100110, false, false, true, op_73_lip},
	{ 0b0100111, false, false, true, op_73_lip},

	{ 0b0101000, false, false, true,  NULL},
	{ 0b0101001, false, false, false, NULL},
	{ 0b0101010, false, false, false, NULL},
	{ 0b0101011, false, false, false, NULL},
	{ 0b0101100, false, false, false, NULL},
	{ 0b0101101, false, false, false, NULL},
	{ 0b0101110, false, false, false, NULL},
	{ 0b0101111, false, false, false, NULL},

	{ 0b0110000, false, false, false, NULL},
	{ 0b0110001, false, false, false, NULL},
	{ 0b0110010, false, false, false, NULL},
	{ 0b0110011, false, false, false, NULL},
	{ 0b0110100, false, false, false, NULL},
	{ 0b0110101, false, false, false, NULL},
	{ 0b0110110, false, false, false, NULL},
	{ 0b0110111, false, false, false, NULL},

	{ 0b0111000, false, false, false, NULL},
	{ 0b0111001, false, false, false, NULL},
	{ 0b0111010, false, false, false, NULL},
	{ 0b0111011, false, false, false, NULL},
	{ 0b0111100, false, false, false, NULL},
	{ 0b0111101, false, false, false, NULL},
	{ 0b0111110, false, false, false, NULL},
	{ 0b0111111, false, false, false, NULL},

	{ 0b1000000, false, true, true, op_73_hlt},
	{ 0b1000001, false, true, true, op_73_hlt},
	{ 0b1000010, false, true, true, op_73_hlt},
	{ 0b1000011, false, true, true, op_73_hlt},
	{ 0b1000100, false, true, true, op_73_hlt},
	{ 0b1000101, false, true, true, op_73_hlt},
	{ 0b1000110, false, true, true, op_73_hlt},
	{ 0b1000111, false, true, true, op_73_hlt},

	{ 0b1001000, false, false, true, op_73_mcl},
	{ 0b1001001, false, false, true, op_73_mcl},
	{ 0b1001010, false, false, true, op_73_mcl},
	{ 0b1001011, false, false, true, op_73_mcl},
	{ 0b1001100, false, false, true, op_73_mcl},
	{ 0b1001101, false, false, true, op_73_mcl},
	{ 0b1001110, false, false, true, op_73_mcl},
	{ 0b1001111, false, false, true, op_73_mcl},

	{ 0b1010000, false, false, true, op_73_cit},
	{ 0b1010001, false, false, true, op_73_sil},
	{ 0b1010010, false, false, true, op_73_siu},
	{ 0b1010011, false, false, true, op_73_sit},
	{ 0b1010100, false, false, true, op_73_sind},
	{ 0b1010101, false, false, false, NULL},
	{ 0b1010110, false, false, false, NULL},
	{ 0b1010111, false, false, false, NULL},

	{ 0b1011000, false, false, true, op_73_gil},
	{ 0b1011001, false, false, true, op_73_gil},
	{ 0b1011010, false, false, true, op_73_gil},
	{ 0b1011011, false, false, true, op_73_gil},
	{ 0b1011100, false, false, true, op_73_gil},
	{ 0b1011101, false, false, true, op_73_gil},
	{ 0b1011110, false, false, true, op_73_gil},
	{ 0b1011111, false, false, true, op_73_gil},

	{ 0b1100001, false, false, true, op_73_lip},
	{ 0b1100000, false, false, true, op_73_lip},
	{ 0b1100010, false, false, true, op_73_lip},
	{ 0b1100011, false, false, true, op_73_lip},
	{ 0b1100101, false, false, true, op_73_lip},
	{ 0b1100100, false, false, true, op_73_lip},
	{ 0b1100110, false, false, true, op_73_lip},
	{ 0b1100111, false, false, true, op_73_lip},

	{ 0b1101000, false, false, false, NULL},
	{ 0b1101001, false, false, false, NULL},
	{ 0b1101010, false, false, false, NULL},
	{ 0b1101011, false, false, false, NULL},
	{ 0b1101100, false, false, false, NULL},
	{ 0b1101101, false, false, false, NULL},
	{ 0b1101110, false, false, false, NULL},
	{ 0b1101111, false, false, false, NULL},

	{ 0b1110000, false, false, false, NULL},
	{ 0b1110001, false, false, false, NULL},
	{ 0b1110010, false, false, false, NULL},
	{ 0b1110011, false, false, false, NULL},
	{ 0b1110100, false, false, false, NULL},
	{ 0b1110101, false, false, false, NULL},
	{ 0b1110110, false, false, false, NULL},
	{ 0b1110111, false, false, false, NULL},

	{ 0b1111000, false, false, false, NULL},
	{ 0b1111001, false, false, false, NULL},
	{ 0b1111010, false, false, false, NULL},
	{ 0b1111011, false, false, false, NULL},
	{ 0b1111100, false, false, false, NULL},
	{ 0b1111101, false, false, false, NULL},
	{ 0b1111110, false, false, false, NULL},
	{ 0b1111111, false, false, false, NULL}

};

struct opdef iset_74[] = {
	{ 0, true, false, false, op_74_uj},
	{ 1, true, false, false, op_74_jl},
	{ 2, true, false, false, op_74_je},
	{ 3, true, false, false, op_74_jg},
	{ 4, true, false, false, op_74_jz},
	{ 5, true, false, false, op_74_jm},
	{ 6, true, false, false, op_74_jn},
	{ 7, true, false, false, op_74_lj}
};

struct opdef iset_75[] = {
	{ 0, true, false, false, op_75_ld},
	{ 1, true, false, false, op_75_lf},
	{ 2, true, false, false, op_75_la},
	{ 3, true, false, false, op_75_ll},
	{ 4, true, false, false, op_75_td},
	{ 5, true, false, false, op_75_tf},
	{ 6, true, false, false, op_75_ta},
	{ 7, true, false, false, op_75_tl}
};

struct opdef iset_76[] = {
	{ 0, true, false, false, op_76_rd},
	{ 1, true, false, false, op_76_rf},
	{ 2, true, false, false, op_76_ra},
	{ 3, true, false, false, op_76_rl},
	{ 4, true, false, false, op_76_pd},
	{ 5, true, false, false, op_76_pf},
	{ 6, true, false, false, op_76_pa},
	{ 7, true, false, false, op_76_pl}
};

struct opdef iset_77[] = {
	{ 0, true, false, true,  op_77_mb},
	{ 1, true, false, true,  op_77_im},
	{ 2, true, false, true,  op_77_ki},
	{ 3, true, false, true,  op_77_fi},
	{ 4, true, false, true,  op_77_sp},
	{ 5, true, false, false, op_77_md},
	{ 6, true, false, false, op_77_rz},
	{ 7, true, false, false, op_77_ib}
};

// vim: tabstop=4 shiftwidth=4 autoindent
