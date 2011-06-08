/*
  Copyright (C) 2007-2009 Gustavo Iñiguez Goya

  gnome-inm-forecast is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software Foundation, 
  Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
*/
#include "preferences.h"
#include <string.h>

void 		create_preferences_win ( AppletData *applet_data )
{
	GnomeVFSResult result;
	int size=0;
	char **buf;
	char **tokens=0;
	GtkWidget *loc_lbl=0;
	GtkWidget *cmd_ok=0, *cmd_cancel=0;
	GtkWidget *table6=0;
	GladeXML *xml=0;
	GList *dir_list;
	GnomeVFSFileInfo *dir_info;
	int i=0, days=0;
	GtkTreeIter iter;
	char *tmp=0;

	xml = glade_xml_new (PACKAGE_DIR"/gnome-inm-glade.glade", "win_prefs", NULL);
	applet_data->prefs->win = glade_xml_get_widget (xml, "win_prefs");
	applet_data->prefs->entry_code = glade_xml_get_widget (xml, "entry_code");
	applet_data->prefs->spin_interval = glade_xml_get_widget (xml, "spin_interval");
	applet_data->prefs->spin_days = glade_xml_get_widget (xml, "spin_days");
	//applet_data->prefs->tree_prov = glade_xml_get_widget (xml, "tree_prov");
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
			gtk_list_store_append (applet_data->prefs->list_store, &iter);
			gtk_list_store_set (applet_data->prefs->list_store, &iter, 0, tokens[x], -1);
			gtk_list_store_set (applet_data->prefs->list_store, &iter, 1, tokens[x+1], -1);
			x+=2;
		}
		g_strfreev (tokens);
		tokens = 0;
	}
	
	if (buf){
		g_free (buf);
		buf = 0;
	}
	//gtk_tree_iter_free (&iter);
	
	gtk_entry_completion_set_model (GTK_ENTRY_COMPLETION(applet_data->prefs->entry_cmpt), GTK_TREE_MODEL(applet_data->prefs->list_store));
	gtk_entry_completion_set_text_column (applet_data->prefs->entry_cmpt, 1);

	/* Creates the meteorological stations tree */
	//create_stations_tree (applet_data);

	applet_data->prefs->combo_theme = gtk_combo_box_entry_new_text ();
	gtk_table_attach (GTK_TABLE(table6), GTK_WIDGET(applet_data->prefs->combo_theme), 1, 2, 4, 5, 0, 0, 0, 0);
	loc_lbl = glade_xml_get_widget (xml, "location_lbl");
	//printf ("City saved: %s\n", (char *)panel_applet_gconf_get_string (PANEL_APPLET(applet_data->applet), "city", NULL));

	tmp = (char *)panel_applet_gconf_get_string (PANEL_APPLET(applet_data->applet), "city", NULL);
	gtk_entry_set_text (GTK_ENTRY(applet_data->prefs->entry_code), tmp);
	g_strlcpy (applet_data->prefs->code, (char *)panel_applet_gconf_get_string (PANEL_APPLET(applet_data->applet), "code", NULL), 12);

	if (tmp){
		g_free (tmp);
		tmp = NULL;
	}
	
	//gtk_entry_set_text (GTK_ENTRY(applet_data->prefs->prov_search_entry), (char *)panel_applet_gconf_get_string (PANEL_APPLET(applet_data->applet), "station_name", NULL));
	//applet_data->prefs->station_code = (char *)panel_applet_gconf_get_string (PANEL_APPLET(applet_data->applet), "station_code", NULL);
	//gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(applet_data->prefs->chk_station), 
	//		panel_applet_gconf_get_bool (PANEL_APPLET(applet_data->applet), "show_station", NULL)
	//	);
	
	i = panel_applet_gconf_get_int (PANEL_APPLET(applet_data->applet), "interval", NULL);
	if (i < 1 || i > 60)
		i = 60;

	applet_data->prefs->theme_entry = gtk_bin_get_child(GTK_BIN(applet_data->prefs->combo_theme));

	tmp = (char *)panel_applet_gconf_get_string (PANEL_APPLET(applet_data->applet), "theme", NULL);
	gtk_entry_set_text (GTK_ENTRY(applet_data->prefs->theme_entry), tmp);
	if (tmp){
		g_free (tmp);
		tmp = NULL;
	}

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
	
	tmp = (char *)panel_applet_gconf_get_string (PANEL_APPLET(applet_data->applet), "days", NULL);
	days = (int)g_ascii_strtod (tmp, NULL);
	if (days == 0)
		days = 10;

	if (tmp){
		g_free (tmp);
		tmp = NULL;
	}

	gtk_spin_button_set_value (GTK_SPIN_BUTTON(applet_data->prefs->spin_interval), (int)i);
	gtk_spin_button_set_value (GTK_SPIN_BUTTON(applet_data->prefs->spin_days), (int)days);
	
	gtk_label_set_text (GTK_LABEL(loc_lbl), applet_data->city_long_desc);

	gtk_window_set_icon_from_file ((GtkWindow *)applet_data->prefs->win, "/usr/share/pixmaps/"PACKAGE".png", NULL);
	cmd_ok = glade_xml_get_widget (xml, "cmd_ok");
	cmd_cancel = glade_xml_get_widget (xml, "cmd_cancel");
	g_signal_connect (G_OBJECT(applet_data->prefs->win), "destroy", G_CALLBACK(gtk_widget_destroyed), &applet_data->prefs->win);
	//g_signal_connect (G_OBJECT(applet_data->prefs->win), "destroy", G_CALLBACK(on_preferences_destroy), applet_data);
	g_signal_connect (G_OBJECT(cmd_ok), "clicked", G_CALLBACK(on_cmd_ok_clicked), applet_data);
	g_signal_connect (G_OBJECT(cmd_cancel), "clicked", G_CALLBACK(on_cmd_cancel_clicked), applet_data);
	g_signal_connect (G_OBJECT(applet_data->prefs->entry_code), "changed", G_CALLBACK(location_entry_changed), applet_data);
	g_signal_connect (G_OBJECT(applet_data->prefs->prov_search_entry), "changed", G_CALLBACK(station_entry_changed), applet_data);
	
	g_signal_connect (G_OBJECT(applet_data->prefs->entry_code), "key-press-event", G_CALLBACK(location_entry_event), applet_data);
	g_signal_connect (G_OBJECT(applet_data->prefs->entry_code), "button-press-event", G_CALLBACK(location_entry_event), applet_data);

	g_signal_connect (G_OBJECT(applet_data->prefs->prov_search_entry), "key-press-event", G_CALLBACK(station_entry_event), applet_data);
	g_signal_connect (G_OBJECT(applet_data->prefs->prov_search_entry), "button-press-event", G_CALLBACK(station_entry_event), applet_data);
	
	//g_signal_connect (G_OBJECT(applet_data->prefs->tree_prov), "row-activated", G_CALLBACK(on_station_tree_selection), applet_data);
	gtk_widget_show_all (applet_data->prefs->win);
	
	g_object_unref (G_OBJECT (xml));
}

/*
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
				if (strcmp (tokens[x+2], str_prov) != 0){
					gtk_tree_store_append(applet_data->prefs->tree_store, &it, NULL);
					gtk_tree_store_set (applet_data->prefs->tree_store, &it, 0, tokens[x+2], -1); // Provincia
				}

				// Ciudad
				gtk_tree_store_append(applet_data->prefs->tree_store, &child, &it);
				gtk_tree_store_set (applet_data->prefs->tree_store, &child, 0, tokens[x+1], -1);

				// Codigo
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
*/










void 		on_preferences_destroy ( GtkWidget *widget, AppletData *applet_data )
{
	printf ("Destroying object preferences\n");
	close_prefs_window (applet_data->prefs);
}

void		on_cmd_cancel_clicked ( GtkWidget *widget, AppletData *applet_data )
{
	close_prefs_window (applet_data->prefs);
}

void		on_cmd_ok_clicked ( GtkWidget *widget, AppletData *applet_data )
{
	char *text=0;

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
		text = gtk_combo_box_get_active_text (GTK_COMBO_BOX(applet_data->prefs->combo_theme));
		panel_applet_gconf_set_string (PANEL_APPLET(applet_data->applet), "theme", text, NULL);
		g_snprintf (applet_data->theme, 254, "%s\0", text);
		g_free (text);

		update_location (applet_data);
		close_prefs_window (applet_data->prefs);
	}
}

/*
void 		on_station_tree_selection ( GtkTreeView *tree_view, GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data)
{
	GtkTreeSelection *selection;
	GtkTreeModel     *model;
	GtkTreeIter       iter;
	AppletData *applet_data = (AppletData *)user_data;

// This will only work in single or browse selection mode! 

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
*/

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
	gchar *code=0;
	gchar *city=0;
	gboolean valid;

	if (type == 0){
		valid = gtk_tree_model_get_iter_first (GTK_TREE_MODEL(applet_data->prefs->list_store), &iter);
	}
	else{
		valid = gtk_tree_model_get_iter_first (GTK_TREE_MODEL(applet_data->prefs->tree_store), &iter);
	}
	
	while (valid){
		if (type == 0){ // city EntryText changed
			gtk_tree_model_get (GTK_TREE_MODEL(applet_data->prefs->list_store), &iter, 1, &city, -1);
			if (city && strcasecmp (city, gtk_entry_get_text (GTK_ENTRY(applet_data->prefs->entry_code))) == 0){
				gtk_tree_model_get (GTK_TREE_MODEL(applet_data->prefs->list_store), &iter, 0, &code, -1);
				
				if (city && code){
					g_snprintf (applet_data->prefs->code, 12, "%s\0", code);
					//printf ("0-Code for (%d) \"%s\": %s\n", strlen(applet_data->prefs->code), applet_data->prefs->code, code);
				}

				g_free (city);
				g_free (code);

				break;
			}
		}
		else{
			gtk_tree_model_get (GTK_TREE_MODEL(applet_data->prefs->tree_store), &iter, 0, &city, -1);
			if (city && strcasecmp (city, gtk_entry_get_text (GTK_ENTRY(applet_data->prefs->prov_search_entry))) == 0){
				gtk_tree_model_get (GTK_TREE_MODEL(applet_data->prefs->tree_store), &iter, 1, &code, -1);

				if (city && code){
					g_snprintf (applet_data->prefs->code, 12, "%s\0", code);
				}
				
				g_free (code);
				g_free (city);

				break;
			}
		}

		if (type == 0)
			valid = gtk_tree_model_iter_next (GTK_TREE_MODEL(applet_data->prefs->list_store), &iter);
		else
			valid = gtk_tree_model_iter_next (GTK_TREE_MODEL(applet_data->prefs->tree_store), &iter);
	}

	return FALSE;
}

/*
gchar*		translate_city_to_url ( const char *str_city )
{
	int i=0,x=0;
	gchar* str_tmp=NULL;
	gchar* str_dup=NULL;
	gchar* tmp_buf=NULL;
	
	if (str_city && g_utf8_validate (str_city, -1, NULL)){
		str_dup = g_strdup (str_city);
		tmp_buf = g_convert (str_dup, -1, "ASCII//TRANSLIT", "UTF-8", 0, 0, NULL);
		str_tmp = g_ascii_strdown (tmp_buf, -1);

		for (i=0;i < strlen(str_tmp); i++){
			if (str_tmp[i] == '/'){
				str_tmp[i] = '-';
			}
			else if (str_tmp[i] == 0x20){
				str_tmp[i] = '-';
			}
			else if (str_tmp[i] == '.' || str_tmp[i] == '(' || str_tmp[i] == ')'){
				for (x=i; x < strlen(str_tmp)+1; x++){
					printf ("punto: %c | %c\n", str_tmp[x], str_tmp[x+1]);
					if (str_tmp[x+1])
						str_tmp[x] = str_tmp[x+1];
				}
				str_tmp[strlen(str_tmp)-1] = '\0';
				i--;
			}
			else if (str_tmp[i] == ','){
				str_tmp[i] = '-';
				for (x=i+1; x < strlen(str_tmp); x++){
					if (str_tmp[x+1])
						str_tmp[x] = str_tmp[x+1];
				}
				str_tmp[strlen(str_tmp)-1] = '\0';
			}
			else if (str_tmp[i] == '\''){
				str_tmp[i] = '-';
			}
			else if (str_tmp[i] == 'ç'){
				str_tmp[i] = '-';
			}
		}

		if (str_tmp[strlen(str_tmp)-1] == '-')
			str_tmp[strlen(str_tmp)-1] = '\0';

		//printf ("translate_city_to_url: %s - new -> %s\n", str_city, str_tmp);

		g_free (str_dup);
		g_free (tmp_buf);
		
		return str_tmp;
	}

	return NULL;
}
*/

void 		close_prefs_window ( PrefsWin *prefs )
{
	/* Clean not needed data */

	//gtk_tree_view_set_model (GTK_TREE_VIEW(applet_data->prefs->tree_prov), NULL);
	//g_object_unref (G_OBJECT (applet_data->prefs->tree_store));
	
	printf ("Freeing memory: entry_cmpt\n");
	gtk_entry_completion_set_model (prefs->entry_cmpt, NULL);
	g_object_unref (G_OBJECT (prefs->entry_cmpt));

	printf ("Freeing memory: list_store\n");
	gtk_list_store_clear (GTK_LIST_STORE(prefs->list_store));
	g_object_unref (G_OBJECT (prefs->list_store));

	printf ("Freeing memory: combo_theme\n");
	if (prefs->combo_theme)
		gtk_widget_destroy (GTK_WIDGET(prefs->combo_theme));
	
	printf ("Freeing memory: common widgets\n");
	gtk_widget_destroy (GTK_WIDGET (prefs->entry_code));

	if (prefs->win != 0x0)
		quit (prefs->win);
}
