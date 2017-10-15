cmake_minimum_required(VERSION 2.8)
project(SEPLIB-vplot-filters-xtlib)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../seplib_base/include)
set (SOURCE1 
appdata.c  xtcolors.c	 xtdoc.h	   xtframe.h	   xtplot.c
popup.c    xtcommands.c  	   xtgetpoint.c  xtpaint.c     xtpoint.c
xtarea.c   xtcommands.h   xtinteract.c  xtpen.h       xtraster.c
xtattr.c    xterase.c	   xtmessage.c	 xtpixmap.c
xtclose.c    xtframe.c	   	 xtpixmap.h)

set(SOURCES ${SOURCE1} xt_dovplot.c xtopen.c xtconf.c)
set(SOURCES_SEP  ${SOURCE1} xt_dovplot_SEP.c xtopen_SEP.c xtconf_SEP.c)




add_library(xt_pen ${SOURCES})
add_library(xt_pen_SEP ${SOURCES_SEP})
install(TARGETS xt_pen xt_pen_SEP DESTINATION lib)
