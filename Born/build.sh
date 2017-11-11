#!/bin/bash
export CMAKE_PREFIX_PATH=$TBBROOT:/usr/local/lib:$CMAKE_PREFIX_PATH

sudo rm -rf build
mkdir build
cd build
cmake -GNinja -DgenericIO_DIR=/opt/scc/born/genericIO/lib -DCMAKE_INSTALL_PREFIX=/opt/scc/born/Born2 -Dhypercube_DIR=/opt/scc/born/hypercube/lib ..
ninja
sudo ninja install
