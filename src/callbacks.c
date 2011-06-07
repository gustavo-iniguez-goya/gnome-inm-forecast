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

// FIXME: Create a common function for this if()'s
void on_sat_combo_hour_changed			 ( GtkWidget *widget, gpointer data )
{
	GtkImage *img = (GtkImage *) data;
	GdkPixbuf *pixbuf;
	char *tmp_buf=0;
	#define rdr_size 512
	char img_radar[rdr_size]={0};
	if (img){
		tmp_buf = gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget));
		if (strncmp (tmp_buf, "19:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_SAT_IMG, "1900.gif");
		}
		else if (strncmp (tmp_buf, "20:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_SAT_IMG, "2000.gif");
		}
		else if (strncmp (tmp_buf, "21:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_SAT_IMG, "2100.gif");
		}
		else if (strncmp (tmp_buf, "22:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_SAT_IMG, "2200.gif");
		}
		else if (strncmp (tmp_buf, "23:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_SAT_IMG, "2300.gif");
		}
		else if (strncmp (tmp_buf, "00:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_SAT_IMG, "0000.gif");
		}
		else if (strncmp (tmp_buf, "01:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_SAT_IMG, "0100.gif");
		}
		else if (strncmp (tmp_buf, "02:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_SAT_IMG, "0200.gif");
		}
		else if (strncmp (tmp_buf, "03:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_SAT_IMG, "0300.gif");
		}
		else if (strncmp (tmp_buf, "04:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_SAT_IMG, "0400.gif");
		}
		else if (strncmp (tmp_buf, "05:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_SAT_IMG, "0500.gif");
		}
		else if (strncmp (tmp_buf, "06:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_SAT_IMG, "0600.gif");
		}
		else if (strncmp (tmp_buf, "07:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_SAT_IMG, "0700.gif");
		}
		else if (strncmp (tmp_buf, "08:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_SAT_IMG, "0800.gif");
		}
		else if (strncmp (tmp_buf, "09:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_SAT_IMG, "0900.gif");
		}
		else if (strncmp (tmp_buf, "10:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_SAT_IMG, "1000.gif");
		}
		else if (strncmp (tmp_buf, "11:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_SAT_IMG, "1100.gif");
		}
		else if (strncmp (tmp_buf, "12:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_SAT_IMG, "1200.gif");
		}
		else if (strncmp (tmp_buf, "13:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_SAT_IMG, "1300.gif");
		}
		else if (strncmp (tmp_buf, "14:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_SAT_IMG, "1400.gif");
		}
		else if (strncmp (tmp_buf, "15:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_SAT_IMG, "1500.gif");
		}
		else if (strncmp (tmp_buf, "16:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_SAT_IMG, "1600.gif");
		}
		else if (strncmp (tmp_buf, "17:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_SAT_IMG, "1700.gif");
		}
		else if (strncmp (tmp_buf, "18:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_SAT_IMG, "1800.gif");
		}
		else
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_SAT_IMG, "1900.gif");

		pixbuf = load_image ((char *)&img_radar);
		if (pixbuf){
			gtk_image_set_from_pixbuf (GTK_IMAGE(img), pixbuf);
			g_object_unref (G_OBJECT (pixbuf));
			pixbuf = 0;
		}
		if (tmp_buf)
			g_free (tmp_buf);
	}
}

// FIXME: Create a common function for this if()'s
void on_rayos_combo_hour_changed			 ( GtkWidget *widget, gpointer data )
{
	GtkImage *img = (GtkImage *) data;
	GdkPixbuf *pixbuf;
	char *tmp_buf=0;
	#define rdr_size 512
	char img_radar[rdr_size]={0};
	if (img){
		tmp_buf = gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget));
		if (strncmp (tmp_buf, "19:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RAYOS_IMG, "1900.gif");
		}
		else if (strncmp (tmp_buf, "20:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RAYOS_IMG, "2000.gif");
		}
		else if (strncmp (tmp_buf, "21:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RAYOS_IMG, "2100.gif");
		}
		else if (strncmp (tmp_buf, "22:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RAYOS_IMG, "2200.gif");
		}
		else if (strncmp (tmp_buf, "23:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RAYOS_IMG, "2300.gif");
		}
		else if (strncmp (tmp_buf, "00:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RAYOS_IMG, "0000.gif");
		}
		else if (strncmp (tmp_buf, "01:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RAYOS_IMG, "0100.gif");
		}
		else if (strncmp (tmp_buf, "02:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RAYOS_IMG, "0200.gif");
		}
		else if (strncmp (tmp_buf, "03:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RAYOS_IMG, "0300.gif");
		}
		else if (strncmp (tmp_buf, "04:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RAYOS_IMG, "0400.gif");
		}
		else if (strncmp (tmp_buf, "05:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RAYOS_IMG, "0500.gif");
		}
		else if (strncmp (tmp_buf, "06:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RAYOS_IMG, "0600.gif");
		}
		else if (strncmp (tmp_buf, "07:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RAYOS_IMG, "0700.gif");
		}
		else if (strncmp (tmp_buf, "08:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RAYOS_IMG, "0800.gif");
		}
		else if (strncmp (tmp_buf, "09:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RAYOS_IMG, "0900.gif");
		}
		else if (strncmp (tmp_buf, "10:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RAYOS_IMG, "1000.gif");
		}
		else if (strncmp (tmp_buf, "11:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RAYOS_IMG, "1100.gif");
		}
		else if (strncmp (tmp_buf, "12:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RAYOS_IMG, "1200.gif");
		}
		else if (strncmp (tmp_buf, "13:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RAYOS_IMG, "1300.gif");
		}
		else if (strncmp (tmp_buf, "14:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RAYOS_IMG, "1400.gif");
		}
		else if (strncmp (tmp_buf, "15:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RAYOS_IMG, "1500.gif");
		}
		else if (strncmp (tmp_buf, "16:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RAYOS_IMG, "1600.gif");
		}
		else if (strncmp (tmp_buf, "17:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RAYOS_IMG, "1700.gif");
		}
		else if (strncmp (tmp_buf, "18:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RAYOS_IMG, "1800.gif");
		}
		else
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RAYOS_IMG, "1900.gif");

		pixbuf = load_image ((char *)&img_radar);
		if (pixbuf){
			gtk_image_set_from_pixbuf (GTK_IMAGE(img), pixbuf);
			g_object_unref (G_OBJECT (pixbuf));
			pixbuf = 0;
		}
		if (tmp_buf)
			g_free (tmp_buf);
	}
}

// FIXME: Create a common function for this if()'s
void on_rayos_canarias_combo_hour_changed		 ( GtkWidget *widget, gpointer data )
{
	GtkImage *img = (GtkImage *) data;
	GdkPixbuf *pixbuf;
	char *tmp_buf=0;
	#define rdr_size 512
	char img_radar[rdr_size]={0};
	if (img){
		tmp_buf = gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget));
		if (strncmp (tmp_buf, "19:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RAYOS_CANARIAS_IMG, "1900.gif");
		}
		else if (strncmp (tmp_buf, "20:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RAYOS_CANARIAS_IMG, "2000.gif");
		}
		else if (strncmp (tmp_buf, "21:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RAYOS_CANARIAS_IMG, "2100.gif");
		}
		else if (strncmp (tmp_buf, "22:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RAYOS_CANARIAS_IMG, "2200.gif");
		}
		else if (strncmp (tmp_buf, "23:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RAYOS_CANARIAS_IMG, "2300.gif");
		}
		else if (strncmp (tmp_buf, "00:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RAYOS_CANARIAS_IMG, "0000.gif");
		}
		else if (strncmp (tmp_buf, "01:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RAYOS_CANARIAS_IMG, "0100.gif");
		}
		else if (strncmp (tmp_buf, "02:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RAYOS_CANARIAS_IMG, "0200.gif");
		}
		else if (strncmp (tmp_buf, "03:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RAYOS_CANARIAS_IMG, "0300.gif");
		}
		else if (strncmp (tmp_buf, "04:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RAYOS_CANARIAS_IMG, "0400.gif");
		}
		else if (strncmp (tmp_buf, "05:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RAYOS_CANARIAS_IMG, "0500.gif");
		}
		else if (strncmp (tmp_buf, "06:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RAYOS_CANARIAS_IMG, "0600.gif");
		}
		else if (strncmp (tmp_buf, "07:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RAYOS_CANARIAS_IMG, "0700.gif");
		}
		else if (strncmp (tmp_buf, "08:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RAYOS_CANARIAS_IMG, "0800.gif");
		}
		else if (strncmp (tmp_buf, "09:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RAYOS_CANARIAS_IMG, "0900.gif");
		}
		else if (strncmp (tmp_buf, "10:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RAYOS_CANARIAS_IMG, "1000.gif");
		}
		else if (strncmp (tmp_buf, "11:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RAYOS_CANARIAS_IMG, "1100.gif");
		}
		else if (strncmp (tmp_buf, "12:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RAYOS_CANARIAS_IMG, "1200.gif");
		}
		else if (strncmp (tmp_buf, "13:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RAYOS_CANARIAS_IMG, "1300.gif");
		}
		else if (strncmp (tmp_buf, "14:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RAYOS_CANARIAS_IMG, "1400.gif");
		}
		else if (strncmp (tmp_buf, "15:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RAYOS_CANARIAS_IMG, "1500.gif");
		}
		else if (strncmp (tmp_buf, "16:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RAYOS_CANARIAS_IMG, "1600.gif");
		}
		else if (strncmp (tmp_buf, "17:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RAYOS_CANARIAS_IMG, "1700.gif");
		}
		else if (strncmp (tmp_buf, "18:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RAYOS_CANARIAS_IMG, "1800.gif");
		}
		else
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RAYOS_CANARIAS_IMG, "1900.gif");

		pixbuf = load_image ((char *)&img_radar);
		if (pixbuf){
			gtk_image_set_from_pixbuf (GTK_IMAGE(img), pixbuf);
			g_object_unref (G_OBJECT (pixbuf));
			pixbuf = 0;
		}
		if (tmp_buf)
			g_free (tmp_buf);
	}
}

// FIXME: Create a common function for this if()'s
void on_radar_combo_hour_changed			 ( GtkWidget *widget, gpointer data )
{
	GtkImage *img = (GtkImage *) data;
	GdkPixbuf *pixbuf;
	char *tmp_buf=0;
	#define rdr_size 512
	char img_radar[rdr_size]={0};
	if (img){
		tmp_buf = gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget));
		if (strncmp (tmp_buf, "19:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_IMG, "1900.gif");
		}
		else if (strncmp (tmp_buf, "20:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_IMG, "2000.gif");
		}
		else if (strncmp (tmp_buf, "21:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_IMG, "2100.gif");
		}
		else if (strncmp (tmp_buf, "22:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_IMG, "2200.gif");
		}
		else if (strncmp (tmp_buf, "23:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_IMG, "2300.gif");
		}
		else if (strncmp (tmp_buf, "00:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_IMG, "0000.gif");
		}
		else if (strncmp (tmp_buf, "01:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_IMG, "0100.gif");
		}
		else if (strncmp (tmp_buf, "02:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_IMG, "0200.gif");
		}
		else if (strncmp (tmp_buf, "03:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_IMG, "0300.gif");
		}
		else if (strncmp (tmp_buf, "04:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_IMG, "0400.gif");
		}
		else if (strncmp (tmp_buf, "05:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_IMG, "0500.gif");
		}
		else if (strncmp (tmp_buf, "06:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_IMG, "0600.gif");
		}
		else if (strncmp (tmp_buf, "07:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_IMG, "0700.gif");
		}
		else if (strncmp (tmp_buf, "08:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_IMG, "0800.gif");
		}
		else if (strncmp (tmp_buf, "09:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_IMG, "0900.gif");
		}
		else if (strncmp (tmp_buf, "10:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_IMG, "1000.gif");
		}
		else if (strncmp (tmp_buf, "11:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_IMG, "1100.gif");
		}
		else if (strncmp (tmp_buf, "12:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_IMG, "1200.gif");
		}
		else if (strncmp (tmp_buf, "13:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_IMG, "1300.gif");
		}
		else if (strncmp (tmp_buf, "14:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_IMG, "1400.gif");
		}
		else if (strncmp (tmp_buf, "15:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_IMG, "1500.gif");
		}
		else if (strncmp (tmp_buf, "16:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_IMG, "1600.gif");
		}
		else if (strncmp (tmp_buf, "17:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_IMG, "1700.gif");
		}
		else if (strncmp (tmp_buf, "18:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_IMG, "1800.gif");
		}
		else
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_IMG, "1900.gif");

		pixbuf = load_image ((char *)&img_radar);
		if (pixbuf){
			gtk_image_set_from_pixbuf (GTK_IMAGE(img), pixbuf);
			g_object_unref (G_OBJECT (pixbuf));
			pixbuf = 0;
		}
		if (tmp_buf)
			g_free (tmp_buf);
	}
}

// FIXME: Create a common function for this if()'s
void on_radar_canarias_combo_hour_changed		 ( GtkWidget *widget, gpointer data )
{
	GtkImage *img = (GtkImage *) data;
	GdkPixbuf *pixbuf;
	char *tmp_buf=0;
	#define rdr_size 512
	char img_radar[rdr_size]={0};
	if (img){
		tmp_buf = gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget));
		if (strncmp (tmp_buf, "19:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_CANARIAS_IMG, "1900.gif");
		}
		else if (strncmp (tmp_buf, "20:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_CANARIAS_IMG, "2000.gif");
		}
		else if (strncmp (tmp_buf, "21:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_CANARIAS_IMG, "2100.gif");
		}
		else if (strncmp (tmp_buf, "22:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_CANARIAS_IMG, "2200.gif");
		}
		else if (strncmp (tmp_buf, "23:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_CANARIAS_IMG, "2300.gif");
		}
		else if (strncmp (tmp_buf, "00:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_CANARIAS_IMG, "0000.gif");
		}
		else if (strncmp (tmp_buf, "01:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_CANARIAS_IMG, "0100.gif");
		}
		else if (strncmp (tmp_buf, "02:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_CANARIAS_IMG, "0200.gif");
		}
		else if (strncmp (tmp_buf, "03:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_CANARIAS_IMG, "0300.gif");
		}
		else if (strncmp (tmp_buf, "04:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_CANARIAS_IMG, "0400.gif");
		}
		else if (strncmp (tmp_buf, "05:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_CANARIAS_IMG, "0500.gif");
		}
		else if (strncmp (tmp_buf, "06:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_CANARIAS_IMG, "0600.gif");
		}
		else if (strncmp (tmp_buf, "07:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_CANARIAS_IMG, "0700.gif");
		}
		else if (strncmp (tmp_buf, "08:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_CANARIAS_IMG, "0800.gif");
		}
		else if (strncmp (tmp_buf, "09:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_CANARIAS_IMG, "0900.gif");
		}
		else if (strncmp (tmp_buf, "10:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_CANARIAS_IMG, "1000.gif");
		}
		else if (strncmp (tmp_buf, "11:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_CANARIAS_IMG, "1100.gif");
		}
		else if (strncmp (tmp_buf, "12:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_CANARIAS_IMG, "1200.gif");
		}
		else if (strncmp (tmp_buf, "13:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_CANARIAS_IMG, "1300.gif");
		}
		else if (strncmp (tmp_buf, "14:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_CANARIAS_IMG, "1400.gif");
		}
		else if (strncmp (tmp_buf, "15:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_CANARIAS_IMG, "1500.gif");
		}
		else if (strncmp (tmp_buf, "16:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_CANARIAS_IMG, "1600.gif");
		}
		else if (strncmp (tmp_buf, "17:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_CANARIAS_IMG, "1700.gif");
		}
		else if (strncmp (tmp_buf, "18:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_CANARIAS_IMG, "1800.gif");
		}
		else
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_CANARIAS_IMG, "1900.gif");

		pixbuf = load_image ((char *)&img_radar);
		if (pixbuf){
			gtk_image_set_from_pixbuf (GTK_IMAGE(img), pixbuf);
			g_object_unref (G_OBJECT (pixbuf));
			pixbuf = 0;
		}
		if (tmp_buf)
			g_free (tmp_buf);
	}
}

// FIXME: Create a common function for this if()'s
void on_airmass_combo_hour_changed		 ( GtkWidget *widget, gpointer data )
{
	GtkImage *img = (GtkImage *) data;
	GdkPixbuf *pixbuf;
	char *tmp_buf=0;
	#define rdr_size 512
	char img_radar[rdr_size]={0};

	if (img){
		GDate *d = NULL;
		d = g_date_new ();
		g_date_set_time_t (d, time(NULL));

		tmp_buf = gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget));
		if (strncmp (tmp_buf, "19:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%d%.2d%.2d%s", INM_RADAR_AIR_MASS_IMG, g_date_get_year(d), g_date_get_month(d), g_date_get_day(d), "1900_s91g.jpg");
		}
		else if (strncmp (tmp_buf, "20:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%d%.2d%.2d%s", INM_RADAR_AIR_MASS_IMG, g_date_get_year(d), g_date_get_month(d), g_date_get_day(d), "2000_s91g.jpg");
		}
		else if (strncmp (tmp_buf, "21:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%d%.2d%.2d%s", INM_RADAR_AIR_MASS_IMG, g_date_get_year(d), g_date_get_month(d), g_date_get_day(d), "2100_s91g.jpg");
		}
		else if (strncmp (tmp_buf, "22:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%d%.2d%.2d%s", INM_RADAR_AIR_MASS_IMG, g_date_get_year(d), g_date_get_month(d), g_date_get_day(d), "2200_s91g.jpg");
		}
		else if (strncmp (tmp_buf, "23:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%d%.2d%.2d%s", INM_RADAR_AIR_MASS_IMG, g_date_get_year(d), g_date_get_month(d), g_date_get_day(d), "2300_s91g.jpg");
		}
		else if (strncmp (tmp_buf, "00:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%d%.2d%.2d%s", INM_RADAR_AIR_MASS_IMG, g_date_get_year(d), g_date_get_month(d), g_date_get_day(d), "0000_s91g.jpg");
		}
		else if (strncmp (tmp_buf, "01:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%d%.2d%.2d%s", INM_RADAR_AIR_MASS_IMG, g_date_get_year(d), g_date_get_month(d), g_date_get_day(d), "0100_s91g.jpg");
		}
		else if (strncmp (tmp_buf, "02:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%d%.2d%.2d%s", INM_RADAR_AIR_MASS_IMG, g_date_get_year(d), g_date_get_month(d), g_date_get_day(d), "0200_s91g.jpg");
		}
		else if (strncmp (tmp_buf, "03:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%d%.2d%.2d%s", INM_RADAR_AIR_MASS_IMG, g_date_get_year(d), g_date_get_month(d), g_date_get_day(d), "0300_s91g.jpg");
		}
		else if (strncmp (tmp_buf, "04:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%d%.2d%.2d%s", INM_RADAR_AIR_MASS_IMG, g_date_get_year(d), g_date_get_month(d), g_date_get_day(d), "0400_s91g.jpg");
		}
		else if (strncmp (tmp_buf, "05:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%d%.2d%.2d%s", INM_RADAR_AIR_MASS_IMG, g_date_get_year(d), g_date_get_month(d), g_date_get_day(d), "0500_s91g.jpg");
		}
		else if (strncmp (tmp_buf, "06:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%d%.2d%.2d%s", INM_RADAR_AIR_MASS_IMG, g_date_get_year(d), g_date_get_month(d), g_date_get_day(d), "0600_s91g.jpg");
		}
		else if (strncmp (tmp_buf, "07:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%d%.2d%.2d%s", INM_RADAR_AIR_MASS_IMG, g_date_get_year(d), g_date_get_month(d), g_date_get_day(d), "0700_s91g.jpg");
		}
		else if (strncmp (tmp_buf, "08:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%d%.2d%.2d%s", INM_RADAR_AIR_MASS_IMG, g_date_get_year(d), g_date_get_month(d), g_date_get_day(d), "0800_s91g.jpg");
		}
		else if (strncmp (tmp_buf, "09:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%d%.2d%.2d%s", INM_RADAR_AIR_MASS_IMG, g_date_get_year(d), g_date_get_month(d), g_date_get_day(d), "0900_s91g.jpg");
		}
		else if (strncmp (tmp_buf, "10:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%d%.2d%.2d%s", INM_RADAR_AIR_MASS_IMG, g_date_get_year(d), g_date_get_month(d), g_date_get_day(d), "1000_s91g.jpg");
		}
		else if (strncmp (tmp_buf, "11:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%d%.2d%.2d%s", INM_RADAR_AIR_MASS_IMG, g_date_get_year(d), g_date_get_month(d), g_date_get_day(d), "1100_s91g.jpg");
		}
		else if (strncmp (tmp_buf, "12:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%d%.2d%.2d%s", INM_RADAR_AIR_MASS_IMG, g_date_get_year(d), g_date_get_month(d), g_date_get_day(d), "1200_s91g.jpg");
		}
		else if (strncmp (tmp_buf, "13:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%d%.2d%.2d%s", INM_RADAR_AIR_MASS_IMG, g_date_get_year(d), g_date_get_month(d), g_date_get_day(d), "1300_s91g.jpg");
		}
		else if (strncmp (tmp_buf, "14:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%d%.2d%.2d%s", INM_RADAR_AIR_MASS_IMG, g_date_get_year(d), g_date_get_month(d), g_date_get_day(d), "1400_s91g.jpg");
		}
		else if (strncmp (tmp_buf, "15:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%d%.2d%.2d%s", INM_RADAR_AIR_MASS_IMG, g_date_get_year(d), g_date_get_month(d), g_date_get_day(d), "1500_s91g.jpg");
		}
		else if (strncmp (tmp_buf, "16:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%d%.2d%.2d%s", INM_RADAR_AIR_MASS_IMG, g_date_get_year(d), g_date_get_month(d), g_date_get_day(d), "1600_s91g.jpg");
		}
		else if (strncmp (tmp_buf, "17:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%d%.2d%.2d%s", INM_RADAR_AIR_MASS_IMG, g_date_get_year(d), g_date_get_month(d), g_date_get_day(d), "1700_s91g.jpg");
		}
		else if (strncmp (tmp_buf, "18:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%d%.2d%.2d%s", INM_RADAR_AIR_MASS_IMG, g_date_get_year(d), g_date_get_month(d), g_date_get_day(d), "1800_s91g.jpg");
		}
		else
			g_snprintf ((char *)&img_radar, rdr_size, "%s%d%.2d%.2d%s", INM_RADAR_AIR_MASS_IMG, g_date_get_year(d), g_date_get_month(d), g_date_get_day(d), "1900_s91g.jpg");

		pixbuf = load_image ((char *)&img_radar);
		if (pixbuf){
			gtk_image_set_from_pixbuf (GTK_IMAGE(img), pixbuf);
			g_object_unref (G_OBJECT (pixbuf));
			pixbuf = 0;
		}
		if (tmp_buf)
			g_free (tmp_buf);
	
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
