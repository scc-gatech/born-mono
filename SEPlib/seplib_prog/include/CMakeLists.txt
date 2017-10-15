cmake_minimum_required(VERSION 2.8)
project(SEPLIB-prog-include)

set(INCLUDES    sep.startup 
 sep.wrapup sep.main)


install(FILES ${INCLUDES} DESTINATION include)
