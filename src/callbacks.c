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

#include "callbacks.h"

const char *str_hours[] = { 
	"00", "01", "02", "03",
	"04", "05", "06", "07",
	"08", "09", "10", "11",
	"12", "13", "14", "15",
	"16", "17", "18", "19",
	"20", "21", "22", "23",
};


void on_radar_img_size_allocate 	( GtkWidget *widget, GtkAllocation *allocate, gpointer data )
{
/*
	GdkPixbuf *pix=0;
	int w=0, h=0;
	GtkImage *img = (GtkImage *) data;
	pix = gtk_image_get_pixbuf (GTK_IMAGE(img));
	//w = gdk_pixbuf_get_width (pix);
	//h = gdk_pixbuf_get_height (pix);
	gtk_window_get_size (GTK_WINDOW(widget), &w, &h);

	gtk_image_set_from_pixbuf (GTK_IMAGE(img), 
		gdk_pixbuf_scale_simple ( pix, w, h, GDK_INTERP_BILINEAR)
	);
*/
}

void on_window_terminate 	( GtkWidget *widget, gpointer data )
{
	GtkWidget *img = (GtkWidget *) data;
	if (img){
		GdkPixbuf *pix = gtk_image_get_pixbuf (GTK_IMAGE(img));
		if (pix){
			g_object_unref (G_OBJECT(pix));
			pix = 0;
		}
	}
	
	quit (widget);
}

void on_daily_temp_rb1_toggled			 ( GtkWidget *widget, gpointer data )
{
	GtkImage *img = (GtkImage *) data;
	GdkPixbuf *pixbuf;
	if (img){
		pixbuf = load_image (INM_DAILY_TEMPERATURE_MIN);
		if (pixbuf){
			gtk_image_set_from_pixbuf (GTK_IMAGE(img), pixbuf);
			gtk_widget_show (GTK_WIDGET (img));
			g_object_unref (G_OBJECT (pixbuf));
			pixbuf = 0;
		}
	}
}

void on_daily_temp_rb2_toggled			 ( GtkWidget *widget, gpointer data )
{
	GtkImage *img = (GtkImage *) data;
	GdkPixbuf *pixbuf;
	if (img){
		pixbuf = load_image (INM_DAILY_TEMPERATURE_MAX);
		if (pixbuf){
			gtk_image_set_from_pixbuf (GTK_IMAGE(img), pixbuf);
			gtk_widget_show (GTK_WIDGET(img));
			g_object_unref (G_OBJECT (pixbuf));
			pixbuf = 0;
		}
	}
}

void on_sat_combo_hour_changed			 ( GtkWidget *widget, gpointer data )
{
	GtkImage *img = (GtkImage *) data;
	GdkPixbuf *pixbuf;
	#define rdr_size 512
	char img_radar[rdr_size]={0};
	if (img){
		GDate *d = NULL;
		d = g_date_new ();
		g_date_set_time_t (d, time(NULL));

		g_snprintf ((char *)&img_radar, rdr_size, "%s%d%.2d%.2d%s%s", INM_SAT_IMG, g_date_get_year(d), g_date_get_month(d), g_date_get_day(d), str_hours[gtk_combo_box_get_active(GTK_COMBO_BOX(widget))], "00_s93g.gif");
		printf ("Hour: %s - %d\n%s\n", str_hours[gtk_combo_box_get_active(GTK_COMBO_BOX(widget))], gtk_combo_box_get_active(GTK_COMBO_BOX(widget)), img_radar);

		pixbuf = load_image ((char *)&img_radar);
		if (pixbuf){
			gtk_image_set_from_pixbuf (GTK_IMAGE(img), pixbuf);
			g_object_unref (G_OBJECT (pixbuf));
			pixbuf = 0;
		}

		g_date_free (d);
	}
}

void on_rayos_combo_hour_changed			 ( GtkWidget *widget, gpointer data )
{
	GtkImage *img = (GtkImage *) data;
	GdkPixbuf *pixbuf;
	#define rdr_size 512
	char img_radar[rdr_size]={0};
	if (img){
		GDate *d = NULL;
		d = g_date_new ();
		g_date_set_time_t (d, time(NULL));

		g_snprintf ((char *)&img_radar, rdr_size, "%s%d%.2d%.2d%s%s", INM_RAYOS_IMG, g_date_get_year(d), g_date_get_month(d), g_date_get_day(d), str_hours[gtk_combo_box_get_active(GTK_COMBO_BOX(widget))], "00_r79g.gif");
		printf ("Hour: %s - %d\n%s\n", str_hours[gtk_combo_box_get_active(GTK_COMBO_BOX(widget))], gtk_combo_box_get_active(GTK_COMBO_BOX(widget)), img_radar);

		pixbuf = load_image ((char *)&img_radar);
		if (pixbuf){
			gtk_image_set_from_pixbuf (GTK_IMAGE(img), pixbuf);
			g_object_unref (G_OBJECT (pixbuf));
			pixbuf = 0;
		}
		
		g_date_free (d);
	}
}

void on_rayos_canarias_combo_hour_changed		 ( GtkWidget *widget, gpointer data )
{
	GtkImage *img = (GtkImage *) data;
	GdkPixbuf *pixbuf;
	#define rdr_size 512
	char img_radar[rdr_size]={0};
	if (img){
		GDate *d = NULL;
		d = g_date_new ();
		g_date_set_time_t (d, time(NULL));

		g_snprintf ((char *)&img_radar, rdr_size, "%s%d%.2d%.2d%s%s", INM_RAYOS_CANARIAS_IMG, g_date_get_year(d), g_date_get_month(d), g_date_get_day(d), str_hours[gtk_combo_box_get_active(GTK_COMBO_BOX(widget))], "00_r78g.gif");

		pixbuf = load_image ((char *)&img_radar);
		if (pixbuf){
			gtk_image_set_from_pixbuf (GTK_IMAGE(img), pixbuf);
			g_object_unref (G_OBJECT (pixbuf));
			pixbuf = 0;
		}

		g_date_free (d);
	}
}

void on_radar_combo_hour_changed			 ( GtkWidget *widget, gpointer data )
{
	GtkImage *img = (GtkImage *) data;
	GdkPixbuf *pixbuf;
	#define rdr_size 512
	char img_radar[rdr_size]={0};
	if (img){
		GDate *d = NULL;
		d = g_date_new ();
		g_date_set_time_t (d, time(NULL));

		g_snprintf ((char *)&img_radar, rdr_size, "%s%d%.2d%.2d%s%s", INM_RADAR_IMG, g_date_get_year(d), g_date_get_month(d), g_date_get_day(d), str_hours[gtk_combo_box_get_active(GTK_COMBO_BOX(widget))], "00_r8pb.gif");

		pixbuf = load_image ((char *)&img_radar);
		if (pixbuf){
			gtk_image_set_from_pixbuf (GTK_IMAGE(img), pixbuf);
			g_object_unref (G_OBJECT (pixbuf));
			pixbuf = 0;
		}

		g_date_free (d);
	}
}

void on_radar_canarias_combo_hour_changed		 ( GtkWidget *widget, gpointer data )
{
	GtkImage *img = (GtkImage *) data;
	GdkPixbuf *pixbuf;
	#define rdr_size 512
	char img_radar[rdr_size]={0};
	if (img){
		GDate *d = NULL;
		d = g_date_new ();
		g_date_set_time_t (d, time(NULL));

		g_snprintf ((char *)&img_radar, rdr_size, "%s%d%.2d%.2d%s%s", INM_RADAR_CANARIAS_IMG, g_date_get_year(d), g_date_get_month(d), g_date_get_day(d), str_hours[gtk_combo_box_get_active(GTK_COMBO_BOX(widget))], "00_r98g.gif");

		pixbuf = load_image ((char *)&img_radar);
		if (pixbuf){
			gtk_image_set_from_pixbuf (GTK_IMAGE(img), pixbuf);
			g_object_unref (G_OBJECT (pixbuf));
			pixbuf = 0;
		}

		g_date_free (d);
	}
}

void on_airmass_combo_hour_changed		 ( GtkWidget *widget, gpointer data )
{
	GtkImage *img = (GtkImage *) data;
	GdkPixbuf *pixbuf;
	#define rdr_size 512
	char img_radar[rdr_size]={0};

	if (img){
		GDate *d = NULL;
		d = g_date_new ();
		g_date_set_time_t (d, time(NULL));

		g_snprintf ((char *)&img_radar, rdr_size, "%s%d%.2d%.2d%s%s", INM_RADAR_AIR_MASS_IMG, g_date_get_year(d), g_date_get_month(d), g_date_get_day(d), str_hours[gtk_combo_box_get_active(GTK_COMBO_BOX(widget))], "00_s91g.jpg");
		printf ("Hour: %s - %d\n%s\n", str_hours[gtk_combo_box_get_active(GTK_COMBO_BOX(widget))], gtk_combo_box_get_active(GTK_COMBO_BOX(widget)), img_radar);

		pixbuf = load_image ((char *)&img_radar);
		if (pixbuf){
			gtk_image_set_from_pixbuf (GTK_IMAGE(img), pixbuf);
			g_object_unref (G_OBJECT (pixbuf));
			pixbuf = 0;
		}
	
		g_date_free (d);
	}
}

void on_prob_precip_toggled_005 ( GtkWidget *widget, gpointer data )
{
	GtkImage *img = (GtkImage *) data;
	GdkPixbuf *pixbuf=0;

	pixbuf = load_image (INM_PROB_PRECIP_005);
	if (pixbuf){
		gtk_image_set_from_pixbuf (GTK_IMAGE(img), pixbuf);
		g_object_unref (G_OBJECT (pixbuf));
		pixbuf = 0;
	}
}

void on_prob_precip_toggled_020 ( GtkWidget *widget, gpointer data )
{
	GtkImage *img = (GtkImage *) data;
	GdkPixbuf *pixbuf=0;

	pixbuf = load_image (INM_PROB_PRECIP_020);
	if (pixbuf){
		gtk_image_set_from_pixbuf (GTK_IMAGE(img), pixbuf);
		g_object_unref (G_OBJECT (pixbuf));
		pixbuf = 0;
	}
}

void on_prob_precip_toggled_100 ( GtkWidget *widget, gpointer data )
{
	GtkImage *img = (GtkImage *) data;
	GdkPixbuf *pixbuf=0;

	pixbuf = load_image (INM_PROB_PRECIP_100);
	if (pixbuf){
		gtk_image_set_from_pixbuf (GTK_IMAGE(img), pixbuf);
		g_object_unref (G_OBJECT (pixbuf));
		pixbuf = 0;
	}
}

void on_prob_precip_toggled_200 ( GtkWidget *widget, gpointer data )
{
	GtkImage *img = (GtkImage *) data;
	GdkPixbuf *pixbuf;
	printf ("precip_200: %s\n", widget->name);
	pixbuf = load_image (INM_PROB_PRECIP_200);
	if (pixbuf){
		gtk_image_set_from_pixbuf (GTK_IMAGE(img), pixbuf);
		g_object_unref (G_OBJECT (pixbuf));
		pixbuf = 0;
	}
}

void on_prob_precip_toggled_avg ( GtkWidget *widget, gpointer data )
{
	GtkImage *img = (GtkImage *) data;
	GdkPixbuf *pixbuf=0;

	pixbuf = load_image (INM_PROB_PRECIP_AVG);
	if (pixbuf){
		gtk_image_set_from_pixbuf (GTK_IMAGE(img), pixbuf);
		g_object_unref (G_OBJECT (pixbuf));
		pixbuf = 0;
	}
}

void on_prob_precip_toggled_max ( GtkWidget *widget, gpointer data )
{
	GtkImage *img = (GtkImage *) data;
	GdkPixbuf *pixbuf=0;

	pixbuf = load_image (INM_PROB_PRECIP_MAX);
	if (pixbuf){
		gtk_image_set_from_pixbuf (GTK_IMAGE(img), pixbuf);
		g_object_unref (G_OBJECT (pixbuf));
		pixbuf = 0;
	}
}

void on_applet_destroy 			( GtkWidget *widget, AppletData *applet_data )
{
	int x=0;
	printf ("Applet destroy\n");
	if (applet_data){
		gtk_timeout_remove (applet_data->timer);
		gtk_widget_destroy (applet_data->temp_lbl);
		g_object_ref_sink (G_OBJECT (applet_data->tips));
		g_object_unref (G_OBJECT (applet_data->tips));
		for (x=0;x < 10;x++){
			g_free (applet_data->day_info[x].state);
			g_free (applet_data->day_info[x].precip);
			g_free (applet_data->day_info[x].cota_nieve);
			g_free (applet_data->day_info[x].t_max);
			g_free (applet_data->day_info[x].t_min);
			g_free (applet_data->day_info[x].day);
			g_free (applet_data->day_info[x].humidity);
			g_free (applet_data->day_info[x].wind);
			gtk_container_remove (GTK_CONTAINER (applet_data->event_box[x]), applet_data->image[x]);
		}
		g_free (applet_data->day_info);
		g_free (applet_data->prefs->code);
		g_free (applet_data->prefs->station_code);
		g_free (applet_data->prefs);
		g_free (applet_data->show_days);
		g_free (applet_data->theme);
		g_free (applet_data->city_name);
		g_free (applet_data->city_long_desc);
		g_free (applet_data->last_update);
		g_free (applet_data->provincia);
		if (applet_data->buffer){
			g_free (applet_data->buffer);
		}
		gtk_widget_destroy (applet_data->hbox);
		gtk_widget_destroy (GTK_WIDGET (applet_data->applet));
		g_free (applet_data);

		// free allocated memory by gnome vfs subsystem
		gnome_vfs_shutdown ();
	}

}

gboolean on_image_button_press 	( GtkWidget *event_box, GdkEventButton *event, AppletData *applet_data )
{
	if (event->button != 1)
		return FALSE;
		
	//printf ("event: %s\n", event_box->name);
	/*
	if (strcmp (event_box->name, "event11") == 0)
		check_latest_data (applet_data);
	else
	*/
	create_window ( applet_data, event_box->name );

	return TRUE;
}
