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

#include <string.h>
#include "callbacks.h"
#include "gconf_callbacks.h"
#include "main.h"
#include "preferences.h"
#include <libxml/HTMLtree.h>
#include <libxml/HTMLparser.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

static char *convert_str_to_utf8 	( char *string )
{
	char *bk_utf8=0;
	char *temp_buf=0;

	if (string){
		bk_utf8 = strndup (string, 64);
		
		if (!g_utf8_validate (bk_utf8, -1, NULL))
			//strncpy (applet_data->city_name, string, 64);
		//else{
			temp_buf = g_locale_to_utf8(bk_utf8, -1, NULL, NULL, NULL);
			if (!temp_buf)
				//g_utf8_strncpy (applet_data->city_name, temp_buf, 64);
			//else{
				temp_buf = g_convert (bk_utf8, -1, "UTF-8", "ISO-8859-1", 0, 0, NULL);
				if (!temp_buf)
					//strncpy (applet_data->city_name, temp_buf, 64);
				//else
					strcpy (temp_buf, "");
		
		if (bk_utf8){
			free (bk_utf8);
			bk_utf8=0;
		}
		return temp_buf;
	}
	else
		return NULL;
}

static void message_box 	( AppletData *applet_data, GtkMessageType type, GtkButtonsType buttons, const char *text, const char *secondary_text )
{
	GtkWidget *dialog=0;
	dialog = gtk_message_dialog_new (NULL, GTK_DIALOG_MODAL, type, buttons, text);
	gtk_message_dialog_format_secondary_text (GTK_MESSAGE_DIALOG(dialog), secondary_text);
	gtk_dialog_run (GTK_DIALOG(dialog));
	gtk_widget_destroy (dialog);
}

static void unset_images 	( AppletData *applet_data )
{
	int x=0;
	strcpy (applet_data->city_name,"");
	strcpy (applet_data->provincia,"");
	strcpy (applet_data->last_update, "");
	set_tooltip (applet_data, -1, "");
	for (x=0;x < 10;x++){
		gtk_image_set_from_file (GTK_IMAGE(applet_data->image[x]), "");
		strcpy (applet_data->day_info[x].day,"");
	}
}

char *parse_week_day_name	( const char *day_name )
{
	char *day_temp=0;
	char **tokens=0;
	/* This mem is free at line 364 and succesives */
	day_temp = (char *)g_malloc0(64);
	
	if (day_name){
		tokens = g_strsplit_set (day_name, " ", -1);

		if (tokens[1]){
			if (strstr (day_name, "Mi&eacute;rcoles"))
				snprintf (day_temp, 64, _("Wednesday %s"), tokens[1]);
			else if (strstr (day_name, "S&aacute;bado"))
				snprintf (day_temp, 64, _("Saturday %s"), tokens[1]);
			else if (strstr (day_name, "Lun "))
				snprintf (day_temp, 64, _("Monday %s"), tokens[1]);
			else if (strstr (day_name, "Mar "))
				snprintf (day_temp, 64, _("Tuesday %s"), tokens[1]);
			else if (strstr (day_name, "Mi&eacute; "))
				snprintf (day_temp, 64, _("Wednesday %s"), tokens[1]);
			else if (strstr (day_name, "Jue "))
				snprintf (day_temp, 64, _("Thursday %s"), tokens[1]);
			else if (strstr (day_name, "Vie "))
				snprintf (day_temp, 64, _("Friday %s"), tokens[1]);
			else if (strstr (day_name, "S&aacute;b "))
				snprintf (day_temp, 64, _("Saturday %s"), tokens[1]);
			else if (strstr (day_name, "Dom "))
				snprintf (day_temp, 64, _("Sunday %s"), tokens[1]);
			else
				snprintf (day_temp, 64, "%s", day_name);
		
			g_strfreev (tokens);
		}
		tokens = 0;

	}
	return day_temp;
}

void quit 			( GtkWidget *widget )
{
	if (widget){
		gtk_widget_hide (widget);
		gtk_widget_destroy (widget);
		widget = 0;
	}
}

void create_window 		( AppletData *applet_data, const char *name )
{
	GladeXML *xml;
	static GdkPixbuf *pix;
	static GtkWidget *win=0;
	GtkWidget *lb1=0, *lb2=0, *lb3=0, *lb4=0, *lb5=0, *lb6=0, *lb7=0, *img=0;// *table=0;
	int x,y=0;
	short int idx=0,img_idx=0;
	char *day;
	GdkColor color;
	static int window_shown;

	if (strcmp (name, "event11") == 0){
		return ;
	}
	if (window_shown){
		quit (win);
		win = 0;
		/*
		if (pix){
			g_object_unref (G_OBJECT (pix));
			pix = 0;
		}
		*/
	}
	else{
		day = g_new(char,128);

		xml = glade_xml_new (PACKAGE_DIR"/gnome-inm-glade.glade", "win_day", NULL);
		win = glade_xml_get_widget (xml, "win_day");
		lb1 = glade_xml_get_widget (xml, "label_day1");
		lb2 = glade_xml_get_widget (xml, "label_day2");
		lb3 = glade_xml_get_widget (xml, "label_day3");
		lb4 = glade_xml_get_widget (xml, "label_day4");
		lb5 = glade_xml_get_widget (xml, "label_day5");
		lb6 = glade_xml_get_widget (xml, "label_day6");
		lb7 = glade_xml_get_widget (xml, "label_day7");
		img = glade_xml_get_widget (xml, "img");
		
		g_signal_connect (G_OBJECT(win), "destroy", G_CALLBACK(on_window_terminate), NULL);

		color.red = 65535;
		color.blue = 65535;
		color.green = 65535;
		gtk_widget_modify_bg (win, GTK_STATE_NORMAL, &color);
		
		if (strcmp (name, "event1") == 0){
			img_idx=0;
			idx = 0;
		}
		else if (strcmp (name, "event2") == 0){
			img_idx=1;
			idx = 0;
		}
		else if (strcmp (name, "event3") == 0){
			img_idx=2;
			idx = 2;
		}
		else if (strcmp (name, "event4") == 0){
			img_idx=3;
			idx = 2;
		}
		else if (strcmp (name, "event5") == 0){
			img_idx=4;
			idx = 4;
		}
		else if (strcmp (name, "event6") == 0){
			img_idx=5;
			idx = 5;
		}
		else if (strcmp (name, "event7") == 0){
			img_idx=6;
			idx = 6;
		}
		else if (strcmp (name, "event8") == 0){
			img_idx=7;
			idx = 7;
		}
		else if (strcmp (name, "event9") == 0){
			img_idx=8;
			idx = 8;
		}
		else if (strcmp (name, "event10") == 0){
			img_idx=9;
			idx = 9;
		}
		//printf ("event name: %s - %d - %d\n", name, idx, img_idx);
			
		gtk_label_set_markup (GTK_LABEL(lb1), applet_data->day_info[idx].state);
		gtk_label_set_markup (GTK_LABEL(lb2), applet_data->day_info[idx].precip);
		gtk_label_set_text (GTK_LABEL(lb3), applet_data->day_info[idx].cota_nieve);
		gtk_label_set_markup (GTK_LABEL(lb4), applet_data->day_info[idx].t_max);
		gtk_label_set_markup (GTK_LABEL(lb5), applet_data->day_info[idx].t_min);
		gtk_label_set_markup (GTK_LABEL(lb6), applet_data->day_info[idx].wind);
		pix = gtk_image_get_pixbuf (GTK_IMAGE(applet_data->image[img_idx]));
		gtk_image_set_from_pixbuf (GTK_IMAGE(img), pix);
		
		snprintf (day, 128, "<span weight=\"bold\">%s</span>", applet_data->day_info[idx].day);
		//printf ("Day: %s\n", day);
		gtk_label_set_markup (GTK_LABEL(lb7), day);
		//printf ("state: %s\n",  applet_data->day_info[idx].state);

		gtk_window_get_position (GTK_WINDOW (win), &x, &y);
		y += applet_data->applet_size;
		gtk_window_move (GTK_WINDOW(win), x, y);
		gtk_widget_show (win);
		x = y = 0;
		g_object_unref (G_OBJECT (xml));
	}
	window_shown = !window_shown;
	g_free (day);
}

void set_tooltip		( AppletData *applet_data, const int id, const gchar* tip )
{
	int x=0;
	char *temp=0;
	char *str_morning=_("Morning");
	char *str_afternoon=_("Afternoon");
	temp = g_new0 (char, 512);
	if (id == 0 || id == 2 || id == 4)
		snprintf (temp, 512, "%s - %s\n%s (%s)\n\n%s\n%s", applet_data->city_name, applet_data->provincia, applet_data->day_info[id].day, str_morning, tip, applet_data->last_update);
	else if (id == 1 || id == 3 || id == 5)
		snprintf (temp, 512, "%s - %s\n%s (%s)\n\n%s\n%s", applet_data->city_name, applet_data->provincia, applet_data->day_info[id].day, str_afternoon, tip, applet_data->last_update);
	else if (id == 10)
		snprintf (temp, 512, "%s", tip);
	else
		snprintf (temp, 512, "%s - %s\n%s\n\n%s\n%s", applet_data->city_name, applet_data->provincia, applet_data->day_info[id].day, tip, applet_data->last_update);


	if (id == -1){
		for (x=0;x < 10;x++){
			gtk_tooltips_set_tip (applet_data->tips, applet_data->event_box[x], "", NULL);
		}
	}
	else{
		gtk_tooltips_set_tip (applet_data->tips, applet_data->event_box[id], temp, NULL);
		gtk_tooltips_enable (applet_data->tips);
	}
	g_free (temp);
	temp = 0;
}

void parse_sky_data 		( PanelApplet *applet, AppletData *applet_data, char *buf )
{
	const char delimiters[] = "=\"";
	char *temp=0;
	char *temp_buf=0;
	GdkPixbuf *temp_pixbuf=0, *temp_pixbuf2=0;
	int x=0;
	//int iDays = g_ascii_strtod (panel_applet_gconf_get_string (PANEL_APPLET(applet_data->applet), "days", NULL), NULL);
	int iDays = 10;
	char *t = (char *)panel_applet_gconf_get_string (PANEL_APPLET(applet_data->applet), "theme", NULL);
	if (!t)
		strcpy (t, "inm");

	//printf ("\n\n\n\n\n=======================\n\nSKY:%s\n===================\n\n", buf);
	if (applet && applet_data && buf){
		char *theme = g_new0(char, 1024);

		temp_buf = strdup (buf);
		temp = strtok (strstr (temp_buf, "Estado del cielo"), delimiters);
		temp = strtok (NULL, delimiters);
		for (x=0;x < iDays;x++){
			temp = strtok (NULL, delimiters); /* Imagen */
			//printf ("IMAGE: %d: %s\n", x, temp);
			if (applet_data->image[x] && temp){
				if (strstr (temp, "11.gif")){
					snprintf (theme, 1024, "%s%s/11.png", PIXMAPS_DIR, t);
					gtk_image_set_from_file (GTK_IMAGE(applet_data->image[x]), theme);
				}
				else if (strstr (temp, "12.gif")){
					snprintf (theme, 1024, "%s%s/12.png", PIXMAPS_DIR, t);
					gtk_image_set_from_file (GTK_IMAGE(applet_data->image[x]), theme);
				}
				else if (strstr (temp, "13.gif")){
					snprintf (theme, 1024, "%s%s/13.png", PIXMAPS_DIR, t);
					gtk_image_set_from_file (GTK_IMAGE(applet_data->image[x]), theme);
				}
				else if (strstr (temp, "14.gif")){
					snprintf (theme, 1024, "%s%s/14.png", PIXMAPS_DIR, t);
					gtk_image_set_from_file (GTK_IMAGE(applet_data->image[x]), theme);
				}
				else if (strstr (temp, "15.gif")){
					snprintf (theme, 1024, "%s%s/15.png", PIXMAPS_DIR, t);
					gtk_image_set_from_file (GTK_IMAGE(applet_data->image[x]), theme);
				}
				else if (strstr (temp, "16.gif")){
					snprintf (theme, 1024, "%s%s/16.png", PIXMAPS_DIR, t);
					gtk_image_set_from_file (GTK_IMAGE(applet_data->image[x]), theme);
				}
				else if (strstr (temp, "23.gif")){
					snprintf (theme, 1024, "%s%s/23.png", PIXMAPS_DIR, t);
					gtk_image_set_from_file (GTK_IMAGE(applet_data->image[x]), theme);
				}
				else if (strstr (temp, "24.gif")){
					snprintf (theme, 1024, "%s%s/24.png", PIXMAPS_DIR, t);
					gtk_image_set_from_file (GTK_IMAGE(applet_data->image[x]), theme);
				}
				else if (strstr (temp, "25.gif")){
					snprintf (theme, 1024, "%s%s/25.png", PIXMAPS_DIR, t);
					gtk_image_set_from_file (GTK_IMAGE(applet_data->image[x]), theme);
				}
				else if (strstr (temp, "26.gif")){
					snprintf (theme, 1024, "%s%s/26.png", PIXMAPS_DIR, t);
					gtk_image_set_from_file (GTK_IMAGE(applet_data->image[x]), theme);
				}
				else if (strstr (temp, "33.gif")){
					snprintf (theme, 1024, "%s%s/33.png", PIXMAPS_DIR, t);
					gtk_image_set_from_file (GTK_IMAGE(applet_data->image[x]), theme);
				}
				else if (strstr (temp, "34.gif")){
					snprintf (theme, 1024, "%s%s/34.png", PIXMAPS_DIR, t);
					gtk_image_set_from_file (GTK_IMAGE(applet_data->image[x]), theme);
				}
				else if (strstr (temp, "35.gif")){
					snprintf (theme, 1024, "%s%s/35.png", PIXMAPS_DIR, t);
					gtk_image_set_from_file (GTK_IMAGE(applet_data->image[x]), theme);
				} 
				else if (strstr (temp, "36.gif")){
					snprintf (theme, 1024, "%s%s/36.png", PIXMAPS_DIR, t);
					gtk_image_set_from_file (GTK_IMAGE(applet_data->image[x]), theme);
				}
			
				if (temp){
					temp_pixbuf = gtk_image_get_pixbuf (GTK_IMAGE(applet_data->image[x]));
					temp_pixbuf2 = gdk_pixbuf_scale_simple (temp_pixbuf, applet_data->applet_size - 3, applet_data->applet_size - 3, GDK_INTERP_BILINEAR);
					gtk_image_set_from_pixbuf (GTK_IMAGE(applet_data->image[x]), temp_pixbuf2);
					if (temp_pixbuf && temp_pixbuf2){
						printf ("Borrando pixbufs\n");
						g_object_unref (temp_pixbuf);
						g_object_unref (temp_pixbuf2);
						temp_pixbuf = 0;
						temp_pixbuf2 = 0;
					}

				}
			}	
	
			temp = strtok (NULL, delimiters);
			temp = strtok (NULL, delimiters);
			temp = strtok (NULL, delimiters);
			temp = strtok (NULL, delimiters); /* Comentario */
			//printf ("%d: %s\n", x, temp);
			if (temp){
				set_tooltip (applet_data, x, temp);
				strncpy (applet_data->day_info[x].state, temp, 36);
			}
			temp = strtok (NULL, delimiters);
			temp = strtok (NULL, delimiters);
			
		}
		if (temp_buf)
			free (temp_buf);

		temp=0;
		temp_buf=0;
		if (theme)
			g_free (theme);

		theme = 0;
	}
	else{
		printf ("parse_sky_data() no applet\n");
	}
	if (t)
		g_free (t);
}
	
void parse_dates_data		( AppletData *applet_data, char *buf, int type )
{
	char *temp_buf=0, *temp_buf2=0;
	int x=0;
	char **tokens=0;
	char *day_temp=0;

	if (buf && applet_data){
		temp_buf = strdup (buf);
		if (type == 0){
			tokens = g_strsplit_set (temp_buf, "<>&;", 20); /* 0 - 16 */
			if (tokens[0])
				strncpy (applet_data->last_update, tokens[0], 64);
			if (tokens[16]){
				temp_buf2 = convert_str_to_utf8 (tokens[16]);
				if (temp_buf2){
					strncpy (applet_data->city_long_desc, temp_buf2, 64);
					g_free (temp_buf2);
					temp_buf2 = 0;
				}
				else
					strcpy (applet_data->city_long_desc, "");
			}

			g_strfreev (tokens);
		}
		else if (type == 2){
			tokens = g_strsplit_set (temp_buf, ":-", 5);
			//printf ("Localidad: %s\n", tokens[1]);
			
			if (tokens[1]){
				temp_buf2 = convert_str_to_utf8 (tokens[1]);
				if (temp_buf2){
					strncpy (applet_data->city_name, temp_buf2, 64);
					g_free (temp_buf2);
					temp_buf2 = 0;
				}
				else
					strcpy (applet_data->city_name, "");
			}
			if (tokens[2]){
				temp_buf2 = convert_str_to_utf8 (tokens[2]);
				if (temp_buf2){
					strncpy (applet_data->provincia, temp_buf2, 64);
					g_free (temp_buf2);
					temp_buf2 = 0;
				}
				else
					strcpy (applet_data->provincia, "");
			}

			g_strfreev (tokens);
		}
		else if (type == 1){
			tokens = g_strsplit_set (temp_buf, "<>", -1);
			x = 4;
			printf ("f1 [%d]: %s\n", x, tokens[x]);
			if (tokens[x]){
				day_temp = parse_week_day_name(tokens[x]);
				strncpy (applet_data->day_info[0].day, day_temp, 32);
				strncpy (applet_data->day_info[1].day, day_temp, 32);
				g_free (day_temp);
			}
			x += 12;
			printf ("f2 [%d]: %s\n", x, tokens[x]);
			if (tokens[x]){
				day_temp = parse_week_day_name(tokens[x]);
				strncpy (applet_data->day_info[2].day, day_temp, 32);
				strncpy (applet_data->day_info[3].day, day_temp, 32);
				g_free (day_temp);
			}
			x += 12;
			printf ("f3 [%d]: %s\n", x, tokens[x]);
			if (tokens[x]){
				day_temp = parse_week_day_name(tokens[x]);
				strncpy (applet_data->day_info[4].day, day_temp, 32);
				strncpy (applet_data->day_info[5].day, day_temp, 32);
				g_free (day_temp);
			}
			printf ("f4 [%d]: %s\n", x, tokens[40]);
			if (tokens){
				day_temp = parse_week_day_name(tokens[40]);
				strncpy (applet_data->day_info[6].day, day_temp, 32);
				g_free (day_temp);
			}
			printf ("f5 [%d]: %s\n", x, tokens[44]);
			if (tokens[44]){
				day_temp = parse_week_day_name(tokens[44]);
				strncpy (applet_data->day_info[7].day, day_temp, 32);
				g_free (day_temp);
			}
			printf ("f6 [%d]: %s\n", x, tokens[48]);
			if (tokens[48]){
				day_temp = parse_week_day_name(tokens[48]);
				strncpy (applet_data->day_info[8].day, day_temp, 32);
				g_free (day_temp);
			}
			
			printf ("f7 [%d]: %s\n", x, tokens[52]);
			if (tokens[52]){
				day_temp = parse_week_day_name(tokens[52]);
				strncpy (applet_data->day_info[9].day, day_temp, 32);
				g_free (day_temp);
			}
			
			g_strfreev (tokens);
		}
		free (temp_buf);
	}
	temp_buf = 0;
	temp_buf2 = 0;
	tokens=0;
	day_temp = 0;
}

void print_element_names 		( xmlNode *root_element )
{
	xmlNode *cur_node=NULL, *a_node=NULL;
	a_node = root_element;
	for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
		if (cur_node->type == XML_ELEMENT_NODE) {
			printf("TEMP node type: Element, name: %s\n", cur_node->name);
		}
		print_element_names(cur_node->children);
	}
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
		tokens = g_strsplit_set (temp_buf, "<>", -1);
		/* FIXME: Totally ugly code. The horror of the horrors... any suggestion? */
			if (type == WIND){
				for (yy=9;yy < 64;yy+=6){
					if (tokens[yy] && strstr(tokens[yy], "TITLE") && strstr(tokens[yy], "Sur"))
						strncpy (applet_data->day_info[idx].wind, _("Sur"), 4);
					else if (tokens[yy] && strstr(tokens[yy], "TITLE") && strstr(tokens[yy], "Sureste"))
						strncpy (applet_data->day_info[idx].wind, _("Sureste"), 12);
					else if (tokens[yy] && strstr(tokens[yy], "TITLE") && strstr(tokens[yy], "Suroeste"))
						strncpy (applet_data->day_info[idx].wind, _("Suroeste"), 12);
					else if (tokens[yy] && strstr(tokens[yy], "TITLE") && strstr(tokens[yy], "Norte"))
						strncpy (applet_data->day_info[idx].wind, _("Norte"), 6);
					else if (tokens[yy] && strstr(tokens[yy], "TITLE") && strstr(tokens[yy], "Noreste"))
						strncpy (applet_data->day_info[idx].wind, _("Noreste"), 12);
					else if (tokens[yy] && strstr(tokens[yy], "TITLE") && strstr(tokens[yy], "Noroeste"))
						strncpy (applet_data->day_info[idx].wind, _("Noroeste"), 12);
					else if (tokens[yy] && strstr(tokens[yy], "TITLE") && strstr(tokens[yy], "Este"))
						strncpy (applet_data->day_info[idx].wind, _("Este"), 6);
					else if (tokens[yy] && strstr(tokens[yy], "TITLE") && strstr(tokens[yy], "Oeste"))
						strncpy (applet_data->day_info[idx].wind, _("Oeste"), 6);
					else
						strcpy (applet_data->day_info[idx].wind, "");

				idx++;
				}
			}
			else
			if (tokens[4]){
				if (type == MAX){ /* temperatura */
					strncpy (applet_data->day_info[0].t_max, tokens[4], 4);
					strncpy (applet_data->day_info[1].t_max, tokens[4], 4);
				}
				else if (type == MIN){
					strncpy (applet_data->day_info[0].t_min, tokens[4], 4);
					strncpy (applet_data->day_info[1].t_min, tokens[4], 4);
				}
				else if (type == PRECIP){
					strncpy (applet_data->day_info[0].precip, tokens[4], 4);
					strncpy (applet_data->day_info[1].precip, tokens[4], 4);
				}
				else if (type == SNOW){
					tk_snow = g_strsplit_set (tokens[4], ";", 2);
					if (tk_snow[1]){
						strncpy (applet_data->day_info[0].cota_nieve, tk_snow[1], 10);
						strncpy (applet_data->day_info[1].cota_nieve, tk_snow[1], 10);
						g_strfreev (tk_snow);
					}
					tk_snow = 0;
				}
			}
			if (tokens[8]){
				if (type == MAX){
					strncpy (applet_data->day_info[2].t_max, tokens[8], 4);
					strncpy (applet_data->day_info[3].t_max, tokens[8], 4);
				}
				else if (type == MIN){
					strncpy (applet_data->day_info[2].t_min, tokens[8], 4);
					strncpy (applet_data->day_info[3].t_min, tokens[8], 4);
				}
				else if (type == PRECIP){
					strncpy (applet_data->day_info[2].precip, tokens[8], 4);
					strncpy (applet_data->day_info[3].precip, tokens[8], 4);
				}
				else if (type == SNOW){
					tk_snow = g_strsplit_set (tokens[8], ";", 2);
					if (tk_snow[1]){
						strncpy (applet_data->day_info[2].cota_nieve, tk_snow[1], 10);
						strncpy (applet_data->day_info[3].cota_nieve, tk_snow[1], 10);
						g_strfreev (tk_snow);
					}
					tk_snow = 0;
				}
			}
			if (tokens[12]){
				if (type == MAX){
					strncpy (applet_data->day_info[4].t_max, tokens[12], 4);
					strncpy (applet_data->day_info[5].t_max, tokens[12], 4);
				}
				else if (type == MIN){
					strncpy (applet_data->day_info[4].t_min, tokens[12], 4);
					strncpy (applet_data->day_info[5].t_min, tokens[12], 4);
				}
				else if (type == PRECIP){
					strncpy (applet_data->day_info[4].precip, tokens[12], 4);
					strncpy (applet_data->day_info[5].precip, tokens[12], 4);
				}
				else if (type == SNOW){
					tk_snow = g_strsplit_set (tokens[12], ";", 2);
					if (tk_snow[1]){
						strncpy (applet_data->day_info[4].cota_nieve, tk_snow[1], 10);
						strncpy (applet_data->day_info[5].cota_nieve, tk_snow[1], 10);
						g_strfreev (tk_snow);
					}
					tk_snow = 0;
				}
			}
			if (tokens[16]){
				if (type == MAX)
					strncpy (applet_data->day_info[6].t_max, tokens[16], 4);
				else if (type == MIN)
					strncpy (applet_data->day_info[6].t_min, tokens[16], 4);
				else if (type == PRECIP)
					strncpy (applet_data->day_info[6].precip, tokens[16], 4);
				else if (type == SNOW){
					tk_snow = g_strsplit_set (tokens[16], ";", 2);
					if (tk_snow[1]){
						strncpy (applet_data->day_info[6].cota_nieve, tk_snow[1], 10);
						g_strfreev (tk_snow);
					}
					tk_snow = 0;
				}
			}
			if (tokens[20]){
				if (type == MAX)
					strncpy (applet_data->day_info[7].t_max, tokens[20], 4);
				else if (type == MIN)
					strncpy (applet_data->day_info[7].t_min, tokens[20], 4);
				else if (type == PRECIP)
					strncpy (applet_data->day_info[7].precip, tokens[20], 4);
				else if (type == SNOW){
					tk_snow = g_strsplit_set (tokens[20], ";", 2);
					if (tk_snow[1]){
						strncpy (applet_data->day_info[7].cota_nieve, tk_snow[1], 10);
						g_strfreev (tk_snow);
					}
					tk_snow = 0;
				}
			}
			if (tokens[24]){
				if (type == MAX)
					strncpy (applet_data->day_info[8].t_max, tokens[24], 4);
				else if (type == MIN)
					strncpy (applet_data->day_info[8].t_min, tokens[24], 4);
				else if (type == PRECIP)
					strncpy (applet_data->day_info[8].precip, tokens[24], 4);
				else if (type == SNOW){
					tk_snow = g_strsplit_set (tokens[24], ";", 2);
					if (tk_snow[1]){
						strncpy (applet_data->day_info[8].cota_nieve, tk_snow[1], 10);
						g_strfreev (tk_snow);
					}
					tk_snow = 0;
				}
			}
			if (tokens[28]){
				if (type == MAX)
					strncpy (applet_data->day_info[9].t_max, tokens[28], 4);
				else if (type == MIN)
					strncpy (applet_data->day_info[9].t_min, tokens[28], 4);
				else if (type == PRECIP)
					strncpy (applet_data->day_info[9].precip, tokens[28], 4);
				else if (type == SNOW){
					tk_snow = g_strsplit_set (tokens[28], ";", 2);
					if (tk_snow[1]){
						strncpy (applet_data->day_info[9].cota_nieve, tk_snow[1], 10);
						g_strfreev (tk_snow);
					}
					tk_snow = 0;
				}
			}
			//printf ("Max: %s\n", tokens[idx]);
		g_strfreev (tokens);
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
		
	if (temp_buf)
		free (temp_buf);
	temp_buf = 0;
	tokens=0;
	tk_snow=0;
}

gboolean check_latest_data	( AppletData *applet_data )
{
	GnomeVFSResult    result;
	int		size=0;
	guchar		**buf;
	char		*temp, *tp;
	char **tokens=0;
	int i=0;
	const char	*suffix[10] = {
		"HO", "HH", "TT", "TR", "PP", 
		"VV", "DV", "RM", "P0", "TP"
	};

	temp = (char *)malloc (32);
	tp = g_new (char, 512);
	sprintf (tp, "Station: %s\n\n", (char *)panel_applet_gconf_get_string (PANEL_APPLET(applet_data->applet), "station_name", NULL));

	result = gnome_vfs_read_entire_file (INM_LATEST_DATA, &size, &buf);

	if (result == GNOME_VFS_OK){
		for (i=0;i < 10;i++){
			snprintf (temp, 24, "%s%s", applet_data->prefs->station_code, suffix[i]);
			if (strstr(buf, temp)){
				tokens = g_strsplit_set (strstr(buf, temp), "]=\"", 5);
				if (tokens[3]){
					printf ("%s: %s\n", suffix[i], tokens[3]);

					if (strcmp(suffix[i], "TT") == 0){ /* Temperatura */
						if (strcmp (tokens[3], "") != 0){
							snprintf (temp, 32, _("%s C"), tokens[3]);
						}
						else
							snprintf (temp, 32, "--", tokens[3]);

						printf ("\nTemperature: %s\n\n", tokens[3]);
						gtk_label_set_text (GTK_LABEL(applet_data->temp_lbl), temp);
						snprintf (temp, 24, _("Temperature: %s\n"), tokens[3]);
						strncat (tp, temp, 50);
					}
					if (strcmp(suffix[i], "RM") == 0){ /* Hora oficial */
						snprintf (temp, 64, _("Maximum wind velocity: %s Km/h\n"), tokens[3]);
						strncat (tp, temp, 50);
					}
					if (strcmp(suffix[i], "P0") == 0){ /* Presion atmosferica */
						snprintf (temp, 64, _("Atmosferic presion: %s\n"), tokens[3]);
						strncat (tp, temp, 50);
					}
					if (strcmp(suffix[i], "PP") == 0){ /* Precipitacion acumulada */
						snprintf (temp, 32, _("Rainfall: %s\n"), tokens[3]);
						strncat (tp, temp, 50);
					}
					if (strcmp(suffix[i], "TR") == 0){ /* Humedad relativa */
						snprintf (temp, 24, _("Humidity: %s %\n"), tokens[3]);
						strncat (tp, temp, 50);
					}
					if (strcmp(suffix[i], "VV") == 0){ /* Direccion del viento */
						snprintf (temp, 32, _("Wind velocity: %s Km/h\n"), tokens[3]);
						strncat (tp, temp, 50);
					}
					if (strcmp(suffix[i], "DV") == 0){ /* Direccion del viento */
						snprintf (temp, 24, _("Wind: %s\n"), tokens[3]);
						strncat (tp, temp, 50);
					}
					if (strcmp(suffix[i], "HO") == 0){ /* Hora oficial */
						snprintf (temp, 24, _("Last update: %sh\n"), tokens[3]);
						strncat (tp, temp, 50);
					}
				}

				g_strfreev (tokens);
			}
		}
		g_free (buf);
		if (strcmp (temp, "") == 0){
			strcpy (temp, "--");
			gtk_label_set_text (GTK_LABEL(applet_data->temp_lbl), temp);
			strncat (tp, _("No data for this meteorological station"), 64);
		}
		set_tooltip (applet_data, 10, tp);
	}
	else{
		gtk_label_set_text (GTK_LABEL(applet_data->temp_lbl), "?");
		strncat (tp, _("Error getting latest data from meteorological station"), 64);
		set_tooltip (applet_data, 10, tp);
		printf ("Error getting latest data from meteorological station\n");
	}

	free (temp);
	g_free (tp);

	return TRUE;
}

/*
gboolean check_station_data	( AppletData *applet_data )
{
	void *ctx=0;
	char *buf=0;
	int ret=-2;
	const char delimiters[] = "[]=\"";
	char *temp=0;
	int x=0;

	if (applet_data && applet_data->applet){
		stationCode *st_code = g_new0 (stationCode, 2);
		st_code[0].name = g_new0 (char, 32);
		st_code[0].provincia = g_new0 (char, 32);
		st_code[0].code = g_new0 (char, 6);
		st_code[1].name = g_new0 (char, 32);
		st_code[1].provincia = g_new0 (char, 32);
		st_code[1].code = g_new0 (char, 6);
		buf = (char *)malloc (36000);

		xmlNanoHTTPInit ();
		ctx = (void *)xmlNanoHTTPOpen (INM_NAMES_LATEST_DATA, NULL);
		while (ret != 0){
			ret = xmlNanoHTTPRead (ctx, buf, 36000);
		}

		temp = strtok (buf, delimiters);
		for (x=0; x < 10 ;x++){
			temp = strtok (NULL, delimiters);
			printf ("MIERDA: %s\n", temp);
		}

		while(temp){
			temp = strtok (NULL, delimiters); 
			temp = strtok (NULL, delimiters); 
			printf ("Station: code\t%s - ", temp);
			if (temp)
				strncpy (st_code[0].code, temp, 6);

			temp = strtok (NULL, delimiters); 
			printf ("name\t%s - ", temp);
			if (temp)
				strncpy (st_code[0].name, temp, 6);
	
			temp = strtok (NULL, delimiters);
			if (temp)
				strncpy (st_code[0].name, temp, 6);
			temp = strtok (NULL, delimiters);
			temp = strtok (NULL, delimiters);
			if (temp)
				strncpy (st_code[0].provincia, temp, 6);

			printf ("Provincia\t: %s\n", temp);
			temp = strtok (NULL, delimiters);
			temp = strtok (NULL, delimiters); 
			temp = strtok (NULL, delimiters);
			
			temp = strtok (NULL, delimiters);
			temp = strtok (NULL, delimiters); 
			temp = strtok (NULL, delimiters);
	
		}
		if (ret == -1){
			printf ("Connection error\n");
			//message_box (applet_data, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, _("Connection error"), _("The connection can not be established"));
		}
		//xmlNanoHTTPCleanup ();
		xmlNanoHTTPClose (ctx);
		

		free (buf);
		g_free (st_code->name);
		g_free (st_code->code);
		g_free (st_code);
	}

	return TRUE;
}
*/

static void check_inm_url_close		( GnomeVFSAsyncHandle *handle, GnomeVFSResult result, gpointer callback_data)
{
	int x=0;
	AppletData *applet_data = (AppletData *)callback_data;
		if (applet_data->buffer){
			//printf ("BUFFER: %s\n", applet_data->buffer);
			if (strstr (applet_data->buffer, "Localidad: "))
				parse_dates_data (applet_data, strstr(applet_data->buffer, "Localidad: "), 2);

			if (strstr (applet_data->buffer, "Elaborado:"))
				parse_dates_data (applet_data, strstr(applet_data->buffer, "Elaborado:"), 0);
			
			if (strstr (applet_data->buffer, "Fecha"))
				parse_dates_data (applet_data, strstr(applet_data->buffer, "Fecha"), 1);
			
			if (strstr (applet_data->buffer, "Estado del cielo<"))
				parse_sky_data (PANEL_APPLET(applet_data->applet), applet_data, strstr(applet_data->buffer, "Estado del cielo<"));
			
			if (strstr (applet_data->buffer, "Prob. precipitaci"))
				parse_temperatures_data (applet_data, strstr(applet_data->buffer, "Prob. precipit"), PRECIP);
	
			if (strstr (applet_data->buffer, "T. M&aacute;xima"))
				parse_temperatures_data (applet_data, strstr(applet_data->buffer, "T. M&aacute;xima"), MAX);
	
			if (strstr (applet_data->buffer, "T. M&iacute;nima"))
				parse_temperatures_data (applet_data, strstr(applet_data->buffer, "T. M&iacute;nima"), MIN);
	
			if (strstr (applet_data->buffer, "Cota de nieve"))
				parse_temperatures_data (applet_data, strstr(applet_data->buffer, "Cota de nieve"), SNOW);
			else if (strstr (applet_data->buffer, "nieve"))
				parse_temperatures_data (applet_data, strstr(applet_data->buffer, "nieve"), SNOW);
			else{
				for (x=0;x < 10;x++)
					strcpy (applet_data->day_info[x].cota_nieve, "");
			}
			
			if (strstr (applet_data->buffer, "Viento"))
				parse_temperatures_data (applet_data, strstr(applet_data->buffer, "Viento"), WIND);
		}
	if (applet_data->buffer)
		g_free (applet_data->buffer);
	
	applet_data->buffer = 0;
}

static void check_inm_url_read 		( GnomeVFSAsyncHandle *handle,
						GnomeVFSResult result,
						gpointer buffer,
						GnomeVFSFileSize bytes_requested,
						GnomeVFSFileSize bytes_read,
						gpointer callback_data)
{
	AppletData *applet_data = (AppletData *) callback_data;
	gchar *buf, *temp=0;
	buf = (gchar *)buffer;
	buf[bytes_read] = '\0';

	if (applet_data->buffer == NULL)
		applet_data->buffer = g_strdup(buf);
	else{
		temp = g_strdup (applet_data->buffer);
		g_free (applet_data->buffer);
		applet_data->buffer = g_strdup_printf ("%s%s", temp, buf);
		g_free (temp);
	}

	//printf ("gvfs_async_read(): %s\n", gnome_vfs_result_to_string (result));
	if (result == GNOME_VFS_ERROR_EOF){
		printf ("Fin de lectura gnome_vfs_async_read()\n");
		gnome_vfs_async_close (handle, check_inm_url_close, applet_data);
		if (buf)
			g_free (buf);

		buf = 0;
	}
	else if (result != GNOME_VFS_OK){
		applet_data->gvfs_handle = NULL;
		printf ("Error de lectura gnome_vfs_asyncred()\n");
		//message_box (applet_data, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, _("Error gnome vfs (async read)"), gnome_vfs_result_to_string (result));
		unset_images (applet_data);
		if (buf)
			g_free (buf);

		buf = 0;
	}
	else{	
		gnome_vfs_async_read (handle, buf, 8192, check_inm_url_read, applet_data);
	}
}

static void check_inm_url_status 	( GnomeVFSAsyncHandle *handle,
						GnomeVFSResult result,
						gpointer callback_data)
{
	char *buf=0;
	buf = g_malloc0(8192);
	AppletData *applet_data = (AppletData *) callback_data;
	printf ("gvfs_async_open(): %s\n", gnome_vfs_result_to_string (result));

	if (applet_data->buffer)
		g_free (applet_data->buffer);
	
	applet_data->buffer = NULL;

	if (result != GNOME_VFS_OK){
		printf ("Conexion no abierta\n");
		//message_box (applet_data, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, _("Connection error"), gnome_vfs_result_to_string (result));
		unset_images (applet_data);
		applet_data->gvfs_handle = NULL;
		applet_data->buffer = NULL;
	}
	else{
		gnome_vfs_async_read (handle, buf, 8192, check_inm_url_read, applet_data);
		printf ("gvfs_async_status()\n");
	}
}

gboolean check_inm_url 			( AppletData *applet_data )
{
	char temp[512];
	
	if (applet_data && applet_data->applet){
		snprintf (temp, 50, "%s%s", INM_FORECAST_URL, (char *)panel_applet_gconf_get_string (PANEL_APPLET(applet_data->applet), "code", NULL));

		gnome_vfs_async_open (&applet_data->gvfs_handle, temp, GNOME_VFS_OPEN_READ, 0, check_inm_url_status, applet_data);
		
		/*
		xmlDoc *doc=NULL;
		xmlNode *root_element=NULL;
		xmlNode *cur_node=NULL, *a_node=NULL;
		doc = xmlReadMemory (buf, strlen(buf), NULL, NULL, XML_PARSE_RECOVER);
		root_element = xmlDocGetRootElement (doc);
		a_node = root_element;
		for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
			if (cur_node->type == XML_ELEMENT_NODE) {
				printf("node type: Element, name: %s\n", cur_node->name);
			}
			//print_element_names(cur_node->children);
	        }
		xmlFreeDoc (doc);
		*/
		
	}
	else{
		printf ("check_inm_url() no applet no fun\n");
	}
		
	return TRUE;
}

void update_location 			( AppletData *applet_data )
{
	gboolean show_station;
 	int iDays=0,x=0,max_days=10;
	if (applet_data){
		show_station = panel_applet_gconf_get_bool (PANEL_APPLET(applet_data->applet), "show_station", NULL);
		if (show_station){
			gtk_widget_show (applet_data->event_box[10]);
			check_latest_data (applet_data);
 			applet_data->timer_station = gtk_timeout_add(applet_data->interval * 1000, (GtkFunction)check_latest_data, applet_data );
		}
		else{
 			gtk_timeout_remove (applet_data->timer_station);
			gtk_widget_hide (applet_data->event_box[10]);
		}
 		iDays = atoi (applet_data->show_days);
 		printf ("Days to show: %d\n", iDays);
 		for (x=0;x < iDays;x++)
 			gtk_widget_show (applet_data->event_box[x]);
 		
 		for (x=iDays;x < max_days;x++)
 			gtk_widget_hide (applet_data->event_box[x]);

		gtk_timeout_remove (applet_data->timer);
		check_inm_url (applet_data);
		applet_data->timer = gtk_timeout_add(applet_data->interval * 1000, (GtkFunction)check_inm_url, applet_data );
	}
	else
		printf ("update_location() no *applet object\n");
}

void update_data			 ( BonoboUIComponent *uic, gpointer user_data, const char *name )
{
	AppletData *applet_data = (AppletData *) user_data;
	gboolean show_station;

	if (applet_data){
		show_station = panel_applet_gconf_get_bool (PANEL_APPLET(applet_data->applet), "show_station", NULL);
		if (show_station){
			check_latest_data (applet_data);
 			applet_data->timer_station = gtk_timeout_add(applet_data->interval * 1000, (GtkFunction)check_latest_data, applet_data );
		}
		else
 			gtk_timeout_remove (applet_data->timer_station);
		
		gtk_timeout_remove (applet_data->timer);
		check_inm_url (applet_data);
		applet_data->timer = gtk_timeout_add(applet_data->interval * 1000, (GtkFunction)check_inm_url, applet_data );
	}
	else
		printf ("update_data() no *applet object\n");
}

void display_preferences_dialog		 ( BonoboUIComponent *uic, gpointer user_data, const char *name )
{
//	printf ("display_prefs_dg: %s\n", applet_data->prefs->code);
	AppletData *applet_data = (AppletData *) user_data;
	create_preferences_win (applet_data);
}

void display_inm_website		 ( BonoboUIComponent *uic, gpointer user_data, const char *name )
{
	GnomeVFSResult gvfs_result;
	AppletData *applet_data = (AppletData *) user_data;

	gvfs_result = gnome_vfs_url_show (INM_URL);
	if (gvfs_result != GNOME_VFS_OK)
		message_box (applet_data, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, _("Error loading www.inm.es"), gnome_vfs_result_to_string (gvfs_result));
		
}

void display_wwarnings_t		 ( BonoboUIComponent *uic, gpointer user_data, const char *name )
{
	GnomeVFSResult gvfs_result;
	AppletData *applet_data = (AppletData *) user_data;

	gvfs_result = gnome_vfs_url_show (INM_WWARNINGS_TODAY_URL);
	if (gvfs_result != GNOME_VFS_OK)
		message_box (applet_data, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, _("Error loading http://www.inm.es/web/infmet/avi/pr/conavi_c.php"), gnome_vfs_result_to_string (gvfs_result));
}

void display_wwarnings_tm		 ( BonoboUIComponent *uic, gpointer user_data, const char *name )
{
	GnomeVFSResult gvfs_result;
	AppletData *applet_data = (AppletData *) user_data;

	gvfs_result = gnome_vfs_url_show (INM_WWARNINGS_TOMORROW_URL);
	if (gvfs_result != GNOME_VFS_OK)
		message_box (applet_data, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, _("Error loading http://www.inm.es/web/infmet/avi/pr/conavi_mm.php"), gnome_vfs_result_to_string (gvfs_result));
}

void display_wwarnings_n		 ( BonoboUIComponent *uic, gpointer user_data, const char *name )
{
	GnomeVFSResult gvfs_result;
	AppletData *applet_data = (AppletData *) user_data;

	gvfs_result = gnome_vfs_url_show (INM_WWARNINGS_NEXT_DAYS_URL);
	if (gvfs_result != GNOME_VFS_OK)
		message_box (applet_data, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, _("Error loading http://www.inm.es/web/infmet/avi/pr/conavi_pp.php"), gnome_vfs_result_to_string (gvfs_result));
}

static void gvfs_close_cb		( GnomeVFSAsyncHandle *handle, GnomeVFSResult result, gpointer callback_data)
{
	GladeXML *xml;
	static GtkWidget *textview;
	static GtkWidget *win;
	static GtkTextBuffer *textview_buffer;
	char **tokens=0;
	char *temp=0;
	gchar *buf = (gchar *) callback_data;

	temp = g_strdup (buf);
	if (strstr(temp, "PREDICCION") || strstr(temp, "Informaci&oacute;n nivol&oacute;gica y riesgo de aludes")){
		xml = glade_xml_new (PACKAGE_DIR"/gnome-inm-glade.glade", "win_today_forecast", NULL);
		win = glade_xml_get_widget (xml, "win_today_forecast");
		g_signal_connect (G_OBJECT(win), "destroy", G_CALLBACK(on_window_terminate), NULL);
		
		textview = glade_xml_get_widget (xml, "textview");
		textview_buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW(textview));
		
		if (temp){
			tokens = g_strsplit_set (strstr(temp, "PREDICCION"), "<>", 3);
			if (tokens){
				//printf ("FOREEEEEEEEEEEEE:\n%s\n", tokens[0]);
			
				gtk_text_buffer_set_text (textview_buffer, tokens[0], strlen(tokens[0]));
				gtk_window_set_title (GTK_WINDOW(win), _("Next days forecast"));
				gtk_widget_show (win);
				g_strfreev (tokens);
				tokens = 0;
			}
		}
	}
	if (temp){
		g_free (temp);
		temp = 0;
	}
}

static void gvfs_read_cb 		( GnomeVFSAsyncHandle *handle,
						GnomeVFSResult result,
						gpointer buffer,
						GnomeVFSFileSize bytes_requested,
						GnomeVFSFileSize bytes_read,
						gpointer callback_data)
{
	AppletData *applet_data = (AppletData *) callback_data;
	gchar *buf, *temp;
	buf = (gchar *)buffer;
	buf[bytes_read] = '\0';
	//printf ("Buf: %s\n", buf);
	
	if (applet_data->buffer == NULL)
		applet_data->buffer = g_strdup(buf);
	else{
		temp = g_strdup (applet_data->buffer);
		g_free (applet_data->buffer);
		applet_data->buffer = g_strdup_printf ("%s%s", temp, buf);
		g_free (temp);
	}


	//printf ("gvfs_async_read(): %s\n", gnome_vfs_result_to_string (result));
	if (result == GNOME_VFS_ERROR_EOF){
		printf ("Fin de lectura gnome_vfs_async_read()\n");
		if (strstr(applet_data->buffer, "PREDICCION")){
			gnome_vfs_async_close (handle, gvfs_close_cb, applet_data->buffer);
		}
		else{
			printf ("No forecast data\n");
		}
		if (applet_data->buffer){
			g_free (applet_data->buffer);
			applet_data->buffer = NULL;
		}
		if (buf){
			g_free (buf);
			buf = 0;
		}
	}
	else if (result != GNOME_VFS_OK){
		applet_data->gvfs_handle = NULL;
		printf ("Error de lectura gnome_vfs_async_read(): %s\n", gnome_vfs_result_to_string (result));
		if (applet_data->buffer){
			g_free (applet_data->buffer);
			applet_data->buffer = NULL;
		}
		if (buf){
			g_free (buf);
			buf = 0;
		}
	}
	else{	
		//printf ("\n\n\n======================\n%s\n==================\n\n\n", (char *)buf);
			
		gnome_vfs_async_read (handle, buf, 16384, gvfs_read_cb, applet_data);
	}
}

static void gvfs_status_cb			( GnomeVFSAsyncHandle *handle,
						GnomeVFSResult result,
						gpointer callback_data)
{
	AppletData *applet_data = (AppletData *) callback_data;
	char *buf=0;
	buf = (char *)malloc(8192);

	printf ("gvfs_async_open(): %s\n", gnome_vfs_result_to_string (result));
	if (result != GNOME_VFS_OK){
		printf (_("Connection error\n"));
		//message_box (applet_data, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, _("Connection error"), gnome_vfs_result_to_string (result));
		unset_images (applet_data);
		applet_data->gvfs_handle = NULL;
		g_free (buf);
	}
	else{
		gnome_vfs_async_read (handle, buf, 8192, gvfs_read_cb, applet_data);
		printf ("gvfs_async_status()\n");
	}

}

GdkPixbuf *load_image				( const char *url )
{
	GnomeVFSHandle   *handle;
	GnomeVFSResult    result;
	GdkPixbufLoader  *loader;
	GdkPixbuf        *pixbuf;
	guchar             buffer[8192];
	GnomeVFSFileSize  bytes_read;

	result = gnome_vfs_open (&handle, url, GNOME_VFS_OPEN_READ);
	loader = gdk_pixbuf_loader_new ();

	while (result == GNOME_VFS_OK) {
		result = gnome_vfs_read (handle, buffer,
                                        8192, &bytes_read);
	gdk_pixbuf_loader_write (loader, buffer, bytes_read, NULL);
	}

	gdk_pixbuf_loader_close (loader, NULL);
	pixbuf = gdk_pixbuf_loader_get_pixbuf (loader);
	gnome_vfs_close (handle);

	if (!pixbuf) {
		return NULL;
	}
	else
	       return pixbuf;
}


void display_satellite_radar 		( BonoboUIComponent *uic, gpointer user_data, const char *name )
{
	GladeXML *xml;
	static GtkWidget *img;
	static GtkWidget *win;
	GtkWidget *rb1, *rb2, *rb3, *rb4;
	GdkPixbuf *pixbuf;

	xml = glade_xml_new (PACKAGE_DIR"/gnome-inm-glade.glade", "win_radar", NULL);
	win = glade_xml_get_widget (xml, "win_radar");
	img = glade_xml_get_widget (xml, "radar_img");
	rb1 = glade_xml_get_widget (xml, "rb_00");
	rb2 = glade_xml_get_widget (xml, "rb_06");
	rb3 = glade_xml_get_widget (xml, "rb_12");
	rb4 = glade_xml_get_widget (xml, "rb_18");
	g_signal_connect (G_OBJECT(win), "destroy", G_CALLBACK(on_window_terminate), img);
	g_signal_connect (G_OBJECT(rb1), "toggled", G_CALLBACK(on_rb_00_toggled), img);
	g_signal_connect (G_OBJECT(rb2), "toggled", G_CALLBACK(on_rb_06_toggled), img);
	g_signal_connect (G_OBJECT(rb3), "toggled", G_CALLBACK(on_rb_12_toggled), img);
	g_signal_connect (G_OBJECT(rb4), "toggled", G_CALLBACK(on_rb_18_toggled), img);

	pixbuf = load_image (INM_RADAR0000_IMG);
	if (pixbuf){
		gtk_image_set_from_pixbuf (GTK_IMAGE(img), pixbuf);
		gtk_window_set_title (GTK_WINDOW(win), _("Satellite image"));
		gtk_widget_show (win);
		g_object_unref (G_OBJECT (pixbuf));
		pixbuf = 0;
	}
}

void display_spanish_forecast_img 	( BonoboUIComponent *uic, gpointer user_data, const char *name )
{
	GdkPixbuf *pixbuf;
	GladeXML *xml;
	static GtkWidget *img;
	static GtkWidget *win;
	GtkWidget *rb1, *rb2, *rb3, *rb4;

	xml = glade_xml_new (PACKAGE_DIR"/gnome-inm-glade.glade", "win_radar", NULL);
	win = glade_xml_get_widget (xml, "win_radar");
	img = glade_xml_get_widget (xml, "radar_img");
	rb1 = glade_xml_get_widget (xml, "rb_00");
	rb2 = glade_xml_get_widget (xml, "rb_06");
	rb3 = glade_xml_get_widget (xml, "rb_12");
	rb4 = glade_xml_get_widget (xml, "rb_18");
	gtk_widget_hide (rb1);
	gtk_widget_hide (rb2);
	gtk_widget_hide (rb3);
	gtk_widget_hide (rb4);
	
	g_signal_connect (G_OBJECT(win), "destroy", G_CALLBACK(on_window_terminate), img);
	
	pixbuf = load_image (INM_NATIONAL_FORECAST_IMG);
	if (pixbuf){
		gtk_image_set_from_pixbuf (GTK_IMAGE(img), pixbuf);
		gtk_window_set_title (GTK_WINDOW(win), _("Spanish weather forecast image"));
		gtk_widget_show (win);
		g_object_unref (G_OBJECT (pixbuf));
		pixbuf = 0;
	}
}

void display_daily_temperatures 	( BonoboUIComponent *uic, gpointer user_data, const char *name )
{
	GdkPixbuf *pixbuf;
	GladeXML *xml;
	static GtkWidget *img, *img2;
	static GtkWidget *win;
	GtkWidget *rb1, *rb2;

	xml = glade_xml_new (PACKAGE_DIR"/gnome-inm-glade.glade", "win_day_temps", NULL);
	win = glade_xml_get_widget (xml, "win_day_temps");
	img = glade_xml_get_widget (xml, "temp_img");
	img2 = glade_xml_get_widget (xml, "temp_img1");
	rb1 = glade_xml_get_widget (xml, "rb_1");
	rb2 = glade_xml_get_widget (xml, "rb_2");
	//gtk_widget_hide (rb1);
	//gtk_widget_hide (rb2);
	
	g_signal_connect (G_OBJECT(win), "destroy", G_CALLBACK(on_window_terminate), img);
	g_signal_connect (G_OBJECT(rb1), "toggled", G_CALLBACK(on_daily_temp_rb1_toggled), img);
	g_signal_connect (G_OBJECT(rb2), "toggled", G_CALLBACK(on_daily_temp_rb2_toggled), img);
	
	pixbuf = load_image (INM_SPAIN_IMG);
	if (pixbuf){
		gtk_image_set_from_pixbuf (GTK_IMAGE(img2), pixbuf);
		gtk_window_set_title (GTK_WINDOW(win), _("Temperatures for the next days"));
		gtk_widget_show (win);
		g_object_unref (G_OBJECT (pixbuf));
		pixbuf = 0;
	}
	pixbuf = load_image (INM_DAILY_TEMPERATURE_MIN);
	if (pixbuf){
		gtk_image_set_from_pixbuf (GTK_IMAGE(img), pixbuf);
		gtk_window_set_title (GTK_WINDOW(win), _("Temperatures for the next days"));
		gtk_widget_show (win);
		g_object_unref (G_OBJECT (pixbuf));
		pixbuf = 0;
	}
}

void display_nextdays_forecast	 	( BonoboUIComponent *uic, gpointer user_data, const char *name )
{
	AppletData *applet_data = (AppletData *) user_data;
	applet_data->gvfs_cnx_type = 3;
	gnome_vfs_async_open (&applet_data->gvfs_handle, INM_FORECAST_NEXTDAYS_URL, GNOME_VFS_OPEN_READ, 0, gvfs_status_cb, applet_data);
}

void display_tomorrow_forecast		 ( BonoboUIComponent *uic, gpointer user_data, const char *name )
{
	AppletData *applet_data = (AppletData *) user_data;
	applet_data->gvfs_cnx_type = 2;
	gnome_vfs_async_open (&applet_data->gvfs_handle, INM_FORECAST_TOMORROW_URL, GNOME_VFS_OPEN_READ, 0, gvfs_status_cb, applet_data);
}

void display_today_forecast		 ( BonoboUIComponent *uic, gpointer user_data, const char *name )
{
	AppletData *applet_data = (AppletData *) user_data;
	applet_data->gvfs_cnx_type = 1;
	gnome_vfs_async_open (&applet_data->gvfs_handle, INM_FORECAST_TODAY_URL, GNOME_VFS_OPEN_READ, 0, gvfs_status_cb, applet_data);
}

void display_snow_warnings		 ( BonoboUIComponent *uic, gpointer user_data, const char *name )
{
	AppletData *applet_data = (AppletData *) user_data;
	applet_data->gvfs_cnx_type = 6;
	gnome_vfs_async_open (&applet_data->gvfs_handle, INM_SNOW_WARNINGS_URL, GNOME_VFS_OPEN_READ, 0, gvfs_status_cb, applet_data);
}

void display_about_dialog 		( BonoboUIComponent *uic, gpointer user_data, const char *name )
{
	static GtkWidget *dialog;
	static GladeXML *xml;
	static gboolean shown=FALSE;
	static GdkPixbuf *pix;
	static GtkWidget *img_logo;

	AppletData *applet_data = (AppletData *) user_data;

	if (applet_data){
		if (shown){
			if (pix)
				g_object_unref (G_OBJECT(pix));
			if (img_logo)
				gtk_widget_destroy (GTK_WIDGET(img_logo));
			
			gtk_widget_destroy (dialog);
			g_object_unref (G_OBJECT (xml));
			dialog = 0;
			img_logo = 0;
			pix = 0;
			xml = 0;
			shown = FALSE;
		}
		xml = glade_xml_new (PACKAGE_DIR"/gnome-inm-glade.glade", "aboutdialog1", NULL);
		dialog = glade_xml_get_widget (xml, "aboutdialog1");
		img_logo = gtk_image_new_from_file ("/usr/share/pixmaps/"PACKAGE".png");
		pix = gtk_image_get_pixbuf (GTK_IMAGE (img_logo));
		gtk_about_dialog_set_name (GTK_ABOUT_DIALOG(dialog), "Gnome INM forecast - ");
		gtk_about_dialog_set_version (GTK_ABOUT_DIALOG(dialog), VERSION);
		gtk_about_dialog_set_logo (GTK_ABOUT_DIALOG(dialog), pix);
		g_signal_connect (G_OBJECT(dialog), "response", G_CALLBACK(on_window_terminate), NULL);
		gtk_widget_show (dialog);
		shown = TRUE;
	}
}

gboolean update_panel 			( AppletData *applet_data )
{
	//printf ("update_panel()\n");
//	check_inm_url (applet_data);
	return FALSE;
}


gboolean start_applet 			( PanelApplet *applet, const gchar *iid, gpointer data )
{
	static const BonoboUIVerb menu_callbacks [] = {
		BONOBO_UI_VERB ("Update", update_data),
		BONOBO_UI_VERB ("TodayForecast", display_today_forecast),
		BONOBO_UI_VERB ("TomorrowForecast", display_tomorrow_forecast),
		BONOBO_UI_VERB ("NextDaysForecast", display_nextdays_forecast),
		BONOBO_UI_VERB ("WeatherWarnings1", display_wwarnings_t),
		BONOBO_UI_VERB ("WeatherWarnings2", display_wwarnings_tm),
		BONOBO_UI_VERB ("WeatherWarnings3", display_wwarnings_n),
		BONOBO_UI_VERB ("SatelliteImages", display_satellite_radar),
		BONOBO_UI_VERB ("SpanishForecastImg", display_spanish_forecast_img),
		BONOBO_UI_VERB ("Temperatures", display_daily_temperatures),
		BONOBO_UI_VERB ("InformeAludes", display_snow_warnings),
		BONOBO_UI_VERB ("Properties", display_preferences_dialog),
		BONOBO_UI_VERB ("VisitINM", display_inm_website),
		BONOBO_UI_VERB ("About", display_about_dialog),
		BONOBO_UI_VERB_END
		};
	char *eventbox_name[11]= {
		"event1", "event2", "event3", 
		"event4", "event5", "event6", 
		"event7", "event8", "event9", 
		"event10", "event11"};
	int x=0;
	
	if (strcmp (iid, "OAFIID:GnomeINM") != 0)
		return FALSE;

	AppletData *applet_data = g_new0(AppletData, 1);
	applet_data->prefs = g_new0 (PrefsWin, 1);
	applet_data->day_info = g_new0 (DayInf, 10);
	applet_data->prefs->code = g_new0 (char, 12);
	applet_data->prefs->station_code = g_new0 (char, 12);
	applet_data->city_name = g_new0 (char, 64);
	applet_data->city_long_desc = g_new0 (char, 64);
	applet_data->provincia = g_new0 (char, 64);
	applet_data->last_update = g_new0 (char, 64);
	applet_data->show_days = g_new0 (char, 12);
	applet_data->st_info = NULL;
	applet_data->st_list = NULL;
	applet_data->buffer = NULL;

	applet_data->client = gconf_client_get_default ();
	
	/* struct initialization */
	applet_data->applet = GTK_WIDGET(applet);
	applet_data->interval = 3600;
	applet_data->show_ma = 0;
	applet_data->timer = 0;
	applet_data->applet_size = panel_applet_get_size (applet);
	applet_data->tips = gtk_tooltips_new();

	/* GConf */
	panel_applet_add_preferences (PANEL_APPLET(applet), "/schemas/apps/gnome-inm/prefs", NULL);
	//panel_applet_gconf_set_string (PANEL_APPLET(applet), "code", applet_data->prefs->code, NULL);
	applet_data->interval = panel_applet_gconf_get_int (PANEL_APPLET(applet), "interval", NULL);
	if (applet_data->interval == 0)
		applet_data->interval = 3600;


	/* Translations initialization */
	setlocale (LC_ALL, "");
	bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
	textdomain (GETTEXT_PACKAGE);

	//printf ("startapplet.Code: %s\n",  (char *)panel_applet_gconf_get_string (PANEL_APPLET(applet), "code", NULL));
	//gconf_client_notify_add (applet_data->gclient, "/apps/gnome-inm/prefs/days", gconf_notify_days, applet_data, NULL, NULL);
	for (x=0;x < 10;x++){
		applet_data->day_info[x].state = g_new0 (char, 36);
		applet_data->day_info[x].precip = g_new0 (char, 12);
		applet_data->day_info[x].cota_nieve = g_new0 (char, 12);
		applet_data->day_info[x].t_max = g_new0 (char, 6);
		applet_data->day_info[x].t_min = g_new0 (char, 6);
		applet_data->day_info[x].day = g_new0 (char, 32);
		applet_data->day_info[x].wind = g_new0 (char, 12);
	}

	LIBXML_TEST_VERSION
	
	applet_data->hbox = gtk_hbox_new (FALSE, 1);
	gtk_container_add (GTK_CONTAINER (applet), applet_data->hbox);
	/* We can not add tooltips directly to an image widget, 
	 * so we add firstly an EventBox widget to the HBox, and then the GtkImage to the EventBox
	 * */
	for (x=0;x < 10;x++){
		applet_data->event_box[x] = gtk_event_box_new ();
		gtk_widget_set_name (applet_data->event_box[x], eventbox_name[x]);
		applet_data->image[x] = gtk_image_new_from_file ("");
		gtk_container_add (GTK_CONTAINER (applet_data->event_box[x]), applet_data->image[x]);
		gtk_container_add (GTK_CONTAINER (applet_data->hbox), applet_data->event_box[x]);
		g_signal_connect (G_OBJECT (applet_data->event_box[x]), "button_press_event", G_CALLBACK (on_image_button_press), applet_data);
	}
	/* Temperature label */
	applet_data->temp_lbl = gtk_label_new ("");
	applet_data->event_box[10] = gtk_event_box_new ();
	gtk_widget_set_name (applet_data->event_box[10], eventbox_name[10]);
	gtk_container_add (GTK_CONTAINER (applet_data->event_box[10]), applet_data->temp_lbl);
	gtk_container_add (GTK_CONTAINER (applet_data->hbox), applet_data->event_box[10]);
	g_signal_connect (G_OBJECT (applet_data->event_box[10]), "button_press_event", G_CALLBACK (on_image_button_press), applet_data);

	g_signal_connect (G_OBJECT (applet), "destroy", G_CALLBACK (on_applet_destroy), applet_data);
	
	panel_applet_setup_menu_from_file (PANEL_APPLET (applet), PACKAGE_DIR, "GNOME_INM_menu.xml", NULL, menu_callbacks, applet_data);

	check_inm_url (applet_data);
	applet_data->timer = gtk_timeout_add(applet_data->interval * 1000, (GtkFunction)check_inm_url, applet_data );
	gtk_widget_show_all (GTK_WIDGET (applet));
	gtk_widget_hide (applet_data->event_box[10]);
	//gtk_widget_hide (applet_data->temp_lbl);

	return TRUE;
}

PANEL_APPLET_BONOBO_FACTORY ("OAFIID:GnomeINM_Factory",
                             PANEL_TYPE_APPLET,
                             "Gnome INM forecast Applet",
                             "0",
                             start_applet,
                             NULL);
