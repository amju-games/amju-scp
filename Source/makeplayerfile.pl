#!/usr/bin/perl -w
use strict;

my $outfile = 
#"hello.txt"; 
    "/Users/Shared/Amju Super Cool Pool/abcdefghijklmnopqrstuvwxyz1234567890.cfg";


open(MYFILE, ">$outfile") or die ("Oh no, failed to open file!");


print MYFILE "// Hello, generated by script\n";
print MYFILE "// Version\n11\n";
print MYFILE "// Num games\n0\n";
print MYFILE "// Num bonus types\n2\n";
print MYFILE "// Type\n1\n// Num\n1000\n";
print MYFILE "// Type\n2\n// Num\n1000\n";
print MYFILE "// Num solved rooms\n0\n";
print MYFILE "// Checksum\n0\n";


close (MYFILE);

