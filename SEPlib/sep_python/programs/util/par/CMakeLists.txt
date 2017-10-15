cmake_minimum_required(VERSION 2.8)
project(SEPLIB-python-base-core)


set(PYS
Parallel
)
install(PROGRAMS ${PYS} DESTINATION bin)
