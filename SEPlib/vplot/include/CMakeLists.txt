cmake_minimum_required(VERSION 2.8)
project(SEPLIB-vplot-include)

set(INCLUDES  glplot.h  prototypes.h    vplot.h)


install(FILES ${INCLUDES} DESTINATION include)
