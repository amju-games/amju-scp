#!/bin/sh

./glue data.glue

for f in *.{it,wav,ini,bmp,bill,func,lev,lite,md2,menu,move,room,txt,leafb,comp,text,attr,pbil,tex3,amju2b}
do
    echo "Adding \"$f\" to glue archive.."
    ./glue data.glue $f 

done

