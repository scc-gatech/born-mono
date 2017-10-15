cmake_minimum_required(VERSION 2.8)
project(SEPLIB-vplot-filters-vplib)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../seplib_base/include)
set (SOURCE1 
vpattr.c      vpdoc.h	   vplogmessage.c  vpopen.c    vpreset.c	 vpvector.c
vpclose.c     vpdovplot.c  vplogvector.c   vpplot.c  
     vperase.c    vpmarker.c	   vppoly.c   
 vp.h	   vpmessage.c	   vpraster.c  vptext.c)

set(SOURCES ${SOURCE1} vpsepwhere.c vpconf.c)
set(SOURCES_SEP  ${SOURCE1} vpsepwhere_SEP.c vpconf_SEP.c)




add_library(vp_pen ${SOURCES})
add_library(vp_pen_SEP ${SOURCES_SEP})
install(TARGETS vp_pen vp_pen_SEP DESTINATION lib)
