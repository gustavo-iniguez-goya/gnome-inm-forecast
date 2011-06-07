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

#include "connection.h"


void check_inm_url_close		( GnomeVFSAsyncHandle *handle, GnomeVFSResult result, gpointer callback_data)
{
	int x=0;
	AppletData *applet_data = (AppletData *)callback_data;
		if (applet_data->buffer){
			//printf ("BUFFER: %s\n", applet_data->buffer);
			
			if (strstr (applet_data->buffer, "localidades.xsd"))
				parse_xml_data (PANEL_APPLET(applet_data->applet), applet_data, applet_data->buffer);
		
		g_free (applet_data->buffer);
		applet_data->buffer = 0;
		}
	
}

void check_inm_url_read 		( GnomeVFSAsyncHandle *handle,
						GnomeVFSResult result,
						gpointer buffer,
						GnomeVFSFileSize bytes_requested,
						GnomeVFSFileSize bytes_read,
						gpointer callback_data)
{
	AppletData *applet_data = (AppletData *) callback_data;
	gchar *buf, *temp=0;
	buf = (gchar *)buffer;
	buf[bytes_read] = '\0';

	if (applet_data->buffer == NULL)
		applet_data->buffer = g_strdup(buf);
	else{
		temp = g_strdup (applet_data->buffer);
		g_free (applet_data->buffer);
		applet_data->buffer = NULL;
		applet_data->buffer = g_strdup_printf ("%s%s", temp, buf);
		g_free (temp);
		temp = NULL;
	}

	//printf ("gvfs_async_read(): %s\n", gnome_vfs_result_to_string (result));
	if (result == GNOME_VFS_ERROR_EOF){
//		printf ("Fin de lectura gnome_vfs_async_read()\n");
		gnome_vfs_async_close (handle, check_inm_url_close, applet_data);
		if (buf){
			g_free (buf);
			buf = 0;
		}
	}
	else if (result != GNOME_VFS_OK){
		applet_data->gvfs_handle = NULL;
		printf ("Error de lectura gnome_vfs_asyncred()\n");
		//message_box (applet_data, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, _("Error gnome vfs (async read)"), gnome_vfs_result_to_string (result));
		gnome_vfs_async_close (handle, check_inm_url_close, applet_data);
		unset_images (applet_data);
		if (buf)
			g_free (buf);

		buf = 0;
	}
	else{	
		gnome_vfs_async_read (handle, buf, 8192, check_inm_url_read, applet_data);
	}
}

void check_inm_url_status 	( GnomeVFSAsyncHandle *handle,
						GnomeVFSResult result,
						gpointer callback_data)
{
	char *buf=0;
	buf = g_malloc0(8192);
	AppletData *applet_data = (AppletData *) callback_data;
	printf ("check_inm_url_status() gvfs_async_open(): %s\n", gnome_vfs_result_to_string (result));

	if (applet_data->buffer){
		g_free (applet_data->buffer);
		applet_data->buffer = NULL;
	}
	

	if (result != GNOME_VFS_OK){
		printf ("Conexion no abierta: %s - Code: %s\n", gnome_vfs_result_to_string (result), applet_data->prefs->code);
		//message_box (applet_data, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, _("Connection error"), gnome_vfs_result_to_string (result));
		unset_images (applet_data);
		applet_data->gvfs_handle = NULL;
		applet_data->buffer = NULL;
	}
	else{
		gnome_vfs_async_read (handle, buf, 8192, check_inm_url_read, applet_data);
//		printf ("gvfs_async_status()\n");
	}
}


/* Asynchronous calls to download weather reports */

void gvfs_close_cb		( GnomeVFSAsyncHandle *handle, GnomeVFSResult result, gpointer callback_data)
{
	GladeXML *xml;
	GtkWidget *textview;
	GtkWidget *win;
	GtkTextBuffer *textview_buffer;
	char **tokens=0;
	char *temp=0, *temp2=0, *temp3=0;
	//char *str_regx=0;
	AppletData *applet_data = (AppletData *) callback_data;
	//gchar *buf = applet_data->buffer;
	int x=0, i=0;
	//GRegex *regx = g_regex_new ("<.*>", G_REGEX_CASELESS | G_REGEX_RAW , G_REGEX_MATCH_NOTEMPTY, NULL);

	temp = g_strdup (applet_data->buffer);
	if (strstr(temp, "texto_entradilla") || 
			strstr(temp, "texto_normal2") || 
			strstr(temp, "Informaci&oacute;n nivol&oacute;gica") ||
			strstr(temp, "ZONAS NO PROTEGIDAS") ||
			strstr(temp, "ESTABILIDAD DEL MANTO") || strstr (temp, "RIESGO DE ALUDES")){
		xml = glade_xml_new (PACKAGE_DIR"/gnome-inm-glade.glade", "win_today_forecast", NULL);
		win = glade_xml_get_widget (xml, "win_today_forecast");
		g_signal_connect_swapped (G_OBJECT(win), "destroy", G_CALLBACK(quit), win);
		
		gtk_window_set_icon_from_file ((GtkWindow *)win, "/usr/share/pixmaps/"PACKAGE".png", NULL);
		textview = glade_xml_get_widget (xml, "textview");
		textview_buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW(textview));
		gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW(textview), GTK_WRAP_WORD_CHAR);
		
		if (temp){
			if (strstr(temp, "texto_entradilla"))
				tokens = g_strsplit_set (strstr(temp, "texto_entradilla"), "<>", 2);
			else if (strstr(temp, "texto_normal2"))
				tokens = g_strsplit_set (strstr(temp, "texto_normal2"), "<>", 2);

			if (tokens){
				//printf ("REGEX:\n\n%s\n\nREGEX\n", g_regex_replace (regx, tokens[0], -1, 0, "", G_REGEX_MATCH_NOTEMPTY, NULL));

				temp2 = g_strdup (tokens[1]);
				if (!temp2)
					temp2 = g_strdup (tokens[0]);

				for (x=0;x < strlen(temp2);x++){
					if (temp2[x] && temp2[x] == '\t'){
						temp2[x] = ' ';
					}
					// break the loop here and now if we found '<form'
					if (temp2[x] && temp2[x+1] && temp2[x+2] && temp2[x+3] &&
						 temp2[x] == '<' && temp2[x+1] == 'f' && temp2[x+2] == 'o' && temp2[x+3] == 'r'){
						temp2[x] = '\0';
						break;
					}
					// clean up 
					if (temp2[x] && temp2[x] == '<'){
						for (i=x;i < strlen(temp2);i++){
							if (temp2[i] && temp2[i] == '>'){
								temp2[i] = ' ';
								break;
							}
							if ((temp2[i] &&  temp2[i] == 'b' && temp2[i+1] == 'r') ||
								(temp2[i] &&  temp2[i] == 'R' && temp2[i+1] == 'R')){
								temp2[i] = '\r';
								temp2[i+1] = '\n';
							}else
							// break the loop here and now if we found '="disclaimer"'
							if (temp2[x] && temp2[x+1] && temp2[x+2] && temp2[x+3] && temp2[x+4] && temp2[x+5] &&
								temp2[x+6] && temp2[x+7] && temp2[x+8] && temp2[x+9] &&temp2[x+10] &&temp2[x+11] && temp2[x+12] &&
								temp2[x] == '=' && temp2[x+1] == '"' && temp2[x+2] == 'd' && temp2[x+3] == 'i' &&
								temp2[x+4] == 's' && temp2[x+5] == 'c' && temp2[x+6] == 'l' && temp2[x+7] == 'a' &&
								temp2[x+8] == 'i' && temp2[x+9] == 'm' && temp2[x+10] == 'e' && temp2[x+11] == 'r' && temp2[x+12] == '"'){
								temp2[x] = '\0';
								break;
							}else
							if (temp2[i])
								temp2[i] = ' ';
						x++;
						}
					}
				}
				//str_regx = g_regex_replace (regx, tokens[0], -1, 0, "", G_REGEX_MATCH_NOTEMPTY, NULL);
				//str_regx = tokens[0];
			
				temp3 = convert_str_to_utf8 (temp2);
//				printf ("FOREEEEEEEEEEEEE:\n%s\n", temp3);
				gtk_text_buffer_set_text (textview_buffer, temp3, strlen(temp3));
				gtk_window_set_title (GTK_WINDOW(win), _("Next days forecast"));
				gtk_widget_show (win);
				g_strfreev (tokens);
				tokens = 0;
				//if (str_regx)
				//	g_free (str_regx);
			}
			else{
				temp2 = convert_str_to_utf8 (temp);
				gtk_text_buffer_set_text (textview_buffer, temp2, strlen(temp2));
				gtk_window_set_title (GTK_WINDOW(win), _("Snowfalls reports"));
				gtk_widget_show (win);
			}
		}
		else{
			gtk_text_buffer_set_text (textview_buffer, _("No data"), 10);
			gtk_window_set_title (GTK_WINDOW(win), _("No data"));
			gtk_widget_show (win);
		}
		g_object_unref (G_OBJECT(xml));
	}
	else{
		gtk_text_buffer_set_text (textview_buffer, _("No data"), 10);
		gtk_window_set_title (GTK_WINDOW(win), _("No data"));
		gtk_widget_show (win);
	}
	//g_regex_unref (regx);
	if (temp){
		g_free (temp);
		temp = 0;
	}
	if (temp2){
		g_free (temp2);
		temp2 = 0;
	}
	if (temp3){
		g_free (temp3);
		temp3 = 0;
	}
	if (applet_data->buffer){
	
		//save_forecast_report (applet_data->buffer, SNOWFALLS_1);

		g_free (applet_data->buffer);
		applet_data->buffer = NULL;
	}
}

void gvfs_read_cb 		( GnomeVFSAsyncHandle *handle,
						GnomeVFSResult result,
						gpointer buffer,
						GnomeVFSFileSize bytes_requested,
						GnomeVFSFileSize bytes_read,
						gpointer callback_data)
{
	AppletData *applet_data = (AppletData *) callback_data;
	gchar *buf=0, *temp=0;
	buf = (gchar *)buffer;
	buf[bytes_read] = '\0';
	//printf ("Buf: %s\n", buf);
	
	if (applet_data->buffer == NULL)
		applet_data->buffer = g_strdup(buf);
	else{
		temp = g_strdup (applet_data->buffer);
		g_free (applet_data->buffer);
		applet_data->buffer = NULL;
		applet_data->buffer = g_strdup_printf ("%s%s", temp, buf);
		g_free (temp);
		temp = 0;
	}


	//printf ("gvfs_async_read(): %s\n", gnome_vfs_result_to_string (result));
	if (result == GNOME_VFS_ERROR_EOF){
		printf ("Fin de lectura gnome_vfs_async_read()\n");
		if (strstr(applet_data->buffer, "texto_entradilla") || 
				strstr(applet_data->buffer, "texto_normal2") ||
				strstr(applet_data->buffer, "PREDICCION") || 
				strstr(applet_data->buffer, "Informaci&oacute;n nivol&oacute;gica") ||
				strstr(applet_data->buffer, "ZONAS NO PROTEGIDAS") ||
				strstr(applet_data->buffer, "ESTABILIDAD DEL MANTO") ||
				strstr(applet_data->buffer, "RIESGO DE ALUDES")){
			gnome_vfs_async_close (handle, gvfs_close_cb, applet_data);
		}
		else{
			printf ("1 No forecast data\n");
		}
		if (buf){
			g_free (buf);
			buf = 0;
		}
	}
	else if (result != GNOME_VFS_OK){
		applet_data->gvfs_handle = NULL;
		printf ("Error de lectura gnome_vfs_async_read(): %s\n", gnome_vfs_result_to_string (result));
		if (applet_data->buffer){
			g_free (applet_data->buffer);
			applet_data->buffer = NULL;
		}
		if (buf){
			g_free (buf);
			buf = 0;
		}
	}
	else{	
		//printf ("\n\n\n======================\n%s\n==================\n\n\n", (char *)buf);
			
		gnome_vfs_async_read (handle, buf, 16384, gvfs_read_cb, applet_data);
	}
}

void gvfs_status_cb			( GnomeVFSAsyncHandle *handle,
						GnomeVFSResult result,
						gpointer callback_data)
{
	AppletData *applet_data = (AppletData *) callback_data;
	char *buf=0;
	buf = (char *)malloc(8192);

	printf ("gvfs_status_cb() gvfs_async_open(): %s\n", gnome_vfs_result_to_string (result));
	if (result != GNOME_VFS_OK){
		printf (_("Connection error\n"));
		//message_box (applet_data, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, _("Connection error"), gnome_vfs_result_to_string (result));
		unset_images (applet_data);
		applet_data->gvfs_handle = NULL;
		g_free (buf);
	}
	else{
		gnome_vfs_async_read (handle, buf, 8192, gvfs_read_cb, applet_data);
		printf ("gvfs_async_status()\n");
	}

}
