/*
  Copyright (C) 2007 Gustavo Iñiguez Goya

  gnome-inm-forecast is free software; you can redistribute it and/or modify
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
#include <string.h>

void 		create_preferences_win ( AppletData *applet_data )
{
	GnomeVFSResult result;
	int size=0;
	char **buf;
	char **tokens=0;
	GtkWidget *win=0;
	GtkWidget *loc_lbl=0;
	GtkWidget *cmd_ok=0, *cmd_cancel=0;
	GtkWidget *table6=0;
	GladeXML *xml=0;
	GList *dir_list;
	GnomeVFSFileInfo *dir_info;
	int i=0, days=0;

	xml = glade_xml_new (PACKAGE_DIR"/gnome-inm-glade.glade", "win_prefs", NULL);
	applet_data->prefs->win = win = glade_xml_get_widget (xml, "win_prefs");
	applet_data->prefs->entry_code = glade_xml_get_widget (xml, "entry_code");
	applet_data->prefs->spin_interval = glade_xml_get_widget (xml, "spin_interval");
	applet_data->prefs->spin_days = glade_xml_get_widget (xml, "spin_days");
	applet_data->prefs->tree_prov = glade_xml_get_widget (xml, "tree_prov");
	applet_data->prefs->chk_station = glade_xml_get_widget (xml, "chk_station");
	applet_data->prefs->prov_search_entry = glade_xml_get_widget (xml, "prov_search_entry");
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
	tokens = 0;
	if (buf){
		g_free (buf);
		buf = 0;
	}
	
	gtk_entry_completion_set_model (GTK_ENTRY_COMPLETION(applet_data->prefs->entry_cmpt), GTK_TREE_MODEL(applet_data->prefs->list_store));
	gtk_entry_completion_set_text_column (applet_data->prefs->entry_cmpt, 1);

	/* Creates the meteorological stations tree */
	//create_stations_tree (applet_data);

	applet_data->prefs->combo_theme = gtk_combo_box_entry_new_text ();
	gtk_table_attach (GTK_TABLE(table6), GTK_WIDGET(applet_data->prefs->combo_theme), 1, 2, 4, 5, 0, 0, 0, 0);
	loc_lbl = glade_xml_get_widget (xml, "location_lbl");
	//printf ("City saved: %s\n", (char *)panel_applet_gconf_get_string (PANEL_APPLET(applet_data->applet), "city", NULL));
	gtk_entry_set_text (GTK_ENTRY(applet_data->prefs->entry_code), (char *)panel_applet_gconf_get_string (PANEL_APPLET(applet_data->applet), "city", NULL));
	applet_data->prefs->code = (char *)panel_applet_gconf_get_string (PANEL_APPLET(applet_data->applet), "code", NULL);
	
	//gtk_entry_set_text (GTK_ENTRY(applet_data->prefs->prov_search_entry), (char *)panel_applet_gconf_get_string (PANEL_APPLET(applet_data->applet), "station_name", NULL));
	//applet_data->prefs->station_code = (char *)panel_applet_gconf_get_string (PANEL_APPLET(applet_data->applet), "station_code", NULL);
	//gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(applet_data->prefs->chk_station), 
	//		panel_applet_gconf_get_bool (PANEL_APPLET(applet_data->applet), "show_station", NULL)
	//	);
	
	i = panel_applet_gconf_get_int (PANEL_APPLET(applet_data->applet), "interval", NULL);
	if (i < 1 || i > 60)
		i = 60;

	applet_data->prefs->theme_entry = gtk_bin_get_child(GTK_BIN(applet_data->prefs->combo_theme));
	gtk_entry_set_text (GTK_ENTRY(applet_data->prefs->theme_entry), (char *)panel_applet_gconf_get_string (PANEL_APPLET(applet_data->applet), "theme", NULL));
	/* Load theme directories */
	gnome_vfs_directory_list_load (&dir_list, PIXMAPS_DIR, GNOME_VFS_FILE_INFO_DEFAULT);
	while (dir_list){
		dir_info = (GnomeVFSFileInfo *)dir_list->data;
		if (strcmp (dir_info->name, ".") != 0 && strcmp (dir_info->name, "..") != 0 && dir_info->type == GNOME_VFS_FILE_TYPE_DIRECTORY){
			//printf ("Theme: %s\n", (char *)dir_info->name);
			gtk_combo_box_append_text (GTK_COMBO_BOX(applet_data->prefs->combo_theme), dir_info->name);
		}

		dir_list = dir_list->next;
	}
	g_list_free (dir_list);
	dir_list = 0;
	
	days = (int)g_ascii_strtod (panel_applet_gconf_get_string (PANEL_APPLET(applet_data->applet), "days", NULL), NULL);
	if (days == 0)
		days = 10;
	gtk_spin_button_set_value (GTK_SPIN_BUTTON(applet_data->prefs->spin_interval), (int)i);
	gtk_spin_button_set_value (GTK_SPIN_BUTTON(applet_data->prefs->spin_days), (int)days);
	
	gtk_label_set_text (GTK_LABEL(loc_lbl), applet_data->city_long_desc);

	cmd_ok = glade_xml_get_widget (xml, "cmd_ok");
	cmd_cancel = glade_xml_get_widget (xml, "cmd_cancel");
	g_signal_connect (G_OBJECT(applet_data->prefs->win), "destroy", G_CALLBACK(gtk_widget_destroyed), &applet_data->prefs->win);
	g_signal_connect (G_OBJECT(win), "destroy", G_CALLBACK(on_preferences_destroy), applet_data);
	g_signal_connect (G_OBJECT(cmd_ok), "clicked", G_CALLBACK(on_cmd_ok_clicked), applet_data);
	g_signal_connect (G_OBJECT(cmd_cancel), "clicked", G_CALLBACK(on_cmd_cancel_clicked), applet_data);
	g_signal_connect (G_OBJECT(applet_data->prefs->entry_code), "changed", G_CALLBACK(location_entry_changed), applet_data);
	g_signal_connect (G_OBJECT(applet_data->prefs->prov_search_entry), "changed", G_CALLBACK(station_entry_changed), applet_data);
	
	g_signal_connect (G_OBJECT(applet_data->prefs->entry_code), "key-press-event", G_CALLBACK(location_entry_event), applet_data);
	g_signal_connect (G_OBJECT(applet_data->prefs->entry_code), "button-press-event", G_CALLBACK(location_entry_event), applet_data);

	g_signal_connect (G_OBJECT(applet_data->prefs->prov_search_entry), "key-press-event", G_CALLBACK(station_entry_event), applet_data);
	g_signal_connect (G_OBJECT(applet_data->prefs->prov_search_entry), "button-press-event", G_CALLBACK(station_entry_event), applet_data);
	
	g_signal_connect (G_OBJECT(applet_data->prefs->tree_prov), "row-activated", G_CALLBACK(on_station_tree_selection), applet_data);
	gtk_widget_show_all (win);
	
	g_object_unref (G_OBJECT (xml));
}
void 		create_stations_tree ( AppletData *applet_data )
{
	GnomeVFSResult result;
	GtkTreeIter it, child;
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *col;
	char **buf;
	char **tokens=0;
	int size=0;
	char *str_prov=0;
	
	str_prov = g_new (char, 24);
	applet_data->prefs->tree_store = gtk_tree_store_new (2, G_TYPE_STRING, G_TYPE_STRING);
	col = gtk_tree_view_column_new ();
	gtk_tree_view_column_set_title (col, _("Cities"));

	gtk_tree_view_append_column (GTK_TREE_VIEW (applet_data->prefs->tree_prov), col);
	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_column_pack_start (col, renderer, TRUE);
	gtk_tree_view_column_add_attribute (col, renderer, "text", 0);

	
	result = gnome_vfs_read_entire_file (PACKAGE_DIR"/stations_list_es.txt", &size, &buf);
	if (result == GNOME_VFS_OK){
		tokens = g_strsplit_set ((const char *)buf, "\|\n", -1);
		int x=0;
		while (tokens[x] && strcmp(tokens[x], "") != 0){
			if (tokens[x+1] && tokens[x+2]){
				//printf ("[%02d]: t1: %s t2: %s - PROV: %s\n", x, tokens[x], tokens[x+1], str_prov);
				if (strcmp (tokens[x+2], str_prov) != 0){
					gtk_tree_store_append(applet_data->prefs->tree_store, &it, NULL);
					gtk_tree_store_set (applet_data->prefs->tree_store, &it, 0, tokens[x+2], -1); /* Provincia */
				}
				
				/* Ciudad */
				gtk_tree_store_append(applet_data->prefs->tree_store, &child, &it);
				gtk_tree_store_set (applet_data->prefs->tree_store, &child, 0, tokens[x+1], -1);
				
				/* Codigo */
				gtk_tree_store_set (applet_data->prefs->tree_store, &child, 1, tokens[x], -1);
				strncpy (str_prov, tokens[x+2], 24);
			}
			x+=3;
		}
	}
	gtk_tree_view_set_model (GTK_TREE_VIEW(applet_data->prefs->tree_prov), GTK_TREE_MODEL(applet_data->prefs->tree_store));

	gtk_tree_view_expand_all (GTK_TREE_VIEW(applet_data->prefs->tree_prov));
	
	g_strfreev (tokens);
	if (buf){
		g_free (buf);
		buf = 0;
	}
	g_free (str_prov);	
	str_prov = 0;
}











void 		on_preferences_destroy ( GtkWidget *widget, AppletData *applet_data )
{
	printf ("Destroying object preferences\n");
	close_prefs_window (applet_data);
}

void		on_cmd_cancel_clicked ( GtkWidget *widget, AppletData *applet_data )
{
	printf ("cmd cancel\n");
	close_prefs_window (applet_data);
}

void		on_cmd_ok_clicked ( GtkWidget *widget, AppletData *applet_data )
{
	if (applet_data){
		applet_data->interval = (int)gtk_spin_button_get_value (GTK_SPIN_BUTTON(applet_data->prefs->spin_interval));
		panel_applet_gconf_set_string (PANEL_APPLET(applet_data->applet), "code", applet_data->prefs->code, NULL);
		panel_applet_gconf_set_string (PANEL_APPLET(applet_data->applet), "city", gtk_entry_get_text (GTK_ENTRY(applet_data->prefs->entry_code)), NULL);
		panel_applet_gconf_set_int (PANEL_APPLET(applet_data->applet), "interval", applet_data->interval, NULL);
		panel_applet_gconf_set_string (PANEL_APPLET(applet_data->applet), "station_name", gtk_entry_get_text (GTK_ENTRY(applet_data->prefs->prov_search_entry)), NULL);
		panel_applet_gconf_set_string (PANEL_APPLET(applet_data->applet), "station_code", applet_data->prefs->station_code, NULL);
		panel_applet_gconf_set_bool (PANEL_APPLET(applet_data->applet), "show_station", gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(applet_data->prefs->chk_station)), NULL);

		snprintf (applet_data->show_days, 12, "%d", (int)gtk_spin_button_get_value (GTK_SPIN_BUTTON(applet_data->prefs->spin_days)));
		panel_applet_gconf_set_string (PANEL_APPLET(applet_data->applet), "days", applet_data->show_days, NULL);
		
		//printf ("cmd ok\nCode: %s\n", applet_data->prefs->code);
		//printf ("Theme: %s\n", gtk_combo_box_get_active_text (GTK_COMBO_BOX(applet_data->prefs->combo_theme)));
		panel_applet_gconf_set_string (PANEL_APPLET(applet_data->applet), "theme", gtk_combo_box_get_active_text (GTK_COMBO_BOX(applet_data->prefs->combo_theme)), NULL);
		update_location (applet_data);
		close_prefs_window (applet_data);
	}
}

void 		on_station_tree_selection ( GtkTreeView *tree_view, GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data)
{
	GtkTreeSelection *selection;
	GtkTreeModel     *model;
	GtkTreeIter       iter;
	AppletData *applet_data = (AppletData *)user_data;

/* This will only work in single or browse selection mode! */

	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(applet_data->prefs->tree_prov));
	if (gtk_tree_selection_get_selected(selection, &model, &iter))
	{
		gchar *name, *code;
		gtk_tree_model_get (model, &iter, 0, &name, 1, &code, -1);
		//g_print ("selected row is: %s - %s\n", name, code);
		
		gtk_entry_set_text (GTK_ENTRY(applet_data->prefs->prov_search_entry), name);
		
		panel_applet_gconf_set_bool (PANEL_APPLET(applet_data->applet), "show_station", 
				gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(applet_data->prefs->chk_station)), 
				NULL);
		if (name){
			if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(applet_data->prefs->chk_station)))
				panel_applet_gconf_set_string (PANEL_APPLET(applet_data->applet), "station_name", name, NULL);

			g_free(name);
			name = 0;
		}
		if (code){
			if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(applet_data->prefs->chk_station))){
				panel_applet_gconf_set_string (PANEL_APPLET(applet_data->applet), "station_code", code, NULL);
				strncpy (applet_data->prefs->station_code, code, 12);
	
				//update_station_data (applet_data);
			}

			g_free(code);
			code = 0;
		}
	}
}

void 		location_entry_changed ( GtkWidget *widget, AppletData *applet_data )
{
	find_location_code (widget, applet_data, 0);
}

void 		station_entry_changed ( GtkWidget *widget, AppletData *applet_data )
{
	find_location_code (widget, applet_data, 1);
}

gboolean 	location_entry_event ( GtkWidget *widget, GdkEventKey *event, AppletData *applet_data )
{
	return find_location_code (widget, applet_data, 0);
}

gboolean 	station_entry_event ( GtkWidget *widget, GdkEventKey *event, AppletData *applet_data )
{
	return find_location_code (widget, applet_data, 1);
}

gboolean 	find_location_code ( GtkWidget *widget, AppletData *applet_data, int type )
{
	GtkTreeIter iter;
	gchar *code;
	gchar *city;
	gboolean valid;
	if (type == 0){
		valid = gtk_tree_model_get_iter_first (GTK_TREE_MODEL(applet_data->prefs->list_store), &iter);
	}
	else{
		valid = gtk_tree_model_get_iter_first (GTK_TREE_MODEL(applet_data->prefs->tree_store), &iter);
	}
	
	while (valid){
		if (type == 0){
			gtk_tree_model_get (GTK_TREE_MODEL(applet_data->prefs->list_store), &iter, 1, &city, -1);

			if (city && strcasecmp (city, gtk_entry_get_text (GTK_ENTRY(applet_data->prefs->entry_code))) == 0){
				gtk_tree_model_get (GTK_TREE_MODEL(applet_data->prefs->list_store), &iter, 0, &code, -1);
				strncpy (applet_data->prefs->code, code, 5);
				printf ("0-Code for %s: %s\n", city, code);
				if (code)
					g_free (code);
				
				if (city)
					g_free (city);
				break;
			}
		}
		else{
			gtk_tree_model_get (GTK_TREE_MODEL(applet_data->prefs->tree_store), &iter, 0, &city, -1);
			if (city && strcasecmp (city, gtk_entry_get_text (GTK_ENTRY(applet_data->prefs->prov_search_entry))) == 0){
				gtk_tree_model_get (GTK_TREE_MODEL(applet_data->prefs->tree_store), &iter, 1, &code, -1);
				if (code){
					strncpy (applet_data->prefs->code, code, 5);
					printf ("1-Code for %s: %s\n", city, code);
					g_free (code);
				}
				if (city)
					g_free (city);
				break;
			}
		}
		if (city)
			g_free (city);

		if (type == 0)
			valid = gtk_tree_model_iter_next (GTK_TREE_MODEL(applet_data->prefs->list_store), &iter);
		else
			valid = gtk_tree_model_iter_next (GTK_TREE_MODEL(applet_data->prefs->tree_store), &iter);
	}

	return FALSE;
}


void 		close_prefs_window ( AppletData *applet_data )
{
	/* Clean not needed data */
	gtk_entry_completion_set_model (applet_data->prefs->entry_cmpt, NULL);
	g_object_unref (G_OBJECT (applet_data->prefs->list_store));
	gtk_tree_view_set_model (GTK_TREE_VIEW(applet_data->prefs->tree_prov), NULL);
	g_object_unref (G_OBJECT (applet_data->prefs->tree_store));
	g_object_unref (G_OBJECT (applet_data->prefs->entry_cmpt));
	gtk_widget_destroy (GTK_WIDGET(applet_data->prefs->combo_theme));
	if (applet_data->prefs->win)
		quit (applet_data->prefs->win);
}
