#!/usr/bin/env bash

source /opt/intel/bin/compilervars.sh intel64
export CC=icc
export CXX=icpc

cd hypercube && ./build.sh && cd ..
cd genericIO && ./build.sh && cd ..
cd Born && ./build.sh && cd ..
