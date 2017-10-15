cmake_minimum_required(VERSION 2.8)
project(SEPLIB-python-base-parallel)


set(PYS
distribute_prog.py  par_msgs.py  pf_copy_nfs.py  pf_split.py   pj_split.py
mach_base.py	    pc_base.py	 pf_copy.py	 pj_base.py    sep_socket.py
mach_file.py	    pc_mpich.py  pf_irreg.py	 pj_func.py
mach_list.py	    pc.py	 pf_patch.py	 pj_nfs.py
mach.py		    pf_base.py	 pf_resplit.py	 pj_simple.py
)

install(FILES ${PYS} DESTINATION lib/python/SEP)
