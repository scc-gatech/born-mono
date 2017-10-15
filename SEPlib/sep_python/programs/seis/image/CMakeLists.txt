cmake_minimum_required(VERSION 2.8)
project(SEPLIB-python-base-core)


set(PYS
mig_off_nfs  model_off
mig_off	  mig_ph       OFF2ANG_Par
)

install(PROGRAMS ${PYS} DESTINATION bin)
