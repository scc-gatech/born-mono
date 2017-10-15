cmake_minimum_required(VERSION 2.8)
project(SEPLIB-python-base-inversion)


set(PYS 
cgstep.py	    op_oc_serial.py	pv_split.py    vec_base.py
op_base.py	    op_scale.py		solv_base.py   vec_oc.py
op_combo.py	    pv_always_split.py	solv_prec.py   vec_scmplx.py
op_oc_par.py	    pv_copy_nfs.py	solv_reg.py    vec_sep.py
op_oc_par_split.py  pv_copy.py		solv_shape.py  vec_sfloat.py
op_oc.py	    pv_func.py		solv_smp.py    vec_super.py
)

install(FILES ${PYS} DESTINATION lib/python/SEP)
