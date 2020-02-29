#!/bin/bash

if [ -z "$BUILD_TYPE" ]; then
  export BUILD_TYPE=Debug
fi

if [ -z "$CC" ]; then
  export CC=clang
fi

if [ -z "$CXX" ]; then
  export CXX=clang++
fi
