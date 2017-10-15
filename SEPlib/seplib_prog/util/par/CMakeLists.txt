project(SEPLIB-prog-util-header)
cmake_minimum_required(VERSION 2.8)

include_directories("${CMAKE_CURRENT_BINARY_DIR}/../../../seplib_base/lib/corelibs")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../seplib_base/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../include")


set(FORTRANSEP )

set(CSEP
Copy_join.csep	       Join_first.csep	     Send_msg.csep
Copy_split.csep        Patch_join.csep	     Sep3d_split.csep
Distribute_join.csep   Patch_split.csep      Solver_ops_split.csep
Distribute_split.csep  Planewave_split.csep  Split_first.csep
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



TARGET_LINK_LIBRARIES(Copy_join sepaux superset sep3d sep)
TARGET_LINK_LIBRARIES(Copy_split superset sep3d sep)
TARGET_LINK_LIBRARIES(Distribute_join sepaux superset sep3d sep)
TARGET_LINK_LIBRARIES(Distribute_split superset sep3d sep)
TARGET_LINK_LIBRARIES(Join_first sepaux superset sep3d sep)
TARGET_LINK_LIBRARIES(Patch_join superset sep3d sep)
TARGET_LINK_LIBRARIES(Patch_split sepaux superset sep3d sep)
TARGET_LINK_LIBRARIES(Planewave_split superset sep3d sep)
TARGET_LINK_LIBRARIES(Send_msg sepaux superset sep3d sep)
TARGET_LINK_LIBRARIES(Sep3d_split superset sep3d sep)
TARGET_LINK_LIBRARIES(Solver_ops_split sepaux superset sep3d sep)
TARGET_LINK_LIBRARIES(Split_first superset sep3d sep)


install(PROGRAMS ${progs} DESTINATION bin)

