cmake_minimum_required(VERSION 2.8)
project(SEPLIB-python-base-core)


set(PYS
sort3d.py  stack.py  transp.py	window.py
)
install(FILES ${PYS} DESTINATION lib/python/SEP)
