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

#ifndef CONNECTION_H
#define CONNECTION_H

#include "main.h"


void check_inm_url_close		( GnomeVFSAsyncHandle *handle, 
						GnomeVFSResult result, 
						gpointer callback_data);
void check_inm_url_read 		( GnomeVFSAsyncHandle *handle,
						GnomeVFSResult result,
						gpointer buffer,
						GnomeVFSFileSize bytes_requested,
						GnomeVFSFileSize bytes_read,
						gpointer callback_data);
void check_inm_url_status 	( GnomeVFSAsyncHandle *handle,
						GnomeVFSResult result,
						gpointer callback_data);

void gvfs_close_cb		( GnomeVFSAsyncHandle *handle, GnomeVFSResult result, gpointer callback_data);
void gvfs_read_cb 		( GnomeVFSAsyncHandle *handle,
						GnomeVFSResult result,
						gpointer buffer,
						GnomeVFSFileSize bytes_requested,
						GnomeVFSFileSize bytes_read,
						gpointer callback_data);
void gvfs_status_cb			( GnomeVFSAsyncHandle *handle,
						GnomeVFSResult result,
						gpointer callback_data);

#endif
