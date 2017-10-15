cmake_minimum_required(VERSION 2.8)
project(SEPLIB-base-lib-seis-velan)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../include)


set (SOURCES 
hypint.f90  nmo_subs.f90    rms2int.f90    velansub.f90  vint2rms.f90
hypsum.f90  nmo_varoff.f90  vconvsubs.f90  velsimp.f90)


add_library(sepvelan ${SOURCES})
install(TARGETS sepvelan DESTINATION lib)
