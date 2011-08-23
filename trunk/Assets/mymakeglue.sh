#!/bin/sh

# Copy files to new directory, strip comments and add to glue file.

echo "Copying all data files to makegluedir..."

rm -rf makegluedir
mkdir makegluedir
cp * makegluedir
cd makegluedir

echo "Check binaries exist..."
ls -l glue
ls -l makelevelbin
ls -l makeleafbin

echo "Stripping comments..."

. stripcomments.sh

echo "Making all leaves binary..."

. makeallleaves.sh

echo "Making all levels binary..."

rm *.amju2b
for f in *.amju2 
do
    echo "Calling makelevelbin on level file \"$f\""
    ./makelevelbin $f
done

echo "Calling makeglue..."

. makeglue.sh

echo "Copying new data.glue to Data dir..."

rm ../data.glue
mv data.glue ..

echo "Finished!"



