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

#include "gui.h"


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
	PanelAppletOrient panel_orient;

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
		
		panel_orient = panel_applet_get_orient (PANEL_APPLET(applet_data->applet));
		if (panel_orient == PANEL_APPLET_ORIENT_DOWN){ // panel on the up side
			y += applet_data->applet_size;
			gtk_window_move (GTK_WINDOW(win), x, y);
			gtk_widget_show (win);
		}
		else if (panel_orient == PANEL_APPLET_ORIENT_UP){ // panel on the bottom side
			y -= applet_data->applet_size;
			gtk_window_move (GTK_WINDOW(win), x, y);
			gtk_widget_show (win);
		}
		else if (panel_orient == PANEL_APPLET_ORIENT_LEFT){ // panel on the right side
			x -= applet_data->applet_size;
			gtk_window_move (GTK_WINDOW(win), x, y); 
			gtk_widget_show (win);
		}
		else if (panel_orient == PANEL_APPLET_ORIENT_RIGHT){ // panel on the left side...
			x += applet_data->applet_size;
			gtk_window_move (GTK_WINDOW(win), x, y);
			gtk_widget_show (win);
		}
			
		x = y = 0;

		g_object_unref (G_OBJECT (xml));
		if (day){
			g_free (day);
			day = NULL;
		}
	}
	window_shown = !window_shown;
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

	gtk_window_set_icon_from_file ((GtkWindow *)win, "/usr/share/pixmaps/"PACKAGE".png", NULL);
	
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
	GDate *g_date=NULL;
	GTimeVal g_timeval;
	gchar *str_date=NULL;
	gchar *str_tmp=NULL;

	g_get_current_time (&g_timeval);
	g_date = g_date_new ();
	g_date_set_time_val (g_date, &g_timeval);

	if (!g_date_valid (g_date)){
		printf ("display_spanish_forecast_img(): the g_date object is not valid. I couldn't parse the date\n");
		return;
	}

	xml = glade_xml_new (PACKAGE_DIR"/gnome-inm-glade.glade", "win_radar", NULL);
	win = glade_xml_get_widget (xml, "win_radar");
	img = glade_xml_get_widget (xml, "radar_img");
	combo_hour = glade_xml_get_widget (xml, "combo_hour");
	
	gtk_widget_hide (combo_hour);
	gtk_window_set_icon_from_file ((GtkWindow *)win, "/usr/share/pixmaps/"PACKAGE".png", NULL);

	g_signal_connect (G_OBJECT(win), "destroy", G_CALLBACK(on_window_terminate), img);
	
	str_date = g_new0 (gchar, 24);
	str_tmp = g_new0 (gchar, 1024);
	
	// If the map we want to get, is not the current one, we have to increase
	// the current day by 1 or 2.
	if (strncmp (name, "SpanishForecastImg_Tomorrow0012", strlen(name)) == 0 ||
			strncmp (name, "SpanishForecastImg_Tomorrow1224", strlen(name)) == 0)
		g_date_add_days (g_date, 1);
	//else if (strncmp (name, "SpanishForecastImg_ATomorrow1224", strlen(name)) == 0 ||
	//		strncmp (name, "SpanishForecastImg_ATomorrow0012", strlen(name)) == 0)
	//	g_date_add_days (g_date, 2);

	g_date_strftime (str_date, 24, "%Y%m%d", g_date);
	
	if (strncmp (name, "SpanishForecastImg_Today0012", strlen(name)) == 0 ||
			strncmp (name, "SpanishForecastImg_Tomorrow0012", strlen(name)) == 0)
	//		strncmp (name, "SpanishForecastImg_ATomorrow0012", strlen(name)) == 0 )
		snprintf (str_tmp, 1024, "%s%s%s\0", INM_NATIONAL_FORECAST_IMG, str_date, INM_NATIONAL_FORECAST_0012);
	else if (strncmp (name, "SpanishForecastImg_Today1224", strlen(name)) == 0 ||
			strncmp (name, "SpanishForecastImg_Tomorrow1224", strlen(name)) == 0)
	//		strncmp (name, "SpanishForecastImg_ATomorrow1224", strlen(name)) == 0)
		snprintf (str_tmp, 1024, "%s%s%s\0", INM_NATIONAL_FORECAST_IMG, str_date, INM_NATIONAL_FORECAST_1224);

	printf ("Loading %s\n", str_tmp);
	pixbuf = load_image (str_tmp);
	if (pixbuf){
		gtk_image_set_from_pixbuf (GTK_IMAGE(img), pixbuf);
		gtk_window_set_title (GTK_WINDOW(win), _("Spanish weather forecast image"));
		gtk_widget_show (win);
		g_object_unref (G_OBJECT (pixbuf));
		pixbuf = 0;
	}
	else if (!pixbuf && strncmp (name, "SpanishForecastImg_Tomorrow1224", strlen(name)) == 0)
		message_box ((AppletData *)user_data, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, _("The map weather forecast is not still available"), "Check it out later.");

	g_object_unref (G_OBJECT(xml));
	g_free (str_date);
	g_free (str_tmp);
	g_date_free (g_date);
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
	
	gtk_window_set_icon_from_file ((GtkWindow *)win, "/usr/share/pixmaps/"PACKAGE".png", NULL);

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
	
	gtk_window_set_icon_from_file ((GtkWindow *)win, "/usr/share/pixmaps/"PACKAGE".png", NULL);

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
	
	gtk_window_set_icon_from_file ((GtkWindow *)win, "/usr/share/pixmaps/"PACKAGE".png", NULL);
	
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
	
	gtk_window_set_icon_from_file ((GtkWindow *)win, "/usr/share/pixmaps/"PACKAGE".png", NULL);
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
	
	gtk_window_set_icon_from_file ((GtkWindow *)win, "/usr/share/pixmaps/"PACKAGE".png", NULL);
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

void display_snow_warnings_arg		 ( BonoboUIComponent *uic, gpointer user_data, const char *name )
{
	AppletData *applet_data = (AppletData *) user_data;
	applet_data->gvfs_cnx_type = 6;
	gnome_vfs_async_open (&applet_data->gvfs_handle, INM_SNOW_WARNINGS_URL_3, GNOME_VFS_OPEN_READ, 0, gvfs_status_cb, applet_data);
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
