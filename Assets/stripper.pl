
# 
# stripper.pl
# strip comment lines out of the specified text file.
# Comment lines begin with "//".
#
#

# Get argument
#filename = $ARGV[0];
#print "Filename: ", $ARGV[0], "\n";
open (TEXTFILE, $ARGV[0]) or die;


# This means "for each line in the file.."
for $line (<TEXTFILE>) 
{ 
  # Test for comment line. These begin with 2 forward slashes.
  # Unfortunately forward slash is used to delimit regexps, so this test
  # looks a bit yucky..
  # (Ok, you've totally forgotten what this means.
  # =~ means "matches regular expression"
  # the expression is delimited by "/". "^" means "at the beginning",
  # so this line means "if $line begins with "//"...)

  if ($line =~ /^\/\//)
  {
	#print "That looks like a comment to me.\n";
  }
  else
  {
    print $line;
  }
}
close TEXTFILE;

## END ##