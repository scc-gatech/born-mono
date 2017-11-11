project(hypercube)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

# configure a header file to pass some of the CMake settings

set(hypercube_MAJOR_VERSION 0)
set(hypercube_MINOR_VERSION 0)
set(hypercube_VERSION_STRING
  ${hypercube_MAJOR_VERSION}.${hypercube_MINOR_VERSION})

# suppress warnings... sorry Bob!
set(CMAKE_BUILD_TYPE Release)
# suppress warnings... sorry Bob!
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ip -ipo")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -qopt-report=1 -qopt-report-phase=vec")

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_BINARY_DIR}")


include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

enable_language(CXX)


 SET(hypercube_DIRNAME_lib     "lib")
 SET(hypercube_DIRNAME_include "include")
INCLUDE(CMakePackageConfigHelpers)
  CONFIGURE_PACKAGE_CONFIG_FILE(
  "${CMAKE_CURRENT_SOURCE_DIR}/hypercubeConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/hypercubeConfig.cmake"
  INSTALL_DESTINATION
    ${hypercube_DIRNAME_lib}
  PATH_VARS
    hypercube_DIRNAME_include
    hypercube_DIRNAME_lib
  )


  
  INSTALL(
  FILES
    ${CMAKE_CURRENT_BINARY_DIR}/hypercubeConfig.cmake
  DESTINATION
    lib
  )


  INSTALL(
  EXPORT
   hypercubeExports
  DESTINATION
    ${hypercube_DIRNAME_lib}
  )
  
add_subdirectory(lib)

