#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
. $DIR/default_vars.sh

mkdir -p build/docs
docker run --rm -it -v `pwd`/docs:/src -v `pwd`/build/docs:/build triggerhappy/slatedocs:latest

echo "Documentation may be found in the build/docs" directory
