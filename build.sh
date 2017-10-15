#!/usr/bin/env bash

module load cuda
module load cmake

cd SEPlib && ./build.sh && cd ..
cd hypercube && ./build.sh && cd ..
cd genericIO && ./build.sh && cd ..
cd Born && ./build.sh && cd ..
