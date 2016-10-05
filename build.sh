#!/bin/bash

# Do not move build.sh. You can execute it from anywhere though.
cd $(dirname $0)

SRC_PATH=$(pwd)
BUILD_PATH="$SRC_PATH/build"
EXE_NAME="progtran"

mkdir "$BUILD_PATH"
cd "$BUILD_PATH"

#TODO: add any possible cmake variables here
cmake "$SRC_PATH" -DCMAKE_BUILD_TYPE=DEBUG

make -j $(nproc)
chmod a+x "$EXE_NAME"
cp "$EXE_NAME" ..
