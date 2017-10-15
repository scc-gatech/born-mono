cmake_minimum_required(VERSION 2.8)
project(SEPLIB-base-lib-util-convert)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../include)


set (SOURCES 
cubicspline.c  ealloc.c     intcub.c  sep_cwp.f90  xdrbhdrsub.c  xindex.c
cwp_alloc.c      pfafft.c  valpkge.c    xdrhdrsub.c)


add_library(sepcwp ${SOURCES})
install(TARGETS sepcwp DESTINATION lib)
