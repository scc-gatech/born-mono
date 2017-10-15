project(SEPLIB-prog-converters-csv)
cmake_minimum_required(VERSION 2.8)

include_directories("${CMAKE_CURRENT_BINARY_DIR}/../../../seplib_base/lib/corelibs")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../seplib_base/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../..")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../include")

set(FORTRANSEP 
)

set(CSRCS
Sep2Csv.c Csv2Sep.c
)


FOREACH(fort ${FORTRANSEP})
  STRING(REGEX REPLACE ".F90sep\$" ".f90" fortFile "${fort}")
  STRING(REGEX REPLACE ".F90sep\$" "" prog "${fort}")
  set(PROGS "${PROGS} ${prog}")
   ADD_CUSTOM_COMMAND(OUTPUT  "${fortFile}"
     COMMAND  ${CMAKE_CURRENT_SOURCE_DIR}/../../../seplib_base/include/sep_doc_it <${CMAKE_CURRENT_SOURCE_DIR}/${fort} -fortran   |${CMAKE_CURRENT_SOURCE_DIR}/../../../seplib_base/tools/spp/spp ${FORTRAN_FLAGS}  >${fortFile}
     DEPENDS "${fort}")
    ADD_EXECUTABLE(${prog}  ${prog}.f90)
ENDFOREACH(fort)


FOREACH(cprog ${CSRCS})
  STRING(REGEX REPLACE ".c\$" "" prog "${cprog}")
  set(PROGS "${PROGS} ${prog}")
    ADD_EXECUTABLE(${prog}  ${prog}.c)
    set_target_properties (${prog} PROPERTIES COMPILE_DEFINITIONS SOURCE="${CMAKE_CURRENT_SOURCE_DIR}/${prog}.c")
  install(TARGETS ${prog} DESTINATION bin)
ENDFOREACH(cprog)

TARGET_LINK_LIBRARIES(Csv2Sep  superset   sep3d sep)
TARGET_LINK_LIBRARIES(Sep2Csv  superset  sep3d sep)

install(FILES ${progs} DESTINATION bin)

