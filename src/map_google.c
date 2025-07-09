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

#ifdef HAVE_CONFIG_H
  #include "config.h"
#endif  // HAVE_CONFIG_H

#include "snprintf.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <ctype.h>
#include <sys/types.h>
#include <pwd.h>
#include <errno.h>

#include <dirent.h>
#include <netinet/in.h>
#include <Xm/XmAll.h>

#ifdef HAVE_X11_XPM_H
  #include <X11/xpm.h>
  #ifdef HAVE_LIBXPM // if we have both, prefer the extra library
    #undef HAVE_XM_XPMI_H
  #endif // HAVE_LIBXPM
#endif // HAVE_X11_XPM_H

#ifdef HAVE_XM_XPMI_H
  #include <Xm/XpmI.h>
#endif // HAVE_XM_XPMI_H

#include <X11/Xlib.h>

#include <math.h>

#include "xastir.h"
#include "maps.h"
#include "fetch_remote.h"
#include "util.h"
#include "main.h"
#include "color.h"
#include "xa_config.h"

#include "map_cache.h"

#include "tile_mgmnt.h"
#include "dlm.h"
#include "map_google.h"

#define CHECKMALLOC(m)  if (!m) { fprintf(stderr, "***** Malloc Failed *****\\n"); exit(0); }

#ifdef HAVE_MAGICK
  #if HAVE_SYS_TIME_H
    #include <sys/time.h>
  #endif // HAVE_SYS_TIME_H
  #include <time.h>
  // TVR: "stupid ImageMagick"
  // The problem is that magick/api.h includes Magick's config.h file, and that
  // pulls in all the same autoconf-generated defines that we use.
  // plays those games below, but I don't think in the end that they actually
  // make usable macros with our own data in them.
  // Fortunately, we don't need them, so I'll just undef the ones that are
  // causing problems today.  See main.c for fixes that preserve our values.
  #undef PACKAGE
  #undef VERSION
  /* JMT - stupid ImageMagick */
  #define XASTIR_PACKAGE_BUGREPORT PACKAGE_BUGREPORT
  #undef PACKAGE_BUGREPORT
  #define XASTIR_PACKAGE_NAME PACKAGE_NAME
  #undef PACKAGE_NAME
  #define XASTIR_PACKAGE_STRING PACKAGE_STRING
  #undef PACKAGE_STRING
  #define XASTIR_PACKAGE_TARNAME PACKAGE_TARNAME
  #undef PACKAGE_TARNAME
  #define XASTIR_PACKAGE_VERSION PACKAGE_VERSION
  #undef PACKAGE_VERSION
  #ifdef HAVE_MAGICK
    #ifdef HAVE_MAGICKCORE_MAGICKCORE_H
      #include <MagickCore/MagickCore.h>
    #else
      #ifdef HAVE_MAGICK_API_H
        #include <magick/api.h>
      #endif // HAVE_MAGICK_API_H
    #endif //HAVE_MAGICKCORE_MAGICKCORE_H
  #endif //HAVE_MAGICK
  #undef PACKAGE_BUGREPORT
  #define PACKAGE_BUGREPORT XASTIR_PACKAGE_BUGREPORT
  #undef XASTIR_PACKAGE_BUGREPORT
  #undef PACKAGE_NAME
  #define PACKAGE_NAME XASTIR_PACKAGE_NAME
  #undef XASTIR_PACKAGE_NAME
  #undef PACKAGE_STRING
  #define PACKAGE_STRING XASTIR_PACKAGE_STRING
  #undef XASTIR_PACKAGE_STRING
  #undef PACKAGE_TARNAME
  #define PACKAGE_TARNAME XASTIR_PACKAGE_TARNAME
  #undef XASTIR_PACKAGE_TARNAME
  #undef PACKAGE_VERSION
  #define PACKAGE_VERSION XASTIR_PACKAGE_VERSION
  #undef XASTIR_PACKAGE_VERSION

  // This matte color was chosen empirically to work well with the
  // contours from topOSM.
  #if (QuantumDepth == 8)
    #define MATTE_RED   (0xa7)
    #define MATTE_GREEN (0xa7)
    #define MATTE_BLUE  (0xa7)
    #define MATTE_OPACITY (0x00)
    #define MATTE_COLOR_STRING "xc:#a7a7a7"
  #elif (QuantumDepth == 16)
    #define MATTE_RED   (0xa700)
    #define MATTE_GREEN (0xa700)
    #define MATTE_BLUE  (0xa700)
    #define MATTE_OPACITY (0x0000)
    #define MATTE_COLOR_STRING "xc:#a700a700a700"
  #else
    #error "QuantumDepth != 16 or 8"
  #endif // QuantumDepth

#endif // HAVE_MAGICK

// Must be last include file
#include "leak_detection.h"

#define xastirColorsMatch(p,q) (((p).red == (q).red) && ((p).blue == (q).blue) \
        && ((p).green == (q).green))

// Google Maps zoom levels are from 0 to 20, same as OSM
#define MAX_GOOGLE_ZOOM_LEVEL 20
#define GOOGLE_ZOOM_LEVELS    (MAX_GOOGLE_ZOOM_LEVEL + 1)

// Google Maps scale mapping - same as OSM since they use the same Web Mercator projection
static long google_scale_x(long xastir_scale_x)
{
  long google_level[GOOGLE_ZOOM_LEVELS] = {1, 2, 4, 8, 15, 31, 62, 124, \
                                           247, 494, 989, 1978, 3955, 7910, 15820, 31641,\
                                           63281, 126563, 253125, 506250, 1012500
                                          };
  long google_scale_x = google_level[0];
  int i = 0;

  for (i=1; i <= MAX_GOOGLE_ZOOM_LEVEL; i++)
  {
    if (xastir_scale_x > google_level[i])
    {
      continue;
    }
    else
    {
      if (labs(google_level[i - 1] - xastir_scale_x) < labs(google_level[i] - xastir_scale_x))
      {
        google_scale_x = google_level[i - 1];
      }
      else
      {
        google_scale_x = google_level[i];
      }
      break;
    }
  }
  return google_scale_x;
}

// Adjust scale to Google Maps level
void adj_to_google_level( long *new_scale_x, long *new_scale_y)
{
  *new_scale_x = google_scale_x(*new_scale_x);
  *new_scale_y = *new_scale_x;
}

// Get Google Maps zoom level from scale
unsigned int google_zoom_level(long scale_x)
{
  long google_level[GOOGLE_ZOOM_LEVELS] = {1, 2, 4, 8, 15, 31, 62, 124, \
                                           247, 494, 989, 1978, 3955, 7910, 15820, 31641,\
                                           63281, 126563, 253125, 506250, 1012500
                                          };
  unsigned int i;

  for (i=0; i <= MAX_GOOGLE_ZOOM_LEVEL; i++)
  {
    if (scale_x <= google_level[i])
    {
      return i;
    }
  }
  return MAX_GOOGLE_ZOOM_LEVEL;
}

// Google Maps tile server selection - round-robin between mt0-mt3
static int google_tile_server(unsigned long x, unsigned long y)
{
  return (x + y) % 4;
}

// Build Google Maps tile URL
static void build_google_tile_url(char *url, size_t url_size, 
                                  char *style, unsigned long x, unsigned long y, 
                                  unsigned int zoom, char *api_key)
{
  int server = google_tile_server(x, y);
  
  if (api_key && strlen(api_key) > 0)
  {
    // Use official Google Maps API with key
    xastir_snprintf(url, url_size, 
                    "https://maps.googleapis.com/maps/api/staticmap?center=%lu,%lu&zoom=%u&size=256x256&maptype=%s&key=%s",
                    y, x, zoom, style, api_key);
  }
  else
  {
    // Use public tile servers (no API key required)
    xastir_snprintf(url, url_size,
                    "https://mt%d.google.com/vt/lyrs=%s&x=%lu&y=%lu&z=%u",
                    server, style, x, y, zoom);
  }
}

// Get Google Maps style parameter from map name
static char* get_google_style(char *mapname)
{
  if (strstr(mapname, "roadmap") || strstr(mapname, "Roadmap"))
    return "m";
  else if (strstr(mapname, "satellite") || strstr(mapname, "Satellite"))
    return "s";
  else if (strstr(mapname, "terrain") || strstr(mapname, "Terrain"))
    return "t";
  else if (strstr(mapname, "hybrid") || strstr(mapname, "Hybrid"))
    return "y";
  else
    return "m"; // default to roadmap
}

#ifdef HAVE_MAGICK

// Copy image drawing functions from map_OSM.c
static void draw_image(
  Widget w,
  Image *image,
  ExceptionInfo *except_ptr,
  int offsetx,
  int offsety)
{
  int l;
  XColor my_colors[256];
  PixelPacket *pixel_pack;
  PixelPacket temp_pack;
  IndexPacket *index_pack;
  int image_row, image_col;
  long scr_x, scr_y;

  if (image->storage_class == PseudoClass)
  {
    index_pack = GetIndexes(image);
    if (!index_pack)
    {
      fprintf(stderr,"draw_image: Unable to getIndexes\n");
      return;
    }
    pixel_pack = SetImagePixels(image, 0, 0, image->columns, image->rows);
    if (!pixel_pack)
    {
      fprintf(stderr,"draw_image: Unable to setImagePixels\\n");
      return;
    }
    for (l=0; l<(int)image->colors; l++)
    {
      temp_pack = image->colormap[l];
      if (QuantumDepth == 16)
      {
        my_colors[l].red   = temp_pack.red * raster_map_intensity;
        my_colors[l].green = temp_pack.green * raster_map_intensity;
        my_colors[l].blue  = temp_pack.blue * raster_map_intensity;
      }
      else
      {
        my_colors[l].red   = (temp_pack.red * 256) * raster_map_intensity;
        my_colors[l].green = (temp_pack.green * 256) * raster_map_intensity;
        my_colors[l].blue  = (temp_pack.blue * 256) * raster_map_intensity;
      }
      if (visual_type == NOT_TRUE_NOR_DIRECT)
      {
        XAllocColor(XtDisplay(w), cmap, &my_colors[l]);
      }
      else
      {
        pack_pixel_bits(my_colors[l].red, my_colors[l].green, my_colors[l].blue,
                        &my_colors[l].pixel);
      }
    }
  }
  else
  {
    pixel_pack = GetImagePixels(image, 0, 0, image->columns, image->rows);
    if (!pixel_pack)
    {
      fprintf(stderr,"draw_image: Unable to getImagePixels\\n");
      return;
    }
  }

  for (image_row = 0; image_row < (int)image->rows; image_row++)
  {
    HandlePendingEvents(app_context);
    if (interrupt_drawing_now)
    {
      (void)XCopyArea(XtDisplay(da),
                      pixmap,
                      XtWindow(da),
                      gc,
                      0,
                      0,
                      (unsigned int)screen_width,
                      (unsigned int)screen_height,
                      0,
                      0);
      return;
    }

    scr_y = image_row + offsety;

    for (image_col = 0; image_col < (int)image->columns; image_col++)
    {
      scr_x = image_col + offsetx;
      l = image_col + (image_row * image->columns);
      if (image->storage_class == PseudoClass)
      {
        if (xastirColorsMatch(pixel_pack[l],image->matte_color))
        {
          continue;
        }
        XSetForeground(XtDisplay(w), gc, my_colors[(int)index_pack[l]].pixel);
      }
      else
      {
        if (pixel_pack[l].opacity == TransparentOpacity)
        {
          continue;
        }

        if (QuantumDepth==16)
        {
          my_colors[0].red=pixel_pack[l].red;
          my_colors[0].green=pixel_pack[l].green;
          my_colors[0].blue=pixel_pack[l].blue;
        }
        else
        {
          my_colors[0].red=pixel_pack[l].red*256;
          my_colors[0].green=pixel_pack[l].green*256;
          my_colors[0].blue=pixel_pack[l].blue*256;
        }
        pack_pixel_bits(my_colors[0].red * raster_map_intensity,
                        my_colors[0].green * raster_map_intensity,
                        my_colors[0].blue * raster_map_intensity,
                        &my_colors[0].pixel);
        XSetForeground(XtDisplay(w), gc, my_colors[0].pixel);
      }
      (void)XFillRectangle (XtDisplay (w),pixmap,gc,scr_x,scr_y,1,1);
    }
  }
  return;
}

// Draw Google Maps tiles
void draw_google_tiles(Widget w,
                       char *filenm,
                       int destination_pixmap,
                       char *server_url,
                       char *tileCacheDir,
                       char *mapName,
                       char *tileExt,
                       char *apiKey)
{
  // Reset failure counter at the beginning of new map download session
  reset_map_download_failure_count();
  char serverURL[MAX_FILENAME];
  char tileRootDir[MAX_FILENAME];
  char map_it[MAX_FILENAME];
  char short_filenm[MAX_FILENAME];
  char *style;
  
  // Extract short filename for display
  if (filenm && strlen(filenm) > 0) {
    char *temp_ptr = strrchr(filenm, '/');
    if (temp_ptr) {
      xastir_snprintf(short_filenm, sizeof(short_filenm), "%s", temp_ptr + 1);
    } else {
      xastir_snprintf(short_filenm, sizeof(short_filenm), "%s", filenm);
    }
  } else {
    xastir_snprintf(short_filenm, sizeof(short_filenm), "Unknown");
  }
  int google_zl;
  tileArea_t tiles;
  tiepoint NWcorner;
  tiepoint SEcorner;
  unsigned long tilex, tiley;
  unsigned long tileCnt = 0;
  unsigned long numTiles;
  int interrupted = 0;

  ExceptionInfo exception;
  Image *canvas = NULL;
  Image *tile = NULL;
  ImageInfo *canvas_info = NULL;
  ImageInfo *tile_info = NULL;
  unsigned int row, col;
  unsigned int offset_x, offset_y;
  char tmpString[MaxTextExtent];
  char temp_file_path[MAX_FILENAME];
  char tile_url[MAX_FILENAME];

  // Check whether we're indexing or drawing the map
  if ( (destination_pixmap == INDEX_CHECK_TIMESTAMPS)
       || (destination_pixmap == INDEX_NO_TIMESTAMPS) )
  {
    // We're indexing only.  Save the extents in the index.
    // Force the extents to the edges of the earth for the
    // index file.
    index_update_xastir(filenm, // Filename only
                        64800000l,      // Bottom
                        0l,             // Top
                        0l,             // Left
                        129600000l,     // Right
                        0);             // Default Map Level

    // Update statusline
    xastir_snprintf(map_it,
                    sizeof(map_it),
                    langcode ("BBARSTA039"),  // Indexing %s
                    short_filenm);
    statusline(map_it,0);       // Indexing

    return; // Done indexing this file
  }

  // Initialize the exception structure
  GetExceptionInfo(&exception);

  // Set up tile cache directory
  if (tileCacheDir[0] != '\0')
  {
    if (tileCacheDir[0] == '/')
    {
      xastir_snprintf(tileRootDir, sizeof(tileRootDir),
                      "%s", tileCacheDir);
    }
    else
    {
      xastir_snprintf(tileRootDir, sizeof(tileRootDir),
                      "%s", get_user_base_dir(tileCacheDir, temp_file_path, sizeof(temp_file_path)));
    }
  }
  else
  {
    xastir_snprintf(tileRootDir, sizeof(tileRootDir),
                    "%s", get_user_base_dir("GoogleTiles", temp_file_path, sizeof(temp_file_path)));
  }

  if (mapName[0] != '\0')
  {
    xastir_snprintf(tmpString, sizeof(tmpString), "/%s", mapName);
    strncat(tileRootDir, tmpString, sizeof(tileRootDir) - 1 - strlen(tileRootDir));
  }

  // Get Google Maps style
  style = get_google_style(mapName);

  // Set up server URL - we'll build per-tile URLs later
  if (server_url[0] != '\0')
  {
    xastir_snprintf(serverURL, sizeof(serverURL), "%s", server_url);
  }
  else
  {
    xastir_snprintf(serverURL, sizeof(serverURL), "https://mt0.google.com/vt");
  }

  // Calculate zoom level and tile area
  google_zl = google_zoom_level(scale_x);
  calcTileArea(f_NW_corner_longitude, f_NW_corner_latitude,
               f_SE_corner_longitude, f_SE_corner_latitude,
               google_zl, &tiles);

  xastir_snprintf(map_it, sizeof(map_it), "%s",
                  langcode ("BBARSTA050")); // Downloading tiles...
  statusline(map_it,0);
  XmUpdateDisplay(text);

  // Make sure all the map directories exist
  mkOSMmapDirs(tileRootDir, tiles.startx, tiles.endx, google_zl);

  // Check how many tiles need to be downloaded
  numTiles = tilesMissing(tiles.startx, tiles.endx, tiles.starty,
                          tiles.endy, google_zl, tileRootDir,
                          tileExt[0] != '\0' ? tileExt : "png");

  // Download the tiles
  tileCnt = 1;
  for (tilex = tiles.startx; tilex <= tiles.endx; tilex++)
  {
    for (tiley = tiles.starty; tiley <= tiles.endy; tiley++)
    {
      if ((numTiles > 0) & (tileCnt <= numTiles))
      {
        xastir_snprintf(map_it, sizeof(map_it), langcode("BBARSTA051"),
                        tileCnt, numTiles);
        statusline(map_it,0);
        XmUpdateDisplay(text);
      }

      // Build the tile URL
      build_google_tile_url(tile_url, sizeof(tile_url), style, tilex, tiley,
                            google_zl, apiKey);

      // Queue the tile for download using custom URL
      DLM_queue_tile(tile_url, tilex, tiley,
                     google_zl, tileRootDir, tileExt[0] != '\0' ? tileExt : "png");
      tileCnt++;
    }
  }

  // Process downloads
  DLM_do_transfers();
  if (interrupt_drawing_now)
  {
    interrupted = 1;
  }

  if (interrupted != 1)
  {
    // Calculate tie points
    NWcorner.img_x = 0;
    NWcorner.img_y = 0;
    NWcorner.x_long = tiles.startx * 256;
    NWcorner.y_lat = tiles.starty * 256;

    SEcorner.img_x = (256 * ((tiles.endx + 1) - tiles.startx)) - 1;
    SEcorner.img_y = (256 * ((tiles.endy + 1) - tiles.starty)) - 1;
    SEcorner.x_long = (tiles.endx + 1) * 256;
    SEcorner.y_lat = (tiles.endy + 1) * 256;

    // Create canvas for tile composition
    canvas_info=CloneImageInfo((ImageInfo *)NULL);
    xastir_snprintf(tmpString, sizeof(tmpString), "%dx%d",
                    SEcorner.img_x + 1, SEcorner.img_y + 1);
    CloneString(&canvas_info->size, tmpString);
    canvas_info->units = PixelsPerInchResolution;
    CloneString((char **)&canvas_info->filename, MATTE_COLOR_STRING);
    canvas = ReadImage(canvas_info, &exception);
    if (exception.severity != UndefinedException)
    {
      CatchException(&exception);
    }

    if (canvas == NULL)
    {
      fprintf(stderr, "draw_google_tiles: Failed to create canvas\\n");
      if (canvas_info)
      {
        DestroyImageInfo(canvas_info);
      }
      return;
    }

    // Composite tiles onto canvas
    tile_info = CloneImageInfo((ImageInfo *)NULL);
    tile_info->units = PixelsPerInchResolution;

    row = 0;
    for (tiley = tiles.starty; tiley <= tiles.endy; tiley++)
    {
      col = 0;
      for (tilex = tiles.startx; tilex <= tiles.endx; tilex++)
      {
        xastir_snprintf(tmpString, sizeof(tmpString), "%s/%d/%lu/%lu.%s",
                        tileRootDir, google_zl, tilex, tiley,
                        tileExt[0] != '\0' ? tileExt : "png");

        if (filethere(tmpString))
        {
          CloneString((char **)&tile_info->filename, tmpString);
          tile = ReadImage(tile_info, &exception);
          if (exception.severity != UndefinedException)
          {
            CatchException(&exception);
          }

          if (tile)
          {
            offset_x = col * 256;
            offset_y = row * 256;
            CompositeImage(canvas, OverCompositeOp, tile, offset_x, offset_y);
            DestroyImage(tile);
          }
        }
        col++;
      }
      row++;
    }

    // Draw the composed image (use same function as simple draw)
    draw_image(w, canvas, &exception, 0, 0);

    // Clean up
    if (canvas)
    {
      DestroyImage(canvas);
    }
    if (canvas_info)
    {
      DestroyImageInfo(canvas_info);
    }
    if (tile_info)
    {
      DestroyImageInfo(tile_info);
    }
  }

  DestroyExceptionInfo(&exception);
}

#endif // HAVE_MAGICK

#ifdef HAVE_MAGICK
// Stub for static maps (future enhancement)
void draw_google_map(Widget w,
                     char *filenm,
                     int destination_pixmap,
                     char *server_url,
                     char *apiKey)
{
  // This function would implement Google Static Maps API support
  // For now, redirect to tile-based implementation
  char empty_string[1] = "";
  draw_google_tiles(w, filenm, destination_pixmap, server_url,
                    empty_string, empty_string, empty_string, apiKey);
}
#else
// Stub when no ImageMagick support
void draw_google_map(Widget w,
                     char *filenm,
                     int destination_pixmap,
                     char *server_url,
                     char *apiKey)
{
  fprintf(stderr, "Google Maps support requires ImageMagick/GraphicsMagick\\n");
}
#endif // HAVE_MAGICK