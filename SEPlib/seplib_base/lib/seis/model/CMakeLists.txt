cmake_minimum_required(VERSION 2.8)
project(SEPLIB-base-lib-seis-model)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../include)


set (SOURCES 
data.f90		image.f90	 slow.f90
extrapolation.f90	kernal.f90	 wave.f90
extrapolation_type.f90	rtm_section.f90  wavelet_mod.f90)


add_library(sepmodel ${SOURCES})
install(TARGETS sepmodel DESTINATION lib)
