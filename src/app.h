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

#define PV_APP_TYPE (pv_app_get_type ())
#define PV_APP(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), PV_APP_TYPE, PvApp))

typedef struct _PvApp PvApp;
typedef struct _PvAppClass PvAppClass;

GType pv_app_get_type (void);
PvApp *pv_app_new (void);
