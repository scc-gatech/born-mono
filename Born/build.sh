#!/bin/bash
module load compiler/xlc
module load tbb
module load cmake cuda
export CMAKE_PREFIX_PATH=$TBBROOT:/usr/local/lib:$CMAKE_PREFIX_PATH

sudo rm -rf build
mkdir build
cd build
cmake -DSEPlib_DIR=/opt/scc/born/SEPlib/lib -DgenericIO_DIR=/opt/scc/born/genericIO/lib -DCMAKE_INSTALL_PREFIX=/opt/scc/born/Born2 -Dhypercube_DIR=/opt/scc/born/hypercube/lib ..
make -j
sudo make install
