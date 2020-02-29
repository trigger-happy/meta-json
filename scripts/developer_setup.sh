#!/bin/bash

for i in scripts/git-hooks/* ; do
  filename=`basename $i`
  ln -s -f ../../scripts/git-hooks/$filename .git/hooks/$filename
done
