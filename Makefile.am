
#
# Copyright (C) 2000-2023 The Xastir Group
#

AUTOMAKE_OPTIONS = gnu dist-bzip2
ACLOCAL_AMFLAGS = -I m4

D=      `date +%G%m%d.%H%M%S`

SUBDIRS = src config help scripts symbols callpass

# DISTCLEANFILES =

EXTRA_DIST = xastir.spec.in xastir.spec xastir-min.spec.in xastir-min.spec config.guess config.sub install-sh \
        missing placeholder FAQ LICENSE README README.GIT \
        README.CYGWIN CONTRIBUTING.md \
        README.MAPS README.OSM_maps README.sudo testdbfawk.1 \
        DEBUG_LEVELS bootstrap.sh INSTALL.md UPGRADE callpass.1 xastir.1 xastir_udp_client.1 \
        worldhi.map \
        Davis LaCrosse \
        OSM_template \
        CC_OpenStreetMap_logo.png CC_OpenStreetMap_txt.png \
        COPYING COPYING.LIB.LESSTIF AUTHORS

MAINTAINERCLEANFILES = configure aclocal.m4 Makefile.in Makefile \
        config.guess config.sub install-sh missing mkinstalldirs \
        xastir.spec xastir-min.spec

countydir=${pkgdatadir}/Counties
county_DATA=placeholder

fccdir=${pkgdatadir}/fcc
fcc_DATA=placeholder

gnisdir=${pkgdatadir}/GNIS
gnis_DATA=placeholder

mapdir=${pkgdatadir}/maps
map_DATA=worldhi.map CC_OpenStreetMap_logo.png CC_OpenStreetMap_txt.png

onlinedir=${pkgdatadir}/maps/Online
online_DATA=OSM_tiled_cycle.geo OSM_tiled_mapnik.geo \
        OSM_tiled_fosm.geo \
        Google_roadmap.geo Google_satellite.geo Google_hybrid.geo Google_terrain.geo

nationalmapdir=${pkgdatadir}/maps/Online/nationalmap.gov
nationalmap_DATA=WMS_USGS_Hydrography.geo WMS_USGS_ImageryOnly.geo \
        WMS_USGS_ImageryTopo.geo WMS_USGS_ShadedReliefOnly.geo \
        WMS_USGS_Topo.geo

geogratismapdir=${pkgdatadir}/maps/Online/geogratis.gc.ca
geogratismap_DATA=National.geo Regional.geo Sub_national.geo Sub_regional.geo

gpsdir=${pkgdatadir}/maps/GPS
gps_DATA=placeholder

sounddir=${pkgdatadir}/sounds
sound_DATA=placeholder

docdir=${prefix}/share/doc/xastir
doc_DATA=AUTHORS FAQ ChangeLog LICENSE README README.GIT README.CYGWIN \
        README.MAPS README.sudo COPYING INSTALL.md UPGRADE \
        README.OSM_maps CONTRIBUTING.md \
        COPYING.LIB.LESSTIF

# It'd be nice to remove the directory and contents at
# "${pkgdatadir}/doc" then install a symlink called "doc" there pointing
# to "${prefix}/share/doc/xastir/".  The new location for the
# documentation is FHS compliant.

install-exec-hook:
	-rm -rf $(DESTDIR)${pkgdatadir}/doc

install-data-hook:
	cd $(DESTDIR)$(mapdir) && \
	rm -f CC_OpenStreetMap.png && \
	$(LN_S) CC_OpenStreetMap_logo.png CC_OpenStreetMap.png
 
mandir=${prefix}/share/man
man_MANS=xastir.1 callpass.1 xastir_udp_client.1 testdbfawk.1

# Use tiles for these maps
OSM_tiled_cycle.geo: OSM_template
	sed -e '/THIS IS A TEMPLATE FILE/d' -e's/^#OSM_TILED_MAP/OSM_TILED_MAP/' \
	    -e's!^#URL tileURL!URL http://tile.opencyclemap.org/cycle/!' \
	    -e 's/-STYLE/-cycle/' \
	    < $(top_srcdir)/OSM_template >$@

OSM_tiled_mapnik.geo: OSM_template
	sed -e '/THIS IS A TEMPLATE FILE/d' -e's/^#OSM_TILED_MAP/OSM_TILED_MAP/' \
	    -e's!^#URL tileURL!URL http://tile.openstreetmap.org/!' \
	    -e 's/-STYLE/-mapnik/' \
	    < $(top_srcdir)/OSM_template >$@

OSM_tiled_fosm.geo: OSM_template
	sed -e '/THIS IS A TEMPLATE FILE/d' -e's/^#OSM_TILED_MAP/OSM_TILED_MAP/' \
	    -e's!^#URL tileURL!URL http://map.fosm.org/default/!' \
	    -e 's/-STYLE/-fosm/' \
	    < $(top_srcdir)/OSM_template >$@

ChangeLog::


