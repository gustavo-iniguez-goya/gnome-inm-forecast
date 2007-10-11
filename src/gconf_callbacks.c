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
#include "gconf_callbacks.h"
#include "main.h"

 /* How many days we show on the panel */
void gconf_notify_days 		( GConfClient *client, guint id, GConfEntry *entry, gpointer data )
{
	AppletData *applet_data = (AppletData *)data;
	GConfValue *value;
	gchar *str;
	value = gconf_entry_get_value (entry);
	str = gconf_value_get_string (value);
	//gtk_spin_button_set_string (GTK_SPIN_BUTTON(), str);
}
/* Choose if we show the morning or afternoon, or both forecast */
void gconf_notify_ma 		( GConfClient *client, guint id, GConfEntry *entry, gpointer data )
{
	GConfValue *value;
	gboolean checked;
	value = gconf_entry_get_value (entry);
	checked = gconf_value_get_bool (value);
	//gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(), checked);
}

void gconf_notify_code 		( GConfClient *client, guint id, GConfEntry *entry, gpointer data )
{
	AppletData *applet_data = (AppletData *)data;
	GConfValue *value;
	gchar *str;
	value = gconf_entry_get_value (entry);
	str = gconf_value_get_string (value);
	//gtk_entry_set_text (GTK_ENTRY(), str);
}
