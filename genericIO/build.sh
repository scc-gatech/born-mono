#!/bin/bash
module load compiler/xlc

export CMAKE_PREFIX_PATH=/opt/scc/born/hypercube/lib:$CMAKE_PREFIX_PATH
rm -rf build
mkdir build
cd build
cmake  -DCMAKE_INSTALL_PREFIX=/opt/scc/born/genericIO ..
make
sudo make install
