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


#ifndef GUI_H
#define GUI_H

#include "main.h"
#include "callbacks.h"
#include "connection.h"



void display_tomorrow_forecast	( BonoboUIComponent *uic, gpointer user_data, const char *name );
void display_nextdays_forecast	( BonoboUIComponent *uic, gpointer user_data, const char *name );
void display_wwarnings_t	( BonoboUIComponent *uic, gpointer user_data, const char *name );
void display_wwarnings_tm	( BonoboUIComponent *uic, gpointer user_data, const char *name );
void display_wwarnings_n	( BonoboUIComponent *uic, gpointer user_data, const char *name );
void display_today_forecast	( BonoboUIComponent *uic, gpointer user_data, const char *name );
void display_satellite_radar 	( BonoboUIComponent *uic, gpointer user_data, const char *name );
void display_spanish_forecast_img	( BonoboUIComponent *uic, gpointer user_data, const char *name );
void display_prob_precipitation	( BonoboUIComponent *uic, gpointer user_data, const char *name );
void display_snow_warnings	( BonoboUIComponent *uic, gpointer user_data, const char *name );
void display_snow_warnings_nav		 ( BonoboUIComponent *uic, gpointer user_data, const char *name );
void display_snow_warnings_arg		 ( BonoboUIComponent *uic, gpointer user_data, const char *name );
void display_snow_warnings_cat		 ( BonoboUIComponent *uic, gpointer user_data, const char *name );
void display_mountain_forecast	( BonoboUIComponent *uic, gpointer user_data, const char *name );
void display_marine_forecast	( BonoboUIComponent *uic, gpointer user_data, const char *name );
void display_preferences_dialog	( BonoboUIComponent *uic, gpointer user_data, const char *name );
void display_inm_website	( BonoboUIComponent *uic, gpointer user_data, const char *name );
void display_about_dialog 	( BonoboUIComponent *uic, gpointer user_data, const char *name );
void display_daily_temperatures 	( BonoboUIComponent *uic, gpointer user_data, const char *name );
void display_air_mass	 	( BonoboUIComponent *uic, gpointer user_data, const char *name );
void display_radar	 	( BonoboUIComponent *uic, gpointer user_data, const char *name );
void display_lightnings	 	( BonoboUIComponent *uic, gpointer user_data, const char *name );
void create_window 		( AppletData *applet_data, const char *name );


#endif
