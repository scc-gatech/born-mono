#!/bin/bash
module load compiler/xlc
sudo rm -rf build
mkdir build

cd build
cmake -DCMAKE_INSTALL_PREFIX=/opt/scc/born/hypercube ..

make 
sudo make install
