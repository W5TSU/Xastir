#
# Copyright (C) 2000-2023 The Xastir Group
#

bin_PROGRAMS = xastir xastir_udp_client testdbfawk

SUBDIRS = rtree
DIST_SUBDIRS = rtree

EXTRA_DIST=icon.xbm

XASTIR_SRC = \
    alert.c alert.h \
    awk.c awk.h \
    bulletin_gui.c bulletin_gui.h \
    color.c color.h \
    datum.c datum.h \
    db.c database.h \
    db_gis.c db_gis.h \
    dbfawk.c dbfawk.h \
    dlm.c dlm.h \
    draw_symbols.c draw_symbols.h \
    fcc_data.c fcc_data.h \
    festival.c festival.h \
    fetch_remote.c fetch_remote.h \
    forked_getaddrinfo.c forked_getaddrinfo.h \
    gps.c gps.h \
    hashtable.c hashtable_private.h hashtable.h \
    hashtable_itr.c hashtable_itr.h \
    igate.c igate.h \
    interface.c interface.h \
    interface_gui.c \
    io-common.c io-mmap.c io.h \
    lang.c lang.h \
    leak_detection.h \
    list_gui.c list_gui.h \
    locate_gui.c \
    location.c \
    location_gui.c \
    main.c main.h \
    maps.c maps.h \
    map_cache.c map_cache.h \
    map_dos.c \
    map_geo.c \
    map_gnis.c \
    map_OSM.c map_OSM.h \
    map_google.c map_google.h \
    map_pop.c \
    map_shp.c \
    map_tif.c \
    map_WMS.c \
    messages.c messages.h \
    messages_gui.c \
    objects.h objects.c \
    popup.h \
    popup_gui.c \
    rac_data.c rac_data.h \
    rotated.c rotated.h \
    rpl_malloc.c rpl_malloc.h \
    shp_hash.c shp_hash.h \
    snprintf.c snprintf.h \
    sound.c symbols.h \
    tile_mgmnt.c tile_mgmnt.h \
    track_gui.c track_gui.h \
    util.c util.h \
    view_message_gui.c \
    wx.c wx.h \
    wx_gui.c \
    x_spider.c x_spider.h \
    xa_config.c xa_config.h \
    xastir.h

xastir_SOURCES = $(XASTIR_SRC) compiledate.c

xastir_udp_client_SOURCES = \
    xastir_udp_client.c

testdbfawk_SOURCES = \
    testdbfawk.c \
    awk.c \
    dbfawk.c \
    rpl_malloc.c rpl_malloc.h

.PHONY: compiledate.c

compiledate.c: $(XASTIR_SRC)
	rm -f compiledate.c compiledate.o
	echo 'char gitstring[] = "'"`$(top_srcdir)/scripts/XastirGitStamp.sh $(top_srcdir)`"'";' >> compiledate.c

remove_compiledate:
	rm -f compiledate.c compiledate.o

xastir_LDADD=-Lrtree -lrtree

xastir_LINK=$(CC) $(AM_CFLAGS) $(CFLAGS) $(LDFLAGS) -o $@

xastir_udp_client_LINK=$(CC) $(AM_CFLAGS) $(CFLAGS) $(LDFLAGS) -o $@

testdbfawk_LINK=$(CC) $(AM_CFLAGS) $(CFLAGS) $(LDFLAGS) -o $@
