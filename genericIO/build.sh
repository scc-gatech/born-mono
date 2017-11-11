#!/bin/bash

export CMAKE_PREFIX_PATH=/opt/scc/born/hypercube/lib:$CMAKE_PREFIX_PATH
rm -rf build
mkdir build
cd build
cmake -GNinja  -DCMAKE_INSTALL_PREFIX=/opt/scc/born/genericIO ..
ninja
sudo ninja install
