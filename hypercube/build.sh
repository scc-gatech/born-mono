#!/bin/bash
sudo rm -rf build
mkdir build

cd build
cmake -GNinja -DCMAKE_INSTALL_PREFIX=/opt/scc/born/hypercube ..

ninja
sudo ninja install
