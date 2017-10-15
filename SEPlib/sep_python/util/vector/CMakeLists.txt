cmake_minimum_required(VERSION 2.8)
project(SEPLIB-python-base-core)


set(PYS
math_base.py
)
install(FILES ${PYS} DESTINATION lib/python/SEP)
