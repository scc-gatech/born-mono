cmake_minimum_required(VERSION 2.8)
project(SEPLIB-base-lib-util-math)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../include)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/../sepaux)

set (SOURCES 
pctrist.f90   rand.f90	 vtris.f90
 ptridiag.f90  rand_mod.f90 cluster.c
 quant.f90	obb.cpp   sqroot.c
  rand01.f90    subfslan.f90)


add_library(sepmath ${SOURCES})
target_link_libraries(sepmath sepaux sep m)
install(TARGETS sepmath DESTINATION lib)
