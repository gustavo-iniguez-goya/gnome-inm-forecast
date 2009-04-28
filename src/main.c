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

#define _GNU_SOURCE
#include <string.h>
#include "callbacks.h"
#include "main.h"
#include "html_parser.h"
#include "connection.h"
#include "preferences.h"
#include <libxml/HTMLtree.h>
#include <libxml/HTMLparser.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

char *convert_str_to_utf8 	( char *string )
{
	char *bk_utf8=0;
	char *temp_buf=0;

	if (string){
		bk_utf8 = strdup (string);
		
		if (!g_utf8_validate (bk_utf8, -1, NULL))
			//strncpy (applet_data->city_name, string, 64);
		//else{
			temp_buf = g_locale_to_utf8(bk_utf8, -1, NULL, NULL, NULL);
			if (!temp_buf)
				//g_utf8_strncpy (applet_data->city_name, temp_buf, 64);
			//else{
				temp_buf = g_convert (bk_utf8, -1, "UTF-8", "ISO-8859-1", 0, 0, NULL);
				if (!temp_buf)
					//strncpy (applet_data->city_name, temp_buf, 64);
				//else
					strcpy (temp_buf, "");
		
		if (bk_utf8){
			g_free (bk_utf8);
			bk_utf8=0;
		}
		return temp_buf;
	}
	else
		return NULL;
}

/*
static void save_forecast_report	( const char *buf, int type )
{
	GnomeVFSHandle *ghandle=0;
	GDate *d;
	char filename[256]={0};
	char dirname[128]={0};
	char dir1[128]={0};
	int m_ret=0;
	GnomeVFSFileSize buf_len=0;
	GnomeVFSFileSize bytes_written;
	GnomeVFSResult gvfs_result;
	char *report_type[TOTAL_REPORTS] = {
			"snowfalls-pirineo-navarro-report",
			"snowfalls-pirineo-catalan-report",
			"today-report",
			"dayaftertomorrow-report",
			"nextdays-report",
			"picos-de-europa-report",
			"sierra-de-gredos-report",
			"sierra-de-guadarrama-report",
			"iberica-riojana-report",
			"iberica-aragonesa-report",
			"pirineo-navarro-report",
			"pirineo-catalan-report",
			"sierra-nevada-report"
	};

	d = g_date_new ();
	g_date_set_time_t (d, time(NULL));

	g_snprintf (dirname, 128, "/tmp/aemet/%d/%d\0", g_date_get_year(d), g_date_get_month(d));

	m_ret = g_mkdir_with_parents (dirname, 755);
	if (m_ret == -1)
		printf ("Error creando %s\n", dirname);

	g_snprintf (filename, 256, "%s/%s-%d%d%d.txt", dirname, report_type[type], g_date_get_day(d), g_date_get_month(d), g_date_get_year(d));
	printf ("filename: %s - %s\n", filename, report_type[type]);

	buf_len = strlen (buf);
	printf ("buffer length: %d\n", buf_len);

	gvfs_result = gnome_vfs_create (&ghandle, filename, GNOME_VFS_OPEN_WRITE, TRUE, 644);
	if (gvfs_result == GNOME_VFS_OK){
	
		gvfs_result = gnome_vfs_open (&ghandle, filename, GNOME_VFS_OPEN_WRITE);
		if (gvfs_result == GNOME_VFS_OK){
			if (gnome_vfs_write (ghandle, buf, buf_len, &bytes_written) == GNOME_VFS_OK){
				printf ("Datos guardados correctamente. %d escritos\n", bytes_written);
			}
			else{
				printf ("Error writing file %s:\n%s\n", filename, gnome_vfs_result_to_string(gvfs_result));
			}
		}
		else{
			printf ("Error opening %s: %s\n", filename, gnome_vfs_result_to_string(gvfs_result));
		}
	}
	else{
		printf ("Error creating %s: %s\n", filename, gnome_vfs_result_to_string(gvfs_result));
	}
	if (gnome_vfs_close (&ghandle) == GNOME_VFS_OK){
		printf ("Fichero cerrado correctamente: %s\n", filename);
	}
	
	g_date_free (d);
}
*/

static void message_box 	( AppletData *applet_data, GtkMessageType type, GtkButtonsType buttons, const char *text, const char *secondary_text )
{
	GtkWidget *dialog=0;
	dialog = gtk_message_dialog_new (NULL, GTK_DIALOG_MODAL, type, buttons, text);
	gtk_message_dialog_format_secondary_text (GTK_MESSAGE_DIALOG(dialog), secondary_text);
	gtk_dialog_run (GTK_DIALOG(dialog));
	gtk_widget_destroy (dialog);
}

void unset_images	 	( AppletData *applet_data )
{
	int x=0;
	GdkPixbuf *pix=0;
	strcpy (applet_data->city_name,"");
	strcpy (applet_data->provincia,"");
	strcpy (applet_data->last_update, "");
	set_tooltip (applet_data, -1, "");
	for (x=0;x < MAX_DAYS;x++){
		pix = gtk_image_get_pixbuf (GTK_IMAGE(applet_data->image[x]));
		g_object_unref (G_OBJECT (pix));
		gtk_image_set_from_file (GTK_IMAGE(applet_data->image[x]), "");
		strcpy (applet_data->day_info[x].day,"");
	}
}

void quit 			( GtkWidget *widget )
{
	if (widget){
		gtk_widget_hide (widget);
		gtk_widget_destroy (widget);
		widget = NULL;
	}
}

void create_window 		( AppletData *applet_data, const char *name )
{
	GladeXML *xml;
	static GdkPixbuf *pix;
	static GtkWidget *win=0;
	GtkWidget *lb1=0, *lb2=0, *lb3=0, *lb4=0, *lb5=0, *lb6=0, *lb7=0, *img=0;// *table=0;
	int x,y=0;
	short int idx=0,img_idx=0;
	char *day;
	GdkColor color;
	static int window_shown;

	if (strcmp (name, "event11") == 0){
		return ;
	}
	if (window_shown){
		quit (win);
		win = 0;
		/*
		if (pix){
			g_object_unref (G_OBJECT (pix));
			pix = 0;
		}
		*/
	}
	else{
		day = g_new(char,128);

		xml = glade_xml_new (PACKAGE_DIR"/gnome-inm-glade.glade", "win_day", NULL);
		win = glade_xml_get_widget (xml, "win_day");
		lb1 = glade_xml_get_widget (xml, "label_day1");
		lb2 = glade_xml_get_widget (xml, "label_day2");
		lb3 = glade_xml_get_widget (xml, "label_day3");
		lb4 = glade_xml_get_widget (xml, "label_day4");
		lb5 = glade_xml_get_widget (xml, "label_day5");
		lb6 = glade_xml_get_widget (xml, "label_day6");
		lb7 = glade_xml_get_widget (xml, "label_day7");
		img = glade_xml_get_widget (xml, "img");
		
		g_signal_connect_swapped (G_OBJECT(win), "destroy", G_CALLBACK(quit), win);

		color.red = 65535;
		color.blue = 65535;
		color.green = 65535;
		gtk_widget_modify_bg (win, GTK_STATE_NORMAL, &color);
		
		if (strcmp (name, "event1") == 0){
			img_idx=0;
			idx = 0;
		}
		else if (strcmp (name, "event2") == 0){
			img_idx=1;
			idx = 1;
		}
		else if (strcmp (name, "event3") == 0){
			img_idx=2;
			idx = 2;
		}
		else if (strcmp (name, "event4") == 0){
			img_idx=3;
			idx = 3;
		}
		else if (strcmp (name, "event5") == 0){
			img_idx=4;
			idx = 4;
		}
		else if (strcmp (name, "event6") == 0){
			img_idx=5;
			idx = 5;
		}
		else if (strcmp (name, "event7") == 0){
			img_idx=6;
			idx = 6;
		}
		else if (strcmp (name, "event8") == 0){
			img_idx=7;
			idx = 7;
		}
		else if (strcmp (name, "event9") == 0){
			img_idx=8;
			idx = 8;
		}
		else if (strcmp (name, "event10") == 0){
			img_idx=9;
			idx = 9;
		}
		//printf ("event name: %s - %d - %d\n", name, idx, img_idx);
		if (!gtk_image_get_pixbuf (GTK_IMAGE(applet_data->image[9])))
			idx += 1;
			
		gtk_label_set_markup (GTK_LABEL(lb1), applet_data->day_info[idx].state);
		gtk_label_set_markup (GTK_LABEL(lb2), applet_data->day_info[idx].precip);
		gtk_label_set_text (GTK_LABEL(lb3), applet_data->day_info[idx].cota_nieve);
		gtk_label_set_markup (GTK_LABEL(lb4), applet_data->day_info[idx].t_max);
		gtk_label_set_markup (GTK_LABEL(lb5), applet_data->day_info[idx].t_min);
		gtk_label_set_markup (GTK_LABEL(lb6), applet_data->day_info[idx].wind);
		pix = gtk_image_get_pixbuf (GTK_IMAGE(applet_data->image[img_idx]));
		gtk_image_set_from_pixbuf (GTK_IMAGE(img), pix);
		
		snprintf (day, 128, "<span weight=\"bold\">%s</span>", applet_data->day_info[idx].day);
		//printf ("Day: %s\n", day);
		gtk_label_set_markup (GTK_LABEL(lb7), day);
		//printf ("state: %s\n",  applet_data->day_info[idx].state);

		gtk_window_get_position (GTK_WINDOW (win), &x, &y);
		y += applet_data->applet_size;
		gtk_window_move (GTK_WINDOW(win), x, y);
		gtk_widget_show (win);
		x = y = 0;
		g_object_unref (G_OBJECT (xml));
		if (day){
			g_free (day);
			day = NULL;
		}
	}
	window_shown = !window_shown;
}

void set_tooltip		( AppletData *applet_data, const int id, const gchar* tip )
{
	int x=0;
	char *temp=0;
	char *str_morning=_("Morning");
	char *str_afternoon=_("Afternoon");
	temp = g_new0 (char, 512);
	
	if (!applet_data) return;

	if (id == 0 || id == 2 || id == 4)
		snprintf (temp, 512, "%s %s\n%s (%s)\n\n%s\n%s", applet_data->city_name, applet_data->provincia, applet_data->day_info[id].day, str_morning, tip, applet_data->last_update);
	else if (id == 1 || id == 3 || id == 5)
		snprintf (temp, 512, "%s %s\n%s (%s)\n\n%s\n%s", applet_data->city_name, applet_data->provincia, applet_data->day_info[id].day, str_afternoon, tip, applet_data->last_update);
	else if (id == MAX_DAYS)
		snprintf (temp, 512, "%s", tip);
	else if (id == -1)
		strcpy (temp, "");
	else
		snprintf (temp, 512, "%s %s\n%s\n\n%s\n%s", applet_data->city_name, applet_data->provincia, applet_data->day_info[id].day, tip, applet_data->last_update);


	if (id == -1){
		for (x=0;x < MAX_DAYS;x++){
			gtk_tooltips_set_tip (applet_data->tips, applet_data->event_box[x], "", NULL);
		}
	}
	else{
		gtk_tooltips_set_tip (applet_data->tips, applet_data->event_box[id], temp, NULL);
		gtk_tooltips_enable (applet_data->tips);
	}
	g_free (temp);
	temp = 0;
}

void print_element_names 		( xmlNode *root_element )
{
	xmlNode *cur_node=NULL, *a_node=NULL;
	a_node = root_element;
	for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
		if (cur_node->type == XML_ELEMENT_NODE) {
			printf("TEMP node type: Element, name: %s\n", cur_node->name);
		}
		print_element_names(cur_node->children);
	}
}

gboolean check_latest_data	( AppletData *applet_data )
{
	GnomeVFSResult    result;
	int		size=0;
	guchar		**buf={0};
	char		*temp=0, *tp=0;
	char **tokens=0;
	int i=0;
	const char	*suffix[10] = {
		"HO", "HH", "TT", "TR", "PP", 
		"VV", "DV", "RM", "P0", "TP"
	};

	temp = (char *)malloc (128);
	tp = g_new (char, 512);
	sprintf (tp, _("Station: %s\n\n"), (char *)panel_applet_gconf_get_string (PANEL_APPLET(applet_data->applet), "station_name", NULL));
	applet_data->prefs->station_code = (char *)panel_applet_gconf_get_string (PANEL_APPLET(applet_data->applet), "station_code", NULL);

	result = gnome_vfs_read_entire_file (INM_LATEST_DATA, &size, &buf);

	if (result == GNOME_VFS_OK){
		for (i=0;i < MAX_DAYS;i++){
			snprintf (temp, 24, "%s%s", applet_data->prefs->station_code, suffix[i]);
			if (strstr(buf, temp)){
				tokens = g_strsplit_set (strstr(buf, temp), "]=\"", 5);
				if (tokens[3]){
					printf ("%s: %s\n", suffix[i], tokens[3]);

					if (strcmp(suffix[i], "TT") == 0){ /* Temperatura */
						if (strcmp (tokens[3], "") != 0){
							snprintf (temp, 32, _("%s C"), tokens[3]);
						}
						else
							strncpy (temp, "--", 32);

						printf ("\nTemperature: %s\n\n", tokens[3]);
						gtk_label_set_text (GTK_LABEL(applet_data->temp_lbl), temp);
						snprintf (temp, 128, _("Temperature: %s\n"), tokens[3]);
						strncat (tp, temp, 50);
					}
					if (strcmp(suffix[i], "RM") == 0){ /* Hora oficial */
						snprintf (temp, 128, _("Maximum wind velocity: %s Km/h\n"), tokens[3]);
						strncat (tp, temp, 50);
					}
					if (strcmp(suffix[i], "P0") == 0){ /* Presion atmosferica */
						snprintf (temp, 128, _("Atmosferic presion: %s\n"), tokens[3]);
						strncat (tp, temp, 50);
					}
					if (strcmp(suffix[i], "PP") == 0){ /* Precipitacion acumulada */
						snprintf (temp, 128, _("Rainfall: %s\n"), tokens[3]);
						strncat (tp, temp, 50);
					}
					if (strcmp(suffix[i], "TR") == 0){ /* Humedad relativa */
						snprintf (temp, 128, _("Humidity: %s %\n"), tokens[3]);
						strncat (tp, temp, 50);
					}
					if (strcmp(suffix[i], "VV") == 0){ /* Direccion del viento */
						snprintf (temp, 128, _("Wind velocity: %s Km/h\n"), tokens[3]);
						strncat (tp, temp, 50);
					}
					if (strcmp(suffix[i], "DV") == 0){ /* Direccion del viento */
						snprintf (temp, 128, _("Wind: %s\n"), tokens[3]);
						strncat (tp, temp, 50);
					}
					if (strcmp(suffix[i], "HO") == 0){ /* Hora oficial */
						snprintf (temp, 128, _("Last update: %sh\n"), tokens[3]);
						strncat (tp, temp, 50);
					}
				}

				if (tokens)
					g_strfreev (tokens);
			}
		}
		if (buf){
			g_free (buf);
			buf = NULL;
		}

		if (strcmp (temp, "") == 0){
			strcpy (temp, "--");
			gtk_label_set_text (GTK_LABEL(applet_data->temp_lbl), temp);
			strncat (tp, _("No data for this meteorological station"), 64);
		}
		set_tooltip (applet_data, MAX_DAYS, tp);
	}
	else{
		gtk_label_set_text (GTK_LABEL(applet_data->temp_lbl), "?");
		strncat (tp, _("Error getting latest data from meteorological station"), 64);
		set_tooltip (applet_data, MAX_DAYS, tp);
		printf ("Error getting latest data from meteorological station\n");
	}

	if (temp){
		g_free (temp);
		temp = NULL;
	}
	if (tp){
		g_free (tp);
		tp = NULL;
	}

	return TRUE;
}


gboolean check_inm_url 			( AppletData *applet_data )
{
	char temp[512];
	char *code = (char *)panel_applet_gconf_get_string (PANEL_APPLET(applet_data->applet), "code", NULL);
	
	if (applet_data && applet_data->applet){
		snprintf (temp, 512, "%s%s\0", INM_FORECAST_URL, code);

		gnome_vfs_async_open (&applet_data->gvfs_handle, temp, GNOME_VFS_OPEN_READ, 0, check_inm_url_status, applet_data);
		
		/*
		xmlDoc *doc=NULL;
		xmlNode *root_element=NULL;
		xmlNode *cur_node=NULL, *a_node=NULL;
		doc = xmlReadMemory (buf, strlen(buf), NULL, NULL, XML_PARSE_RECOVER);
		root_element = xmlDocGetRootElement (doc);
		a_node = root_element;
		for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
			if (cur_node->type == XML_ELEMENT_NODE) {
				printf("node type: Element, name: %s\n", cur_node->name);
			}
			//print_element_names(cur_node->children);
	        }
		xmlFreeDoc (doc);
		*/
		
	}
	else{
		printf ("check_inm_url() no applet no fun\n");
	}

	if (code){
		g_free (code);
		code = NULL;
	}
		
	return TRUE;
}

void update_station_data 		( AppletData *applet_data )
{
	gboolean show_station;
	show_station = panel_applet_gconf_get_bool (PANEL_APPLET(applet_data->applet), "show_station", NULL);
	/*
 	g_source_remove (applet_data->timer_station);
	if (show_station){
		gtk_widget_show (applet_data->event_box[10]);
		check_latest_data (applet_data);
 		applet_data->timer_station = g_timeout_add(applet_data->interval * INTERVAL_TIME, (GtkFunction)check_latest_data, applet_data );
	}
	else{
		gtk_widget_hide (applet_data->event_box[10]);
	}
	*/
}

void update_location 			( AppletData *applet_data )
{
 	int iDays=0,x=0;
	if (applet_data){
		// Do not update the station data since it has been removed from the inm web :(
		//update_station_data (applet_data);

 		iDays = atoi (applet_data->show_days);
// 		printf ("Days to show: %d\n", iDays);
 		for (x=0;x < iDays;x++)
 			gtk_widget_show (applet_data->event_box[x]);
 		
 		for (x=iDays;x < MAX_DAYS;x++)
 			gtk_widget_hide (applet_data->event_box[x]);

		g_source_remove (applet_data->timer);
		check_inm_url (applet_data);
		applet_data->timer = g_timeout_add(applet_data->interval * INTERVAL_TIME, (GtkFunction)check_inm_url, applet_data );
	}
	else
		printf ("update_location() no *applet object\n");
}

void update_data			 ( BonoboUIComponent *uic, gpointer user_data, const char *name )
{
	AppletData *applet_data = (AppletData *) user_data;
	gboolean show_station;

	if (applet_data){
		show_station = panel_applet_gconf_get_bool (PANEL_APPLET(applet_data->applet), "show_station", NULL);
		/*
		if (show_station){
			check_latest_data (applet_data);
 			applet_data->timer_station = g_timeout_add(applet_data->interval * INTERVAL_TIME, (GtkFunction)check_latest_data, applet_data );
		}
		else
 			g_source_remove (applet_data->timer_station);
		
		*/
		g_source_remove (applet_data->timer);
		check_inm_url (applet_data);
		applet_data->timer = g_timeout_add(applet_data->interval * INTERVAL_TIME, (GtkFunction)check_inm_url, applet_data );
	}
	else
		printf ("update_data() no *applet object\n");
}

void display_preferences_dialog		 ( BonoboUIComponent *uic, gpointer user_data, const char *name )
{
//	printf ("display_prefs_dg: %s\n", applet_data->prefs->code);
	AppletData *applet_data = (AppletData *) user_data;
	if (!applet_data->prefs->win)
		create_preferences_win (applet_data);
}

void display_inm_website		 ( BonoboUIComponent *uic, gpointer user_data, const char *name )
{
	GnomeVFSResult gvfs_result;
	AppletData *applet_data = (AppletData *) user_data;

	gvfs_result = gnome_vfs_url_show (INM_URL);
	if (gvfs_result != GNOME_VFS_OK)
		message_box (applet_data, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, _("Error loading www.aemet.es"), gnome_vfs_result_to_string (gvfs_result));
		
}

void display_wwarnings_t		 ( BonoboUIComponent *uic, gpointer user_data, const char *name )
{
	GnomeVFSResult gvfs_result;
	AppletData *applet_data = (AppletData *) user_data;

	gvfs_result = gnome_vfs_url_show (INM_WWARNINGS_TODAY_URL);
	if (gvfs_result != GNOME_VFS_OK)
		message_box (applet_data, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, _("Error loading "INM_WWARNINGS_TODAY_URL), gnome_vfs_result_to_string (gvfs_result));
}

void display_wwarnings_tm		 ( BonoboUIComponent *uic, gpointer user_data, const char *name )
{
	GnomeVFSResult gvfs_result;
	AppletData *applet_data = (AppletData *) user_data;

	gvfs_result = gnome_vfs_url_show (INM_WWARNINGS_TOMORROW_URL);
	if (gvfs_result != GNOME_VFS_OK)
		message_box (applet_data, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, _("Error loading "INM_WWARNINGS_TOMORROW_URL), gnome_vfs_result_to_string (gvfs_result));
}

void display_wwarnings_n		 ( BonoboUIComponent *uic, gpointer user_data, const char *name )
{
	GnomeVFSResult gvfs_result;
	AppletData *applet_data = (AppletData *) user_data;

	gvfs_result = gnome_vfs_url_show (INM_WWARNINGS_NEXT_DAYS_URL);
	if (gvfs_result != GNOME_VFS_OK)
		message_box (applet_data, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, _("Error loading "INM_WWARNINGS_NEXT_DAYS_URL), gnome_vfs_result_to_string (gvfs_result));
}


GdkPixbuf *load_image				( const char *url )
{
	GnomeVFSHandle   *handle;
	GnomeVFSResult    result;
	GdkPixbufLoader  *loader;
	GdkPixbuf        *pixbuf;
	guchar             buffer[8192];
	GnomeVFSFileSize  bytes_read;

	result = gnome_vfs_open (&handle, url, GNOME_VFS_OPEN_READ);
	loader = gdk_pixbuf_loader_new ();

	while (result == GNOME_VFS_OK) {
		result = gnome_vfs_read (handle, buffer,
                                        8192, &bytes_read);
	gdk_pixbuf_loader_write (loader, buffer, bytes_read, NULL);
	}

	gdk_pixbuf_loader_close (loader, NULL);
	pixbuf = gdk_pixbuf_loader_get_pixbuf (loader);
	gnome_vfs_close (handle);

	if (!pixbuf) {
		return NULL;
	}
	else
	       return pixbuf;
}


void display_satellite_radar 		( BonoboUIComponent *uic, gpointer user_data, const char *name )
{
	GladeXML *xml=0;
	GtkWidget *img=0;
	GtkWidget *win=0;
	GtkWidget *combo_hour=0;
	GdkPixbuf *pixbuf=0;
	char img_radar[256]={0};
	
	xml = glade_xml_new (PACKAGE_DIR"/gnome-inm-glade.glade", "win_radar", NULL);
	win = glade_xml_get_widget (xml, "win_radar");
	img = glade_xml_get_widget (xml, "radar_img");
	combo_hour = glade_xml_get_widget (xml, "combo_hour");
	
	g_signal_connect (G_OBJECT(win), "destroy", G_CALLBACK(on_window_terminate), img);
	g_signal_connect (G_OBJECT(combo_hour), "changed", G_CALLBACK(on_sat_combo_hour_changed), img);

	gtk_combo_box_set_active (GTK_COMBO_BOX(combo_hour), 0);

	g_snprintf ((char *)&img_radar, 256, "%s%s", INM_SAT_IMG, "1900.gif");
	pixbuf = load_image ((char *)&img_radar);
	if (pixbuf){
		gtk_image_set_from_pixbuf (GTK_IMAGE(img), pixbuf);
		gtk_window_set_title (GTK_WINDOW(win), _("Satellite image"));
		gtk_widget_show (win);
		g_object_unref (G_OBJECT (pixbuf));
		pixbuf = 0;
	}
	g_object_unref (G_OBJECT(xml));
}

void display_spanish_forecast_img 	( BonoboUIComponent *uic, gpointer user_data, const char *name )
{
	GdkPixbuf *pixbuf;
	GladeXML *xml;
	GtkWidget *img;
	GtkWidget *win;
	GtkWidget *combo_hour;

	xml = glade_xml_new (PACKAGE_DIR"/gnome-inm-glade.glade", "win_radar", NULL);
	win = glade_xml_get_widget (xml, "win_radar");
	img = glade_xml_get_widget (xml, "radar_img");
	combo_hour = glade_xml_get_widget (xml, "combo_hour");
	
	gtk_widget_hide (combo_hour);

	g_signal_connect (G_OBJECT(win), "destroy", G_CALLBACK(on_window_terminate), img);
	
	pixbuf = load_image (INM_NATIONAL_FORECAST_IMG);
	if (pixbuf){
		gtk_image_set_from_pixbuf (GTK_IMAGE(img), pixbuf);
		gtk_window_set_title (GTK_WINDOW(win), _("Spanish weather forecast image"));
		gtk_widget_show (win);
		g_object_unref (G_OBJECT (pixbuf));
		pixbuf = 0;
	}
	g_object_unref (G_OBJECT(xml));
}

void display_daily_temperatures 	( BonoboUIComponent *uic, gpointer user_data, const char *name )
{
	GdkPixbuf *pixbuf;
	GladeXML *xml;
	GtkWidget *img, *img2;
	GtkWidget *win;
	GtkWidget *rb1, *rb2;
	
	xml = glade_xml_new (PACKAGE_DIR"/gnome-inm-glade.glade", "win_day_temps", NULL);
	win = glade_xml_get_widget (xml, "win_day_temps");
	img = glade_xml_get_widget (xml, "temp_img");
	rb1 = glade_xml_get_widget (xml, "rb_1");
	rb2 = glade_xml_get_widget (xml, "rb_2");
	//gtk_widget_hide (rb1);
	//gtk_widget_hide (rb2);
	
	g_signal_connect (G_OBJECT(win), "destroy", G_CALLBACK(on_window_terminate), img);
	g_signal_connect (G_OBJECT(rb1), "toggled", G_CALLBACK(on_daily_temp_rb1_toggled), img);
	g_signal_connect (G_OBJECT(rb2), "toggled", G_CALLBACK(on_daily_temp_rb2_toggled), img);
	
	/*
	pixbuf = load_image (INM_SPAIN_IMG);
	if (pixbuf){
		gtk_image_set_from_pixbuf (GTK_IMAGE(img2), pixbuf);
		gtk_window_set_title (GTK_WINDOW(win), _("Temperatures for the next days"));
		gtk_widget_show (win);
		g_object_unref (G_OBJECT (pixbuf));
		pixbuf = 0;
	}
	*/
	pixbuf = load_image (INM_DAILY_TEMPERATURE_MIN);
	if (pixbuf){
		gtk_image_set_from_pixbuf (GTK_IMAGE(img), pixbuf);
		gtk_window_set_title (GTK_WINDOW(win), _("Temperatures for the next days"));
		gtk_widget_show (win);
		g_object_unref (G_OBJECT (pixbuf));
		pixbuf = 0;
	}
	g_object_unref (G_OBJECT(xml));
}

void display_prob_precipitation 	( BonoboUIComponent *uic, gpointer user_data, const char *name )
{
	GdkPixbuf *pixbuf=0;
	GladeXML *xml=0;
	GtkWidget *img=0;
	GtkWidget *win=0;
	GtkWidget *rb_005=0, *rb_020=0, *rb_100=0, *rb_200=0, *rb_avg=0, *rb_max=0;
	
	xml = glade_xml_new (PACKAGE_DIR"/gnome-inm-glade.glade", "win_prob_precip", NULL);
	win = glade_xml_get_widget (xml, "win_prob_precip");
	img = glade_xml_get_widget (xml, "temp_img1");
	rb_005 = glade_xml_get_widget (xml, "rb_005");
	rb_020 = glade_xml_get_widget (xml, "rb_020");
	rb_100 = glade_xml_get_widget (xml, "rb_100");
	rb_200 = glade_xml_get_widget (xml, "rb_200");
	rb_avg = glade_xml_get_widget (xml, "rb_avg");
	rb_max = glade_xml_get_widget (xml, "rb_max");
	//gtk_widget_hide (rb1);
	//gtk_widget_hide (rb2);
	
	g_signal_connect (G_OBJECT(win), "destroy", G_CALLBACK(on_window_terminate), img);
	g_signal_connect (G_OBJECT(rb_005), "toggled", G_CALLBACK(on_prob_precip_toggled_005), img);
	g_signal_connect (G_OBJECT(rb_020), "toggled", G_CALLBACK(on_prob_precip_toggled_020), img);
	g_signal_connect (G_OBJECT(rb_100), "toggled", G_CALLBACK(on_prob_precip_toggled_100), img);
	g_signal_connect (G_OBJECT(rb_200), "toggled", G_CALLBACK(on_prob_precip_toggled_200), img);
	g_signal_connect (G_OBJECT(rb_avg), "toggled", G_CALLBACK(on_prob_precip_toggled_avg), img);
	g_signal_connect (G_OBJECT(rb_max), "toggled", G_CALLBACK(on_prob_precip_toggled_max), img);
	
	pixbuf = load_image (INM_PROB_PRECIP_005);
	if (pixbuf){
		gtk_image_set_from_pixbuf (GTK_IMAGE(img), pixbuf);
		gtk_window_set_title (GTK_WINDOW(win), _("Rainfall probability"));
		gtk_widget_show (win);
		g_object_unref (G_OBJECT (pixbuf));
		pixbuf = 0;
	}
	g_object_unref (G_OBJECT(xml));
}

void display_air_mass	 	( BonoboUIComponent *uic, gpointer user_data, const char *name )
{
	GdkPixbuf *pixbuf=0;
	GladeXML *xml=0;
	GtkWidget *img=0;
	GtkWidget *win=0;
	GtkWidget *combo_hour=0;
	char rdr_img[256]={0};
	
	xml = glade_xml_new (PACKAGE_DIR"/gnome-inm-glade.glade", "win_radar", NULL);
	win = glade_xml_get_widget (xml, "win_radar");
	img = glade_xml_get_widget (xml, "radar_img");
	combo_hour = glade_xml_get_widget (xml, "combo_hour");
	
	g_signal_connect (G_OBJECT(win), "destroy", G_CALLBACK(on_window_terminate), img);
	g_signal_connect (G_OBJECT(combo_hour), "changed", G_CALLBACK(on_airmass_combo_hour_changed), img);

	gtk_combo_box_set_active (GTK_COMBO_BOX(combo_hour), 0);
	
	g_snprintf ((char *)&rdr_img, 256, "%s%s", INM_RADAR_AIR_MASS_IMG, "1900.jpg");
	printf ("displaying %s\n", rdr_img);
	pixbuf = load_image ((char *)&rdr_img);
	if (pixbuf){
		gtk_image_set_from_pixbuf (GTK_IMAGE(img), pixbuf);
		gtk_window_set_title (GTK_WINDOW(win), _("Air mass"));
		gtk_widget_show (win);
		g_object_unref (G_OBJECT (pixbuf));
		pixbuf = 0;
	}
	g_object_unref (G_OBJECT(xml));
}

void display_radar	 	( BonoboUIComponent *uic, gpointer user_data, const char *name )
{
	GdkPixbuf *pixbuf=0;
	GladeXML *xml=0;
	GtkWidget *img=0;
	GtkWidget *win=0;
	GtkWidget *combo_hour=0;
	char rdr_img[256]={0};
	
	xml = glade_xml_new (PACKAGE_DIR"/gnome-inm-glade.glade", "win_radar", NULL);
	win = glade_xml_get_widget (xml, "win_radar");
	img = glade_xml_get_widget (xml, "radar_img");
	combo_hour = glade_xml_get_widget (xml, "combo_hour");
	
	g_signal_connect (G_OBJECT(win), "destroy", G_CALLBACK(on_window_terminate), img);

	gtk_combo_box_set_active (GTK_COMBO_BOX(combo_hour), 0);
	
	if (strncmp (name, "RadarPeninsulaProb", 18) == 0){
		g_snprintf ((char *)&rdr_img, 256, "%s%s", INM_RADAR_IMG, "1900.gif");
		g_signal_connect (G_OBJECT(combo_hour), "changed", G_CALLBACK(on_radar_combo_hour_changed), img);
	}
	else{
		g_snprintf ((char *)&rdr_img, 256, "%s%s", INM_RADAR_CANARIAS_IMG, "1900.gif");
		g_signal_connect (G_OBJECT(combo_hour), "changed", G_CALLBACK(on_radar_canarias_combo_hour_changed), img);
	}

	printf ("displaying %s\n", rdr_img);
	pixbuf = load_image ((char *)&rdr_img);
	if (pixbuf){
		gtk_image_set_from_pixbuf (GTK_IMAGE(img), pixbuf);
		gtk_window_set_title (GTK_WINDOW(win), _("Radar images"));
		gtk_widget_show (win);
		g_object_unref (G_OBJECT (pixbuf));
		pixbuf = 0;
	}
	g_object_unref (G_OBJECT(xml));
}

void display_lightnings	 	( BonoboUIComponent *uic, gpointer user_data, const char *name )
{
	GdkPixbuf *pixbuf=0;
	GladeXML *xml=0;
	GtkWidget *img=0;
	GtkWidget *win=0;
	GtkWidget *combo_hour=0;
	char rdr_img[256]={0};
	
	xml = glade_xml_new (PACKAGE_DIR"/gnome-inm-glade.glade", "win_radar", NULL);
	win = glade_xml_get_widget (xml, "win_radar");
	img = glade_xml_get_widget (xml, "radar_img");
	combo_hour = glade_xml_get_widget (xml, "combo_hour");
	
	g_signal_connect (G_OBJECT(win), "destroy", G_CALLBACK(on_window_terminate), img);

	gtk_combo_box_set_active (GTK_COMBO_BOX(combo_hour), 0);
	
	if (strncmp (name, "RayosP", 6) == 0){
		g_snprintf ((char *)&rdr_img, 256, "%s%s", INM_RAYOS_IMG, "1900.gif");
		g_signal_connect (G_OBJECT(combo_hour), "changed", G_CALLBACK(on_rayos_combo_hour_changed), img);
	}
	else{
		g_snprintf ((char *)&rdr_img, 256, "%s%s", INM_RAYOS_CANARIAS_IMG, "1900.gif");
		g_signal_connect (G_OBJECT(combo_hour), "changed", G_CALLBACK(on_rayos_canarias_combo_hour_changed), img);
	}

	printf ("displaying %s\n", rdr_img);
	pixbuf = load_image ((char *)&rdr_img);
	if (pixbuf){
		gtk_image_set_from_pixbuf (GTK_IMAGE(img), pixbuf);
		gtk_window_set_title (GTK_WINDOW(win), _("Lightnings images"));
		gtk_widget_show (win);
		g_object_unref (G_OBJECT (pixbuf));
		pixbuf = 0;
	}
	g_object_unref (G_OBJECT(xml));
}

void display_nextdays_forecast	 	( BonoboUIComponent *uic, gpointer user_data, const char *name )
{
	AppletData *applet_data = (AppletData *) user_data;
	applet_data->gvfs_cnx_type = 3;
	gnome_vfs_async_open (&applet_data->gvfs_handle, INM_FORECAST_NEXTDAYS_URL, GNOME_VFS_OPEN_READ, 0, gvfs_status_cb, applet_data);
}

void display_tomorrow_forecast		 ( BonoboUIComponent *uic, gpointer user_data, const char *name )
{
	AppletData *applet_data = (AppletData *) user_data;
	applet_data->gvfs_cnx_type = 2;
	gnome_vfs_async_open (&applet_data->gvfs_handle, INM_FORECAST_TOMORROW_URL, GNOME_VFS_OPEN_READ, 0, gvfs_status_cb, applet_data);
}

void display_today_forecast		 ( BonoboUIComponent *uic, gpointer user_data, const char *name )
{
	AppletData *applet_data = (AppletData *) user_data;
	applet_data->gvfs_cnx_type = 1;
	gnome_vfs_async_open (&applet_data->gvfs_handle, INM_FORECAST_TODAY_URL, GNOME_VFS_OPEN_READ, 0, gvfs_status_cb, applet_data);
}

void display_snow_warnings_nav		 ( BonoboUIComponent *uic, gpointer user_data, const char *name )
{
	AppletData *applet_data = (AppletData *) user_data;
	applet_data->gvfs_cnx_type = 6;
	gnome_vfs_async_open (&applet_data->gvfs_handle, INM_SNOW_WARNINGS_URL_1, GNOME_VFS_OPEN_READ, 0, gvfs_status_cb, applet_data);
}

void display_snow_warnings_cat		 ( BonoboUIComponent *uic, gpointer user_data, const char *name )
{
	AppletData *applet_data = (AppletData *) user_data;
	applet_data->gvfs_cnx_type = 6;
	gnome_vfs_async_open (&applet_data->gvfs_handle, INM_SNOW_WARNINGS_URL_2, GNOME_VFS_OPEN_READ, 0, gvfs_status_cb, applet_data);
}

void display_mountain_forecast		 ( BonoboUIComponent *uic, gpointer user_data, const char *name )
{
	AppletData *applet_data = (AppletData *) user_data;
	
	if (strncmp(name, "ForecastPicos", 13) == 0){
		gnome_vfs_async_open (&applet_data->gvfs_handle, INM_MOUNTAIN_FORECAST_PICOS, GNOME_VFS_OPEN_READ, 0, gvfs_status_cb, applet_data);
	}
	else if (strncmp(name, "ForecastGredos", 14) == 0){
		gnome_vfs_async_open (&applet_data->gvfs_handle, INM_MOUNTAIN_FORECAST_GREDOS, GNOME_VFS_OPEN_READ, 0, gvfs_status_cb, applet_data);
	}
	else if (strncmp(name, "ForecastGuadarrama", 18) == 0){
		gnome_vfs_async_open (&applet_data->gvfs_handle, INM_MOUNTAIN_FORECAST_GUADARRAMA, GNOME_VFS_OPEN_READ, 0, gvfs_status_cb, applet_data);
	}
	else if (strncmp(name, "ForecastRioja", 13) == 0){
		gnome_vfs_async_open (&applet_data->gvfs_handle, INM_MOUNTAIN_FORECAST_RIOJA, GNOME_VFS_OPEN_READ, 0, gvfs_status_cb, applet_data);
	}
	else if (strncmp(name, "ForecastAragon", 14) == 0){
		gnome_vfs_async_open (&applet_data->gvfs_handle, INM_MOUNTAIN_FORECAST_IARAGONESA, GNOME_VFS_OPEN_READ, 0, gvfs_status_cb, applet_data);
	}
	else if (strncmp(name, "ForecastPirineoNavarro", 22) == 0){
		gnome_vfs_async_open (&applet_data->gvfs_handle, INM_MOUNTAIN_FORECAST_PIRI_NAVARRO, GNOME_VFS_OPEN_READ, 0, gvfs_status_cb, applet_data);
	}
	else if (strncmp(name, "ForecastPirineoAragones", 23) == 0){
		gnome_vfs_async_open (&applet_data->gvfs_handle, INM_MOUNTAIN_FORECAST_PIRI_ARAGONES, GNOME_VFS_OPEN_READ, 0, gvfs_status_cb, applet_data);
	}
	else if (strncmp(name, "ForecastPirineoCatalan", 22) == 0){
		gnome_vfs_async_open (&applet_data->gvfs_handle, INM_MOUNTAIN_FORECAST_PIRI_CATALAN, GNOME_VFS_OPEN_READ, 0, gvfs_status_cb, applet_data);
	}
	else if (strncmp(name, "ForecastSierraNevada", 20) == 0){
		gnome_vfs_async_open (&applet_data->gvfs_handle, INM_MOUNTAIN_FORECAST_SIERRA_NEVADA, GNOME_VFS_OPEN_READ, 0, gvfs_status_cb, applet_data);
	}
	else{
		printf ("display_mountain_forecast(): No name supplied (%s)\n", name);
	}
	
}

void display_marine_forecast		 ( BonoboUIComponent *uic, gpointer user_data, const char *name )
{
	AppletData *applet_data = (AppletData *) user_data;
	
	if (strncmp(name, "MarineForecast1", 15) == 0){
		gnome_vfs_async_open (&applet_data->gvfs_handle, INM_MARINE_FORECAST_1, GNOME_VFS_OPEN_READ, 0, gvfs_status_cb, applet_data);
	}
	else if (strncmp(name, "MarineForecast2", 15) == 0){
		gnome_vfs_async_open (&applet_data->gvfs_handle, INM_MARINE_FORECAST_2, GNOME_VFS_OPEN_READ, 0, gvfs_status_cb, applet_data);
	}
	else if (strncmp(name, "MarineForecast3", 15) == 0){
		gnome_vfs_async_open (&applet_data->gvfs_handle, INM_MARINE_FORECAST_3, GNOME_VFS_OPEN_READ, 0, gvfs_status_cb, applet_data);
	}
	else if (strncmp(name, "MarineForecast4", 15) == 0){
		gnome_vfs_async_open (&applet_data->gvfs_handle, INM_MARINE_FORECAST_4, GNOME_VFS_OPEN_READ, 0, gvfs_status_cb, applet_data);
	}
	else if (strncmp(name, "MarineForecast5", 15) == 0){
		gnome_vfs_async_open (&applet_data->gvfs_handle, INM_MARINE_FORECAST_5, GNOME_VFS_OPEN_READ, 0, gvfs_status_cb, applet_data);
	}
	else if (strncmp(name, "MarineForecast6", 15) == 0){
		gnome_vfs_async_open (&applet_data->gvfs_handle, INM_MARINE_FORECAST_6, GNOME_VFS_OPEN_READ, 0, gvfs_status_cb, applet_data);
	}
	else if (strncmp(name, "MarineForecast7", 15) == 0){
		gnome_vfs_async_open (&applet_data->gvfs_handle, INM_MARINE_FORECAST_7, GNOME_VFS_OPEN_READ, 0, gvfs_status_cb, applet_data);
	}
	else if (strncmp(name, "MarineForecast8", 15) == 0){
		gnome_vfs_async_open (&applet_data->gvfs_handle, INM_MARINE_FORECAST_8, GNOME_VFS_OPEN_READ, 0, gvfs_status_cb, applet_data);
	}
	else if (strncmp(name, "NAtlantic1", 10) == 0){
		gnome_vfs_async_open (&applet_data->gvfs_handle, INM_MARINE_OPENSEA_1, GNOME_VFS_OPEN_READ, 0, gvfs_status_cb, applet_data);
	}
	else if (strncmp(name, "NAtlantic2", 10) == 0){
		gnome_vfs_async_open (&applet_data->gvfs_handle, INM_MARINE_OPENSEA_2, GNOME_VFS_OPEN_READ, 0, gvfs_status_cb, applet_data);
	}
	else if (strncmp(name, "NAtlantic3", 10) == 0){
		gnome_vfs_async_open (&applet_data->gvfs_handle, INM_MARINE_OPENSEA_3, GNOME_VFS_OPEN_READ, 0, gvfs_status_cb, applet_data);
	}
	else if (strncmp(name, "Med1", 4) == 0){
		gnome_vfs_async_open (&applet_data->gvfs_handle, INM_MARINE_OPENSEA_4, GNOME_VFS_OPEN_READ, 0, gvfs_status_cb, applet_data);
	}
	else if (strncmp(name, "Med2", 4) == 0){
		gnome_vfs_async_open (&applet_data->gvfs_handle, INM_MARINE_OPENSEA_5, GNOME_VFS_OPEN_READ, 0, gvfs_status_cb, applet_data);
	}
	else if (strncmp(name, "Med3", 4) == 0){
		gnome_vfs_async_open (&applet_data->gvfs_handle, INM_MARINE_OPENSEA_6, GNOME_VFS_OPEN_READ, 0, gvfs_status_cb, applet_data);
	}
	else if (strncmp(name, "SAtlantic1", 10) == 0){
		gnome_vfs_async_open (&applet_data->gvfs_handle, INM_MARINE_OPENSEA_7, GNOME_VFS_OPEN_READ, 0, gvfs_status_cb, applet_data);
	}
	else if (strncmp(name, "SAtlantic2", 10) == 0){
		gnome_vfs_async_open (&applet_data->gvfs_handle, INM_MARINE_OPENSEA_8, GNOME_VFS_OPEN_READ, 0, gvfs_status_cb, applet_data);
	}
	else if (strncmp(name, "SAtlantic3", 10) == 0){
		gnome_vfs_async_open (&applet_data->gvfs_handle, INM_MARINE_OPENSEA_9, GNOME_VFS_OPEN_READ, 0, gvfs_status_cb, applet_data);
	}
	else{
		printf ("display_mountain_forecast(): No name supplied (%s)\n", name);
	}
	
}

void display_about_dialog 		( BonoboUIComponent *uic, gpointer user_data, const char *name )
{
	static GtkWidget *dialog;
	static GladeXML *xml;
	static gboolean shown=FALSE;
	static GdkPixbuf *pix;
	static GtkWidget *img_logo;

	AppletData *applet_data = (AppletData *) user_data;

	if (applet_data){
		if (shown){
			if (pix)
				g_object_unref (G_OBJECT(pix));
			if (img_logo)
				gtk_widget_destroy (GTK_WIDGET(img_logo));
			
			gtk_widget_destroy (dialog);
			g_object_unref (G_OBJECT (xml));
			dialog = 0;
			img_logo = 0;
			pix = 0;
			xml = 0;
			shown = FALSE;
		}
		xml = glade_xml_new (PACKAGE_DIR"/gnome-inm-glade.glade", "aboutdialog1", NULL);
		dialog = glade_xml_get_widget (xml, "aboutdialog1");
		img_logo = gtk_image_new_from_file ("/usr/share/pixmaps/"PACKAGE".png");
		pix = gtk_image_get_pixbuf (GTK_IMAGE (img_logo));
		gtk_about_dialog_set_name (GTK_ABOUT_DIALOG(dialog), "GNOME INM forecast - ");
		gtk_about_dialog_set_version (GTK_ABOUT_DIALOG(dialog), VERSION);
		gtk_about_dialog_set_logo (GTK_ABOUT_DIALOG(dialog), pix);
		g_signal_connect_swapped (G_OBJECT(dialog), "response", G_CALLBACK(quit), dialog);
		g_signal_connect_swapped (G_OBJECT(dialog), "destroy", G_CALLBACK(quit), dialog);
		gtk_widget_show (dialog);
		shown = TRUE;
		g_object_unref (G_OBJECT(xml));
	}
}

gboolean update_panel 			( AppletData *applet_data )
{
	//printf ("update_panel()\n");
//	check_inm_url (applet_data);
	return FALSE;
}


gboolean start_applet 			( PanelApplet *applet, const gchar *iid, gpointer data )
{
	static const BonoboUIVerb menu_callbacks [] = {
		BONOBO_UI_VERB ("Update", update_data),
		BONOBO_UI_VERB ("TodayForecast", display_today_forecast),
		BONOBO_UI_VERB ("TomorrowForecast", display_tomorrow_forecast),
		BONOBO_UI_VERB ("NextDaysForecast", display_nextdays_forecast),
		BONOBO_UI_VERB ("WeatherWarnings1", display_wwarnings_t),
		BONOBO_UI_VERB ("WeatherWarnings2", display_wwarnings_tm),
		BONOBO_UI_VERB ("WeatherWarnings3", display_wwarnings_n),
		BONOBO_UI_VERB ("SatelliteImages", display_satellite_radar),
		BONOBO_UI_VERB ("SpanishForecastImg", display_spanish_forecast_img),
		BONOBO_UI_VERB ("Temperatures", display_daily_temperatures),
		BONOBO_UI_VERB ("RainfallProb", display_prob_precipitation),
		BONOBO_UI_VERB ("AirMassProb", display_air_mass),
		BONOBO_UI_VERB ("RadarPeninsulaProb", display_radar),
		BONOBO_UI_VERB ("RadarCanariasProb", display_radar),
		BONOBO_UI_VERB ("RayosP", display_lightnings),
		BONOBO_UI_VERB ("RayosC", display_lightnings),
		BONOBO_UI_VERB ("InformeAludes1", display_snow_warnings_nav),
		BONOBO_UI_VERB ("InformeAludes2", display_snow_warnings_cat),
		BONOBO_UI_VERB ("ForecastPicos", display_mountain_forecast),
		BONOBO_UI_VERB ("ForecastGredos", display_mountain_forecast),
		BONOBO_UI_VERB ("ForecastGuadarrama", display_mountain_forecast),
		BONOBO_UI_VERB ("ForecastRioja", display_mountain_forecast),
		BONOBO_UI_VERB ("ForecastAragon", display_mountain_forecast),
		BONOBO_UI_VERB ("ForecastPirineoNavarro", display_mountain_forecast),
		BONOBO_UI_VERB ("ForecastPirineoAragones", display_mountain_forecast),
		BONOBO_UI_VERB ("ForecastPirineoCatalan", display_mountain_forecast),
		BONOBO_UI_VERB ("ForecastSierraNevada", display_mountain_forecast),
		BONOBO_UI_VERB ("MarineForecast1", display_marine_forecast),
		BONOBO_UI_VERB ("MarineForecast2", display_marine_forecast),
		BONOBO_UI_VERB ("MarineForecast3", display_marine_forecast),
		BONOBO_UI_VERB ("MarineForecast4", display_marine_forecast),
		BONOBO_UI_VERB ("MarineForecast5", display_marine_forecast),
		BONOBO_UI_VERB ("MarineForecast6", display_marine_forecast),
		BONOBO_UI_VERB ("MarineForecast7", display_marine_forecast),
		BONOBO_UI_VERB ("MarineForecast8", display_marine_forecast),
		BONOBO_UI_VERB ("NAtlantic1", display_marine_forecast),
		BONOBO_UI_VERB ("NAtlantic2", display_marine_forecast),
		BONOBO_UI_VERB ("NAtlantic3", display_marine_forecast),
		BONOBO_UI_VERB ("Med1", display_marine_forecast),
		BONOBO_UI_VERB ("Med2", display_marine_forecast),
		BONOBO_UI_VERB ("Med3", display_marine_forecast),
		BONOBO_UI_VERB ("SAtlantic1", display_marine_forecast),
		BONOBO_UI_VERB ("SAtlantic2", display_marine_forecast),
		BONOBO_UI_VERB ("SAtlantic3", display_marine_forecast),
		BONOBO_UI_VERB ("Properties", display_preferences_dialog),
		BONOBO_UI_VERB ("VisitINM", display_inm_website),
		BONOBO_UI_VERB ("About", display_about_dialog),
		BONOBO_UI_VERB_END
		};
	char *eventbox_name[11]= {
		"event1", "event2", "event3", 
		"event4", "event5", "event6", 
		"event7", "event8", "event9", 
		"event10", "event11"};
	int x=0;
	
	if (strcmp (iid, "OAFIID:GnomeINM") != 0)
		return FALSE;

	// Initialize GNOME VFS subsystem
	if (!gnome_vfs_initialized ()){
		if (!gnome_vfs_init ()){
			message_box (NULL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, _("Error loading the GNOME VFS subsystem"), _("An error ocurred loading the GNOME VFS subsystem") );
			return FALSE;
		}
	}

	AppletData *applet_data = g_new0(AppletData, 1);
	applet_data->prefs = g_new0 (PrefsWin, 1);
	applet_data->day_info = g_new0 (DayInf, MAX_DAYS);
	applet_data->prefs->code = g_new0 (char, 12);
	applet_data->prefs->station_code = g_new0 (char, 12);
	applet_data->city_name = g_new0 (char, 128);
	applet_data->city_long_desc = g_new0 (char, 128);
	applet_data->provincia = g_new0 (char, 128);
	applet_data->last_update = g_new0 (char, 128);
	applet_data->show_days = g_new0 (char, 12);
	applet_data->theme = g_new0 (char, 256);
	applet_data->st_info = NULL;
	applet_data->st_list = NULL;
	applet_data->buffer = NULL;

	applet_data->client = gconf_client_get_default ();
	
	/* struct initialization */
	applet_data->applet = GTK_WIDGET(applet);
	applet_data->interval = 3600;
	applet_data->show_ma = 0;
	applet_data->timer = 0;
	applet_data->applet_size = panel_applet_get_size (applet);
	applet_data->tips = gtk_tooltips_new();

	/* GConf */
	panel_applet_add_preferences (PANEL_APPLET(applet), "/schemas/apps/gnome-inm/prefs", NULL);
	//panel_applet_gconf_set_string (PANEL_APPLET(applet), "code", applet_data->prefs->code, NULL);
	applet_data->interval = panel_applet_gconf_get_int (PANEL_APPLET(applet), "interval", NULL);
	if (applet_data->interval == 0)
		applet_data->interval = 3600;
	applet_data->show_days = panel_applet_gconf_get_string (PANEL_APPLET(applet), "days", NULL);
	applet_data->theme = (char *)panel_applet_gconf_get_string (PANEL_APPLET(applet), "theme", NULL);
	int iDays =  g_ascii_strtod(applet_data->show_days, NULL);

	/* Translations initialization */
	setlocale (LC_ALL, "");
	bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
	textdomain (GETTEXT_PACKAGE);

	//printf ("startapplet.Code: %s\n",  (char *)panel_applet_gconf_get_string (PANEL_APPLET(applet), "code", NULL));
	//gconf_client_notify_add (applet_data->gclient, "/apps/gnome-inm/prefs/days", gconf_notify_days, applet_data, NULL, NULL);
	for (x=0;x < MAX_DAYS;x++){
		applet_data->day_info[x].state = g_new0 (char, 36);
		applet_data->day_info[x].precip = g_new0 (char, 12);
		applet_data->day_info[x].cota_nieve = g_new0 (char, 12);
		applet_data->day_info[x].t_max = g_new0 (char, 6);
		applet_data->day_info[x].t_min = g_new0 (char, 6);
		applet_data->day_info[x].day = g_new0 (char, 32);
		applet_data->day_info[x].wind = g_new0 (char, 12);
	}

	LIBXML_TEST_VERSION
	
	applet_data->hbox = gtk_hbox_new (FALSE, 1);
	gtk_container_add (GTK_CONTAINER (applet), applet_data->hbox);
	/* We can not add tooltips directly to an image widget, 
	 * so we add firstly an EventBox widget to the HBox, and then the GtkImage to the EventBox
	 * */
	for (x=0;x < iDays;x++){
		applet_data->event_box[x] = gtk_event_box_new ();
		gtk_widget_set_name (applet_data->event_box[x], eventbox_name[x]);
		applet_data->image[x] = gtk_image_new_from_file ("");
		gtk_container_add (GTK_CONTAINER (applet_data->event_box[x]), applet_data->image[x]);
		gtk_container_add (GTK_CONTAINER (applet_data->hbox), applet_data->event_box[x]);
		g_signal_connect (G_OBJECT (applet_data->event_box[x]), "button_press_event", G_CALLBACK (on_image_button_press), applet_data);
	}
	/* Temperature label */
	applet_data->temp_lbl = gtk_label_new ("");
	applet_data->event_box[iDays] = gtk_event_box_new ();
	gtk_widget_set_name (applet_data->event_box[iDays], eventbox_name[iDays]);
	gtk_container_add (GTK_CONTAINER (applet_data->event_box[iDays]), applet_data->temp_lbl);
	gtk_container_add (GTK_CONTAINER (applet_data->hbox), applet_data->event_box[iDays]);
	g_signal_connect (G_OBJECT (applet_data->event_box[iDays]), "button_press_event", G_CALLBACK (on_image_button_press), applet_data);

	g_signal_connect (G_OBJECT (applet), "destroy", G_CALLBACK (on_applet_destroy), applet_data);
	
	panel_applet_setup_menu_from_file (PANEL_APPLET (applet), PACKAGE_DIR, "GNOME_INM_menu.xml", NULL, menu_callbacks, applet_data);

	check_inm_url (applet_data);
	applet_data->timer = g_timeout_add(applet_data->interval * INTERVAL_TIME, (GtkFunction)check_inm_url, applet_data );
	//update_station_data (applet_data);
	gtk_widget_show_all (GTK_WIDGET (applet));
	//gtk_widget_hide (applet_data->event_box[10]);
	//gtk_widget_hide (applet_data->temp_lbl);

	return TRUE;
}

PANEL_APPLET_BONOBO_FACTORY ("OAFIID:GnomeINM_Factory",
                             PANEL_TYPE_APPLET,
                             "GNOME INM forecast Applet",
                             "0",
                             start_applet,
                             NULL);
