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

#ifndef MAIN_H
#define MAIN_H

#include <locale.h>
#include <libintl.h>
#define _(String) gettext(String)

#include <libxml/parser.h>
#include <libgnomevfs/gnome-vfs.h>
#include <libxml/nanohttp.h>
#include <panel-applet.h>
#include <gtk/gtklabel.h>
#include <gtk/gtk.h>
#include <glade/glade.h>
#include <gconf/gconf-client.h>
#include "config.h"

#define THEME "inm"
#define KEY_GNOME_PROXY_HOST "/system/http_proxy/host"
#define KEY_GNOME_PROXY_PORT "/system/http_proxy/port"
#define KEY_GNOME_PROXY_USER "/system/http_proxy/authentication_user"
#define KEY_GNOME_PROXY_PASS "/system/http_proxy/authentication_password"
#define KEY_GNOME_PROXY_USEIT "/system/http_proxy/use_http_proxy"
#define KEY_GNOME_PROXY_AUTH "/system/http_proxy/use_authentication"

#define INM_URL "http://www.aemet.es"

#define INM_FORECAST_URL "http://www.aemet.es/es/eltiempo/prediccion/localidades?l="
#define INM_RADAR_AIR_MASS_IMG "http://www.aemet.es/imagenes_d/eltiempo/observacion/satelite/s91g"

#define INM_RADAR_IMG "http://www.aemet.es/imagenes_d/eltiempo/observacion/radar/r8pb"
#define INM_RAYOS_IMG "http://www.aemet.es/imagenes_d/eltiempo/observacion/rayos/r79g"

#define INM_RAYOS_CANARIAS_IMG "http://www.aemet.es/imagenes_d/eltiempo/observacion/rayos/r78g"
#define INM_RADAR_CANARIAS_IMG "http://www.aemet.es/imagenes_d/eltiempo/observacion/radar/r98g"

#define INM_SAT_IMG "http://www.aemet.es/imagenes_d/eltiempo/observacion/satelite/s93g"

#define INM_FORECAST_NEXTDAYS_URL "http://www.aemet.es/es/eltiempo/prediccion/espana?w=5"
#define INM_FORECAST_TOMORROW_URL "http://www.aemet.es/es/eltiempo/prediccion/espana?w=3"
#define INM_FORECAST_TODAY_URL "http://www.aemet.es/es/eltiempo/prediccion/espana?w=1"

#define INM_WWARNINGS_TODAY_URL "http://www.aemet.es/es/eltiempo/prediccion/avisos?w=hoy&datos=img"
#define INM_WWARNINGS_TOMORROW_URL "http://www.aemet.es/es/eltiempo/prediccion/avisos?w=mna&datos=img"
#define INM_WWARNINGS_NEXT_DAYS_URL "http://www.aemet.es/es/eltiempo/prediccion/avisos?w=pmna&datos=img"

#define INM_SNOW_WARNINGS_URL_1 "http://www.aemet.es/es/eltiempo/prediccion/montana?w=2&p=nav1"
#define INM_SNOW_WARNINGS_URL_2 "http://www.aemet.es/es/eltiempo/prediccion/montana?p=cat1&w=2"
#define INM_SNOW_WARNINGS_URL_3 "http://www.aemet.es/es/eltiempo/prediccion/montana?p=arn1&w=2"

#define INM_MARINE_FORECAST_1 "http://www.aemet.es/en/eltiempo/prediccion/maritima?opc1=0&opc2=0&zona=can1"
#define INM_MARINE_FORECAST_2 "http://www.aemet.es/en/eltiempo/prediccion/maritima?opc1=0&opc2=0&zona=gal1"
#define INM_MARINE_FORECAST_3 "http://www.aemet.es/en/eltiempo/prediccion/maritima?opc1=0&opc2=0&zona=cat1"
#define INM_MARINE_FORECAST_4 "http://www.aemet.es/en/eltiempo/prediccion/maritima?opc1=0&opc2=0&zona=val1"
#define INM_MARINE_FORECAST_5 "http://www.aemet.es/en/eltiempo/prediccion/maritima?opc1=0&opc2=0&zona=and2"
#define INM_MARINE_FORECAST_6 "http://www.aemet.es/en/eltiempo/prediccion/maritima?opc1=0&opc2=0&zona=and1"
#define INM_MARINE_FORECAST_7 "http://www.aemet.es/en/eltiempo/prediccion/maritima?opc1=0&opc2=0&zona=bal1"
#define INM_MARINE_FORECAST_8 "http://www.aemet.es/en/eltiempo/prediccion/maritima?opc1=0&opc2=0&zona=coo1"
#define INM_MARINE_OPENSEA_1 "http://www.aemet.es/en/eltiempo/prediccion/maritima?opc1=1&opc2=0&zona=atl1&datos=dat"
#define INM_MARINE_OPENSEA_2 "http://www.aemet.es/en/eltiempo/prediccion/maritima?opc1=1&opc2=1&zona=atl1&datos=dat"
#define INM_MARINE_OPENSEA_3 "http://www.aemet.es/en/eltiempo/prediccion/maritima?opc1=1&opc2=2&zona=atl1&datos=dat"
#define INM_MARINE_OPENSEA_4 "http://www.aemet.es/en/eltiempo/prediccion/maritima?opc1=1&opc2=0&zona=med1&datos=dat"
#define INM_MARINE_OPENSEA_5 "http://www.aemet.es/en/eltiempo/prediccion/maritima?opc1=1&opc2=1&zona=med1&datos=dat"
#define INM_MARINE_OPENSEA_6 "http://www.aemet.es/en/eltiempo/prediccion/maritima?opc1=1&opc2=2&zona=med1&datos=dat"
#define INM_MARINE_OPENSEA_7 "http://www.aemet.es/en/eltiempo/prediccion/maritima?opc1=1&opc2=0&zona=atl2&datos=dat"
#define INM_MARINE_OPENSEA_8 "http://www.aemet.es/en/eltiempo/prediccion/maritima?opc1=1&opc2=1&zona=atl2&datos=dat"
#define INM_MARINE_OPENSEA_9 "http://www.aemet.es/en/eltiempo/prediccion/maritima?opc1=1&opc2=2&zona=atl2&datos=dat"

#define INM_MOUNTAIN_FORECAST_PICOS "http://www.aemet.es/es/eltiempo/prediccion/montana?w=&p=peu1"
#define INM_MOUNTAIN_FORECAST_RIOJA "http://www.aemet.es/es/eltiempo/prediccion/montana?w=&p=rio1"
#define INM_MOUNTAIN_FORECAST_IARAGONESA "http://www.aemet.es/es/eltiempo/prediccion/montana?w=&p=arn2"
#define INM_MOUNTAIN_FORECAST_PIRI_NAVARRO "http://www.aemet.es/es/eltiempo/prediccion/montana?w=&p=nav1"
#define INM_MOUNTAIN_FORECAST_PIRI_ARAGONES "http://www.aemet.es/es/eltiempo/prediccion/montana?w=&p=arn1"
#define INM_MOUNTAIN_FORECAST_PIRI_CATALAN "http://www.aemet.es/es/eltiempo/prediccion/montana?w=&p=cat1"
#define INM_MOUNTAIN_FORECAST_GREDOS "http://www.aemet.es/es/eltiempo/prediccion/montana?w=&p=gre1"
#define INM_MOUNTAIN_FORECAST_GUADARRAMA "http://www.aemet.es/es/eltiempo/prediccion/montana?w=&p=mad2"
#define INM_MOUNTAIN_FORECAST_SIERRA_NEVADA "http://www.aemet.es/es/eltiempo/prediccion/montana?w=&p=nev1"
#define INM_NATIONAL_FORECAST_IMG "http://www.aemet.es/imagenes_d/eltiempo/prediccion/espa/"
#define INM_NATIONAL_FORECAST_1224 "1224_p01wesp0.jpg"
#define INM_NATIONAL_FORECAST_0012 "0012_p01wesp0.jpg"

#define INM_DAILY_TEMPERATURE_MAX "http://www.aemet.es/imagenes_d/eltiempo/prediccion/temperaturas/tw2gp099.gif"
#define INM_DAILY_TEMPERATURE_MIN "http://www.aemet.es/imagenes_d/eltiempo/prediccion/temperaturas/tw2gp098.gif"

#define INM_PROB_PRECIP_005 "http://www.aemet.es/imagenes_d/eltiempo/prediccion/precipitacion/dw1gp005.gif"
#define INM_PROB_PRECIP_020 "http://www.aemet.es/imagenes_d/eltiempo/prediccion/precipitacion/dw1gp020.gif"
#define INM_PROB_PRECIP_100 "http://www.aemet.es/imagenes_d/eltiempo/prediccion/precipitacion/dw1gp100.gif"
#define INM_PROB_PRECIP_200 "http://www.aemet.es/imagenes_d/eltiempo/prediccion/precipitacion/dw1gp200.gif"
#define INM_PROB_PRECIP_AVG "http://www.aemet.es/imagenes_d/eltiempo/prediccion/precipitacion/dw1gmedi.gif"
#define INM_PROB_PRECIP_MAX "http://www.aemet.es/imagenes_d/eltiempo/prediccion/precipitacion/dw1gmaxi.gif"

#define INM_NAMES_LATEST_DATA "http://www.inm.es/web/infmet/tobsr/bin/nombres.js"
#define INM_LATEST_DATA "http://www.inm.es/wwb/tobsr/emasdat.js"
#define INM_SPAIN_IMG PACKAGE_DIR"pixmaps/espcan.jpg"

#define INTERVAL_TIME 60000

#define MAX_DAYS 10

enum DATA_TYPE {
	MAX = 0,
	MIN,
	PRECIP,
	SNOW,
	WIND,
	UV
};

enum REPORT_TYPE {
	SNOWFALLS_1 = 0, // Pirineo navarro
	SNOWFALLS_2, // Pirineo catalan
	CURRENT_REPORT,
	DAYAFTERTOMORROW,
	NEXT_DAYS,
	PICOS,
	GREDOS,
	GUADARRAMA,
	RIOJA,
	IARAGONESA,
	PNAVARRO,
	PCATALAN,
	SIERRA_NEVADA,
	TOTAL_REPORTS
};

typedef struct _stationInformation {
	char	*hora_utc;
	char	*hora_oficial;
	char	*altitud; /* de la estacion */
	char	*temp;
	char	*humedad;
	char	*precip;
	char	*viento;
	char	*viento_direc;
	char	*viento_max;
} stationInfo;

typedef struct _stationCode {
	char	*name;
	char	*code;
	char	*provincia;
} stationCode;

typedef struct _DayInformation {
	char		*day;
	char		*state;
	char		*precip;
	char		*cota_nieve;
	char		*t_max, *t_min;
	char 		*wind;
} DayInf;

typedef struct _PreferencesWin {
	GtkWidget *cmd_ok, *cmd_cancel;
	GtkWidget *entry_code;
	GtkWidget *theme_entry, *combo_theme;
	GtkWidget *spin_days, *spin_interval;
	GtkWidget *prov_search_entry, *tree_prov;
	GtkWidget *chk_station;
	GtkWidget *win;
	PanelApplet *applet;
	char *code;
	char *station_code;
	GtkEntryCompletion 	*entry_cmpt;
	GList 		*codes;
	GtkListStore    *list_store;
	GtkTreeIter 	iter, child;
	GtkCellRenderer *renderer;
	GtkTreeStore 	*tree_store;
} PrefsWin;

typedef struct _AppletData {
	PrefsWin	*prefs;
	DayInf		*day_info;
	GtkWidget 	*image[10];
	GtkWidget 	*event_box[11];
	GtkWidget	*applet;
	GtkWidget	*hbox;
	GtkWidget	*temp_lbl;
	GladeXML	*xml;
	GConfClient	*client;
	GtkTooltips	*tips;
	GList		*st_list, *st_info;
	GnomeVFSAsyncHandle *gvfs_handle;
	int		gvfs_cnx_type; /* Connection type */
	int		show_ma; /* Choose morning or afternoon to show, or both */
	char		*show_days; /* How many days to show */
	int		location_code; /* Location code */
	int		timer, timer_station; /* timeout ID */
	int		interval; /* Interval update * 1000 */
	int		applet_size;
	char		*last_update;
	char		*city_name;
	char		*city_long_desc;
	char		*provincia;
	char		*buffer;
	char		*theme;
} AppletData ;

void update_station_data	( AppletData *applet_data );
void update_data		( BonoboUIComponent *uic, gpointer user_data, const char *name );
void update_location		( AppletData *applet_data );
gboolean start_applet 		( PanelApplet *applet, const gchar *iid, gpointer data );

void message_box 		( AppletData *applet_data, GtkMessageType type, GtkButtonsType buttons, const char *text, const char *secondary_text );
char *convert_str_to_utf8 	( char *string );
void unset_images	 	( AppletData *applet_data );
GdkPixbuf *load_image		( const char *url );
void quit 			( GtkWidget *widget );
void set_tooltip		( AppletData *applet_data, const int id, const gchar* tip );
gboolean check_inm_url 		( AppletData *applet_data );
gboolean update_panel		( AppletData *applet_data );
//gboolean check_latest_data	( AppletData *applet_data );
gboolean check_station_data	( AppletData *applet_data );
void print_element_names	( xmlNode *root_element );

#endif
