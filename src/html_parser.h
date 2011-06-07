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

#ifndef HTMLPARSER_H
#define HTMLPARSER_H

#include "main.h"
#include <libxml/xpath.h>


char *parse_week_day_name	( const char *day_name );
void parse_xml_dates		( AppletData *applet_data, xmlDocPtr doc, xmlNodeSetPtr ns );
void parse_xml_sky		( AppletData *applet_data, xmlDocPtr doc, xmlNodeSetPtr ns );
void parse_xml_snow		( AppletData *applet_data, xmlDocPtr doc, xmlNodeSetPtr ns );
void parse_xml_precip		( AppletData *applet_data, xmlDocPtr doc, xmlNodeSetPtr ns );
void parse_xml_temperatures	( AppletData *applet_data, xmlDocPtr doc, xmlNodeSetPtr ns, int type );
void parse_xml_wind		( AppletData *applet_data, xmlDocPtr doc, xmlNodeSetPtr ns );


#endif
