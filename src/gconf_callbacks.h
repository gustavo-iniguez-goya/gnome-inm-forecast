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

#ifndef GCONF_CALLBACKS_H
#define GCONF_CALLBACKS_H

#include "main.h"

void gconf_notify_days 		( GConfClient *client, guint id, GConfEntry *entry, gpointer data );
void gconf_notify_ma 		( GConfClient *client, guint id, GConfEntry *entry, gpointer data );
void gconf_notify_code 		( GConfClient *client, guint id, GConfEntry *entry, gpointer data );

#endif
