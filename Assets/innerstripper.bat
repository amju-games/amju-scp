rem strip comments from one file
move %1 %1.tmp
perl stripper.pl %1.tmp > %1
