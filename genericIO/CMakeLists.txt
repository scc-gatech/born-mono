project(Generic-IO)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

# configure a header file to pass some of the CMake settings

set(genericIO_MAJOR_VERSION 0)
set(genericIO_MINOR_VERSION 2)
set(genericIO_VERSION_STRING
  ${genericIO_MAJOR_VERSION}.${genericIO_MINOR_VERSION})

# suppress warnings... sorry Bob!
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_BINARY_DIR}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -pg")

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

enable_language(C CXX)


FIND_PACKAGE(hypercube NO_MODULE REQUIRED)
INCLUDE_DIRECTORIES(${hypercube_DIRNAME_include})


FIND_PACKAGE(SEPlib NO_MODULE)

#find_path(SEPINCDIR seplib.h /usr/local/SEP/include /usr/local/SEP/SEP8.1/include /opt/SEP/include /opt/SEP/SEP8.1/include)
#find_library(LIBSEP sep /usr/local/SEP/lib /usr/local/SEP/SEP8.1/lib /opt/SEP/lib /opt/SEP/SEP8.1/lib)
#find_library(LIBSEP3D sep3d /usr/local/SEP/lib /usr/local/SEP/SEP8.1/lib /opt/SEP/lib /opt/SEP/SEP8.1/lib)
find_path(RSFINCDIR rsf.h ${RSF_DIR}/include )



if (RSFINCDIR)
  MESSAGE("INSTALLING WITH RSF")
  set(USE_RSF true)
  STRING(REGEX REPLACE "include\$" "lib" luse "${RSFINCDIR}")
  set(GENERIC_EXPORT_LIBS "${GENERIC_EXPORT_LIBS};rsf")
  if(DEFINED GENERIC_EXPORT_INC_DIRS)
    set(GENERIC_EXPORT_INC_DIRS "${GENERIC_EXPORT_INC_DIRS};${RSFINCDIR}")
    set(GENERIC_EXPORT_LIB_DIRS "${GENERIC_EXPORT_LIB_DIRS};${luse}")
   else()
    set(GENERIC_EXPORT_INC_DIRS "${RSFINCDIR}")
    set(GENERIC_EXPORT_LIB_DIRS "${luse}")
   endif()
else()
  MESSAGE("NOT INSTALLING WITH RSF")
endif()

#!if (SEPINCDIR AND LIBSEP AND LIBSEP3D)

if (SEPlib_FOUND)
  MESSAGE("INSTALLING WITH SEPlib")
  set(USE_SEP true)
  set(GENERIC_EXPORT_LIBS "${GENERIC_EXPORT_LIBS};sep3d;sep ")
  if(DEFINED GENERIC_EXPORT_INC_DIRS)
    set(GENERIC_EXPORT_INC_DIRS "${GENERIC_EXPORT_INC_DIRS};${SEPlib_DIRNAME_include}")
    set(GENERIC_EXPORT_LIB_DIRS "${GENERIC_EXPORT_LIB_DIRS};${SEPlib_DIRNAME_lib}")
   else()
     set(GENERIC_EXPORT_INC_DIRS "${SEPlib_DIRNAME_include}")
     set(GENERIC_EXPORT_LIB_DIRS "${SEPlib_DIRNAME_lib}")
   endif()
else()
  MESSAGE("NOT INSTALLING WITH SEPlib")
endif()

string(STRIP "${GENERIC_EXPORT_LIBS}" GENERIC_EXPORT_LIBS)
if(USE_SEP)
  set(DEFAULTIO "SEP")
elseif(USE_RSF)
    set(DEFAULTIO "RSF")
else(USE_SEP)
    set(DEFAULTIO "JSON")
endif(USE_SEP)
include_directories(${PROJECT_BINARY_DIR})

configure_file (
  "${PROJECT_SOURCE_DIR}/ioConfig.h.in"
  "${PROJECT_BINARY_DIR}/ioConfig.h"
  )
 SET(genericIO_DIRNAME_lib     "lib")
 SET(genericIO_DIRNAME_include "include")
  INCLUDE(CMakePackageConfigHelpers)

  CONFIGURE_PACKAGE_CONFIG_FILE(
  "${CMAKE_CURRENT_SOURCE_DIR}/genericIOConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/genericIOConfig.cmake"
  INSTALL_DESTINATION
    ${genericIO_DIRNAME_lib}
  PATH_VARS
    genericIO_DIRNAME_include
  )
  
  INSTALL(
  FILES
    ${CMAKE_CURRENT_BINARY_DIR}/genericIOConfig.cmake
  DESTINATION
    lib
  )


  INSTALL(
  EXPORT
    genericIOExports
  DESTINATION
    ${genericIO_DIRNAME_lib}
  )
  
add_subdirectory(cpp)


option(BUILD_FORTRAN "Build fortran support" OFF)

if(BUILD_FORTRAN)
  enable_language(FORTRAN)
  add_subdirectory(fortran)
endif(BUILD_FORTRAN)

add_subdirectory(util)
install(FILES ${PROJECT_BINARY_DIR}/ioConfig.h  DESTINATION include)
