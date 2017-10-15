cmake_minimum_required(VERSION 2.8)
project(SEPLIB-base-lib-seis-image)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../include)

if(FFTW_LIBRARIES)
set (sppflags "-DUSE_FFTW" )
endif()

set (SOURCES 
fftwflags.f90  srm_storm.f90 amo.f90
kirchfast.f90  omppar.f90 kirmod_mod.f90)

set (FORTRANS
adcig.f90s odcig.f90s)

FOREACH (fort ${FORTRANS})
  STRING(REGEX REPLACE ".f90s\$" ".f90" fortFile "${fort}")
  ADD_CUSTOM_COMMAND(OUTPUT "${fortFile}"
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/../../../tools/spp/spp <${CMAKE_CURRENT_SOURCE_DIR}/${fort} ${sppflags} >${fortFile}
    DEPENDS "${fort}" )
  set (SOURCES ${SOURCES} ${fortFile})
ENDFOREACH(fort)

add_library(sepimage ${SOURCES})

if(FFTW_LIBRARIES)
target_link_libraries(sepimage sepfft sepmath sepvector sep ${FFTW_LIBRARIES} m)
else()
target_link_libraries(sepimage sepfft sepmath sepvector sep m)
endif()

install(TARGETS sepimage DESTINATION lib)
