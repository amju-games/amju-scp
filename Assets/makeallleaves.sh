#!/bin/sh
# Convert text-format leaf files to binary format
for f in *.leaf
do
    echo "Make leaf binary: \"$f\""
    ./makeleafbin $f
done


