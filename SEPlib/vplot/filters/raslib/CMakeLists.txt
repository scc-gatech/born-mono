cmake_minimum_required(VERSION 2.8)
project(SEPLIB-vplot-filters-raslib)



include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../seplib_base/include ${NETPBM_INCLUDE_DIR})
set (SOURCE1 
rasattr.c	rasconf.c   rasreset.c	 rasvector4.c
rasvector2.c  rasvector.c
raserase.c     rasvector3.c)


set(SOURCES ${SOURCE1} rasclose.c rasopen.c )
set(SOURCES_SEP  ${SOURCE1} rasopen_SEP.c rasclose_SEP.c )




add_library(ras_pen ${SOURCES})
add_library(ras_pen_SEP ${SOURCES_SEP})
install(TARGETS ras_pen ras_pen_SEP DESTINATION lib)
