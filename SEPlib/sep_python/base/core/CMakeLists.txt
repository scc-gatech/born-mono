cmake_minimum_required(VERSION 2.8)
project(SEPLIB-python-base-core)


set(PYS args.py      log_base.py  opt_none.py	 prog.py      spawn.py
datapath.py  log.py	   opt_prog.py	 rc.py	      stat_base.py
flow.py       opt_base.py  parameter.py  sep_ff.py   stat_sep.py
__init__.py   opt_data.py  paths.py	 sepfile.py  util.py)

install(FILES ${PYS} DESTINATION lib/python/SEP)
