rem strip comments out of data files
rem requires perl
rem requires per script stripper.pl
for %%f in (*.amju2 *.bill *.attr *.comp *.move *.text *.txt) do call innerstripper.bat %%f