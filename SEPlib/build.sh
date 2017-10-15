#!/usr/bin/env bash

mkdir build && cd build
cmake ..
#TODO where is this installed, and when can we remove the build directory. 
cd .. && rm -rf build
