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

#include <stdlib.h>
#include <plist/plist.h>
#include <glib/gi18n.h>
#include "window.h"

struct _PvAppwin
{
	GtkApplicationWindow parent;
};

struct _PvAppwinClass
{
	GtkApplicationWindowClass parent_class;
};

typedef struct _PvAppwinPrivate
{
	GtkTreeView *treeview;
	GFile *file;
} PvAppwinPrivate;

enum
{
	PROP_0,
	PROP_FILE,
	N_PROPERTIES
};


G_DEFINE_TYPE_WITH_PRIVATE(PvAppwin, pv_appwin, GTK_TYPE_APPLICATION_WINDOW)

static void
pv_appwin_set_property (GObject *obj, guint property, const GValue *value, GParamSpec *pspec)
{
	switch (property)
	{
		case PROP_FILE:
			pv_appwin_set_file (PV_APPWIN(obj), g_value_get_object (value));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (obj, property, pspec);
			break;
	}
}

static void
pv_appwin_get_property (GObject *obj, guint property, GValue *value, GParamSpec *pspec)
{
	switch (property)
	{
		case PROP_FILE:
			g_value_set_object (value, pv_appwin_get_file (PV_APPWIN(obj)));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (obj, property, pspec);
			break;
	}
}

static void
pv_appwin_finalize (GObject *obj)
{
	PvAppwinPrivate *priv = pv_appwin_get_instance_private (PV_APPWIN(obj));

	if (priv->file)
		g_object_unref (priv->file);

	G_OBJECT_CLASS (pv_appwin_parent_class)->finalize (obj);
}

static void
pv_appwin_class_init (PvAppwinClass *klass)
{
	GtkWidgetClass *wid_class = GTK_WIDGET_CLASS(klass);
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);
	GParamSpec *obj_properties[N_PROPERTIES] = { NULL };

	obj_class->finalize = pv_appwin_finalize;
	obj_class->set_property = pv_appwin_set_property;
	obj_class->get_property = pv_appwin_get_property;

	obj_properties[PROP_FILE] = g_param_spec_object ("file",
                        "plist file", "file to show in the window",
						G_TYPE_FILE, G_PARAM_READWRITE);

	g_object_class_install_properties (obj_class, N_PROPERTIES, obj_properties);

	gtk_widget_class_set_template_from_resource (wid_class, "/se/tingping/plist-viewer/ui/window.ui");
	gtk_widget_class_bind_template_child_private (wid_class, PvAppwin, treeview);
}

enum columns {
	COL_KEY,
	COL_VALUE,
	COL_TYPE
};

static const char * const type_strings[] = {
	N_("Boolean"),
	N_("UInt"),
	N_("Real"),
	N_("String"),
	N_("Array"),
	N_("Dictionary"),
	N_("Date"),
	N_("Data"),
	N_("Key"),
	N_("UID"),
	N_("None")
};

G_STATIC_ASSERT(G_N_ELEMENTS(type_strings) == PLIST_NONE + 1);

//#define ITEMS_FORMAT "<span foreground=\"gray\">(%"G_GUINT32_FORMAT" items)</span>"
#define ITEMS_FORMAT "(%"G_GUINT32_FORMAT" items)"

static inline void
parse_plist_into_treestore (GtkTreeStore *store, GtkTreeIter *iter, plist_t node)
{
	g_assert (iter != NULL);
	
	GtkTreeIter new_iter;
	const plist_type node_type = plist_get_node_type (node);
	gtk_tree_store_set (store, iter, COL_TYPE, type_strings[node_type], -1);
	switch (node_type)
	{
		case PLIST_BOOLEAN:
		{
			guint8 val;

			plist_get_bool_val (node, &val);
			gtk_tree_store_set (store, iter, COL_VALUE, val ? "YES" : "NO", -1);

			break;
		}
		case PLIST_UID:
		case PLIST_UINT:
		{
			guint64 val;
			char str_val[32];

			if (node_type == PLIST_UINT)
				plist_get_uint_val (node, &val);
			else
				plist_get_uid_val (node, &val);

			g_snprintf (str_val, sizeof(str_val), "%"G_GUINT64_FORMAT, val);
			gtk_tree_store_set (store, iter, COL_VALUE, str_val, -1);

			break;
		}
		case PLIST_REAL:
		{
			double val;
			char str_val[32];

			plist_get_real_val (node, &val);
			g_snprintf (str_val, sizeof(str_val), "%f", val);
			gtk_tree_store_set (store, iter, COL_VALUE, str_val, -1);

			break;
		}
		case PLIST_DATA:
		{
			char *val;
			guint64 val_len;

			plist_get_data_val (node, &val, &val_len);
			/* FIXME: This can be full of garbage */
			gtk_tree_store_set (store, iter, COL_VALUE, val, -1);

			free (val);
			break;
		}
		case PLIST_STRING:
		{
			char *val;

			plist_get_string_val (node, &val);
			gtk_tree_store_set (store, iter, COL_VALUE, val, -1);

			free (val);
			break;
		}
		case PLIST_ARRAY:
		{
			guint32 size;
			char str_size[64];

			size = plist_array_get_size (node);
			g_snprintf (str_size, sizeof(str_size), ITEMS_FORMAT, size);
			gtk_tree_store_set (store, iter, COL_VALUE, str_size, -1);

			for (guint32 i = 0; i < size; ++i)
			{
				plist_t array_node;
				char item_str[32];

				gtk_tree_store_append (store, &new_iter, iter);

				g_snprintf (item_str, sizeof(item_str), "Item %"G_GUINT32_FORMAT, i);
				gtk_tree_store_set (store, &new_iter, COL_KEY, item_str, -1);

				array_node = plist_array_get_item (node, i);
				parse_plist_into_treestore (store, &new_iter, array_node);
			}

			break;
		}
		case PLIST_DICT:
		{
			guint32 size;
			char str_size[64];
			plist_dict_iter dict_iter = NULL;

			size = plist_dict_get_size (node);
			g_snprintf (str_size, sizeof(str_size), ITEMS_FORMAT, size);
			gtk_tree_store_set (store, iter, COL_VALUE, str_size, -1);

			plist_dict_new_iter (node, &dict_iter);
			for (guint32 i = 0; i < size; ++i)
			{
				plist_t dict_node = NULL;
				char *key = NULL;

				plist_dict_next_item (node, dict_iter, &key, &dict_node);

				gtk_tree_store_append (store, &new_iter, iter);
				gtk_tree_store_set (store, &new_iter, COL_KEY, key, -1);
				parse_plist_into_treestore (store, &new_iter, dict_node);

				free (key);
			}

			break;
		}
		case PLIST_DATE:
		{
			gint32 sec, usec;
			char *time_str;

			plist_get_date_val (node, &sec, &usec);

			GTimeVal tv = { .tv_sec = sec, .tv_usec = usec };
			GDateTime *date_2001 = g_date_time_new_from_timeval_local (&tv);
			if (date_2001 == NULL)
			{
				g_warning ("Failed to create date\n");
				break;
			}

			/* This date starts from 2001... */
			GDateTime *date = g_date_time_add_years (date_2001, 31);
			if (date == NULL)
			{
				/* And this will all fail for dates past 2038... */
				g_warning ("Failed to modify date\n");
				break;
			}

			time_str = g_date_time_format (date, "%c");
			gtk_tree_store_set (store, iter, COL_VALUE, time_str, -1);

			g_date_time_unref (date_2001);
			g_date_time_unref (date);
			g_free (time_str);
			break;
		}
		case PLIST_KEY:
		{
			char *val;

			plist_get_key_val (node, &val);
			gtk_tree_store_append (store, &new_iter, NULL);
			gtk_tree_store_set (store, &new_iter, 0, val, -1);

			free (val);
			break;
		}
		case PLIST_NONE:
			break;
	}
}

GFile *
pv_appwin_get_file (PvAppwin *win)
{
	g_return_val_if_fail (win != NULL, NULL);

	PvAppwinPrivate *priv = pv_appwin_get_instance_private (win);

	return priv->file;
}

static void
pv_appwin_update_title (PvAppwin *win, char *file_name)
{
	char *base_name = g_path_get_basename (file_name);
	char *new_title = g_strdup_printf ("%s (%s)", g_get_application_name (), base_name);

	gtk_window_set_title (GTK_WINDOW(win), new_title);

	g_free (base_name);
	g_free (new_title);
}

gboolean
pv_appwin_set_file (PvAppwin *win, GFile *file)
{
	g_return_val_if_fail (file != NULL, FALSE);
	g_return_val_if_fail (win != NULL, FALSE);

	PvAppwinPrivate *priv = pv_appwin_get_instance_private (win);
	GtkTreeStore *store = GTK_TREE_STORE(gtk_tree_view_get_model (priv->treeview));

	g_return_val_if_fail (store != NULL, FALSE);

	if (priv->file != NULL)
	{
		if (priv->file == file)
		{
			g_warning ("New file is same as old file\n");
			return FALSE;
		}
		g_object_unref (priv->file);
		gtk_tree_store_clear (store);
	}

	g_object_ref (file);
	priv->file = file;

	char *data;
	gsize data_len;
	if (!g_file_load_contents (file, NULL, &data, &data_len, NULL, NULL))
	{
		g_warning ("Failed to load file\n");
		return FALSE;
	}

	plist_t root_node = NULL;

	char *file_path = g_file_get_path (file);
	if (!file_path)
	{
		g_warning ("Failed to get file path\n");
		return FALSE;
	}
	pv_appwin_update_title (win, file_path);

	if (g_str_has_suffix (file_path, ".bin"))
		plist_from_bin (data, data_len, &root_node);
	else
		plist_from_xml (data, data_len, &root_node);

	g_free (file_path);
	g_free (data);
	if (!root_node)
	{
		g_warning ("Failed to parse file\n");
		return FALSE;
	}

	GtkTreeIter iter;
	gtk_tree_store_append (store, &iter, NULL);
	gtk_tree_store_set (store, &iter, COL_KEY, "Root", -1);

	parse_plist_into_treestore (store, &iter, root_node);
	plist_free (root_node);

	/* Expand root dict */
	GtkTreePath *path = gtk_tree_path_new_first ();
	gtk_tree_view_expand_row (priv->treeview, path, FALSE);
	gtk_tree_path_free (path);

	return TRUE;
}

static void
pv_appwin_init (PvAppwin *win)
{
	gtk_widget_init_template (GTK_WIDGET(win));
}

PvAppwin *
pv_appwin_new (PvApp *app)
{
	return g_object_new (PV_APPWIN_TYPE,
						 "application", G_APPLICATION(app),
						 NULL);
}
