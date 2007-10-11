/*
  Copyright (C) 2007 Gustavo Iñiguez Goya

  gnome_inm is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
#include "preferences.h"

void 		create_preferences_win ( AppletData *applet_data )
{
	GnomeVFSResult result;
	int size=0;
	char **buf;
	char **tokens=0;
	GtkWidget *win;
	GtkWidget *loc_lbl;
	GtkWidget *cmd_ok, *cmd_cancel;
	GtkWidget *table6;
	GladeXML *xml;
	GList *dir_list;
	GnomeVFSFileInfo *dir_info;
	int i=0, days=0;

	xml = glade_xml_new (PACKAGE_DIR"/gnome-inm-glade.glade", "win_prefs", NULL);
	applet_data->prefs->win = win = glade_xml_get_widget (xml, "win_prefs");
	applet_data->prefs->entry_code = glade_xml_get_widget (xml, "entry_code");
	applet_data->prefs->spin_interval = glade_xml_get_widget (xml, "spin_interval");
	applet_data->prefs->spin_days = glade_xml_get_widget (xml, "spin_days");
	table6 = glade_xml_get_widget (xml, "table6");
	applet_data->prefs->entry_cmpt = gtk_entry_completion_new ();
	gtk_entry_set_completion (GTK_ENTRY(applet_data->prefs->entry_code), GTK_ENTRY_COMPLETION (applet_data->prefs->entry_cmpt));

	// Create the EntryCompletion list
	applet_data->prefs->list_store = gtk_list_store_new (2, G_TYPE_STRING, G_TYPE_STRING);

	result = gnome_vfs_read_entire_file (PACKAGE_DIR"/local_codes.txt", &size, &buf);
	if (result == GNOME_VFS_OK){
		tokens = g_strsplit_set ((const char *)buf, "\|\n", -1);
		int x=0;
		while (tokens[x] && strcmp(tokens[x], "") != 0){
			//printf ("t1: %s t2: %s\n", tokens[x], tokens[x+1]);
			gtk_list_store_append (applet_data->prefs->list_store, &applet_data->prefs->iter);
			gtk_list_store_set (applet_data->prefs->list_store, &applet_data->prefs->iter, 0, tokens[x], -1);
			gtk_list_store_set (applet_data->prefs->list_store, &applet_data->prefs->iter, 1, tokens[x+1], -1);
			x+=2;
		}
	}
	
	g_strfreev (tokens);
	g_free (buf);
	
	gtk_entry_completion_set_model (GTK_ENTRY_COMPLETION(applet_data->prefs->entry_cmpt), GTK_TREE_MODEL(applet_data->prefs->list_store));
	gtk_entry_completion_set_text_column (applet_data->prefs->entry_cmpt, 1);

	applet_data->prefs->combo_theme = gtk_combo_box_entry_new_text ();
	gtk_table_attach (GTK_TABLE(table6), GTK_WIDGET(applet_data->prefs->combo_theme), 1, 2, 4, 5, 0, 0, 0, 0);
	loc_lbl = glade_xml_get_widget (xml, "location_lbl");
	//printf ("City saved: %s\n", (char *)panel_applet_gconf_get_string (PANEL_APPLET(applet_data->applet), "city", NULL));
	gtk_entry_set_text (GTK_ENTRY(applet_data->prefs->entry_code), (char *)panel_applet_gconf_get_string (PANEL_APPLET(applet_data->applet), "city", NULL));
	applet_data->prefs->code = (char *)panel_applet_gconf_get_string (PANEL_APPLET(applet_data->applet), "code", NULL);
	
	i = panel_applet_gconf_get_int (PANEL_APPLET(applet_data->applet), "interval", NULL);
	if (i == 0)
		i = 3600;

	applet_data->prefs->theme_entry = gtk_bin_get_child(GTK_BIN(applet_data->prefs->combo_theme));
	gtk_entry_set_text (GTK_ENTRY(applet_data->prefs->theme_entry), (char *)panel_applet_gconf_get_string (PANEL_APPLET(applet_data->applet), "theme", NULL));
	/* Load theme directories */
	gnome_vfs_directory_list_load (&dir_list, PIXMAPS_DIR, GNOME_VFS_FILE_INFO_DEFAULT);
	while (dir_list){
		dir_info = (GnomeVFSFileInfo *)dir_list->data;
		if (strcmp (dir_info->name, ".") != 0 && strcmp (dir_info->name, "..") != 0){
			//printf ("Theme: %s\n", (char *)dir_info->name);
			gtk_combo_box_append_text (GTK_COMBO_BOX(applet_data->prefs->combo_theme), dir_info->name);
		}

		dir_list = dir_list->next;
	}
	g_list_free (dir_list);
	
	days = (int)g_ascii_strtod (panel_applet_gconf_get_string (PANEL_APPLET(applet_data->applet), "days", NULL), NULL);
	if (days == 0)
		days = 7;
	gtk_spin_button_set_value (GTK_SPIN_BUTTON(applet_data->prefs->spin_interval), (int)i);
	gtk_spin_button_set_value (GTK_SPIN_BUTTON(applet_data->prefs->spin_days), (int)days);
	
	gtk_label_set_text (GTK_LABEL(loc_lbl), applet_data->city_long_desc);

	cmd_ok = glade_xml_get_widget (xml, "cmd_ok");
	cmd_cancel = glade_xml_get_widget (xml, "cmd_cancel");
	g_signal_connect (G_OBJECT(win), "destroy", G_CALLBACK(on_preferences_destroy), applet_data->prefs);
	g_signal_connect (G_OBJECT(cmd_ok), "clicked", G_CALLBACK(on_cmd_ok_clicked), applet_data);
	g_signal_connect (G_OBJECT(cmd_cancel), "clicked", G_CALLBACK(on_cmd_cancel_clicked), applet_data->prefs);
	g_signal_connect (G_OBJECT(applet_data->prefs->entry_code), "changed", G_CALLBACK(location_entry_changed), applet_data);
	g_signal_connect (G_OBJECT(applet_data->prefs->entry_code), "key-press-event", G_CALLBACK(location_entry_event), applet_data);
	g_signal_connect (G_OBJECT(applet_data->prefs->entry_code), "button-press-event", G_CALLBACK(location_entry_event), applet_data);
	gtk_widget_show_all (win);
	
	g_object_unref (G_OBJECT (xml));
}

void 		on_preferences_destroy ( GtkWidget *widget, PrefsWin *prefs )
{
	printf ("Destroying object preferences\n");
	gtk_entry_completion_set_model (prefs->entry_cmpt, NULL);
	g_object_unref (G_OBJECT (prefs->list_store));
	printf ("1\n");
	g_object_unref (G_OBJECT (prefs->entry_cmpt));
	printf ("2\n");
	gtk_widget_destroy (GTK_WIDGET(prefs->combo_theme));
	printf ("3\n");
	if (widget){
		quit (widget);
	}
}

void		on_cmd_cancel_clicked ( GtkWidget *widget, PrefsWin *prefs )
{
	printf ("cmd cancel\n");
	gtk_entry_completion_set_model (prefs->entry_cmpt, NULL);
	g_object_unref (G_OBJECT (prefs->list_store));
	g_object_unref (G_OBJECT (prefs->entry_cmpt));
	gtk_widget_destroy (GTK_WIDGET(prefs->combo_theme));
	if (prefs->win){
		quit (prefs->win);
	}
}

void		on_cmd_ok_clicked ( GtkWidget *widget, AppletData *applet_data )
{
	if (applet_data){
		applet_data->interval = (int)gtk_spin_button_get_value (GTK_SPIN_BUTTON(applet_data->prefs->spin_interval));
		panel_applet_gconf_set_string (PANEL_APPLET(applet_data->applet), "code", applet_data->prefs->code, NULL);
		panel_applet_gconf_set_string (PANEL_APPLET(applet_data->applet), "city", gtk_entry_get_text (GTK_ENTRY(applet_data->prefs->entry_code)), NULL);
		panel_applet_gconf_set_int (PANEL_APPLET(applet_data->applet), "interval", applet_data->interval, NULL);

		snprintf (applet_data->show_days, 12, "%d", (int)gtk_spin_button_get_value (GTK_SPIN_BUTTON(applet_data->prefs->spin_days)));
		panel_applet_gconf_set_string (PANEL_APPLET(applet_data->applet), "days", applet_data->show_days, NULL);
		
		//printf ("cmd ok\nCode: %s\n", applet_data->prefs->code);
		//printf ("Theme: %s\n", gtk_combo_box_get_active_text (GTK_COMBO_BOX(applet_data->prefs->combo_theme)));
		panel_applet_gconf_set_string (PANEL_APPLET(applet_data->applet), "theme", gtk_combo_box_get_active_text (GTK_COMBO_BOX(applet_data->prefs->combo_theme)), NULL);
		update_location (applet_data);

		/* Clean not needed data */
		gtk_entry_completion_set_model (applet_data->prefs->entry_cmpt, NULL);
		g_object_unref (G_OBJECT (applet_data->prefs->list_store));
		g_object_unref (G_OBJECT (applet_data->prefs->entry_cmpt));
		gtk_widget_destroy (GTK_WIDGET(applet_data->prefs->combo_theme));
		quit (applet_data->prefs->win);
	}
}

void 		location_entry_changed ( GtkWidget *widget, AppletData *applet_data )
{
	find_location_code (widget, applet_data);
}

gboolean 	location_entry_event ( GtkWidget *widget, GdkEventKey *event, AppletData *applet_data )
{
	return find_location_code (widget, applet_data);
}

gboolean 	find_location_code ( GtkWidget *widget, AppletData *applet_data )
{
	GtkTreeIter iter;
	gchar *code;
	gchar *city;
	gboolean valid;
	valid = gtk_tree_model_get_iter_first (GTK_TREE_MODEL(applet_data->prefs->list_store), &iter);
	
	while (valid){
		gtk_tree_model_get (GTK_TREE_MODEL(applet_data->prefs->list_store), &iter, 1, &city, -1);
		if (city && strcasecmp (city, gtk_entry_get_text (GTK_ENTRY(applet_data->prefs->entry_code))) == 0){
			gtk_tree_model_get (GTK_TREE_MODEL(applet_data->prefs->list_store), &iter, 0, &code, -1);
			strncpy (applet_data->prefs->code, code, 5);
			//printf ("Code for %s: %s\n", city, code);
			g_free (code);
			g_free (city);
			break;
		}
		g_free (city);
		valid = gtk_tree_model_iter_next (GTK_TREE_MODEL(applet_data->prefs->list_store), &iter);
	}

	return FALSE;
}
