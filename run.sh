#!/bin/bash

mkdir build
cd build
cmake ..
make

if [[ $? != 0 ]];
then
    exit 1
fi
   

cd ..
./build/progtran

echo ""
echo "Running compiled program:"
echo ""

cd output
./run_output.sh
