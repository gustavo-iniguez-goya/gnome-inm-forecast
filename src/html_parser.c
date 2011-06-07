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
#define TEMPERATURES 0
#define THERMAL_SENSE 1

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
	int id_img=0;

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
		}

		cur_node = cur_node->next;
	}
	
	xmlXPathContextPtr ctx = xmlXPathNewContext(doc);
	xmlXPathCompExprPtr p = xmlXPathCtxtCompile(ctx, (xmlChar *)"/root");
	xmlXPathObjectPtr res = xmlXPathCompiledEval(p, ctx);

	 if (XPATH_NODESET != res->type)
		     return 1;

	fprintf(stdout, "Got object from first query:\n");
	xmlXPathDebugDumpObject(stdout, res, 0);
	xmlNodeSetPtr ns = res->nodesetval;
	if (!ns->nodeNr)
		return 1;
	ctx->node = ns->nodeTab[0];
	xmlXPathFreeObject(res);

	// sky data
	p = xmlXPathCtxtCompile(ctx, (xmlChar *)"prediccion/dia/estado_cielo");
	res = xmlXPathCompiledEval(p, ctx);
	parse_xml_sky (applet_data, doc, res->nodesetval);
	xmlXPathFreeObject(res);
	
	// Rain data
	p = xmlXPathCtxtCompile(ctx, (xmlChar *)"prediccion/dia/prob_precipitacion");
	res = xmlXPathCompiledEval(p, ctx);
	parse_xml_precip (applet_data, doc, res->nodesetval);
	xmlXPathFreeObject(res);
	
	// Snow data
	p = xmlXPathCtxtCompile(ctx, (xmlChar *)"prediccion/dia/cota_nieve_prov");
	res = xmlXPathCompiledEval(p, ctx);
	parse_xml_snow (applet_data, doc, res->nodesetval);
	xmlXPathFreeObject(res);

	// Temperatures data
	p = xmlXPathCtxtCompile(ctx, (xmlChar *)"prediccion/dia/temperatura");
	res = xmlXPathCompiledEval(p, ctx);
	parse_xml_temperatures (applet_data, doc, res->nodesetval, TEMPERATURES);
	xmlXPathFreeObject(res);
	
	// Thermal sense data
	p = xmlXPathCtxtCompile(ctx, (xmlChar *)"prediccion/dia/sens_termica");
	res = xmlXPathCompiledEval(p, ctx);
	parse_xml_temperatures (applet_data, doc, res->nodesetval, THERMAL_SENSE);
	xmlXPathFreeObject(res);
	
	// Wind data
	p = xmlXPathCtxtCompile(ctx, (xmlChar *)"prediccion/dia/viento");
	res = xmlXPathCompiledEval(p, ctx);
	parse_xml_wind (applet_data, doc, res->nodesetval);
	xmlXPathFreeObject(res);
	
	p = xmlXPathCtxtCompile(ctx, (xmlChar *)"prediccion/dia");
	res = xmlXPathCompiledEval(p, ctx);
	//if (XPATH_NODESET != res->type)
	//	return ;
	parse_xml_dates (applet_data, doc, res->nodesetval);
	xmlXPathFreeObject(res);

	
	//xmlXPathDebugDumpObject(stdout, res, 0);
	xmlXPathFreeContext(ctx);
	xmlFreeDoc (doc);
					
	if (applet_data->city_name != NULL && applet_data->provincia != NULL)
		g_snprintf (applet_data->city_long_desc, 64, "%s (%s)\0", applet_data->city_name, applet_data->provincia);

	
	set_tooltip (applet_data, 9, "");
	int x=0;
	GdkPixbuf *temp_pixbuf=0, *temp_pixbuf2=0;

	for (x=0;x < MAX_DAYS;x++){
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


	/*
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
	*/
}

/*
 * parse_xml_dates ()
 * Parses the property fecha="" of the <dia> tags.
 *
 */
void parse_xml_dates		( AppletData *applet_data, xmlDocPtr doc, xmlNodeSetPtr ns )
{
	GDate *g_date=NULL;
	int i = 0;
	int id_img = 0;
	
	if (doc == NULL || ns == 0){
		printf ("parse_xml_dates(). doc or ns null\n");
		return;
	}

	g_date = g_date_new ();
	g_date_set_time_t (g_date, time(NULL));

	// Get the first 4 days, to obtain the morning and afternoon data
	for (i = 0; i < 4;i++){
		xmlChar *fecha = xmlGetProp(ns->nodeTab[i], "fecha");
		printf ("\tfecha[%d]: %s\n", id_img, fecha);
		// date
		g_date_set_parse (g_date, fecha);
		if (g_date_valid(g_date)){
			if (g_date_get_weekday (g_date) == G_DATE_MONDAY){
				snprintf (applet_data->day_info[id_img].day, 32, "%s (%s)", _("Monday"), _("Morning"));
				snprintf (applet_data->day_info[id_img+1].day, 32, "%s (%s)", _("Monday"), _("Afternoon"));
			}
			else if (g_date_get_weekday (g_date) == G_DATE_TUESDAY){
				snprintf (applet_data->day_info[id_img].day, 32, "%s (%s)", _("Tuesday"), _("Morning"));
				snprintf (applet_data->day_info[id_img+1].day, 32, "%s (%s)", _("Tuesday"), _("Afternoon"));
			}
			else if (g_date_get_weekday (g_date) == G_DATE_WEDNESDAY){
				snprintf (applet_data->day_info[id_img].day, 32, "%s (%s)", _("Wednesday"), _("Morning"));
				snprintf (applet_data->day_info[id_img+1].day, 32, "%s (%s)", _("Wednesday"), _("Afternoon"));
			}
			else if (g_date_get_weekday (g_date) == G_DATE_THURSDAY){
				snprintf (applet_data->day_info[id_img].day, 32, "%s (%s)", _("Thursday"), _("Morning"));
				snprintf (applet_data->day_info[id_img+1].day, 32, "%s (%s)", _("Thursday"), _("Afternoon"));
			}
			else if (g_date_get_weekday (g_date) == G_DATE_FRIDAY){
				snprintf (applet_data->day_info[id_img].day, 32, "%s (%s)", _("Friday"), _("Morning"));
				snprintf (applet_data->day_info[id_img+1].day, 32, "%s (%s)", _("Friday"), _("Afternoon"));
			}
			else if (g_date_get_weekday (g_date) == G_DATE_SATURDAY){
				snprintf (applet_data->day_info[id_img].day, 32, "%s (%s)", _("Saturday"), _("Morning"));
				snprintf (applet_data->day_info[id_img+1].day, 32, "%s (%s)", _("Saturday"), _("Afternoon"));
			}
			else if (g_date_get_weekday (g_date) == G_DATE_SUNDAY){
				snprintf (applet_data->day_info[id_img].day, 32, "%s (%s)", _("Sunday"), _("Morning"));
				snprintf (applet_data->day_info[id_img+1].day, 32, "%s (%s)", _("Sunday"), _("Afternoon"));
			}
			else if (g_date_get_weekday (g_date) == G_DATE_BAD_WEEKDAY){
				strncpy (applet_data->day_info[id_img].day, fecha, 32);
				strncpy (applet_data->day_info[id_img+1].day, fecha, 32);
			}
				
			id_img++;
		}
		else{
			strncpy (applet_data->day_info[id_img].day, fecha, 32);
			strncpy (applet_data->day_info[id_img+1].day, fecha, 32);
			id_img++;
		}

		xmlFree(fecha);
		id_img++;
	}
	// Now get the last 3 days, which don't have morning && afternoon data
	for (i = 4; i < 6;i++){
		xmlChar *fecha = xmlGetProp(ns->nodeTab[i], "fecha");
		printf ("\tfecha[%d]: %s\n", id_img, fecha);
		// date
		g_date_set_parse (g_date, fecha);
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
				strncpy (applet_data->day_info[id_img].day, fecha, 32);
		}
		else
			strncpy (applet_data->day_info[id_img].day, fecha, 32);

		xmlFree(fecha);
		id_img++;
	}
	
	g_date_free (g_date);
}

/*
 * parse_xml_sky ()
 * Parses the tags <estado_cielo>
 *
 */
void parse_xml_sky		( AppletData *applet_data, xmlDocPtr doc, xmlNodeSetPtr ns )
{
	int theme_len = 128;
	char *theme = g_new0(char, theme_len);
	int id_img = 0;
	int i=0;

	if (doc == NULL || ns == 0){
		printf ("parse_xml_sky(). doc or ns null\n");
		return;
	}

	for (i = 0; i < ns->nodeNr - 1;i++){
		//printf ("i = %d\n", i);
		xmlChar *horario= xmlGetProp(ns->nodeTab[i], "periodo");
		xmlChar *desc = xmlGetProp(ns->nodeTab[i], "descripcion");
		// img
		xmlChar *img_num = xmlNodeListGetString(doc, ns->nodeTab[i]->xmlChildrenNode, 1);

		if (horario != NULL && (strcmp (horario, "00-12") == 0 || strcmp(horario, "12-24") == 0)){
			strncpy (applet_data->day_info[id_img].state, _(desc), 36);
			printf ("\t[%d]estado_cielo: %s - %s - %s\n", id_img, desc, horario, img_num);
			snprintf (theme, theme_len, "%s%s/%s.png", PIXMAPS_DIR, applet_data->theme, img_num);
			gtk_image_set_from_file (GTK_IMAGE(applet_data->image[id_img]), theme);
			
			id_img++;
		}
		else if (horario == NULL){
			strncpy (applet_data->day_info[id_img].state, _(desc), 36);
			printf ("\t[%d]estado_cielo: %s - %s - %s\n", id_img, desc, horario, img_num);
			snprintf (theme, theme_len, "%s%s/%s.png", PIXMAPS_DIR, applet_data->theme, img_num);
			gtk_image_set_from_file (GTK_IMAGE(applet_data->image[id_img]), theme);

			id_img++;
		}
		xmlFree (desc);
		xmlFree (img_num);
		xmlFree (horario);
	}
	
	if (theme){
		g_free (theme);
		theme = 0;
	}
}
	
/*
 * parse_xml_snow ()
 * Parses the tags <cota_nieve_prov>
 *
 */
void parse_xml_snow		( AppletData *applet_data, xmlDocPtr doc, xmlNodeSetPtr ns )
{
	int i = 0;
	int id_img = 0;
	
	if (doc == NULL || ns == 0){
		printf ("parse_xml_snow(). doc or ns null\n");
		return;
	}

	for (i = 0; i < ns->nodeNr - 1;i++){
		//strcpy (applet_data->day_info[1].cota_nieve, "");
		xmlChar *cota = xmlNodeListGetString(doc, ns->nodeTab[i]->xmlChildrenNode, 1);
		xmlChar *horario= xmlGetProp(ns->nodeTab[i], "periodo");
		if (cota != NULL && horario != NULL && (strcmp (horario, "00-12") == 0 || strcmp(horario, "12-24") == 0)){
			strcpy (applet_data->day_info[id_img].cota_nieve, cota);
			id_img++;
		}
		else if (horario == NULL && cota != NULL){
			strcpy (applet_data->day_info[id_img].cota_nieve, cota);
			id_img++;
		}
		else
			strcpy (applet_data->day_info[id_img].cota_nieve, "-");

		xmlFree (cota);
		xmlFree (horario);
	}
}

/*
 * parse_xml_precip ()
 * Parses the tags <prob_precipitacion>
 *
 */
void parse_xml_precip		( AppletData *applet_data, xmlDocPtr doc, xmlNodeSetPtr ns )
{
	int i = 0;
	int id_img = 0;
	
	if (doc == NULL || ns == 0){
		printf ("parse_xml_precip(). doc or ns null\n");
		return;
	}

	for (i = 0; i < ns->nodeNr - 1;i++){
		//strcpy (applet_data->day_info[1].cota_nieve, "");
		xmlChar *precip = xmlNodeListGetString(doc, ns->nodeTab[i]->xmlChildrenNode, 1);
		xmlChar *horario= xmlGetProp(ns->nodeTab[i], "periodo");
		if (precip != NULL && horario != NULL && (strcmp (horario, "00-12") == 0 || strcmp(horario, "12-24") == 0)){
			snprintf (applet_data->day_info[id_img].precip, 6, "%s%%\0", precip);
			id_img++;
		}
		else if (horario == NULL){
			snprintf (applet_data->day_info[id_img].precip, 6, "%s%%\0", precip);
			id_img++;
		}
		else
			strcpy (applet_data->day_info[id_img].cota_nieve, "-");

		xmlFree (precip);
		xmlFree (horario);
	}
}

/*
 * parse_xml_temperatures ()
 * Parses the tags <temperatura>
 *
 */
void parse_xml_temperatures	( AppletData *applet_data, xmlDocPtr doc, xmlNodeSetPtr ns, int type )
{
	int i = 0;
	volatile int id_img = 0;
	char *temp=0;
	
	if (doc == NULL || ns == 0){
		printf ("parse_xml_temperatures(). doc or ns null\n");
		return;
	}

	if (type == THERMAL_SENSE)
		temp = g_new0 (char, 4);

	for (i = 0; i < ns->nodeNr - 1;i++){
		xmlNodePtr cur_tmax = ns->nodeTab[i]->xmlChildrenNode->next; // maxima
	        xmlNodePtr cur_tmin = cur_tmax->next->next; //minima

		//xmlChar *horario= xmlGetProp(ns->nodeTab[i], "hora");
		xmlChar *tmax = xmlNodeListGetString(doc, cur_tmax->xmlChildrenNode, 1);
		xmlChar *tmin = xmlNodeListGetString(doc, cur_tmin->xmlChildrenNode, 1);
		
		if (tmax != NULL && tmin != NULL && id_img < 10){
			if (type == TEMPERATURES){
				if (tmax != NULL && cur_tmin->next != NULL){
					printf ("Temperatura max.: %s\n", tmax);
					printf ("Temperatura min.: %s\n", tmin);
					// Morning
					snprintf (applet_data->day_info[id_img].t_max, 8, "%s", tmax);
					snprintf (applet_data->day_info[id_img].t_min, 8, "%s", tmin);
					
					id_img++;
					// Afternoon
					snprintf (applet_data->day_info[id_img].t_max, 8, "%s", tmax);
					snprintf (applet_data->day_info[id_img].t_min, 8, "%s", tmin);
				}
				else if (cur_tmin->next == NULL){
					// Last days with general info
					printf ("Temperatura max.: %s\n", tmax);
					printf ("Temperatura min.: %s\n", tmin);
					snprintf (applet_data->day_info[id_img].t_max, 8, "%s", tmax);
					snprintf (applet_data->day_info[id_img].t_min, 8, "%s", tmin);
				}
				else{
					strcpy (applet_data->day_info[id_img].t_max, "-");
					strcpy (applet_data->day_info[id_img].t_min, "-");
				}

				id_img++;
			}
			else{ // <sens_termica>

				if (tmax != NULL && cur_tmin->next != NULL){
					strncpy (temp, applet_data->day_info[id_img].t_max, 4);

					if (strcmp (tmax, applet_data->day_info[id_img].t_max) != 0){
						snprintf (applet_data->day_info[id_img].t_max, 8, "%s (%s)", temp, tmax);
						printf ("Sensacion termica max: %s - %s\n", temp, tmax);
					}

					strncpy (temp, applet_data->day_info[id_img].t_min, 4);
					if (strcmp (tmin, applet_data->day_info[id_img].t_min) != 0){
						snprintf (applet_data->day_info[id_img].t_min, 8, "%s (%s)", temp, tmin);
						printf ("Sensacion termica: %s - %s\n", temp, tmin);
					}
					
					id_img++;
				}
				else if (cur_tmin->next == NULL){
					// Last days with general info
					strncpy (temp, applet_data->day_info[id_img].t_max, 4);
					if (strcmp (tmax, applet_data->day_info[id_img].t_max) != 0){
						snprintf (applet_data->day_info[id_img].t_max, 8, "%s (%s)", temp, tmax);
						printf ("Sensacion termica max: %s - %s\n", temp, tmax);
					}

					strncpy (temp, applet_data->day_info[id_img].t_min, 4);
					if (strcmp (tmin, applet_data->day_info[id_img].t_min) != 0){
						snprintf (applet_data->day_info[id_img].t_min, 8, "%s (%s)", temp, tmin);
						printf ("Sensacion termica: %s - %s\n", temp, tmin);
					}
				}
				else{
				}
				
				id_img++;
			}
		}
		else if (id_img < 10){
			strcpy (applet_data->day_info[id_img].t_max, "-");
			strcpy (applet_data->day_info[id_img].t_min, "-");
		}

		xmlFree (tmax);
		xmlFree (tmin);

		//id_img++;
	}

	if (type == THERMAL_SENSE)
		g_free (temp);
}

/*
 * parse_xml_wind ()
 * Parses the tags <viento>
 *
 */
void parse_xml_wind		( AppletData *applet_data, xmlDocPtr doc, xmlNodeSetPtr ns )
{
	int i = 0;
	int id_img = 0;
	
	if (doc == NULL || ns == 0){
		printf ("parse_xml_wind(). doc or ns null\n");
		return;
	}

	for (i = 0; i < ns->nodeNr - 1;i++){
		xmlNodePtr cur_tdir = ns->nodeTab[i]->xmlChildrenNode->next; // direccion
		xmlNodePtr cur_tvel = cur_tdir->next->next; // velocidad
		xmlChar *direction = xmlNodeListGetString(doc, cur_tdir->xmlChildrenNode, 1);
		xmlChar *vel = xmlNodeListGetString(doc, cur_tvel->xmlChildrenNode, 1);
		xmlChar *horario= xmlGetProp(ns->nodeTab[i], "periodo");

		if (direction != NULL && horario != NULL && (strcmp (horario, "00-12") == 0 || strcmp(horario, "12-24") == 0)){
			snprintf (applet_data->day_info[id_img].wind, 32, "%s (%s km/h)\0", direction, vel);
			id_img++;
		}
		else if (horario == NULL){
			snprintf (applet_data->day_info[id_img].wind, 32, "%s (%s km/h)\0", direction, vel);
			id_img++;
		}
		else
			strcpy (applet_data->day_info[id_img].wind, "-");

		xmlFree (direction);
		xmlFree (horario);
		xmlFree (vel);
	}
}
