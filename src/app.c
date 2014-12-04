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

#include <glib/gi18n.h>
#include "app.h"
#include "window.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

struct _PvApp
{
	GtkApplication parent;
};

struct _PvAppClass
{
	GtkApplicationClass parent_class;
};

G_DEFINE_TYPE(PvApp, pv_app, GTK_TYPE_APPLICATION)

static void
pv_app_about (GSimpleAction *action, GVariant *param, gpointer userdata)
{
	GtkWindow *win = gtk_application_get_active_window (GTK_APPLICATION(userdata));

	gtk_show_about_dialog (win,
		"program-name", PACKAGE_NAME,
		"version", PACKAGE_VERSION,
		"license-type", GTK_LICENSE_GPL_2_0,
		"logo-icon-name", "text-x-generic",
		NULL);
}

static PvAppwin *
find_window_to_open (PvApp *app, GFile *new_file)
{
	GList *windows = gtk_application_get_windows (GTK_APPLICATION(app));
	for (; windows != NULL; windows = g_list_next (windows))
	{
		PvAppwin *win = PV_APPWIN(windows->data);
		GFile *old_file = pv_appwin_get_file (win);

		if (old_file == NULL)
		{
			return win;
		}
		else if (g_file_equal (old_file, new_file))
		{
			gtk_window_present (GTK_WINDOW(win));
			return NULL;
		}
	}

	return pv_appwin_new (app);
;
}

static void
pv_app_openfile (GSimpleAction *action, GVariant *param, gpointer userdata)
{
	PvApp *app = PV_APP(userdata);
	GtkWindow *parent_win;
	GtkFileChooser *dialog;

	parent_win = gtk_application_get_active_window (GTK_APPLICATION(app));
	dialog = GTK_FILE_CHOOSER(gtk_file_chooser_dialog_new (_("Open plist file"), parent_win,
					GTK_FILE_CHOOSER_ACTION_OPEN,
					_("_Cancel"), GTK_RESPONSE_CANCEL,
					_("_Open"), GTK_RESPONSE_ACCEPT, NULL));

	GtkFileFilter *filter = gtk_file_filter_new ();
	gtk_file_filter_set_name (filter, _("plist files"));
	//gtk_file_filter_add_mime_type (filter, "application/xml");
	gtk_file_filter_add_mime_type (filter, "application/x-plist");
	gtk_file_filter_add_pattern (filter, "*.plist");

	gtk_file_chooser_add_filter (dialog, filter);
	gtk_file_chooser_set_select_multiple (dialog, TRUE);

	int result = gtk_dialog_run (GTK_DIALOG (dialog));
	if (result == GTK_RESPONSE_ACCEPT)
	{
		GSList *files = gtk_file_chooser_get_files (dialog);
		for (GSList *l = files; l != NULL; l = g_slist_next (l))
		{
			PvAppwin *win;
			GFile *file = G_FILE(l->data);

			win = find_window_to_open (app, file);
			if (win != NULL)
			{
				pv_appwin_set_file (win, file);
				gtk_window_present (GTK_WINDOW (win));
				g_object_unref (file);
			}
		}

		g_slist_free (files);
	}
	
	gtk_widget_destroy (GTK_WIDGET(dialog));
}

static void
pv_app_quit (GSimpleAction *action, GVariant *param, gpointer userdata)
{
	g_application_quit (G_APPLICATION(userdata));
}

static GActionEntry app_entries[] = {
	{ .name = "about", .activate = pv_app_about },
	{ .name = "quit", .activate = pv_app_quit },
	{ .name = "open", .activate = pv_app_openfile }
};

static void
pv_app_activate (GApplication *app)
{
	GtkWindow *win;

	win = GTK_WINDOW(pv_appwin_new (PV_APP(app)));
	gtk_window_present (win);

	pv_app_openfile (NULL, NULL, app);
}

static void
pv_app_open (GApplication *app, GFile **files, gint n_files, const gchar *hint)
{
	PvAppwin *win;

	for (int i = 0; i < n_files; ++i)
	{
		win = pv_appwin_new (PV_APP(app));
		pv_appwin_set_file (win, files[i]);
		gtk_window_present (GTK_WINDOW(win));
	}
}

static void
pv_app_class_init (PvAppClass *klass)
{
	GApplicationClass *application_class = G_APPLICATION_CLASS (klass);

	application_class->activate = pv_app_activate;
	application_class->open = pv_app_open;
}

static void
pv_app_init (PvApp *app)
{
	g_action_map_add_action_entries (G_ACTION_MAP(app), app_entries, G_N_ELEMENTS(app_entries), app);	
}

PvApp *
pv_app_new (void)
{
	g_set_application_name (_("Plist Viewer"));

	return g_object_new (pv_app_get_type (),
                            "application-id", "se.tingping.plist-viewer",
                            "flags", G_APPLICATION_HANDLES_OPEN,
                            NULL);
}
