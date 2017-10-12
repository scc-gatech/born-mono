cmake_minimum_required(VERSION 2.8)
project(Generic-fortran Fortran)
set(CMAKE_Fortran_MODULE_DIRECTORY  ${CMAKE_INSTALL_PREFIX}/include)

set(SOURCE c_stubs.f90 cIO.f90 c_regfile.f90 c_param.f90 axis.f90 hypercube.f90  param_func.f90 genericFile.f90 genericIO.f90 ioModes.f90)

if (USE_SEP)
#include_directories(${SEPlib_DIRNAME_include})
#   set(SOURCE ${SOURCE}  sep_param.f90  sep_reg_file.f90 sepIO.f90)
endif (USE_SEP)

if(USE_RSF)
  set(SOURCE ${SOURCE} rsfIO.f90  rsf_param.f90  rsf_reg_file.f90)
  include_directories(${RSFINCDIR})
endif (USE_RSF)
add_library(genericFor ${SOURCE})

install(TARGETS genericFor DESTINATION lib EXPORT genericIOExports)


