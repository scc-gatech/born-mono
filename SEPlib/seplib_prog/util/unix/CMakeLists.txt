project(SEPLIB-prog-util-header)
cmake_minimum_required(VERSION 2.8)

include_directories("${CMAKE_CURRENT_BINARY_DIR}/../../../seplib_base/lib/corelibs")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../seplib_base/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../include")

set(FORTRANSEP  )

set(CSEP
Scp3d.csep)



FOREACH(fort ${FORTRANSEP})
  STRING(REGEX REPLACE ".F90sep\$" ".f90" fortFile "${fort}")
  STRING(REGEX REPLACE ".F90sep\$" "" prog "${fort}")
  set(PROGS "${PROGS} ${prog}")
   ADD_CUSTOM_COMMAND(OUTPUT  "${fortFile}"
     COMMAND  ${CMAKE_CURRENT_SOURCE_DIR}/../../../seplib_base/include/sep_doc_it <${CMAKE_CURRENT_SOURCE_DIR}/${fort} -fortran  >${fortFile}
     DEPENDS "${fort}"
  install(TARGETS ${prog} DESTINATION bin)
     )
ENDFOREACH(fort)


FOREACH(cprog ${CSEP})
  STRING(REGEX REPLACE ".csep\$" ".c" fortFile "${cprog}")
  STRING(REGEX REPLACE ".csep\$" "" prog "${cprog}")
  set(PROGS "${PROGS} ${prog}")
   ADD_CUSTOM_COMMAND(OUTPUT  "${fortFile}"
     COMMAND  ${CMAKE_CURRENT_SOURCE_DIR}/../../../seplib_base/include/sep_doc_it <${CMAKE_CURRENT_SOURCE_DIR}/${cprog} -C  >${fortFile}
     DEPENDS "${cprog}")
    ADD_EXECUTABLE(${prog}  ${prog}.c)
    set_target_properties (${prog} PROPERTIES COMPILE_DEFINITIONS SOURCE="${CMAKE_CURRENT_SOURCE_DIR}/${prog}.c")
  install(TARGETS ${prog} DESTINATION bin)
ENDFOREACH(cprog)

TARGET_LINK_LIBRARIES(Scp3d superset sep3d sep)


set(SCRIPTS Sls	 Smv3d	    Srm3d     Zero)
FOREACH(prg ${SCRIPTS})
   configure_file (
  "${PROJECT_SOURCE_DIR}/${prg}"
  "${PROJECT_BINARY_DIR}/${prg}"
  )
   install(PROGRAMS  ${prg} DESTINATION bin)
ENDFOREACH(prg)

install(PROGRAMS ${progs} DESTINATION bin)

if( ${SYMLINK_FOUND} )
   install_symlink( "Srm3d" "${CMAKE_INSTALL_PREFIX}/bin/Rm3d" )
   install_symlink( "Scp3d" "${CMAKE_INSTALL_PREFIX}/bin/Cp3d" )
   if(NOT CMAKE_HOST_APPLE)
   if(NOT APPLE)
      install_symlink( "Srm3d" "${CMAKE_INSTALL_PREFIX}/bin/Rm" )
      install_symlink( "Scp3d" "${CMAKE_INSTALL_PREFIX}/bin/Cp" )
      install_symlink( "Sls" "${CMAKE_INSTALL_PREFIX}/bin/Ls" )
   endif(NOT APPLE)
   endif(NOT CMAKE_HOST_APPLE)
endif( ${SYMLINK_FOUND} )


