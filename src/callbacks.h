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

#ifndef CALLBACKS_H
#define CALLBACKS_H

#include "main.h"

void on_radar_img_size_allocate 	( GtkWidget *widget, GtkAllocation *allocate, gpointer data );
gboolean on_image_button_press 	( GtkWidget *event_box, GdkEventButton *event, AppletData *applet_data );
void on_window_terminate 	( GtkWidget *widget, gpointer data );
void on_daily_temp_rb1_toggled			 ( GtkWidget *widget, gpointer data );
void on_daily_temp_rb2_toggled			 ( GtkWidget *widget, gpointer data );
void on_airmass_combo_hour_changed	( GtkWidget *widget, gpointer data );
void on_sat_combo_hour_changed	( GtkWidget *widget, gpointer data );
void on_rb_00_toggled		( GtkWidget *widget, gpointer data );
void on_rb_06_toggled 		( GtkWidget *widget, gpointer data );
void on_rb_12_toggled 		( GtkWidget *widget, gpointer data );
void on_rb_18_toggled 		( GtkWidget *widget, gpointer data );
void on_prob_precip_toggled_005	( GtkWidget *widget, gpointer data );
void on_prob_precip_toggled_020 ( GtkWidget *widget, gpointer data );
void on_prob_precip_toggled_100 ( GtkWidget *widget, gpointer data );
void on_prob_precip_toggled_200 ( GtkWidget *widget, gpointer data );
void on_prob_precip_toggled_avg ( GtkWidget *widget, gpointer data );
void on_prob_precip_toggled_max ( GtkWidget *widget, gpointer data );
void on_applet_destroy 		( GtkWidget *widget, AppletData *applet_data );

#endif
