/* plist-viewer
 * Copyright (C) 2014
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

#pragma once

#include <gtk/gtk.h>
#include "app.h"

#define PV_APPWIN_TYPE (pv_appwin_get_type ())
#define PV_APPWIN(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), PV_APPWIN_TYPE, PvAppwin))

typedef struct _PvAppwin PvAppwin;
typedef struct _PvAppwinClass PvAppwinClass;

GType pv_appwin_get_type (void);
PvAppwin *pv_appwin_new (PvApp *);
gboolean pv_appwin_set_file (PvAppwin *, GFile *);
GFile *pv_appwin_get_file (PvAppwin *);
