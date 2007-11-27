/*
  Copyright (C) 2007 Gustavo Iñiguez Goya

  gnome_inm is free software; you can redistribute it and/or modify
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

#ifndef PREFERENCES_H
#define PREFERENCES_H

#include "main.h"

void 		create_preferences_win ( AppletData *applet_data );
void 		create_stations_tree ( AppletData *applet_data );
void 		on_preferences_destroy ( GtkWidget *widget, AppletData *applet_data );
void		on_cmd_ok_clicked ( GtkWidget *widget, AppletData *applet_data );
void		on_cmd_cancel_clicked ( GtkWidget *widget, AppletData *applet_data );
void 		on_station_tree_selection ( GtkTreeView *tree_view, GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data);
void 	 	location_entry_changed ( GtkWidget *widget, AppletData *applet_data );
gboolean 	location_entry_event ( GtkWidget *widget, GdkEventKey *event, AppletData *applet_data );
void 	 	station_entry_changed ( GtkWidget *widget, AppletData *applet_data );
gboolean 	station_entry_event ( GtkWidget *widget, GdkEventKey *event, AppletData *applet_data );
gboolean 	find_location_code ( GtkWidget *widget, AppletData *applet_data, int type );
void 		close_prefs_window ( AppletData *applet_data );

#endif
