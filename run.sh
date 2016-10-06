#!/bin/bash

mkdir build
cd build
cmake ..
make

./progtrantest
cd ..
./build/progtran
