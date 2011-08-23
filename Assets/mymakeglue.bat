rem makeglue with comment stripper
rem strip comments from copies of files, not the original files.

mkdir makegluedir
del /Q makegluedir\*.*
copy *.* makegluedir
cd makegluedir

rem strip comments out of text-based data files
call stripcomments.bat

rem Make binary version of every leaf
call makeallleaves.bat

rem Make binary file from each level text file
del *.amju2b
for %%f in (*.amju2) do makelevelbin %%f

call makeglue.bat

del ..\data.glue
move data.glue ..

