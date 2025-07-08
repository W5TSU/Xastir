/*
 *
 * Copyright (C) 2000-2023 The Xastir Group
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * Look at the README for more information on the program.
 *
 */

#ifndef XASTIR_MAP_GOOGLE_H
#define XASTIR_MAP_GOOGLE_H

#include <Xm/XmAll.h>

extern void adj_to_google_level( long *new_scale_x, long *new_scale_y);
extern unsigned int google_zoom_level(long scale_x);

#ifdef HAVE_MAGICK
extern void draw_google_tiles(Widget w,
                              char *filenm,
                              int destination_pixmap,
                              char *server_url,
                              char *tileCacheDir,
                              char *mapName,
                              char *tileExt,
                              char *apiKey);

extern void draw_google_map(Widget w,
                            char *filenm,
                            int destination_pixmap,
                            char *server_url,
                            char *apiKey);
#else
extern void draw_google_map(Widget w,
                            char *filenm,
                            int destination_pixmap,
                            char *server_url,
                            char *apiKey);
#endif // HAVE_MAGICK

#endif // XASTIR_MAP_GOOGLE_H