cmake_minimum_required(VERSION 2.8)
project(SEPLIB-python-base-core)


set(PYS
bandpass.py  dip.py  envelope.py  interp.py  sdip.py  transf.py
)
install(FILES ${PYS} DESTINATION lib/python/SEP)
