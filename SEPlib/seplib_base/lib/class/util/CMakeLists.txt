cmake_minimum_required(VERSION 2.8)
project(SEPLIB-base-lib-class-util)


include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR}/../../corelibs/sep)
include_directories(/usr/include)
include_directories(/usr/local/include)
IF(${FFTW_FOUND})
include_directories(${FFTW_INCLUDES})
endif(${FFTW_FOUND})

set (SOURCES 
adjnull.f90    contran.f90  fth.f90	  null.f90	spot0.f90
boxconv.f90    copy.f90     	  pad2.f90	spot1.f90
boxmo.f90      ctris.f90      quantile.f90	stack1.f90
cadd.f90       diag.f90     ident.f90	  rand01.f90	triangle2.f90
causint.f90    ft1axis.f90  igrad1.f90	  ruffen1.f90	triangle.f90
cinterp1.f90   ft2axis.f90  matmult.f90   signum.f90	zpad1.f90
classutil.f90  ft3D.f90     nmo1.f90	  simpleft.f90)
IF(${FFTW_FOUND})
set (SOURCES ${SOURCES} ftu.f90 halfdifa.f90)
endif(${FFTW_FOUND})



add_library(sepclassutil ${SOURCES})
target_link_libraries(sepclassutil ${FFTW_LIBRARIES})
install(TARGETS sepclassutil DESTINATION lib)
