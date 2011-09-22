export BMP2PNG=../../amjulib/Build/bmp2png/Mac/bmp2png

for f in *.bmp
do
    echo "Converting this bmp file: " $f
    $BMP2PNG $f
done
 

