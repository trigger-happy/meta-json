#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
. $DIR/default_vars.sh

mkdir -p build && \
    cd build && \
    conan install .. --build=missing && \
    cmake .. -DCMAKE_BUILD_TYPE=Debug -DCLANG_CODE_COVERAGE=ON && \
    make && make test && \
    grcov --llvm . -t coveralls+ --token unused --commit-sha unused > $1/coverage.json
