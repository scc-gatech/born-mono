cmake_minimum_required(VERSION 2.8)
project(SEPLIB-python-base-core)


set(PYS
build.py  make.py  tour.py
)
install(FILES ${PYS} DESTINATION lib/python/SEP)
