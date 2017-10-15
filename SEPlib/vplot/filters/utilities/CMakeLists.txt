cmake_minimum_required(VERSION 2.8)
project(SEPLIB-vplot-filters-utilities)

set (SOURCES 
clip.c	   dither.c   error.c	greycorr.c   solve.c  vecoutline.c
dashvec.c  dupside.c  fatvec.c	intersect.c  sort.c   vptodev.c)





add_library(vplotutil ${SOURCES})
install(TARGETS vplotutil DESTINATION lib)
