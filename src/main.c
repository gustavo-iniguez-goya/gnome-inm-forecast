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
#include "gui.h"
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

void message_box 	( AppletData *applet_data, GtkMessageType type, GtkButtonsType buttons, const char *text, const char *secondary_text )
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


void set_tooltip		( AppletData *applet_data, const int id, const gchar* tip )
{
	int x=0;
	char *temp=0;
	char *str_morning=_("Morning");
	char *str_afternoon=_("Afternoon");
	
	if (!applet_data) return;
	
	temp = g_new0 (char, 512);

	if (id == (MAX_DAYS - 1)){ //Afternoon - 9 images
		for (x=1;x < MAX_DAYS;x++){
			if (x == 0 || x == 2 || x == 4)
				snprintf (temp, 512, "%s %s\n%s (%s)\n\n%s\n%s", 
							applet_data->city_name, 
							applet_data->provincia, 
							applet_data->day_info[x].day, 
							str_morning, 
							applet_data->day_info[x].state, 
							applet_data->last_update);
			else if (x == 1 || x == 3 || x == 5)
				snprintf (temp, 512, "%s %s\n%s (%s)\n\n%s\n%s", 
							applet_data->city_name, 
							applet_data->provincia, 
							applet_data->day_info[x].day, 
							str_afternoon, 
							applet_data->day_info[x].state, 
							applet_data->last_update);
			else
				snprintf (temp, 512, "%s %s\n%s\n\n%s\n%s", 
							applet_data->city_name, 
							applet_data->provincia, 
							applet_data->day_info[x].day, 
							tip, 
							applet_data->last_update);
				
	
			gtk_tooltips_set_tip (applet_data->tips, applet_data->event_box[x-1], temp, NULL);
			gtk_tooltips_enable (applet_data->tips);
		}
	}
	else if (id == MAX_DAYS){ // Morning - 10 images
		for (x=0;x < id;x++){
			// FIXME: We should take in mind the number of days displayed on the panel
			if (x == 0 || x == 2 || x == 4)
				snprintf (temp, 512, "%s %s\n%s (%s)\n\n%s\n%s", 
							applet_data->city_name, 
							applet_data->provincia, 
							applet_data->day_info[x].day, 
							str_morning, 
							applet_data->day_info[x].state, 
							applet_data->last_update);
			else if (x == 1 || x == 3 || x == 5)
				snprintf (temp, 512, "%s %s\n%s (%s)\n\n%s\n%s", 
							applet_data->city_name, 
							applet_data->provincia, 
							applet_data->day_info[x].day, 
							str_afternoon, 
							applet_data->day_info[x].state, 
							applet_data->last_update);
			else
				snprintf (temp, 512, "%s %s\n%s\n\n%s\n%s", 
							applet_data->city_name, 
							applet_data->provincia, 
							applet_data->day_info[x].day, 
							tip, 
							applet_data->last_update);

			gtk_tooltips_set_tip (applet_data->tips, applet_data->event_box[x], temp, NULL);
			gtk_tooltips_enable (applet_data->tips);
		}
	}
	else if (id == -1){
		for (x=0;x < MAX_DAYS;x++){
			gtk_tooltips_set_tip (applet_data->tips, applet_data->event_box[x], "", NULL);
		}
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

/*
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

					if (strcmp(suffix[i], "TT") == 0){ // Temperatura
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
					if (strcmp(suffix[i], "RM") == 0){ // Hora oficial 
						snprintf (temp, 128, _("Maximum wind velocity: %s Km/h\n"), tokens[3]);
						strncat (tp, temp, 50);
					}
					if (strcmp(suffix[i], "P0") == 0){ // Presion atmosferica 
						snprintf (temp, 128, _("Atmosferic presion: %s\n"), tokens[3]);
						strncat (tp, temp, 50);
					}
					if (strcmp(suffix[i], "PP") == 0){ // Precipitacion acumulada 
						snprintf (temp, 128, _("Rainfall: %s\n"), tokens[3]);
						strncat (tp, temp, 50);
					}
					if (strcmp(suffix[i], "TR") == 0){ // Humedad relativa 
						snprintf (temp, 128, _("Humidity: %s %\n"), tokens[3]);
						strncat (tp, temp, 50);
					}
					if (strcmp(suffix[i], "VV") == 0){ // Direccion del viento
						snprintf (temp, 128, _("Wind velocity: %s Km/h\n"), tokens[3]);
						strncat (tp, temp, 50);
					}
					if (strcmp(suffix[i], "DV") == 0){ // Direccion del viento 
						snprintf (temp, 128, _("Wind: %s\n"), tokens[3]);
						strncat (tp, temp, 50);
					}
					if (strcmp(suffix[i], "HO") == 0){ // Hora oficial 
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
*/

gboolean check_inm_url 			( AppletData *applet_data )
{
	
	if (applet_data && applet_data->applet){
		gchar *temp = NULL;
		temp = g_new0 (char, 512);
		char *code = g_new0(char, 256);
	
		g_strlcpy (code, (char *)panel_applet_gconf_get_string (PANEL_APPLET(applet_data->applet), "code", NULL), 256);
		g_snprintf (temp, 512, "%s%s.xml\0", INM_FORECAST_URL, code);
		printf ("url: %s\n", temp);

		gnome_vfs_async_open (&applet_data->gvfs_handle, temp, GNOME_VFS_OPEN_READ, 0, check_inm_url_status, applet_data);
		g_free (temp);
		g_free (code);

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
		BONOBO_UI_VERB ("SpanishForecastImg_Today0012", display_spanish_forecast_img),
		BONOBO_UI_VERB ("SpanishForecastImg_Today1224", display_spanish_forecast_img),
		BONOBO_UI_VERB ("SpanishForecastImg_Tomorrow0012", display_spanish_forecast_img),
		BONOBO_UI_VERB ("SpanishForecastImg_Tomorrow1224", display_spanish_forecast_img),
		//BONOBO_UI_VERB ("SpanishForecastImg_ATomorrow0012", display_spanish_forecast_img),
		//BONOBO_UI_VERB ("SpanishForecastImg_ATomorrow1224", display_spanish_forecast_img),
		BONOBO_UI_VERB ("Temperatures", display_daily_temperatures),
		BONOBO_UI_VERB ("RainfallProb", display_prob_precipitation),
		BONOBO_UI_VERB ("AirMassProb", display_air_mass),
		BONOBO_UI_VERB ("RadarPeninsulaProb", display_radar),
		BONOBO_UI_VERB ("RadarCanariasProb", display_radar),
		BONOBO_UI_VERB ("RayosP", display_lightnings),
		BONOBO_UI_VERB ("RayosC", display_lightnings),
		BONOBO_UI_VERB ("InformeAludes1", display_snow_warnings_nav),
		BONOBO_UI_VERB ("InformeAludes2", display_snow_warnings_arg),
		BONOBO_UI_VERB ("InformeAludes3", display_snow_warnings_cat),
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

	printf ("start_applet()\n");

	AppletData *applet_data = g_new0(AppletData, 1);
	applet_data->prefs = g_new0 (PrefsWin, 1);
	applet_data->day_info = g_new0 (DayInf, MAX_DAYS);
	applet_data->prefs->code = g_new0 (char, 256);
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
		applet_data->day_info[x].wind = g_new0 (char, 32);
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
