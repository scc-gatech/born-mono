cmake_minimum_required(VERSION 2.8)
project(SEPLIB-base-lib-class-bei)


include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR}/../../corelibs/sep)


set (SOURCES 
aamig.f90     flathyp.f90    matmul.f90    scale.f90	  tristack.f90
bei.f90       ft1axe.f90     mutter1.f90   slowfit.f90	  velsimp.f90
causint2.f90  gazadj.f90     mutter.f90    spotw.f90	  veltran.f90
cosmig.f90    kdmoslow.f90   nmo0.f90	   stack0.f90	  vint2rms.f90
dmokirch.f90  kirchfast.f90  phasemig.f90  stoltmig.f90   wavemovie.f90
doubint.f90   kirchslow.f90  phasemod.f90  stoltmod.f90   zero.f90
dpbin2.f90    lint1.f90      rms2int.f90   synmarine.f90
eiktau.f90    lmo.f90	     rtris.f90	   trimo.f90)

add_library(sepbei ${SOURCES})
install(TARGETS sepbei DESTINATION lib)
