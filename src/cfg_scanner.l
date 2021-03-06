%{
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

#include "cfg_parser.h"

void cyyerror(char *);

#define YY_USER_ACTION cyylloc.first_line = cyylineno;

%}

%top{
	#define _XOPEN_SOURCE 500
}

%option nounput
%option noinput
%option yylineno

number	[0-9]+
text	[^,: \n\t\r\f]+
chars	[\}\{=:,]
comment	#.*$
string	\".+\"

%%

computer		return COMPUTER;
speed_real		return SPEED_REAL;
timer_step		return TIMER_STEP;
timer_start		return TIMER_START;
cpu_mod			return CPU_MOD;
cpu_user_io_illegal		return CPU_USER_IO_ILLEGAL;
cpu_awp			return CPU_AWP;
mem_elwro		return ELWRO;
mem_mega		return MEGA;
mem_mega_prom	return MEGA_PROM;
mem_mega_boot	return MEGA_BOOT;
cpu_stop_on_nomem return CPU_STOP_ON_NOMEM;
mem_os_segments	return OS_SEG;

log				return LOG;
enabled			return ENABLED;
file			return LFILE;
levels			return LEVELS;

channel			return CHANNEL;
unit			return UNIT;

true|yes		{ cyylval.value.v = 1; cyylval.value.s = strdup(cyytext); return BOOL; }
false|no		{ cyylval.value.v = 0; cyylval.value.s = strdup(cyytext); return BOOL; }

{string} {
	cyytext[strlen(cyytext)-1] = '\0';
	cyylval.value.s = strdup(cyytext+1);
	return STRING;
}

{number} {
	cyylval.value.v = strtol(cyytext, NULL, 10);
	cyylval.value.s = strdup(cyytext);
	return VALUE;
}

{chars} {
	return *cyytext;
}

[ \t\n\r\f]+ ;
{comment}	;

{text} {
	cyylval.value.s = strdup(cyytext);
	return TEXT;
}

%%

int cyywrap(void) {
	return 1;
}

// vim: tabstop=4 shiftwidth=4 autoindent
