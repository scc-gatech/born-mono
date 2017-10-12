#----------------------------------------------------------------
# Generated CMake target import file for configuration "".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "hypercube" for configuration ""
set_property(TARGET hypercube APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(hypercube PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "CXX"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libhypercube.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS hypercube )
list(APPEND _IMPORT_CHECK_FILES_FOR_hypercube "${_IMPORT_PREFIX}/lib/libhypercube.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
