#!/bin/bash
module load compiler/xlc
module load tbb
export CMAKE_PREFIX_PATH=$TBBROOT:/usr/local/lib:$CMAKE_PREFIX_PATH
sudo mkdir -p /opt/scc/born/SEPlib/include
sudo rm -rf build
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=/opt/scc/born/SEPlib ..
sudo make -j
sudo make install
