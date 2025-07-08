#
# Google Maps Roadmap
#
# This file provides Google Maps road map tiles for Xastir.
# Uses Google's publicly available tile servers (no API key required).
#
# For better resolution and features, consider using the Google Maps API
# with an API key by setting GOOGLE_API_KEY in the configuration.
#
# Map data Copyright Google
# See https://maps.google.com/ for terms of service
#

# Google Maps tiled interface
GOOGLE_TILED_MAP-roadmap

# Google tile servers (round-robin between mt0-mt3.google.com)
# Uses lyrs=m parameter for roadmap style
URL https://mt0.google.com/vt

# Tile extension
TILE_EXT png

# Zoom levels (same as OSM)
ZOOM_LEVEL_MIN 0
ZOOM_LEVEL_MAX 20

# Tile cache directory
TILE_DIR GoogleTiles

# Optional: Google Maps API key for enhanced features
# Uncomment and set your API key if you have one
#GOOGLE_API_KEY your_api_key_here