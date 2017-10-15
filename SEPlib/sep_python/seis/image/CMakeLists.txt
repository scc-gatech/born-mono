cmake_minimum_required(VERSION 2.8)
project(SEPLIB-python-base-core)


set(PYS
lloyd.py    wei_ang_mig.py    wei_ang_op.py  wei_off_mig.py    wei_op.py
off2ang.py  wei_ang_model.py  wei_ang.py     wei_off_model.py  wei.py
)
install(FILES ${PYS} DESTINATION lib/python/SEP)
