project(SEPLIB-java-include)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

configure_file (
  "${PROJECT_SOURCE_DIR}/fullpath.csh.in"
  "${PROJECT_BINARY_DIR}/fullpath"
)
