rem makeglue.bat
rem glue every file (which we want) in this directory into a Glue file.

rem make all leaves binary
rem call makeallleaves.bat

rem make glue file
glue data.glue

rem for each file, add to glue file.
for %%f in (*.it *.wav *.ini *.bmp *.bill *.func *.lev *.lite *.md2 *.menu *.move *.room *.txt *.leafb *.comp *.text *.attr *.pbil *.tex3 *.amju2b) do glue data.glue %%f