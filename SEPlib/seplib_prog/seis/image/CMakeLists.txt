project(SEPLIB-prog-seis-filter)
cmake_minimum_required(VERSION 2.8)

include_directories("${CMAKE_CURRENT_BINARY_DIR}/../../../seplib_base/lib/corelibs")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../seplib_base/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../include")


set(FORTRANSEP 
AMO.F90sep  Lloyd_vel.F90sep  Stolt.F90sep
)

if(FFTW_LIBRARIES)
  set(FORTRANSEP ${FORTRANSEP} CAM.F90sep Phase.F90sep OFF2ANG.F90sep SRM.F90sep)
endif(FFTW_LIBRARIES)

set(CSEP
Kirch_2d_depth.csep
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



TARGET_LINK_LIBRARIES(Kirch_2d_depth sepcwp septravel sepaux sepmath superset sep3d sep m)
set_property(TARGET AMO PROPERTY LINKER_LANGUAGE Fortran)
TARGET_LINK_LIBRARIES(AMO sepimage sepfft seppar superset sep3d sep m)
TARGET_LINK_LIBRARIES(Lloyd_vel  sepwei sepgeef90 seppar superset sep3d sep)
set_property(TARGET Stolt PROPERTY LINKER_LANGUAGE Fortran)
TARGET_LINK_LIBRARIES(Stolt sepimage sepmath sepaux superset sep3d sep m)
if(FFTW_LIBRARIES)
set_property(TARGET SRM PROPERTY LINKER_LANGUAGE Fortran)
TARGET_LINK_LIBRARIES(SRM sepimage sepmath sepaux superset sep3d sep ${FFTW_LIBRARIES} m)
set_property(TARGET OFF2ANG PROPERTY LINKER_LANGUAGE Fortran)
TARGET_LINK_LIBRARIES(OFF2ANG sepimage sepmath sepaux superset sep3d sep ${FFTW_LIBRARIES} m)
TARGET_LINK_LIBRARIES(CAM sepwei sepfft seppar superset  sep3d sep ${FFTW_LIBRARIES} m)
TARGET_LINK_LIBRARIES(Phase sepwei sepfft seppar superset sep3d sep ${FFTW_LIBRARIES} m)
endif(FFTW_LIBRARIES)

install(PROGRAMS ${progs} DESTINATION bin)

