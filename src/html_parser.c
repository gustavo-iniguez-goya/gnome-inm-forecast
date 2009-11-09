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

#include "html_parser.h"

char *parse_week_day_name	( const char *day_name )
{
	char *day_temp=0;
	char **tokens=0;
	/* This mem is free at line 364 and succesives */
	day_temp = (char *)g_malloc0(64);
	
	if (day_name){
		tokens = g_strsplit_set (day_name, " ", -1);

		if (tokens[1]){
			if (strstr (day_name, "mi&eacute;rcoles"))
				snprintf (day_temp, 64, _("Wednesday %s"), tokens[1]);
			else if (strstr (day_name, "s&aacute;bado"))
				snprintf (day_temp, 64, _("Saturday %s"), tokens[1]);
			else if (strstr (day_name, "lun "))
				snprintf (day_temp, 64, _("Monday %s"), tokens[1]);
			else if (strstr (day_name, "mar "))
				snprintf (day_temp, 64, _("Tuesday %s"), tokens[1]);
			else if (strstr (day_name, "mi"))
				snprintf (day_temp, 64, _("Wednesday %s"), tokens[1]);
			else if (strstr (day_name, "jue "))
				snprintf (day_temp, 64, _("Thursday %s"), tokens[1]);
			else if (strstr (day_name, "vie "))
				snprintf (day_temp, 64, _("Friday %s"), tokens[1]);
			else if (strstr (day_name, "s&aacute;b ") || strstr (day_name, "sáb ") || strstr (day_name, "b "))
				snprintf (day_temp, 64, _("Saturday %s"), tokens[1]);
			else if (strstr (day_name, "dom "))
				snprintf (day_temp, 64, _("Sunday %s"), tokens[1]);
			else
				snprintf (day_temp, 64, "%s", day_name);
		
			g_strfreev (tokens);
		}
		tokens = 0;

	}
	return day_temp;
}


void parse_sky_data 		( PanelApplet *applet, AppletData *applet_data, char *buf )
{
	const char delimiters[] = "=\"<>&;";
	char *temp=0;
	char *temp_buf=0;
	char **tokens=0;
	GdkPixbuf *temp_pixbuf=0, *temp_pixbuf2=0;
	int x=0,id_img=0;
	//int iDays = g_ascii_strtod (panel_applet_gconf_get_string (PANEL_APPLET(applet_data->applet), "days", NULL), NULL);
	int iDays = MAX_DAYS;
	// FIXME: This value should be dynamic: strlen (PIXMAPS_DIR) + strlen (applet_data->theme)
	int theme_len = 128;

	char *theme = g_new0(char, theme_len);

	if (applet && applet_data && buf){
			tokens = g_strsplit_set (buf, "<>&;=\"", -1); 
			x=0;
			while (tokens[x]){
				if (!strstr (tokens[x], "estado_cielo")){x++; continue;}
				printf ("XXXX [%d]: %s\n", id_img, tokens[x]);
				if (strstr (tokens[x], "11.gif")){
					snprintf (theme, theme_len, "%s%s/11.png", PIXMAPS_DIR, applet_data->theme);
					gtk_image_set_from_file (GTK_IMAGE(applet_data->image[id_img]), theme);
					strncpy (applet_data->day_info[id_img].state, _("Despejado"), 36);
					id_img++;
				}
				else if (strstr (tokens[x], "12.gif")){
					snprintf (theme, theme_len, "%s%s/12.png", PIXMAPS_DIR, applet_data->theme);
					gtk_image_set_from_file (GTK_IMAGE(applet_data->image[id_img]), theme);
					strncpy (applet_data->day_info[id_img].state, _("Poco nuboso"), 36);
					id_img++;
				}
				else if (strstr (tokens[x], "13.gif")){
					snprintf (theme, theme_len, "%s%s/13.png", PIXMAPS_DIR, applet_data->theme);
					gtk_image_set_from_file (GTK_IMAGE(applet_data->image[id_img]), theme);
					strncpy (applet_data->day_info[id_img].state, _("Intervalos nubosos"), 36);
					id_img++;
				}
				else if (strstr (tokens[x], "14.gif")){
					snprintf (theme, theme_len, "%s%s/14.png", PIXMAPS_DIR, applet_data->theme);
					gtk_image_set_from_file (GTK_IMAGE(applet_data->image[id_img]), theme);
					strncpy (applet_data->day_info[id_img].state, _("Nuboso"), 36);
					id_img++;
				}
				else if (strstr (tokens[x], "15.gif")){
					snprintf (theme, theme_len, "%s%s/15.png", PIXMAPS_DIR, applet_data->theme);
					gtk_image_set_from_file (GTK_IMAGE(applet_data->image[id_img]), theme);
					strncpy (applet_data->day_info[id_img].state, _("Muy nuboso"), 36);
					id_img++;
				}
				else if (strstr (tokens[x], "16.gif")){
					snprintf (theme, theme_len, "%s%s/16.png", PIXMAPS_DIR, applet_data->theme);
					gtk_image_set_from_file (GTK_IMAGE(applet_data->image[id_img]), theme);
					id_img++;
				}
				else if (strstr (tokens[x], "23.gif")){
					snprintf (theme, theme_len, "%s%s/23.png", PIXMAPS_DIR, applet_data->theme);
					gtk_image_set_from_file (GTK_IMAGE(applet_data->image[id_img]), theme);
					strncpy (applet_data->day_info[id_img].state, _("Intervalos nubosos con lluvia"), 36);
					id_img++;
				}
				else if (strstr (tokens[x], "24.gif")){
					snprintf (theme, theme_len, "%s%s/24.png", PIXMAPS_DIR, applet_data->theme);
					gtk_image_set_from_file (GTK_IMAGE(applet_data->image[id_img]), theme);
					strncpy (applet_data->day_info[id_img].state, _("Nuboso con lluvia"), 36);
					id_img++;
				}
				else if (strstr (tokens[x], "25.gif")){
					snprintf (theme, theme_len, "%s%s/25.png", PIXMAPS_DIR, applet_data->theme);
					gtk_image_set_from_file (GTK_IMAGE(applet_data->image[id_img]), theme);
					strncpy (applet_data->day_info[id_img].state, _("Muy nuboso con lluvia"), 36);
					id_img++;
				}
				else if (strstr (tokens[x], "26.gif")){
					snprintf (theme, theme_len, "%s%s/26.png", PIXMAPS_DIR, applet_data->theme);
					gtk_image_set_from_file (GTK_IMAGE(applet_data->image[id_img]), theme);
					strncpy (applet_data->day_info[id_img].state, _("Muy nuboso con lluvia"), 36);
					id_img++;
				}
				else if (strstr (tokens[x], "33.gif")){
					snprintf (theme, theme_len, "%s%s/33.png", PIXMAPS_DIR, applet_data->theme);
					gtk_image_set_from_file (GTK_IMAGE(applet_data->image[id_img]), theme);
					strncpy (applet_data->day_info[id_img].state, _("Nieve"), 36);
					id_img++;
				}
				else if (strstr (tokens[x], "34.gif")){
					snprintf (theme, theme_len, "%s%s/34.png", PIXMAPS_DIR, applet_data->theme);
					gtk_image_set_from_file (GTK_IMAGE(applet_data->image[id_img]), theme);
					strncpy (applet_data->day_info[id_img].state, _("Nuboso con nieve"), 36);
					id_img++;
				}
				else if (strstr (tokens[x], "35.gif")){
					snprintf (theme, theme_len, "%s%s/35.png", PIXMAPS_DIR, applet_data->theme);
					gtk_image_set_from_file (GTK_IMAGE(applet_data->image[id_img]), theme);
					strncpy (applet_data->day_info[id_img].state, _("Muy nuboso con nieve"), 36);
					id_img++;
				} 
				else if (strstr (tokens[x], "36.gif")){
					snprintf (theme, theme_len, "%s%s/36.png", PIXMAPS_DIR, applet_data->theme);
					gtk_image_set_from_file (GTK_IMAGE(applet_data->image[id_img]), theme);
					strncpy (applet_data->day_info[id_img].state, _("Muy nuboso con nieve"), 36);
					id_img++;
				}
				x++;
			}
	

		set_tooltip (applet_data, id_img, "");

		for (x=0;x < id_img;x++){
			if (applet_data->image[id_img]){
				temp_pixbuf = gtk_image_get_pixbuf (GTK_IMAGE(applet_data->image[x]));
				temp_pixbuf2 = gdk_pixbuf_scale_simple (temp_pixbuf, applet_data->applet_size - 3, applet_data->applet_size - 3, GDK_INTERP_BILINEAR);
				gtk_image_set_from_pixbuf (GTK_IMAGE(applet_data->image[x]), temp_pixbuf2);
				if (temp_pixbuf && temp_pixbuf2){
					g_object_unref (temp_pixbuf);
					g_object_unref (temp_pixbuf2);
					temp_pixbuf = 0;
					temp_pixbuf2 = 0;
				}
			}
		}
		printf ("Days: %d\n", id_img);	
		// Hide the last item of the panel if it's afternoon
		// since AEMET removes that information.
		if (id_img == 10)
			gtk_widget_show (applet_data->event_box[9]);
		else
			gtk_widget_hide (applet_data->event_box[9]);
	}
	if (tokens)
		g_strfreev (tokens);
		
	if (temp_buf){
		g_free (temp_buf);
		temp_buf = NULL;
	}

	temp=0;
	if (theme){
		g_free (theme);
		theme = 0;
	}
}


void parse_dates_data		( AppletData *applet_data, char *buf, int type )
{
	char *temp_buf=0, *temp_buf2=0;
	int x=0;
	char **tokens=0;
	char *day_temp=0;

	if (buf && applet_data){
//		printf ("\n\n\n##############################################\n\n\nparse_dates_data(%d) %s\n", type, buf);
		temp_buf = strdup (buf);
		if (type == 0){ // Elaborado
			strcpy (applet_data->last_update, "");
			tokens = g_strsplit_set (temp_buf, "<>&;", 6); /* 0 - 16 */
			if (tokens[4]){
				temp_buf2 = convert_str_to_utf8 (tokens[4]);
				if (temp_buf2){
					strncpy (applet_data->last_update, _("Last update: "), 64);
					strncat (applet_data->last_update, temp_buf2, 64);
					g_free (temp_buf2);
					temp_buf2 = 0;
				}
			}
			else
				strcpy (applet_data->last_update, "");

			g_strfreev (tokens);
		}
		else if (type == 2){ // capital
			tokens = g_strsplit_set (temp_buf, "<>&;", 6);
			//printf ("Capital: %s\n", tokens[4]);
			
			if (tokens[4]){
				temp_buf2 = convert_str_to_utf8 (tokens[4]);
				if (temp_buf2){
					strncpy (applet_data->city_name, temp_buf2, 64);
					strncpy (applet_data->city_long_desc, temp_buf2, 64);
					g_free (temp_buf2);
					temp_buf2 = 0;
				}
				else
					strcpy (applet_data->city_name, "");
			}
			else
				strcpy (applet_data->city_name, "");
					
			g_strfreev (tokens);
			// provincia
	
			/*
			tokens = g_strsplit_set (strstr(buf, "selected=\"selected\">"), "<>", 6);
			if (tokens[1])
				strncpy (applet_data->provincia, tokens[1], 64);
			else
				strcpy (applet_data->provincia, "");
			
			g_strfreev (tokens);
			printf ("Prov: %s\n", applet_data->provincia);
			*/
			strcpy (applet_data->provincia, "");
		}
                else if (type == 3){ // Altitud estacion
                        tokens = g_strsplit_set (temp_buf, "<>&:; ", 6);
                        if (tokens[3]){
                                g_snprintf (applet_data->city_long_desc, 126, _("%s\nAltitude: %s mts.\0"), applet_data->city_name, tokens[3]);
                        }
                        g_strfreev (tokens);
                }
		else if (type == 1){ // Fecha
			int idx=0;
			tokens = g_strsplit_set (temp_buf, "<>&;", -1);
			for (x=0;x < 32;x++){
				if (tokens[x]){
					//	printf ("\n\t#############  parse_dates_data(FECHA) %d - %s\n", x, tokens[x]);
					if (strncmp(tokens[x], "/th", 3) == 0) continue;
					if (strncmp(tokens[x], "/tr", 3) == 0) continue;
					if (strncmp(tokens[x], "/td", 3) == 0) continue;
					if (strncmp(tokens[x], "nbsp", 4) == 0) continue;
					if (strncmp(tokens[x], "/div", 4) == 0) continue;
					if (strncmp(tokens[x], "th ", 3) == 0) continue;
					if (strstr(tokens[x], "lun ") || 
							strstr(tokens[x], "mar ") ||
							strstr(tokens[x], "mi") ||
							strstr(tokens[x], "jue ") ||
							strstr(tokens[x], "vie ") ||
							strstr(tokens[x], "sáb") || strstr(tokens[x], "s&aacute;b") || 
							strstr(tokens[x], "b ") ||
							strstr(tokens[x], "dom ")
							){
						day_temp = parse_week_day_name(tokens[x]);
						if (idx == 0){
							strncpy (applet_data->day_info[0].day, day_temp, 32);
							strncpy (applet_data->day_info[1].day, day_temp, 32);
							idx=2;
						}
						else if (idx == 2){
							strncpy (applet_data->day_info[2].day, day_temp, 32);
							strncpy (applet_data->day_info[3].day, day_temp, 32);
							idx=4;
						}
						else if (idx == 4){
							strncpy (applet_data->day_info[4].day, day_temp, 32);
							strncpy (applet_data->day_info[5].day, day_temp, 32);
							idx=6;
						}
						else {
							strncpy (applet_data->day_info[idx].day, day_temp, 32);
							idx++;
						}
						g_free (day_temp);
						day_temp = NULL;
					}
				}
			}
			
			g_strfreev (tokens);
		}
		g_free (temp_buf);
		temp_buf = NULL;
	}
	temp_buf = 0;
	temp_buf2 = 0;
	tokens=0;
	day_temp = 0;
}


void parse_temperatures_data 		( AppletData *applet_data, char *buf, int type )
{
	char *temp_buf=0;
	char **tokens=0;
	char **tk_snow=0;
	int yy=0,idx=0;
	/*
	xmlOutputBufferPtr bufPtr;
	htmlParserCtxtPtr cxt = htmlCreateMemoryParserCtxt (buf, strlen(buf));
	htmlDocPtr docPtr = htmlCtxtReadMemory (cxt, buf, strlen(buf), "http://www.inm.es", "iso-8859-15", HTML_PARSE_RECOVER);
		
	xmlNode *root_element=NULL;
	root_element = xmlDocGetRootElement (docPtr);
	print_element_names (root_element);
	*/	    

	if (buf){
		temp_buf = strdup (buf);
		tokens = g_strsplit_set (temp_buf, "<>&", -1);
		
		if (type == SNOW){
			idx=0;
			//printf ("SNOW[0]: %s\n", tokens[0]);
			for (yy=1;yy < 38;yy++){
				if (strncmp(tokens[yy], "/td", 3) == 0) continue;
				if (strncmp(tokens[yy], "td ", 3) == 0) continue;
				if (strncmp(tokens[yy], "nbsp", 4) == 0) continue;
				//printf ("SNOW[%d]: %s - len: %d - idx: %d\n", yy,(tokens[yy]) ? tokens[yy] : NULL, strlen(tokens[yy]), idx);
				if (tokens[yy] && idx < MAX_DAYS){
					if (strlen(tokens[yy]) == 0 && idx < 10){
						if (idx == 0){
							strcpy (applet_data->day_info[0].cota_nieve, "");
							strcpy (applet_data->day_info[1].cota_nieve, "");
							idx = 2;
						}
						else if (idx == 2){
							strcpy (applet_data->day_info[2].cota_nieve, "");
							strcpy (applet_data->day_info[3].cota_nieve, "");
							idx=4;
						}
						else if (idx == 4){
							strcpy (applet_data->day_info[4].cota_nieve, "");
							strcpy (applet_data->day_info[5].cota_nieve, "");
							idx=6;
						}
						else{
							strcpy (applet_data->day_info[idx].cota_nieve, "");
							idx++;
						}
					}
					else if ((tokens[yy][0] >= '0' && tokens[yy][0] <= '9') || 
						(tokens[yy][1] >= '0' && tokens[yy][1] <= '9')){
						if (idx == 0){
							strncpy (applet_data->day_info[0].cota_nieve, tokens[yy], 10);
							strncpy (applet_data->day_info[1].cota_nieve, tokens[yy], 10);
							idx = 2;
						}
						else if (idx == 2){
							strncpy (applet_data->day_info[2].cota_nieve, tokens[yy], 10);
							strncpy (applet_data->day_info[3].cota_nieve, tokens[yy], 10);
							idx=4;
						}
						else if (idx == 4){
							strncpy (applet_data->day_info[4].cota_nieve, tokens[yy], 10);
							strncpy (applet_data->day_info[5].cota_nieve, tokens[yy], 10);
							idx=6;
						}
						else{
							strncpy (applet_data->day_info[idx].cota_nieve, tokens[yy], 10);
							idx++;
						}
					}
				}

			}
		}
		else if (type == WIND){
			idx = 0;
			for (yy=0;yy < 100;yy++){
				if (strncmp(tokens[yy], "/td", 3) == 0) continue;
				if (strncmp(tokens[yy], "nbsp", 4) == 0) continue;
				if (strncmp(tokens[yy], "td ", 3) == 0) continue;
				//printf ("WIND[%d]: %s - len: %d\n", yy,(tokens[yy]) ? tokens[yy] : NULL, strlen(tokens[yy]));
				if (tokens[yy] && strstr(tokens[yy], "S.gif")){
					strncpy (applet_data->day_info[idx].wind, _("Sur"), 3);
					idx++;
				}
				else if (tokens[yy] && strstr(tokens[yy], "SE.gif")){
					strncpy (applet_data->day_info[idx].wind, _("Sureste"), 8);
					idx++;
				}
				else if (tokens[yy] && strstr(tokens[yy], "SO.gif")){
					strncpy (applet_data->day_info[idx].wind, _("Suroeste"), 8);
					idx++;
				}
				else if (tokens[yy] && strstr(tokens[yy], "N.gif")){
					strncpy (applet_data->day_info[idx].wind, _("Norte"), 8);
					idx++;
				}
				else if (tokens[yy] && strstr(tokens[yy], "NE.gif")){
					strncpy (applet_data->day_info[idx].wind, _("Nordeste"), 8);
					idx++;
				}
				else if (tokens[yy] && strstr(tokens[yy], "NO.gif")){
					strncpy (applet_data->day_info[idx].wind, _("Noroeste"), 8);
					idx++;
				}
				else if (tokens[yy] && strstr(tokens[yy], "E.gif")){
					strncpy (applet_data->day_info[idx].wind, _("Este"), 8);
					idx++;
				}
				else if (tokens[yy] && strstr(tokens[yy], "O.gif")){
					strncpy (applet_data->day_info[idx].wind, _("Oeste"), 8);
					idx++;
				}
				else if (tokens[yy] && strstr(tokens[yy], "C.gif")){
					strncpy (applet_data->day_info[idx].wind, _("Calma"), 8);
					idx++;
				}
				//else if (tokens[yy]){
				//	strncpy (applet_data->day_info[idx].wind, "", 8);
				//	idx++;
				//}
			}
		}
		else if (type == PRECIP){
			idx=0;
			for (yy=1;yy < 40;yy++){
				if (tokens[yy][0] >= '0' && tokens[yy][0] <= '9'){
				//	printf ("\tPRECIP[%d]: %s - len: %d - idx: %d\n", yy,(tokens[yy]) ? tokens[yy] : NULL, strlen(tokens[yy]), idx);
					if (idx < MAX_DAYS){
						if (idx == 0){ // morning 0 + afternoon 1 => idx = 1
							strncpy (applet_data->day_info[0].precip, tokens[yy], 4); // 0
							strncpy (applet_data->day_info[1].precip, tokens[yy], 4);// 1
							idx=2;
							continue;
						}
						else if (idx == 2){
							strncpy (applet_data->day_info[2].precip, tokens[yy], 4); // 2
							strncpy (applet_data->day_info[3].precip, tokens[yy], 4); // 3
							idx=4;
							continue;
						}
						else if (idx == 4){
							strncpy (applet_data->day_info[4].precip, tokens[yy], 4); // 4
							strncpy (applet_data->day_info[5].precip, tokens[yy], 4); // 5
							idx=6;
							continue;
						}
						else{
							strncpy (applet_data->day_info[idx].precip, tokens[yy], 4); 
							idx++;
						}
					}
				}
			}
		}
		else if (type == MAX){
			idx=0;
			for (yy=0;yy < 61;yy++){
				if (strlen(tokens[yy]) > 2 || strlen(tokens[yy]) == 0) continue;
				if (strncmp(tokens[yy], "tr", 2) == 0) continue;
//				printf ("TMAX[%d]: %s - len: %d\n", yy,(tokens[yy]) ? tokens[yy] : NULL, strlen(tokens[yy]));
				if ((tokens[yy][0] >= '0' && tokens[yy][0] <= '9') || tokens[yy][0] == '-'){
			//		printf ("\tTMAX[%d]: %s - len: %d\n", yy,(tokens[yy]) ? tokens[yy] : NULL, strlen(tokens[yy]));
					if (idx < MAX_DAYS){
						if (idx == 0){ // morning 0 + afternoon 1 => idx = 1
							strncpy (applet_data->day_info[0].t_max, tokens[yy], 4);
							strncpy (applet_data->day_info[1].t_max, tokens[yy], 4);
							idx=2;
							continue;
						}
						else if (idx == 2){ // morning 0 + afternoon 1 => idx = 1
							strncpy (applet_data->day_info[2].t_max, tokens[yy], 4);
							strncpy (applet_data->day_info[3].t_max, tokens[yy], 4);
							idx=4;
							continue;
						}
						else if (idx == 4){ // morning 0 + afternoon 1 => idx = 1
							strncpy (applet_data->day_info[4].t_max, tokens[yy], 4);
							strncpy (applet_data->day_info[5].t_max, tokens[yy], 4);
							idx=6;
							continue;
						}
						else{
							strncpy (applet_data->day_info[idx].t_max, tokens[yy], 4);
							idx++;
						}
					}
				}
			}
		}
		else if (type == MIN){
			idx=0;
			for (yy=0;yy < 80;yy++){
				if (strlen(tokens[yy]) > 2 || strlen(tokens[yy]) == 0) continue;
				if (strncmp(tokens[yy], "tr", 2) == 0) continue;
//				printf ("TMIN[%d]: %s - len: %d\n", yy,(tokens[yy]) ? tokens[yy] : NULL, strlen(tokens[yy]));
				if ((tokens[yy][0] >= '0' && tokens[yy][0] <= '9') || tokens[yy][0] == '-'){
				//	printf ("\tTMIN[%d]: %s - len: %d\n", yy,(tokens[yy]) ? tokens[yy] : NULL, strlen(tokens[yy]));
					if (idx < MAX_DAYS){
						if (idx == 0){ // morning 0 + afternoon 1 => idx = 1
							strncpy (applet_data->day_info[0].t_min, tokens[yy], 4);
							strncpy (applet_data->day_info[1].t_min, tokens[yy], 4);
							idx=2;
							continue;
						}
						else if (idx == 2){ // morning 0 + afternoon 1 => idx = 1
							strncpy (applet_data->day_info[2].t_min, tokens[yy], 4);
							strncpy (applet_data->day_info[3].t_min, tokens[yy], 4);
							idx=4;
							continue;
						}
						else if (idx == 4){ // morning 0 + afternoon 1 => idx = 1
							strncpy (applet_data->day_info[4].t_min, tokens[yy], 4);
							strncpy (applet_data->day_info[5].t_min, tokens[yy], 4);
							idx=6;
							continue;
						}
						else{
							strncpy (applet_data->day_info[idx].t_min, tokens[yy], 4);
							idx++;
						}
					}
				}
			}
		}
			//strncpy (applet_data->day_info[9].cota_nieve, tk_snow[1], 10);
			//printf ("Max: %s\n", tokens[idx]);
		g_strfreev (tokens);
		if (temp_buf){
			g_free (temp_buf);
			temp_buf = 0;
		}
	}
	else{
		int x;
		for (x=0;x < 7;x++){
			if (type == MAX){
				strcpy (applet_data->day_info[x].t_max, "");
			}
			else if (type == MIN){
				strcpy (applet_data->day_info[x].t_min, "");
			}	
			else if (type == PRECIP){
				strcpy (applet_data->day_info[x].precip, "");
			}
			else if (type == SNOW){
				strcpy (applet_data->day_info[x].cota_nieve, "");
			}
			else if (type == WIND){
				strcpy (applet_data->day_info[x].wind, "");
			}
		}

	}
		
	tokens=0;
	tk_snow=0;
}
