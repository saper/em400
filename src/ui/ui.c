//  Copyright (c) 2016 Jakub Filipowicz <jakubf@gmail.com>
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

#define _XOPEN_SOURCE 500

#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <strings.h>

#include "errors.h"
#include "ui/ui.h"

extern struct ui_drv ui_cmd;

struct ui_drv* uis[] = {
	&ui_cmd,
	NULL
};

pthread_t ui_th;

// -----------------------------------------------------------------------
struct ui * ui_create(const char *name)
{
	struct ui_drv **drv = uis;
	while (drv && *drv) {
		if (!strncasecmp(name, (*drv)->name, strlen((*drv)->name))) {
			struct ui *ui = calloc(1, sizeof(struct ui));
			if (!ui) {
				gerr = E_ALLOC;
				return NULL;
			}
			ui->drv = *drv;

			// setup the UI
			ui->data = ui->drv->setup(name);
			if (!ui->data) {
				gerr = E_UI_SETUP;
				return NULL;
			}
			return ui;
		}
		drv++;
	}

	gerr = E_UI_NOTFOUND;
	return NULL;
}

// -----------------------------------------------------------------------
static void * ui_loop(void *ptr)
{
	struct ui *ui = (struct ui *) ptr;

	ui->drv->loop(ui->data);

	return NULL;
}

// -----------------------------------------------------------------------
int ui_run(struct ui *ui)
{
	// initialize the UI loop
	if (pthread_create(&ui->th, NULL, ui_loop, ui)) {
		return E_THREAD;
	}

	return E_OK;
}

// -----------------------------------------------------------------------
void ui_shutdown(struct ui *ui)
{
	if (!ui) {
		return;
	}

	ui->drv->stop(ui->data);
	if (ui->th) {
		pthread_join(ui->th, NULL);
	}
	ui->drv->destroy(ui->data);
	free(ui);
}

// vim: tabstop=4 shiftwidth=4 autoindent
