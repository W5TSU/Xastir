# RELEASE NOTES FOR Version - 
------------------------------------------------------------------------
The primary purpose for this release is Google Map intergration.
It does include other bug fixes and updates.

## Google Maps Integration

This fork includes full Google Maps integration for Xastir. Google Maps provides high-quality satellite imagery, road maps, hybrid views, and terrain maps that can be used as background maps for APRS data.

### Features
- **Four map types**: Roadmap, Satellite, Hybrid, and Terrain
- **Tile-based system**: Fast loading with automatic tile caching
- **No API key required**: Uses Google's public tile servers
- **Default map**: Google Maps roadmap is now the default map for new installations
- **Zoom levels**: Supports zoom levels 0-20 for detailed viewing
- **Automatic fallback**: Falls back to cached tiles when offline
- **Tiles are cached** in ~/.xastir/GoogleTiles/ for offline use
- **Weather alert** expiration handling has been successfully fixed
- **Limit tile download failures** when wget is used.

**Google tile download can take a long time based on your zoom level.**

### Requirements
Google Maps requires GraphicsMagick for image processing. When configuring Xastir use:

    ./configure --with-graphicsmagick

Make sure you have GraphicsMagick development packages installed:
- On Ubuntu/Debian: `sudo apt-get install libgraphicsmagick++1-dev libgraphicsmagick1-dev`
- On RHEL/CentOS: `sudo yum install GraphicsMagick-devel GraphicsMagick-c++-devel`

### Technical Details
- Map tiles are downloaded from Google's public tile servers
- Uses Web Mercator projection (EPSG:3857)
- Tile size: 256x256 pixels
- Round-robin server selection for load balancing
- Compatible with existing Xastir map indexing system

### Tile Download Strategy

  On-demand downloading:
  - Only downloads tiles that are currently visible in the map view
  - Calculates which tiles are needed based on the current zoom level and visible area
  - Downloads tiles in a rectangular grid covering the visible map area

  How it determines what to download:

  1. Calculate visible area: Based on current map bounds and zoom level
  2. Determine tile grid: Converts the visible area to tile coordinates (startx, endx, starty, endy)
  3. Check cache: Uses tilesMissing() function to count how many tiles are missing from the local cache
  4. Download only missing tiles: Only downloads tiles that aren't already cached locally

  Caching behavior:

  - Local cache: Tiles are stored in ~/.xastir/GoogleTiles/ directory
  - Persistent cache: Downloaded tiles remain on disk for future use
  - Cache structure: Organized as GoogleTiles/zoom/x/y.png
  - Reuse: If you pan/zoom back to a previously viewed area, cached tiles are used instead of re-downloading

  Performance benefits:

  - Efficient bandwidth usage: Only downloads what's needed
  - Fast redraw: Cached tiles load instantly
  - Offline capability: Can view previously downloaded areas without internet
  - Zoom-specific: Each zoom level has its own tiles, so zooming in/out may require new downloads

  This is much more efficient than downloading all tiles, as Google Maps has millions of tiles across all zoom levels and geographical areas.

### Weather alert expiration handling has been successfully fixed!

  ISSUE:

Weather alerts are cached with a time-stamp in several files. If Xastir is shut down for a period of time and then started, it may bring display weather alerts that should have already expired. We should check the timestamps, load only if they're within a specific window of time, plus actively delete expired alerts from the cache files.

  Summary of improvements implemented:

  Enhanced Expiration Checking

  - At startup: Cleans expired alerts from all log files before loading
  - During operation: Rejects expired alerts when adding new ones
  - Periodic cleanup: Automatically cleans log files every hour
  - Runtime expiration: Continues to expire alerts from memory every 60 seconds

  New Functions Added

  1. is_alert_expired() - Checks if an alert has expired based on timestamp
  2. clean_expired_alerts_from_log() - Removes expired alerts from log files

  Key Improvements

  - Startup cleanup: load_wx_alerts_from_log() now cleans all log files before reading
  - Entry validation: alert_add_entry() now rejects expired alerts
  - Periodic maintenance: alert_expire() cleans log files every hour
  - File safety: Uses temporary files and atomic replacement for safe cleanup

  Files Modified

  - src/alert.h - Added new function declarations
  - src/alert.c - Added expiration functions and enhanced existing ones
  - src/util.c - Added cleanup calls during startup loading

  Benefits

  - Prevents stale alerts: No more displaying expired weather alerts after restart
  - Reduces storage: Automatically removes expired alerts from cache files
  - Improves performance: Smaller log files load faster
  - Better user experience: Only current, relevant alerts are displayed



###Stop wget downloads after repeated failures.

ISSUE:
  This implementation automatically stops tile downloads when API limits are hit, preventing endless retry loops while maintaining the existing interrupt infrastructure.

  Implementation Summary

  1. New Configuration Variable

  - Added max_map_download_failures with default value of 20
  - Configurable via MAX_MAP_DOWNLOAD_FAILURES setting (range: 1-100)
  - Added to src/xastir.h, src/main.c, and src/xa_config.c

  2. Failure Counter System

  - Added map_download_failure_count static variable
  - Three new functions:
    - increment_map_download_failure_count() - increments counter and sets interrupt flag when max reached
    - reset_map_download_failure_count() - resets counter to 0
    - get_map_download_failure_count() - returns current count

  3. Integration Points

  - DLM system (src/dlm.c): wget and libcurl failures now increment counter
  - Legacy system (src/fetch_remote.c): wget and libcurl failures increment counter
  - Map drawing (src/map_OSM.c, src/map_google.c): counter resets at start of new sessions

  4. How It Works

  1. When a new map download session starts, the failure counter resets to 0
  2. Each download failure (wget or libcurl) increments the counter
  3. When the counter reaches max_map_download_failures (default 20), the system:
    - Prints a message: "Maximum map download failures (20) reached. Stopping map downloads."
    - Sets interrupt_drawing_now = 1 to stop further downloads
  4. The existing interrupt system then gracefully stops the download process

  5. Configuration

  Users can adjust the threshold by setting MAX_MAP_DOWNLOAD_FAILURES in their Xastir configuration to any value between 1 and 100.



W5TSU

