cmake_minimum_required(VERSION 2.8)
project(SEPLIB-vplot-filters-xtlib)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../seplib_base/include)
set (SOURCE1 
psarea.c     psdoc.h    ps.h	   psplot.c	     psreset.c
psattr.c   	  pserase.c 	   psraster_dumb.c   pstext.c
psclose.c   psfonts.h   psraster_smart.c  psvector.c)

set (SOURCES ${SOURCE1} psconf.c psopen.c)
set (SOURCES_SEP ${SOURCE1} psconf_SEP.c psopen_SEP.c)






add_library(ps_pen ${SOURCES})
add_library(ps_pen_SEP ${SOURCES_SEP})
install(TARGETS ps_pen ps_pen_SEP DESTINATION lib)
