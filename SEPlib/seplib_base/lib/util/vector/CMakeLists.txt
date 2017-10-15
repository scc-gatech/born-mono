cmake_minimum_required(VERSION 2.8)
project(SEPLIB-base-lib-util-vector)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../include)


set (SOURCES 
adjnull77.f90  ipow.f90   sep_vector.f90  setvali.c
copy77.f90     inv.f90	    scale.f90  setvalf.c       sqrt.f90)


add_library(sepvector ${SOURCES})
target_link_libraries(sepvector sep)
install(TARGETS sepvector DESTINATION lib)
