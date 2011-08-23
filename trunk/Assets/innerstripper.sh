#!/bin/sh

# Parameter: file name to strip
# Rename the given file to a temp file name
mv $1 $1.tmp
# Strip comments from the temp file, giving the original file name 
# as the output file.
perl stripper.pl $1.tmp > $1
rm $1.tmp

