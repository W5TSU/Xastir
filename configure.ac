# Process this file with autoconf to produce a configure script.
#
#
# Copyright (C) 2000-2023 The Xastir Group


#########################################################################
# SET VERSION HERE!
#
# Note that the most standard OSS method of doing numbering is:
#       Major.Minor.PatchLevel
#
# If we are only doing patches, change the PATCHLEVEL number.
# If we add new features, change the MINOR number.
# If we feel we've added enough new features or rewritten enough
#   code to warrant it, change the MAJOR number.
#
# Also:  We adopted the convention of using odd numbers for development
#   or development releases, even numbers for STABLE releases.
#
#
# AC_INIT():
# It should look something like this:  ([xastir], [2.0.8], [xastir@xastir.org])
# The revision number must contain at least one '.' and two digits.
AC_INIT([xastir], [2.3.1], [xastir@xastir.org])
#########################################################################


AC_PREREQ(2.60)

AC_CONFIG_SRCDIR([src/xastir.h])
#AM_CONFIG_HEADER(config.h)
AC_CONFIG_HEADERS(config.h)
AM_INIT_AUTOMAKE([subdir-objects])
AM_SILENT_RULES(yes)

echo ""
echo "Configuring AC_PACKAGE_NAME AC_PACKAGE_VERSION"
echo ""

# Take out the dots in order to create the TOCALL
AC_DEFINE_UNQUOTED(XASTIR_TOCALL, "`echo AC_PACKAGE_VERSION | sed -e 's/^/APX/' -e 's/\.//g'`", [Defines the version tocall])

# Check for host type
AC_CANONICAL_HOST

# Define _GNU_SOURCE if appropriate
# doesn't work with older (heh) autoconfs
#  This macro is now obsolete as of autoconf 2.60 and should no longer
#  be used: AC_GNU_SOURCE
# autoconf 2.60 was released in 2006, so it should be safe to assume all
# users have at least this version by now.
AC_USE_SYSTEM_EXTENSIONS(_GNU_SOURCE)

# Checks for programs.
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_AWK
AC_PROG_RANLIB
#AC_PROG_YACC

AM_PROG_CC_C_O

# test for devices
XASTIR_DETECT_DEVICES

# add includes and libs
XASTIR_ADD_SEARCH_PATHS

# check for pthread first
ACX_PTHREAD

# add compiler flags 
XASTIR_COMPILER_FLAGS

# Check for binaries
use_festival=yes
use_gpsman=yes
use_err_popups=no
use_lsb=no
use_spatial_db=no
use_postgis=no
use_mysql_spatial=no
AC_ARG_WITH(festival,[  --without-festival        Disable festival features.],use_festival=$withval)

AC_ARG_WITH(gpsman,[  --without-gpsman          Disable gpsman features.],use_gpsman=$withval)

AC_ARG_WITH(errorpopups,[  --with-errorpopups        Send error popups to stderr.],use_err_popups=$withval)

AC_ARG_WITH(lsb,[  --with-lsb                Enable Linux Standard Base.],use_lsb=$withval)

# Now that all the various "use_" variables are set, probe for binaries.
XASTIR_DETECT_BINARIES

# if the probes failed, then make sure the "use_" variables are set to no,
# even if we requested them.
if test "x${festival}" = "xno"; then
  use_festival=no
fi
if test "x${gpsman}" = "xno"; then
  use_gpsman=no
fi


# JMT - XXX - what is this and why is it relevant?
#AC_DEFINE(_REENTRANT, 1, [Use reentrant code if available.]) 
AC_DEFINE_UNQUOTED(STIPPLE, 1, [Legacy stuff, use crowbar and lets keep going]) 

# Checks for libraries. 
# 
# 
# Find the X11 include and library directories. 
# 
AC_PATH_XTRA
if test "$no_x" = "yes"; then
  AC_MSG_ERROR(*** No X11!  Install the X Window System development headers/libraries! ***)
fi

CPPFLAGS="$CPPFLAGS $X_CFLAGS" 
LDFLAGS="$LDFLAGS $X_LIBS"
LIBS="$LIBS $X_PRE_LIBS -lX11 $X_EXTRA_LIBS"

AC_SEARCH_LIBS(tan,math m)
AC_CHECK_LIB([Xext], [XextAddDisplay])

#***********************************
# Check for libXp removed on 31 Oct 2015
# This was in place for a very long time, but libxp is now deprecated and
# has even been removed from some distros
#
# XpGetDocumentData is not used *anywhere* in Xastir, but some distros have
# a Motif library that is linked against it.  This is supposed to be
# taken care of by the shared library loader, not by explicit linking, but
# as late as 2007 some distros had it linked wrong.  It appears as of 2015
# that nobody still does that, and this check causes more harm than good.
#
# If the build of Xastir fails at link time looking for XpGetDocumentData,
# then somebody *does* still do that incorrect shared library linking,
# and y9ou'll need to re-enable this on your system
#
#AC_CHECK_LIB([Xp], [XpGetDocumentData])
#************************************
AC_CHECK_LIB([Xt], [XtDisplay]) 
AC_CHECK_LIB([Xm], [XmTextFindString])



#use_html2text=no
#AC_PATH_PROG(html2text, [html2text --version], no, $BINPATH)
#if test "$html2text" != "no"; then
#  AC_DEFINE_UNQUOTED(HAVE_HTML2TEXT, 1, [Define if you have html2text]) 
#  AC_DEFINE_UNQUOTED(HTML2TEXT_PATH, "${html2text}", [Path to html2text]) 
#  use_html2text=yes
#fi



use_sed=no
AC_PATH_PROG(sed, [sed --version], no, $BINPATH)
if test "$sed" != "no"; then
  AC_DEFINE_UNQUOTED(HAVE_SED, 1, [Define if you have sed]) 
  AC_DEFINE_UNQUOTED(SED_PATH, "${sed}", [Path to sed]) 
  use_sed=yes
fi



use_mv=no
AC_PATH_PROG(mv, [mv --version], no, $BINPATH)
if test "$mv" != "no"; then
  AC_DEFINE_UNQUOTED(HAVE_MV, 1, [Define if you have mv]) 
  AC_DEFINE_UNQUOTED(MV_PATH, "${mv}", [Path to mv]) 
  use_mv=yes
fi



use_xfontsel=no
AC_PATH_PROG(xfontsel, xfontsel, no, $BINPATH)
if test "$xfontsel" != "no"; then
  AC_DEFINE_UNQUOTED(HAVE_XFONTSEL, 1, [Define if you have xfontsel]) 
  AC_DEFINE_UNQUOTED(XFONTSEL_PATH, "${xfontsel}", [Path to xfontsel]) 
  use_xfontsel=yes
fi



use_curl=no
curl_desired=yes
# Note: Had to move AC_CHECK_HEADER here instead of inside the first
# "if" statement:  It corrupted later tests, don't know why.  The
# only downside is that we check for the libcurl headers even if
# --without-libcurl was specified on the command-line.
AC_CHECK_HEADER(curl/curl.h, CURL_INC="yes", CURL_INC="no")
AC_ARG_WITH(libcurl,[  --without-libcurl         Disable libcurl features.],curl_desired=$withval)
if test "${curl_desired}" = "yes"; then
  if test "${CURL_INC}" = "yes"; then
    if test "${use_lsb}" = "no"; then 
      # Important: when using the three-argument version of AC_CHECK_LIB, you
      # must do all of what is normally the default behavior in the third argument,
      # not just the extra stuff you want to accomplish:
      AC_CHECK_LIB([curl], [curl_global_init], 
                   [use_curl="yes"
                    LIBS="${LIBS} -lcurl"
                    AC_DEFINE(HAVE_LIBCURL, 1,
                              [Define to 1 if your `curl' library has curl_global_init.])
                    ])
    else
      AC_DEFINE(HAVE_LIBCURL, 1, [Define to 1 if your `curl' library has curl_global_init.] )
      LIBS="${LIBS} -lcurl"
      use_curl=yes
    fi
  fi
fi
# Test for wget only if above libcurl tests fail
use_wget=no
if test "${use_curl}" = "no"; then
  AC_PATH_PROG(wget, [wget --version], no, $BINPATH)
  if test "$wget" != "no"; then
    AC_DEFINE_UNQUOTED(HAVE_WGET, 1, [Define if you have wget]) 
    AC_DEFINE_UNQUOTED(WGET_PATH, "${wget}", [Path to wget]) 
    use_wget=yes
  fi
fi



use_retrieval=no
if test "${use_curl}" = "yes"; then
  use_retrieval="yes (libcurl)"
else
  if test "${use_wget}" = "yes"; then
    use_retrieval="yes (wget)"
  fi
fi



# Test for libgps, used for accessing GPSD daemon
#use_libgps=no
#AC_CHECK_HEADER(gps.h, GPS_INC="yes", GPS_INC="no")
#if test "${GPS_INC}" = "yes"; then
#  AC_CHECK_LIB([gps], [gps_open],
#               [use_libgps="yes"
#                LIBS="${LIBS} -lgps"
#                AC_DEFINE(HAVE_LIBGPS, 1,
#                          [Define to 1 if your `libgps' library has gps_open.])
#                ])
#fi



# JMT - XXX - why is this here?
AC_CHECK_LIB(compat, main, 
    [LIBCOMPAT="-lcompat"
    AC_SUBST(LIBCOMPAT)])
 
# Check for sched_yield in librt (Needed for Solaris)
AC_CHECK_LIB(rt, sched_yield, LIBS="-lrt $LIBS")
 
# Check for endian 
AC_C_BIGENDIAN 
 


# Our old stuff.  Doesn't work in all cases.
# Checks for header files. 
#AC_CHECK_HEADERS([Xm/Xm.h],MOTIF_INC="yes", AC_MSG_ERROR(*** Cannot find Motif include files:  Please install one of Motif/OpenMotif/Lesstif development packages. ***))

XASTIR_PATH_MOTIF
if test "x$xm_includes" != "x" ; then
    CFLAGS="$CFLAGS -I$xm_includes"
    CPPFLAGS="$CPPFLAGS -I$xm_includes"
fi
if test "x$xm_libraries" != "x" ; then
    LDFLAGS="-L$xm_libraries $LDFLAGS"
fi

AC_SUBST(LDFLAGS) 
AC_SUBST(LIBS) 


#
AC_FUNC_ALLOCA 
AC_HEADER_DIRENT 
AC_HEADER_SYS_WAIT 
# We still check for stdarg.h even though it's standard, because a legacy
# file (snprintf.c) still uses its symbol.
AC_CHECK_HEADERS([stdarg.h])
AC_CHECK_HEADERS([sys/file.h sys/ioctl.h sys/param.h sys/socket.h sys/time.h signal.h])
AC_CHECK_HEADERS([termios.h unistd.h]) 

# Checks for typedefs, structures, and compiler characteristics.
#
AC_C_CONST
AC_TYPE_UID_T
AC_C_INLINE
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_STRUCT_TIMEZONE

#Some (Mac?) systems don't define this where it should be, so we kludge
AC_CHECK_TYPES(socklen_t,,,[#include <sys/types.h>
#include <sys/socket.h>])

# some BSD systems do not define sighandler_t, but rather sig_t
AC_CHECK_TYPES(sighandler_t,,,[#include <signal.h>])
AC_CHECK_TYPES(sig_t,,,[#include <signal.h>])

# some BSD systems do not define sighandler_t, but rather sig_t
AC_CHECK_TYPES(sigjmp_buf,,,[#include <setjmp.h>])

#Force this into config.h.in
AH_BOTTOM([#ifndef HAVE_SOCKLEN_T
#define socklen_t int
#endif])


# Check for tm_gmtoff in tm
AC_CHECK_GMTOFF
 
# Checks for library functions. 
# 
AC_FUNC_FORK 
AC_FUNC_GETPGRP 
AC_FUNC_MALLOC 
AC_FUNC_MKTIME 
AC_FUNC_MMAP 
AC_FUNC_STAT 
AC_FUNC_STRFTIME 
AC_FUNC_STRTOD 
AC_CHECK_FUNCS([getcwd getpgrp gethostbyname gethostname gettimeofday])
AC_CHECK_FUNCS([getwd inet_ntoa memmove mempcpy memset mkdir munmap pow])
AC_CHECK_FUNCS([putenv select setenv setlocale settimeofday signal sigignore])
AC_CHECK_FUNCS([snprintf socket sqrt stpcpy strcasecmp strchr strdup])
AC_CHECK_FUNCS([strerror strncasecmp strpbrk strptime strrchr strstr])
AC_CHECK_FUNCS([strtof strtol strtoul tzset vsnprintf])

# Check for deprecated/current pthread functions
AC_CHECK_FUNCS([pthread_mutexattr_setkind_np pthread_mutexattr_settype])

# this is a GNU extension and not present on all systems.
AC_CHECK_FUNCS([strndup])

#this is also a recent extension that should not be counted on
AC_CHECK_FUNCS([roundf])
 
# Check for libproj (need to do this before test for geotiff, which is the
# only thing we have that uses proj

use_proj=yes
AC_ARG_WITH(libproj,[  --without-libproj         Disable libproj features.],use_proj=$withval)
if test "${use_proj}" = "yes"; then
  use_proj=no
  AC_CHECK_LIB([proj], [proj_create_crs_to_crs], [use_proj=yes LIBS="$LIBS -lproj"
    AC_DEFINE(HAVE_PROJ, 1, Define to 1 if you have the `libproj' library (-lproj). )],
    [  AC_CHECK_LIB([proj], [pj_init], [use_proj=yes LIBS="$LIBS -lproj"
    AC_DEFINE(HAVE_PROJ, 1, Define to 1 if you have the `libproj' library (-lproj). )])]

  )
fi


# Check for Shapelib
use_shapelib=yes
shapelib_desired=yes
AC_ARG_WITH(shapelib,[  --without-shapelib        Disable shapelib features.],shapelib_desired=$withval)
if test "${shapelib_desired}" = "no"; then
  use_shapelib=no
fi
if test "${shapelib_desired}" = "yes" ; then
  use_shapelib=no
  AC_CHECK_HEADERS([shapefil.h libshp/shapefil.h],
                   [AC_CHECK_LIB([shp], [DBFOpen],
                                 [use_shapelib=yes
                                  LIBS="$LIBS -lshp"
                                  AC_DEFINE(HAVE_LIBSHP, 1,
                                            [Define to 1 if you have the `shp' library (-lshp).])
                                  ])
                    break
                   ])
fi

# Shapelib requires pcre now
use_pcre=no
if test "${shapelib_desired}" = "yes" -a "${use_shapelib}" = "yes"; then
  AC_CHECK_HEADERS([pcre2.h],
                   [AC_CHECK_LIB([pcre2-8],[pcre2_compile_8],
                                 [use_pcre="yes (PCRE2)"
                                  LIBS="$LIBS -lpcre2-8"
                                  ],
                                 [use_pcre=no
                                  AC_MSG_WARN([Found pcre2 headers but not libraries])
                                 ])
                   ],
                   [use_pcre=no],
                   [
                     #define PCRE2_CODE_UNIT_WIDTH 8
                   ])
   if test "${use_pcre}" = "no"
   then
     AC_MSG_WARN([Modern PCRE2 not found, checking for legacy version])
     AC_CHECK_HEADERS([pcre.h pcre/pcre.h],
                      [AC_CHECK_LIB([pcre], [pcre_compile],
                                    [use_pcre="yes (legacy)"
                                     LIBS="$LIBS -lpcre"
                                     AC_DEFINE(XASTIR_LEGACY_PCRE, 1, Define to 1 if you have the legacy `pcre' library (-lpcre). )
                                                   ])
                       break
                       ])
    fi
 if test "${use_pcre}" = "no"
 then
    AC_MSG_ERROR([Shapelib support requires PCRE, and we could not find PCRE.  Either disable shapelib by specifying "--without-shapelib" or install PCRE libraries and headers])
 fi
fi

if test "${shapelib_desired}" = "yes" -a "${use_shapelib}" = "no"; then
 AC_MSG_WARN([**************************************************************** ])  
 AC_MSG_WARN([Your system does not have shapelib installed.                    ])
 AC_MSG_WARN([Install shapelib on your system to eliminate this warning.       ])
 AC_MSG_WARN([**************************************************************** ])  
 use_shapelib=no
fi




# Check for XPM
use_xpm="no"
if test "${use_lsb}" = "no"; then 
  AC_CHECK_HEADERS(X11/xpm.h,
  [
    #action if found in addition to setting HAVE_X11_XPM_H
    AC_SEARCH_LIBS(XpmWriteFileFromPixmap, [Xpm],
    [
      # action if found (in addition to adding -lXpm to LIBS
      use_xpm="yes"
      AC_DEFINE(HAVE_LIBXPM, 1, [Define to 1 if you have the `Xpm' library (-lXpm).] )
    ])
  ],
  [
    # Action if X11/xpm.h is not found: look for Xm/XpmI.h instead
    AC_CHECK_HEADERS(Xm/XpmI.h,
    [ # Action if found, in addition to setting HAVE_XM_XPMI_H

      AC_SEARCH_LIBS(XmeXpmWriteFileFromPixmap, [Xm],
      [
        # action if -lXm is found, in addition to adding -lXm to LIBS
        use_xpm="yes"
        AC_DEFINE(HAVE_LIBXPM_IN_XM, 1, [Define to 1 if the `Xm' library (-lXm) is used for Xpm.])
      ],
      [ #action if -lXm not found: issue warning
         AC_MSG_WARN([**************************************************************** ])
         AC_MSG_WARN([Your system does not have a library that contains XpmWriteFileFromPixmap])
 AC_MSG_WARN([PNG snapshots cannot work.       ])
 AC_MSG_WARN([**************************************************************** ])
      ])
    ])
  ])
else
  AC_DEFINE(HAVE_LIBXPM, 1, Define to 1 if you have the `Xpm' library (-lXpm). )
  AC_DEFINE(HAVE_LIBXPM_IN_XM, 1, Define to 1 if the `Xm' library (-lXm) is used for Xpm. )
  AC_DEFINE(HAVE_X11_XPM_H, 1, Define to 1 if you have 'X11/xpm.h' )
fi
 
 
#check for libtiff, libgeotiff
# libjpeg.a and libz.a need to be compiled under LSB


use_geotiff=yes
AC_ARG_WITH(geotiff, [  --without-geotiff         Disable geotiff features.], use_geotiff=$withval)
if test "${use_geotiff}" = "yes"; then
    if test "${use_proj}" = "yes" ; then
        if test "${use_lsb}" = "yes" ; then
            use_tiff=yes
            use_geotiff=yes
            LIBS="$LIBS -lgeotiff -ltiff -ljpeg -lz -lproj"
            AC_DEFINE(HAVE_TIFF, 1,Define to 1 if you have the 'tiff' library (-ltiff))
            AC_DEFINE(HAVE_LIBGEOTIFF, 1,Define to 1 if you have the `geotiff' library (-lgeotiff).)
        else
            AC_CHECK_HEADER(xtiffio.h, , use_geotiff="no")
            if test "${use_geotiff}" = "yes"; then
                AC_CHECK_LIB(tiff, TIFFClose, use_tiff=yes
                LIBS="$LIBS -ltiff"
                AC_DEFINE(HAVE_TIFF, 1,Define to 1 if you have the 'tiff' library (-ltiff)))
                if test "${use_tiff}" = "yes"; then
                    use_geotiff=no
                    AC_CHECK_LIB(geotiff, GTIFNew, use_geotiff=yes
                        LIBS="$LIBS -lgeotiff"
                        AC_DEFINE(HAVE_LIBGEOTIFF, 1,Define to 1 if you have the `geotiff' library (-lgeotiff).))
                else
                    echo "*** Warning: geotiff requires libtiff."
                    use_geotiff=no
                fi
            else
                echo "*** Warning: geotiff include files not found."
            fi
        fi
    else
        echo "*** Warning: geotiff requires libproj."
        use_geotiff=no
    fi
fi 

# Check for spatial database support
use_postgis=no
AC_ARG_WITH(postgis, [  --with-postgis            Enable Postgresql with PostGIS.],use_postgis=$withval)
if test "${use_postgis}" != "no"; then
  XASTIR_CHECK_POSTGIS
fi
use_mysql=no
AC_ARG_WITH(mysql, [  --with-mysql	            Enable MySQL, with spatial support if available.],use_mysql=$withval)
if test "${use_mysql}" != "no"; then
  XASTIR_CHECK_MYSQL
fi

if test "${use_postgis}" = "yes"; then
  use_spatial_db=yes
  AC_DEFINE(HAVE_SPATIAL_DB, 1, [Spatial database support available] )
  AC_DEFINE(HAVE_DB, 1, [Database support available] )
fi
if test "${use_mysql_spatial}" = "yes"; then
  use_spatial_db=yes
  AC_DEFINE(HAVE_DB, 1, [Database support available] )
  AC_DEFINE(HAVE_SPATIAL_DB, 1, [Spatial database support available] )
elif test use_mysql_any = "yes"; then
  AC_DEFINE(HAVE_DB, 1, [Database support available] )
fi

# Check for AX.25 library 
use_ax25=yes
AC_ARG_WITH(ax25,[  --without-ax25            Disable ax25 features.],use_ax25=$withval)
if test "${use_ax25}" = "yes"; then
  use_ax25=no
  AC_CHECK_HEADERS(netax25/ax25.h, [AC_CHECK_LIB(ax25, ax25_config_load_ports, use_ax25=yes
  LIBS="$LIBS -lax25"
  AC_DEFINE(HAVE_LIBAX25, 1, Define to 1 if you have the `ax25' library (-lax25). )
  break)])
fi


AC_ARG_ENABLE(davis, [  --enable-davis          Turn on Davis support],
  [case "${enableval}" in
    yes) davis=true ;;
     no) davis=false ;;
      *) AC_MSG_ERROR(bad value ${enableval} for --enable-davis) ;;
  esac],[davis=false])
AM_CONDITIONAL(DAVIS, test x$davis = xtrue)
if test "${davis}" = "true"; then
  davis=yes
else
  davis=no
fi





# Check for LIBGC, a Garbage Collection library.  If found, allow
# linking it with Xastir, but require the "--with-libgc" flag to
# enable it.
use_libgc=no
AC_ARG_WITH(libgc,[  --with-libgc              Enable libgc features.],use_libgc=$withval)
if test "${use_libgc}" = "yes"; then
    AC_CHECK_HEADERS([gc.h], LIBGC_INC="yes", use_libgc=no)
    AC_CHECK_LIB([gc], [GC_init], , use_libgc=no)
fi





# Check whether profiling was requested.  If so, add the "-pg" flag
# to CFLAGS.  Require "--with-profiling" flag to enable it.
use_profiling=no
AC_ARG_WITH(profiling,[  --with-profiling          Enable profiling features.],use_profiling=$withval)
if test "${use_profiling}" = "yes"; then
    CFLAGS="$CFLAGS -pg"
fi





# Check for Berkeley DB.
use_map_cache=yes
AC_ARG_WITH(map_cache,[  --without-map-cache       Disable Berkeley DB Map Caching.],use_map_cache=$withval)
if test "${use_map_cache}" = "yes"; then
  XASTIR_BERKELEY_DB_CHK
fi





# Check for GraphicsMagick.  If not available/desired then check for
# ImageMagick.  These checks are VERY important to have as the last,
# as they mess up previous checks if they fail.
#
use_graphicsmagick=yes
use_imagemagick=no
AC_ARG_WITH(graphicsmagick,[  --without-graphicsmagick  Disable graphicsmagick features.],use_graphicsmagick=$withval)
if test "${use_graphicsmagick}" = "yes"; then
  if test "${use_lsb}" = "no"; then
    XASTIR_CHECK_GRAPHICSMAGICK
  else
    MAGIC_BIN="/opt/lsb-tmp/bin/GraphicsMagick-config"
    CPPFLAGS="$CPPFLAGS `${MAGIC_BIN} --cppflags`"
    CXXFLAGS="$CXXFLAGS `${MAGIC_BIN} --cflags`"
    CFLAGS="$CFLAGS `${MAGIC_BIN} --cflags`"
    LDFLAGS="$LDFLAGS `${MAGIC_BIN} --ldflags`"
    LIBS="${MAGIC_LIB_DIR} `${MAGIC_BIN} --libs` $LIBS"
    AC_DEFINE(HAVE_GRAPHICSMAGICK, 1, [GraphicsMagick image library])
  fi
fi
#
# Note: This must be a separate "if" as the use_graphicsmagick
# variable gets set to "no" if GM is not available above.
#
if test "${use_graphicsmagick}" = "no"; then
  use_imagemagick=yes
  AC_ARG_WITH(imagemagick,[  --without-imagemagick     Disable imagemagick features.],use_imagemagick=$withval)
  if test "${use_imagemagick}" = "yes"; then
    XASTIR_CHECK_IMAGEMAGICK
  fi
fi
#
#
#
if test "${use_imagemagick}" = "yes" -o "${use_graphicsmagick}" = "yes"; then
  AC_DEFINE(HAVE_MAGICK, 1, [GraphicsMagick or ImageMagick library])
fi





# Set XASTIR_DATA_BASE in CPPFLAGS due to Gnu coding standard that requires
# datadir expansion to be deferred until make time.
if test "x${datadir}" = "x"; then 
  CPPFLAGS="$CPPFLAGS -DXASTIR_DATA_BASE=\\\"${ac_default_prefix}/share/xastir\\\""
else 
  CPPFLAGS="$CPPFLAGS -DXASTIR_DATA_BASE=\\\"${datadir}/xastir\\\""
fi 
 


# Add a special option that makes Cygwin link much faster (from
# Henk de Groot)
AC_MSG_CHECKING([if linker supports --no-keep-memory])
save_LDFLAGS="$LDFLAGS"
LDFLAGS="-Wl,--no-keep-memory $LDFLAGS"
AC_LINK_IFELSE(
 [AC_LANG_PROGRAM([],
                  [[return(0);]])],
 [xa_cv_no_keep_memory=yes],
 [xa_cv_no_keep_memory=no])

if test "${xa_cv_no_keep_memory}" = "no"; then 
  LDFLAGS="$save_LDFLAGS"
  AC_MSG_RESULT([no])
else
  AC_MSG_RESULT([yes])
fi

# 
# Assure we have this order for these LIBS: "-lXm -lXt -lX11" by 
# adding them to the beginning.  Add spaces around them for the 
# following steps.  Here I'm adding them to the beginning so the   
# dupe-check portion will take out the later ones that might be in 
# the wrong order.  If they're in the wrong order, we can get 
# "Error: Shell widget xastir has zero width and/or height" when 
# Xastir is started up.   Can also get "Error: attempt to add
# non-widget child "DropSiteManager" to parent", but that can also
# be caused by other problems with libraries.
# 
LIBS=" -lXm -lXt -lX11 $LIBS" 
  
# 
# Remove duplicate entries.  Thanks to Paul Lutt, ke7xt, for this! 
#
# Don't get rid of dupes on the LIBS line.  We may need the same
# "-Llibdir" called out several times, immediately prior to each
# "-llibrary" that needs it.
# 
changequote(,) 
CFLAGS=`echo  "$CFLAGS" | awk '{for(i=1;i<=NF;++i) {if (arg[$i]++ == 0) s = s " " $i} print s}'`
CPPFLAGS=`echo  "$CPPFLAGS" | awk '{for(i=1;i<=NF;++i) {if (arg[$i]++ == 0) s = s " " $i} print s}'`
LDFLAGS=`echo "$LDFLAGS" | awk '{for(i=1;i<=NF;++i) {if (arg[$i]++ == 0) s = s " " $i} print s}'`
#LIBS=`echo "$LIBS" | awk '{for(i=1;i<=NF;++i) {if (arg[$i]++ == 0) s = s " " $i} print s}'`
changequote([,]) 


# 
# Remove "-g" CFLAGS/CPPFLAGS entries.  Note:  This is a _bad_ idea
# as debuggers won't have a symbol table to work from!
#
#changequote(,) 
#CFLAGS=`echo  "$CFLAGS" | awk '{for(i=1;i<=NF;++i) {if ($i != "-g") s = s " " $i} print s}'`
#CPPFLAGS=`echo  "$CPPFLAGS" | awk '{for(i=1;i<=NF;++i) {if ($i != "-g") s = s " " $i} print s}'`
#changequote([,]) 


#
# Change the compiler optimization if using Windows/Cygwin in order
# to reduce memory usage and speed up the compile.  Here we change
# "-O2" to "-O1" or " " in CFLAGS/CPPFLAGS to reduce the level of
# optimization.
# We also wish to remove "-no-undefined" from LDFLAGS on Cygwin, as
# we use GCC for our linking instead of libtool, and GCC doesn't
# like that flag at the link stage.
#
case "$host_os" in
cygwin*)
  changequote(,) 
  CFLAGS=`echo  "$CFLAGS" | awk '{for(i=1;i<=NF;++i) {if ($i != "-O2") s = s " " $i} print s}'`
  CPPFLAGS=`echo  "$CPPFLAGS" | awk '{for(i=1;i<=NF;++i) {if ($i != "-O2") s = s " " $i} print s}'`
  LDFLAGS=`echo  "$LDFLAGS" | awk '{for(i=1;i<=NF;++i) {if ($i != "-no-undefined") s = s " " $i} print s}'`
  changequote([,]) 
;;
esac
 

# Create the ILIBS defined variable.  We add spaces here in the else
# clauses in order to make the REGRESSION_TESTS format nicely.  We
# take them back out before we define the variable that Xastir uses
# for it's Help->About dialog.
#
if test "${use_ax25}" = "yes"; then
  ILIBS="AX25"
else
  ILIBS="    "
fi
if test "${use_festival}" = "yes"; then
  ILIBS="$ILIBS Festival"
else
  ILIBS="$ILIBS         "
fi
if test "${use_gpsman}" = "yes"; then
  ILIBS="$ILIBS GPSMan"
else
  ILIBS="$ILIBS       "
fi
if test "${use_graphicsmagick}" = "yes"; then
    ILIBS="$ILIBS GraphicsMagick"
else
  if test "${use_imagemagick}" = "yes"; then
    ILIBS="$ILIBS ImageMagick   "
  else
    ILIBS="$ILIBS               "
  fi 
fi
if test "${use_proj}" = "yes"; then
  ILIBS="$ILIBS libProj"
else
  ILIBS="$ILIBS        "
fi
if test "${use_geotiff}" = "yes"; then
  ILIBS="$ILIBS GeoTiff"
else
  ILIBS="$ILIBS        "
fi
if test "${use_shapelib}" = "yes"; then
  ILIBS="$ILIBS ShapeLib"
else
  ILIBS="$ILIBS         "
fi
if test "${use_pcre}" != "no"; then
  if test "${use_pcre}" = "yes(legacy)"
  then
    ILIBS="$ILIBS PCRE"
  else
    ILIBS="$ILIBS PCRE2"
  fi
else
  ILIBS="$ILIBS     "
fi
if test "${use_map_cache}" = "yes"; then
  ILIBS="$ILIBS map_caching"
else
  ILIBS="$ILIBS            "
fi
if test "${use_err_popups}" = "yes"; then
  ILIBS="$ILIBS error_popups"
else
  ILIBS="$ILIBS             "
fi
if test "${use_libgc}" = "yes"; then
  ILIBS="$ILIBS libgc"
else
  ILIBS="$ILIBS      "
fi
if test "${use_profiling}" = "yes"; then
  ILIBS="$ILIBS profiling"
else
  ILIBS="$ILIBS          "
fi
if test "${use_lsb}" = "yes"; then
  ILIBS="$ILIBS LSB"
else
  ILIBS="$ILIBS    "
fi
if test "${use_curl}" = "yes"; then
  ILIBS="$ILIBS libcurl"
else
  if test "${use_wget}" = "yes"; then
    ILIBS="$ILIBS wget   "
  else
    ILIBS="$ILIBS        "
  fi
fi

# Write out the library information to "summary.log".  Here we want
# the spaces for the undefined pieces, so REGRESSION_TESTS formats
# nicely.
echo "$ILIBS" >summary.log


# 
# Remove extraneous spaces from output variables (aesthetic) 
# 
ILIBS=`echo $ILIBS | sed -e 's/  */ /g'` 
X_CFLAGS=`echo $X_CFLAGS | sed -e 's/  */ /g'` 
X_PRE_LIBS=`echo $X_PRE_LIBS | sed -e 's/  */ /g'` 
X_LIBS=`echo $X_LIBS | sed -e 's/  */ /g'` 
X_EXTRA_LIBS=`echo $X_EXTRA_LIBS | sed -e 's/  */ /g'` 
 
CC=`echo $CC | sed -e 's/  */ /g'` 
CFLAGS=`echo $CFLAGS | sed -e 's/  */ /g'`
CPPFLAGS=`echo $CPPFLAGS | sed -e 's/  */ /g'` 
CXXFLAGS=`echo $CXXFLAGS | sed -e 's/  */ /g'` 
LDFLAGS=`echo $LDFLAGS | sed -e 's/  */ /g'` 
TESTED_LIBS=`echo $LIBS | sed -e 's/  */ /g'` 
MAGICK_DEP_LIBS=`echo $MAGICK_DEP_LIBS | sed -e 's/  */ /g'` 
LIBS=`echo $LIBS | sed -e 's/  */ /g'` 



tmp_path="$ac_default_prefix"
tmp_bin_path="$ac_default_prefix"
if test "x$prefix" != xNONE; then
  tmp_path="$prefix"
  tmp_bin_path="$prefix"
fi
if test "x$exec_prefix" != xNONE; then
  tmp_bin_path="$exec_prefix"
fi
AC_DEFINE_UNQUOTED(XASTIR_PATH, "`echo $tmp_path`", [Path to installed Xastir files.])
AC_DEFINE_UNQUOTED(XASTIR_BIN_PATH, "`echo $tmp_bin_path`", [Path to installed Xastir binaries.])



AC_DEFINE_UNQUOTED(XASTIR_INSTALLED_LIBS, "`echo $ILIBS`", [Defines the installed libs])


AC_SUBST(LDFLAGS) 
AC_SUBST(LIBS) 
#AC_SUBST(CPPFLAGS) 
AC_SUBST(X_CFLAGS) 
#AC_SUBST(LDFLAGS) 
#AC_SUBST(X_PRE_LIBS) 
#AC_SUBST(X_LIBS) 
#AC_SUBST(X_EXTRA_LIBS) 

AC_CONFIG_FILES([Makefile \
    callpass/Makefile \
    config/Makefile \
    help/Makefile \
    scripts/Makefile \
    src/Makefile \
    src/rtree/Makefile \
    symbols/Makefile \
    xastir.spec \
    xastir-min.spec \
    xastir-lsb.spec]) 
AC_CONFIG_FILES([scripts/get-BOMdata],[chmod +x scripts/get-BOMdata])
AC_CONFIG_FILES([scripts/get-NWSdata],[chmod +x scripts/get-NWSdata])
AC_CONFIG_FILES([scripts/get-gnis],[chmod +x scripts/get-gnis])
AC_CONFIG_FILES([scripts/get-fcc-rac.pl],[chmod +x scripts/get-fcc-rac.pl])
AC_CONFIG_FILES([scripts/icontable.pl],[chmod +x scripts/icontable.pl])
 
AC_OUTPUT 

# Please leave these in as they're very useful for debug when we
# port to new platforms!  Leave them commented out unless doing
# debugging on the configure scripts.
#
#echo "" 
#echo "        LIBS: $LIBS" 
#echo "     LDFLAGS: $LDFLAGS" 
#echo "      CFLAGS: $CFLAGS" 
#echo "    CPPFLAGS: $CPPFLAGS" 
#echo "      X_LIBS: $X_LIBS" 
#echo "  X_PRE_LIBS: $X_PRE_LIBS" 
#echo "X_EXTRA_LIBS: $X_EXTRA_LIBS" 


echo ===========================================
echo
echo AC_PACKAGE_NAME AC_PACKAGE_VERSION has been configured to use the following
echo options and external libraries:
echo
echo MINIMUM OPTIONS:
echo "  ShapeLib (Vector maps) .................... : $use_shapelib"
echo
echo RECOMMENDED OPTIONS:

echo "  Xpm / Snapshots ........................... : $use_xpm"
echo -n "  GraphicsMagick/ImageMagick (Raster maps) .. : "
if test "${use_graphicsmagick}" = "yes"; then 
  echo "yes (GraphicsMagick)"
else
  if test "${use_imagemagick}" = "yes"; then
    echo "yes (ImageMagick)"
  else
    echo "no"
  fi
fi
echo "  pcre (Shapefile customization) ............ : $use_pcre"
echo "  Berkeley DB map caching-Raster map speedups : $use_map_cache"
echo "  internet map retrieval .................... : $use_retrieval"
echo
echo FOR THE ADVENTUROUS:
echo "  AX25 (Linux Kernel I/O Drivers) ........... : $use_ax25"
echo "  libproj (USGS Topos & Aerial Photos) ...... : $use_proj"
echo "  GeoTiff (USGS Topos & Aerial Photos) ...... : $use_geotiff"
echo "  Festival (Text-to-speech) ................. : $use_festival"
echo "  GPSMan/gpsmanshp (GPS downloads) .......... : $use_gpsman"
#echo "  GPSD Client (libgps) ...................... : $use_libgps"

if test "${use_err_popups}" = "yes" -o "${use_libgc}" = "yes" -o "${use_profiling}" = "yes" -o "${use_lsb}" = "yes" -o "${use_spatial_db}" = "yes" -o "$use_mysql_any" = "yes"; then
  echo
  echo DEVELOPER OPTIONS:
  echo "  ErrorPopups (Old Method) .................. : $use_err_popups"
  echo "  libgc (Debug memory usage) ................ : $use_libgc"
  echo "  profiling (Debug code efficiency) ......... : $use_profiling"
  echo "  Linux Standard Base (LSB) ................. : $use_lsb"
  echo "  Spatial database support .................. : $use_spatial_db"
  echo "  Spatial database Postgresql/Postgis ....... : $use_postgis"
  echo "  MySQL ..................................... : $use_mysql_any"
  echo "  MySQL Spatial database support ............ : $use_mysql_spatial"
fi

#echo   Davis/MySQL............................. : $davis
echo 
echo AC_PACKAGE_NAME will be installed in $prefix/bin. 
if test "x$xastirpath" != "x" ; then 
        echo Warning: You have an old copy of Xastir at $xastirpath. 
fi 
echo "Type 'make' to build Xastir (Use 'gmake' instead on some systems)." 


