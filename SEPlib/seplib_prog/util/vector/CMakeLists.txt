project(SEPLIB-prog-util-header)
cmake_minimum_required(VERSION 2.8)

include_directories("${CMAKE_CURRENT_BINARY_DIR}/../../../seplib_base/lib/corelibs")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../seplib_base/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../include")




set(CSEP
Clip.csep  Log.csep  Math_base.csep Solver_ops.csep
)


FOREACH(fort ${FORTRANSEP})
  STRING(REGEX REPLACE ".F90sep\$" ".f90" fortFile "${fort}")
  STRING(REGEX REPLACE ".F90sep\$" "" prog "${fort}")
  set(PROGS "${PROGS} ${prog}")
   ADD_CUSTOM_COMMAND(OUTPUT  "${fortFile}"
     COMMAND  ${CMAKE_CURRENT_SOURCE_DIR}/../../../seplib_base/include/sep_doc_it <${CMAKE_CURRENT_SOURCE_DIR}/${fort} -fortran  >${fortFile}
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
    set_target_properties (${prog} PROPERTIES COMPILE_DEFINITIONS SOURCE="${CMAKE_CURRENT_SOURCE_DIR}/${prog}.c")
    install(TARGETS ${prog} DESTINATION bin)
ENDFOREACH(cprog)


set_property(TARGET Solver_ops PROPERTY LINKER_LANGUAGE Fortran)
TARGET_LINK_LIBRARIES(Solver_ops sepaux sepmath superset sep3d sep)
TARGET_LINK_LIBRARIES(Clip superset sep3d sep m)
TARGET_LINK_LIBRARIES(Log superset sep3d sep m)
TARGET_LINK_LIBRARIES(Math_base superset sep3d sep m)

install(PROGRAMS ${progs} arithpar DESTINATION bin)

if( ${SYMLINK_FOUND} )
   install_symlink(Clip "${CMAKE_INSTALL_PREFIX}/bin/Scale")
   install_symlink(Math_base "${CMAKE_INSTALL_PREFIX}/bin/Cadd")
   install_symlink(Math_base "${CMAKE_INSTALL_PREFIX}/bin/Add")
   install_symlink(Math_base "${CMAKE_INSTALL_PREFIX}/bin/Pow")
   install_symlink(Math_base "${CMAKE_INSTALL_PREFIX}/bin/Gpow")
   install_symlink(Math_base "${CMAKE_INSTALL_PREFIX}/bin/Cabs2")
   install_symlink(Math_base "${CMAKE_INSTALL_PREFIX}/bin/Cabs")
   install_symlink(Math_base "${CMAKE_INSTALL_PREFIX}/bin/Conj")
   install_symlink(Math_base "${CMAKE_INSTALL_PREFIX}/bin/Imag")
   install_symlink(Math_base "${CMAKE_INSTALL_PREFIX}/bin/Real")
   install_symlink(Math_base "${CMAKE_INSTALL_PREFIX}/bin/Rtoc")
   install_symlink(Math_base "${CMAKE_INSTALL_PREFIX}/bin/Cmplx")
endif( ${SYMLINK_FOUND} )

