#!/bin/bash
module load compiler/xlc
module load tbb
export CMAKE_PREFIX_PATH=$TBBROOT:/usr/local/lib:$CMAKE_PREFIX_PATH

sudo rm -rf build
mkdir build
cd build
cmake -DgenericIO_DIR=/opt/scc/born/genericIO/lib -DCMAKE_INSTALL_PREFIX=/opt/scc/born/SEPlib -Dhypercube_DIR=/opt/scc/born/hypercube/lib ..
make -j
sudo make install
