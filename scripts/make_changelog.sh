#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
. $DIR/default_vars.sh

# Check if a commit hash was provided, it will be used as the starting commit
# for auto-changelog
AUTO_CHANGELOG_ARGS=""
if [ ! -z $1 ]; then
  AUTOCHANGELOG_ARGS="--starting-commit=$1"
fi

docker run -v `pwd`:/src --rm -i triggerhappy/auto-changelog:latest auto-changelog $AUTOCHANGELOG_ARGS
