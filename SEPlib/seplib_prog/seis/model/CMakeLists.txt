project(SEPLIB-prog-seis-filter)
cmake_minimum_required(VERSION 2.8)

include_directories("${CMAKE_CURRENT_BINARY_DIR}/../../../seplib_base/lib/corelibs")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../seplib_base/include")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../include")
include_directories("${CMAKE_CURRENT_BINARY_DIR}/../../../seplib_base/lib/seis/image")

set(FORTRANSEP 
Aniso2d.F90sep	Kirmod3d.F90sep      
Gauss.F90sep	Marine_geom3d.F90sep  Scatter3d.F90sep
Iso2d.F90sep	Planar3d.F90sep       Vel.F90sep
)
if(FFTW_LIBRARIES)
set(FORTRANSEP ${FORTRANSEP} Rtm2d.F90sep)
endif(FFTW_LIBRARIES)

set(CSEP
Fdmod.csep  Spike.csep	Surface.csep  Wavelet.csep
)


FOREACH(fort ${FORTRANSEP})
  STRING(REGEX REPLACE ".F90sep\$" ".f90" fortFile "${fort}")
  STRING(REGEX REPLACE ".F90sep\$" "" prog "${fort}")
  set(PROGS "${PROGS} ${prog}")
   ADD_CUSTOM_COMMAND(OUTPUT  "${fortFile}"
     COMMAND  ${CMAKE_CURRENT_SOURCE_DIR}/../../../seplib_base/include/sep_doc_it <${CMAKE_CURRENT_SOURCE_DIR}/${fort} -fortran   |${CMAKE_CURRENT_SOURCE_DIR}/../../../seplib_base/tools/spp/spp ${FORTRAN_FLAGS}  >${fortFile}
     DEPENDS "${fort}")
    ADD_EXECUTABLE(${prog}  ${prog}.f90)
   install(TARGETS ${prog} DESTINATION bin)
ENDFOREACH(fort)


FOREACH(cprog ${CSEP})
  STRING(REGEX REPLACE ".csep\$" ".c" fortFile "${cprog}")
  STRING(REGEX REPLACE ".csep\$" "" prog "${cprog}")
  set(PROGS "${PROGS} ${prog}")
   ADD_CUSTOM_COMMAND(OUTPUT  "${fortFile}"
     COMMAND  ${CMAKE_CURRENT_SOURCE_DIR}/../../../seplib_base/include/sep_doc_it <${CMAKE_CURRENT_SOURCE_DIR}/${cprog} -C  >${fortFile}
     DEPENDS "${cprog}")
    ADD_EXECUTABLE(${prog}  ${prog}.c)
   install(TARGETS ${prog} DESTINATION bin)
    set_target_properties (${prog} PROPERTIES COMPILE_DEFINITIONS SOURCE="${CMAKE_CURRENT_SOURCE_DIR}/${prog}.c")
ENDFOREACH(cprog)



TARGET_LINK_LIBRARIES(Fdmod  sepcwp superset sep3d sep m)
TARGET_LINK_LIBRARIES(Spike  superset sep3d sep)
TARGET_LINK_LIBRARIES(Surface  superset sep3d sep)
TARGET_LINK_LIBRARIES(Wavelet  sepfft superset sep3d sep m)
TARGET_LINK_LIBRARIES(Aniso2d  superset sep3d sep)
set_property(TARGET Kirmod3d PROPERTY LINKER_LANGUAGE Fortran)
TARGET_LINK_LIBRARIES(Kirmod3d sepimage superset sep3d sep)
TARGET_LINK_LIBRARIES(Gauss  superset sep3d sep)
TARGET_LINK_LIBRARIES(Marine_geom3d  superset sep3d sep)
TARGET_LINK_LIBRARIES(Scatter3d  superset sep3d sep)
TARGET_LINK_LIBRARIES(Iso2d  superset sep3d sep)
TARGET_LINK_LIBRARIES(Planar3d  superset sep3d sep)
TARGET_LINK_LIBRARIES(Vel  superset sep3d sep)
if(FFTW_LIBRARIES)
TARGET_LINK_LIBRARIES(Rtm2d  sepmodel superset sep3d sep ${FFTW_LIBRARIES})
endif(FFTW_LIBRARIES)

install(PROGRAMS ${progs} DESTINATION bin)

