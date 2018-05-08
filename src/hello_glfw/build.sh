#!/bin/bash

set -e

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
pushd "$DIR" #> /dev/null
mkdir -p ./build
pushd ./build #> /dev/null
c++ -std=c++11 -o hello_unix.exe ../hello_unix.cpp -I/usr/local/include -I/opt/X11/include -g `pkg-config --cflags glew glfw3` `pkg-config --libs glew glfw3`