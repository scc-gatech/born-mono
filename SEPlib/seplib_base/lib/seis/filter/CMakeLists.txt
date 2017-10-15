cmake_minimum_required(VERSION 2.8)
project(SEPLIB-base-lib-seis-filter)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../include)


set (SOURCES 
boxconv77.f90  energysubs.c  icaf377.f90     sinc.c	      tcai377.f90
boxmo.f90      icai177.f90     sinc_interp.f90  triangle277.f90
burg.f90       halfdifa.f90  icai277.f90     tcaf177.f90      triangle77.f90
causint2.f90   halfint.f90   icai377.f90     tcaf277.f90      tri_smooth_mod.f90
causint77.f90  icaf177.f90   nfilts_io.f90   tcai177.f90
cos_taper.f90  icaf277.f90   sep_filter.f90  tcai277.f90)


add_library(sepfilter ${SOURCES})
target_link_libraries(sepfilter sepgeef90 sepclassutil superset sep3d sep)
install(TARGETS sepfilter DESTINATION lib)
