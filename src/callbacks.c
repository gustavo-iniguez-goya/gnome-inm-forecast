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
			gtk_widget_show (img);
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
			gtk_widget_show (img);
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
	#define rdr_size 512
	char img_radar[rdr_size]={0};
	if (img){
		if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "19:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_SAT_IMG, "1900.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "20:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_SAT_IMG, "2000.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "21:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_SAT_IMG, "2100.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "22:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_SAT_IMG, "2200.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "23:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_SAT_IMG, "2300.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "00:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_SAT_IMG, "0000.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "01:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_SAT_IMG, "0100.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "02:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_SAT_IMG, "0200.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "03:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_SAT_IMG, "0300.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "04:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_SAT_IMG, "0400.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "05:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_SAT_IMG, "0500.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "06:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_SAT_IMG, "0600.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "07:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_SAT_IMG, "0700.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "08:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_SAT_IMG, "0800.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "09:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_SAT_IMG, "0900.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "10:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_SAT_IMG, "1000.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "11:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_SAT_IMG, "1100.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "12:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_SAT_IMG, "1200.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "13:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_SAT_IMG, "1300.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "14:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_SAT_IMG, "1400.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "15:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_SAT_IMG, "1500.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "16:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_SAT_IMG, "1600.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "17:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_SAT_IMG, "1700.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "18:00", 5) == 0){
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
	}
}

// FIXME: Create a common function for this if()'s
void on_radar_combo_hour_changed			 ( GtkWidget *widget, gpointer data )
{
	GtkImage *img = (GtkImage *) data;
	GdkPixbuf *pixbuf;
	#define rdr_size 512
	char img_radar[rdr_size]={0};
	if (img){
		if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "19:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_IMG, "1900.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "20:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_IMG, "2000.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "21:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_IMG, "2100.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "22:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_IMG, "2200.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "23:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_IMG, "2300.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "00:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_IMG, "0000.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "01:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_IMG, "0100.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "02:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_IMG, "0200.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "03:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_IMG, "0300.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "04:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_IMG, "0400.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "05:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_IMG, "0500.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "06:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_IMG, "0600.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "07:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_IMG, "0700.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "08:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_IMG, "0800.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "09:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_IMG, "0900.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "10:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_IMG, "1000.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "11:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_IMG, "1100.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "12:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_IMG, "1200.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "13:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_IMG, "1300.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "14:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_IMG, "1400.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "15:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_IMG, "1500.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "16:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_IMG, "1600.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "17:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_IMG, "1700.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "18:00", 5) == 0){
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
	}
}

// FIXME: Create a common function for this if()'s
void on_radar_canarias_combo_hour_changed		 ( GtkWidget *widget, gpointer data )
{
	GtkImage *img = (GtkImage *) data;
	GdkPixbuf *pixbuf;
	#define rdr_size 512
	char img_radar[rdr_size]={0};
	if (img){
		if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "19:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_CANARIAS_IMG, "1900.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "20:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_CANARIAS_IMG, "2000.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "21:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_CANARIAS_IMG, "2100.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "22:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_CANARIAS_IMG, "2200.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "23:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_CANARIAS_IMG, "2300.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "00:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_CANARIAS_IMG, "0000.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "01:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_CANARIAS_IMG, "0100.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "02:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_CANARIAS_IMG, "0200.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "03:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_CANARIAS_IMG, "0300.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "04:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_CANARIAS_IMG, "0400.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "05:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_CANARIAS_IMG, "0500.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "06:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_CANARIAS_IMG, "0600.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "07:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_CANARIAS_IMG, "0700.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "08:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_CANARIAS_IMG, "0800.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "09:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_CANARIAS_IMG, "0900.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "10:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_CANARIAS_IMG, "1000.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "11:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_CANARIAS_IMG, "1100.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "12:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_CANARIAS_IMG, "1200.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "13:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_CANARIAS_IMG, "1300.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "14:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_CANARIAS_IMG, "1400.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "15:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_CANARIAS_IMG, "1500.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "16:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_CANARIAS_IMG, "1600.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "17:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_CANARIAS_IMG, "1700.gif");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "18:00", 5) == 0){
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
	}
}

// FIXME: Create a common function for this if()'s
void on_airmass_combo_hour_changed		 ( GtkWidget *widget, gpointer data )
{
	GtkImage *img = (GtkImage *) data;
	GdkPixbuf *pixbuf;
	#define rdr_size 512
	char img_radar[rdr_size]={0};
	if (img){
		if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "19:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_AIR_MASS_IMG, "1900.jpg");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "20:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_AIR_MASS_IMG, "2000.jpg");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "21:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_AIR_MASS_IMG, "2100.jpg");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "22:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_AIR_MASS_IMG, "2200.jpg");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "23:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_AIR_MASS_IMG, "2300.jpg");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "00:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_AIR_MASS_IMG, "0000.jpg");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "01:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_AIR_MASS_IMG, "0100.jpg");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "02:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_AIR_MASS_IMG, "0200.jpg");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "03:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_AIR_MASS_IMG, "0300.jpg");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "04:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_AIR_MASS_IMG, "0400.jpg");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "05:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_AIR_MASS_IMG, "0500.jpg");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "06:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_AIR_MASS_IMG, "0600.jpg");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "07:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_AIR_MASS_IMG, "0700.jpg");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "08:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_AIR_MASS_IMG, "0800.jpg");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "09:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_AIR_MASS_IMG, "0900.jpg");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "10:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_AIR_MASS_IMG, "1000.jpg");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "11:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_AIR_MASS_IMG, "1100.jpg");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "12:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_AIR_MASS_IMG, "1200.jpg");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "13:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_AIR_MASS_IMG, "1300.jpg");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "14:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_AIR_MASS_IMG, "1400.jpg");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "15:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_AIR_MASS_IMG, "1500.jpg");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "16:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_AIR_MASS_IMG, "1600.jpg");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "17:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_AIR_MASS_IMG, "1700.jpg");
		}
		else if (strncmp (gtk_combo_box_get_active_text (GTK_COMBO_BOX(widget)), "18:00", 5) == 0){
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_AIR_MASS_IMG, "1800.jpg");
		}
		else
			g_snprintf ((char *)&img_radar, rdr_size, "%s%s", INM_RADAR_AIR_MASS_IMG, "1900.jpg");

		pixbuf = load_image ((char *)&img_radar);
		if (pixbuf){
			gtk_image_set_from_pixbuf (GTK_IMAGE(img), pixbuf);
			g_object_unref (G_OBJECT (pixbuf));
			pixbuf = 0;
		}
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
	printf ("Applet destroy\n");
	if (applet_data){
		gtk_timeout_remove (applet_data->timer);
		g_free (applet_data->day_info);
		g_free (applet_data->prefs);
		g_free (applet_data);
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
