cmake_minimum_required(VERSION 2.8)
project(SEPLIB-base-lib-util-convert)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../include)


set (SOURCES 
convert_char.c	createfloat.c    getbackint.c  sepconvert.f90  zasc.c)


add_library(sepconvert ${SOURCES})
install(TARGETS sepconvert DESTINATION lib)
