/*
  Copyright (C) 2007 Gustavo I�iguez Goya

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

#define INM_URL "http://www.inm.es"
#define INM_FORECAST_URL "http://www.inm.es/cgi-bin/locali.cgi?ig="
#define INM_RADAR0000_IMG "http://www.inm.es/wwb/satel/s15g0000.jpg"
#define INM_RADAR0600_IMG "http://www.inm.es/wwb/satel/s15g0600.jpg"
#define INM_RADAR1200_IMG "http://www.inm.es/wwb/satel/s15g1200.jpg"
#define INM_RADAR1800_IMG "http://www.inm.es/wwb/satel/s15g1800.jpg"
#define INM_FORECAST_NEXTDAYS_URL "http://www.inm.es/cgi-bin/p06hesp1.sh.2001"
#define INM_FORECAST_TOMORROW_URL "http://www.inm.es/cgi-bin/p03hesp1.sh.2001"
#define INM_FORECAST_TODAY_URL "http://www.inm.es/cgi-bin/p07hesp1.sh.2001"
#define INM_WWARNINGS_TODAY_URL "http://www.inm.es/web/infmet/avi/pr/conavi_c.php"
#define INM_WWARNINGS_TOMORROW_URL "http://www.inm.es/web/infmet/avi/pr/conavi_mm.php"
#define INM_WWARNINGS_NEXT_DAYS_URL "http://www.inm.es/web/infmet/avi/pr/conavi_pp.php"
#define INM_SNOW_WARNINGS_URL "http://www.inm.es/cgi-bin/nevadas.cgi.2002?ccaa=arn&producto=p18t"
#define INM_NATIONAL_FORECAST_IMG "http://www.inm.es/wwb/predi/p01gesp1.jpg"
#define INM_DAILY_TEMPERATURE_MAX "http://www.inm.es/wwb/predi/capital_MAX_2001.gif"
#define INM_DAILY_TEMPERATURE_MIN "http://www.inm.es/wwb/predi/capital_MIN_2001.gif"
#define INM_NAMES_LATEST_DATA "http://www.inm.es/web/infmet/tobsr/bin/nombres.js"
#define INM_LATEST_DATA "http://www.inm.es/wwb/tobsr/emasdat.js"
#define INM_SPAIN_IMG PACKAGE_DIR"pixmaps/espcan.jpg"

enum DATA_TYPE {
	MAX = 0,
	MIN,
	PRECIP,
	SNOW,
	WIND,
	UV
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
	GtkWidget *win;
	PanelApplet *applet;
	char *code;
	GtkEntryCompletion 	*entry_cmpt;
	GList 		*codes;
	GtkListStore    *list_store;
	GtkTreeIter 	iter;
	GtkCellRenderer *renderer;
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
	int		timer; /* timeout ID */
	int		interval; /* Interval update * 1000 */
	int		applet_size;
	char		*last_update;
	char		*city_name;
	char		*city_long_desc;
	char		*provincia;
	char		*buffer;
} AppletData ;

void update_data		( BonoboUIComponent *uic, gpointer user_data, const char *name );
void update_location		( AppletData *applet_data );
void display_tomorrow_forecast	( BonoboUIComponent *uic, gpointer user_data, const char *name );
void display_nextdays_forecast	( BonoboUIComponent *uic, gpointer user_data, const char *name );
void display_wwarnings_t	( BonoboUIComponent *uic, gpointer user_data, const char *name );
void display_wwarnings_tm	( BonoboUIComponent *uic, gpointer user_data, const char *name );
void display_wwarnings_n	( BonoboUIComponent *uic, gpointer user_data, const char *name );
void display_today_forecast	( BonoboUIComponent *uic, gpointer user_data, const char *name );
void display_satellite_radar 	( BonoboUIComponent *uic, gpointer user_data, const char *name );
void display_spanish_forecast_img	( BonoboUIComponent *uic, gpointer user_data, const char *name );
void display_snow_warnings	( BonoboUIComponent *uic, gpointer user_data, const char *name );
void display_preferences_dialog	( BonoboUIComponent *uic, gpointer user_data, const char *name );
void display_inm_website	( BonoboUIComponent *uic, gpointer user_data, const char *name );
void display_about_dialog 	( BonoboUIComponent *uic, gpointer user_data, const char *name );
gboolean start_applet 		( PanelApplet *applet, const gchar *iid, gpointer data );

GdkPixbuf *load_image		( const char *url );
void create_window 		( AppletData *applet_data, const char *name );
void quit 			( GtkWidget *widget );
void parse_sky_data 		( PanelApplet *applet, AppletData *applet_data, char *buf );
void parse_dates_data 		( AppletData *applet_data, char *buf, int type );
void parse_temperatures_data 	( AppletData *applet_data, char *buf, int type );
void set_tooltip		( AppletData *applet_data, const int id, const gchar* tip );
gboolean check_inm_url 		( AppletData *applet_data );
gboolean update_panel		( AppletData *applet_data );
//gboolean check_latest_data	( AppletData *applet_data );
gboolean check_station_data	( AppletData *applet_data );
void print_element_names	( xmlNode *root_element );
char *parse_week_day_name	( const char *day_name );

#endif
