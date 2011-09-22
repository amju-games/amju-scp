export LEAF2OBJ=../../amjulib/Build/leaf2obj/Mac/leaf2obj

for f in *.leaf
do
    echo "Converting this leaf file: " $f
    $LEAF2OBJ $f
done
 

