cmake_minimum_required(VERSION 2.8)
project(SEPLIB-base-lib-util-aux)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../include)


set (SOURCES 
arrayreadf.c	      interp2.f90      pad.c		      sgainpar.c
arrayreadi.c	      interpolate.f90  parcat.c		      sorting.f90
aux_string_mod.f90    isamax.f90      patch_oc.c	      sort_subs.f90
cent.c		      iter_mod.f90     pqueue.c		      subrslan.f90
compare.c	      loop_calc.c        time_window.c
	      multihdr.c       sep_aux.f90	      utilities.f90
helixcartmod_big.f90  pad2.f90	       septimers.f90)


add_library(sepaux ${SOURCES})
TARGET_LINK_LIBRARIES(sepaux superset sep3d sep)
install(TARGETS sepaux DESTINATION lib)
