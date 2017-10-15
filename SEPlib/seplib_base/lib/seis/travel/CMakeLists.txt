cmake_minimum_required(VERSION 2.8)
project(SEPLIB-base-lib-seis-travel)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../include)


set (SOURCES 
fastmarch.c    hwt_func.c  hwt_gen.c  hwt_ray.c  hwt_tesel.c)


add_library(septravel ${SOURCES})
install(TARGETS septravel DESTINATION lib)
