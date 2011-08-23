#!/bin/sh

# Strip comments out of data files. Requires perl script stripper.pl
for f in *.{amju2,bill,attr,comp,move,text,txt}
do
    echo "Calling innerstripper on file \"$f\""
    . innerstripper.sh $f
done


