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

void parse_xml_data 		( PanelApplet *applet, AppletData *applet_data, char *buf )
{
	int theme_len = 128;
	char *theme = g_new0(char, theme_len);
	int id_img=0;
	GDate *g_date=NULL;
	GTimeVal *g_timeval=NULL;

	//g_get_current_time (&g_timeval);
	//g_date = g_date_new ();
	//g_date_set_time_val (g_date, &g_timeval);

	xmlDocPtr doc=NULL;
	xmlNodePtr root_element=NULL;
	xmlNodePtr cur_node=NULL;
	xmlNodePtr a_node=NULL;
	
	doc = xmlReadMemory (buf, strlen(buf), NULL, NULL, XML_PARSE_RECOVER);
	root_element = xmlDocGetRootElement (doc);
	a_node = root_element->xmlChildrenNode;
	cur_node = a_node->next;

	// http://xmlsoft.org/tutorial/apg.html
	printf("parse_xml_data(): %s\n", root_element->name);
	
	while (cur_node != NULL){
		if (cur_node->type == XML_ELEMENT_NODE) {
			printf("ELEMENT: %s\n", cur_node->name);
			if (strcmp(cur_node->name, (const char *)"nombre") == 0){
				xmlChar *key1 = xmlNodeListGetString(doc, cur_node->xmlChildrenNode, 1);
				if (key1 != NULL){
					strncpy (applet_data->city_name, key1, 64);
					printf ("\tLocalidad: %s\n", key1);
				}

				xmlFree(key1);
			}
			else
			if (strcmp(cur_node->name, (const char *)"provincia") == 0){
				xmlChar *key2 = xmlNodeListGetString(doc, cur_node->xmlChildrenNode, 1);
				if (key2 != NULL){
					strncpy (applet_data->provincia, key2, 64);
					printf ("\tProvincia: %s\n", key2);
				}
				xmlFree(key2);
			}else
			if (strcmp(cur_node->name, (const char *)"elaborado") == 0){
				xmlChar *key3 = xmlNodeListGetString(doc, cur_node->xmlChildrenNode, 1);
				if (key3 != NULL){
					strncpy (applet_data->last_update, _("Last update: "), 64);
					strncat (applet_data->last_update, key3, 64);
					printf ("\tElaborado: %s\n", key3);
				}
				xmlFree(key3);
			}

			if (strcmp(cur_node->name, (const char *)"prediccion") == 0){
				xmlNodePtr cur = cur_node->xmlChildrenNode;
				while (cur != NULL){
					if (strcmp(cur->name, (const char *)"dia") == 0){
						xmlChar *key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
						xmlChar *key2 = xmlGetProp(cur, "fecha");
						printf ("\tfecha[%d]: %s - Img: %s\n", id_img, key2, key);
						// date
						g_date_set_parse (g_date, key2);
						if (g_date_valid(g_date)){
							if (g_date_get_weekday (g_date) == G_DATE_MONDAY)
								strncpy (applet_data->day_info[id_img].day, _("Monday"), 32);
							else if (g_date_get_weekday (g_date) == G_DATE_TUESDAY)
								strncpy (applet_data->day_info[id_img].day, _("Tuesday"), 32);
							else if (g_date_get_weekday (g_date) == G_DATE_WEDNESDAY)
								strncpy (applet_data->day_info[id_img].day, _("Wednesday"), 32);
							else if (g_date_get_weekday (g_date) == G_DATE_THURSDAY)
								strncpy (applet_data->day_info[id_img].day, _("Thursday"), 32);
							else if (g_date_get_weekday (g_date) == G_DATE_FRIDAY)
								strncpy (applet_data->day_info[id_img].day, _("Friday"), 32);
							else if (g_date_get_weekday (g_date) == G_DATE_SATURDAY)
								strncpy (applet_data->day_info[id_img].day, _("Saturday"), 32);
							else if (g_date_get_weekday (g_date) == G_DATE_SUNDAY)
								strncpy (applet_data->day_info[id_img].day, _("Sunday"), 32);
							else if (g_date_get_weekday (g_date) == G_DATE_BAD_WEEKDAY)
								strncpy (applet_data->day_info[id_img].day, key2, 32);
						}
						else
							strncpy (applet_data->day_info[id_img].day, key2, 32);

						xmlFree(key);
						xmlFree(key2);

						xmlNodePtr cur_dia = cur->xmlChildrenNode;
						while (cur_dia != NULL){
							printf ("\t[%d]dia: %s\n", id_img, cur_dia->name);
							if (strcmp(cur_dia->name, (const char *)"estado_cielo") == 0){
								xmlChar *desc = xmlGetProp(cur_dia, "descripcion");
								xmlChar *img_num = xmlNodeListGetString(doc, cur_dia->xmlChildrenNode, 1);
								// weather description
								strncpy (applet_data->day_info[id_img].state, _(desc), 36);
								printf ("\t[%d]estado_cielo->descripcion: %s - %s\n", id_img, desc, img_num);
								snprintf (theme, theme_len, "%s%s/%s.png", PIXMAPS_DIR, applet_data->theme, img_num);
								gtk_image_set_from_file (GTK_IMAGE(applet_data->image[id_img]), theme);
								xmlFree (desc);
								xmlFree (img_num);
							}
							//strcpy (applet_data->day_info[1].cota_nieve, "");
							else if (strcmp(cur_dia->name, (const char *)"cota_nieve_prov") == 0){
								xmlChar *cota = xmlNodeListGetString(doc, cur_dia->xmlChildrenNode, 1);
								if (cota != NULL)
									strcpy (applet_data->day_info[id_img].cota_nieve, cota);
								else
									strcpy (applet_data->day_info[id_img].cota_nieve, "");

								xmlFree (cota);
							}
							//strncpy (applet_data->day_info[idx].precip, tokens[yy], 4); 
							else if (strcmp(cur_dia->name, (const char *)"prob_precipitacion") == 0){
								xmlChar *precip = xmlNodeListGetString(doc, cur_dia->xmlChildrenNode, 1);
								if (precip != NULL)
									snprintf (applet_data->day_info[id_img].precip, 4, "%s%%\0", precip);
								else
									strcpy (applet_data->day_info[id_img].precip, "");

								xmlFree (precip);
							}
							//strcpy (applet_data->day_info[x].t_max, "");
							else if (strcmp(cur_dia->name, (const char *)"temperatura") == 0){
								xmlNodePtr cur_tmax = cur_dia->xmlChildrenNode->next; // maxima
                                                                xmlNodePtr cur_tmin = cur_tmax->next->next; //minima

								xmlChar *tmax = xmlNodeListGetString(doc, cur_tmax->xmlChildrenNode, 1);
								xmlChar *tmin = xmlNodeListGetString(doc, cur_tmin->xmlChildrenNode, 1);
								if (tmax != NULL){
									snprintf (applet_data->day_info[id_img].t_max, 4, "%s", tmax);
									snprintf (applet_data->day_info[id_img].t_min, 4, "%s", tmin);
								}
								else{
									strcpy (applet_data->day_info[id_img].t_max, "");
									strcpy (applet_data->day_info[id_img].t_min, "");
								}

								xmlFree (tmax);
								xmlFree (tmin);
							}
							//strncpy (applet_data->day_info[idx].wind, _("Calma"), 8);
							else if (strcmp(cur_dia->name, (const char *)"viento") == 0){
								xmlNodePtr cur_tdir = cur_dia->xmlChildrenNode->next; // direccion
                                                                xmlNodePtr cur_tvel = cur_tdir->next->next; // velocidad
								xmlChar *direction = xmlNodeListGetString(doc, cur_tdir->xmlChildrenNode, 1);
								xmlChar *vel = xmlNodeListGetString(doc, cur_tvel->xmlChildrenNode, 1);
								if (direction != NULL)
									snprintf (applet_data->day_info[id_img].wind, 32, "%s (%s km/h)\0", direction, vel);
								else
									strcpy (applet_data->day_info[id_img].wind, "");

								xmlFree (direction);
								xmlFree (vel);
							}
						
						cur_dia = cur_dia->next; // dia
						}
						id_img++;
					} // new day
				cur = cur->next; //prediccion
				}
			}
		}

		cur_node = cur_node->next;
	}
	xmlFreeDoc (doc);
		
	set_tooltip (applet_data, id_img, "");
	int x=0;
	GdkPixbuf *temp_pixbuf=0, *temp_pixbuf2=0;

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


 	int iDays = atoi (applet_data->show_days);
	if (iDays > 7){
		if (id_img == 10){
			gtk_widget_show (applet_data->event_box[9]);
			gtk_widget_show (applet_data->event_box[8]);
			gtk_widget_show (applet_data->event_box[7]);
		}
		else if (id_img == 9){
			gtk_widget_hide (applet_data->event_box[9]);
			gtk_widget_show (applet_data->event_box[8]);
			gtk_widget_show (applet_data->event_box[7]);
		}
		else if (id_img == 8){
			gtk_widget_hide (applet_data->event_box[9]);
			gtk_widget_hide (applet_data->event_box[8]);
			gtk_widget_show (applet_data->event_box[7]);
		}
	}
	
	if (theme){
		g_free (theme);
		theme = 0;
	}
}

void parse_sky_data 		( PanelApplet *applet, AppletData *applet_data, char *buf )
{
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
		if (type == 2){ // capital
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
