cmake_minimum_required(VERSION 2.8)
project(SEPLIB-base-lib-util-par)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../include)


set (SOURCES 
lock.c  pass_mpi.c  sep_param.f90  split.c)


add_library(seppar ${SOURCES})
install(TARGETS seppar DESTINATION lib)
