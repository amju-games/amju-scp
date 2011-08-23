rem ensure there is a newline at the end of every text file.
rem otherwise glue files don't work properly.
rem only needs to be called once.

for %%f in (*.ini *.bill *.func *.lev *.lite *.menu *.move *.room *.txt *.leaf *.comp *.text *.attr *.pbil *.tex3) do copy /B %%f + /B ..\nl.txt %%f /B

rem get rid of DOS end-of-line characters for good measure - requires MINGW
for %%f in (*.ini *.bill *.func *.lev *.lite *.menu *.move *.room *.txt *.leaf *.comp *.text *.attr *.pbil *.tex3) do dos2unix %%f

