#!/usr/local/bin/perl
#
#
# Try to read in a config file, and generate a cpp function which 
# populates a map with what's in the config file.
#
# Jason Colman 08 May 2002
#

# There's no main(), we just leap straight in to the code...
#
#


# Attempt to open the file "Game.cfg".
# TODO use argument to get filename.

open (CONFIGFILE, "../../Data/game.cfg") or die "I couldn't open Game.cfg"; 

# Write the top of the .cpp file.
# (We simply use print, so redirect this to a .cpp file.)

print "\/\/ Automatically generated from config file.\n\n";
print "#include \"Engine.h\"\n\n";
print "namespace Amju\n{\nvoid ConfigHack()\n{\n";


# This means "for each line in the file.."
for $line (<CONFIGFILE>) 
{ 
  #print $line; 

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

  # Now we are using a regexp to test for blank lines. "$" means 
  # "end of line", "\s" means "whitespace character", and 
  # "*" means "zero or more", so this means 
  # "if the line consists of zero or more whitespace characters".
  elsif ($line =~ /^\s*$/)
  {
	#print "Empty line.\n";
  }

  # Test for a single integer. This would be the config file version
  # number. Use a regexp again.. this one means "one or more digits",
  # "\d" meaning "digit".
  # Then allow zero or more whitespace, then the end of the line.

  elsif ($line =~ /^\d+\s*$/)
  {
	#print "Config file version ", $line, "\n";
  }
  else
  {

    # Split line into its key and value tokens.
	# Ok. "@" means "array", so @tokens is an array of tokens in the line.
	# split() is a built-in function which splits strings based on a 
	# regexp. This regexp means "one or more spaces", the effect being
	# to get the key, at the beginning of the line, and the value, which
	# is separated from the key by some spaces.

    @tokens = split(/\s+/, $line);

    #print @tokens[0], "\n";
    #print @tokens[1], "\n";

    #print "  (*pMap)[\"", @tokens[0], "\"] = \"", @tokens[1], "\";\n";

    # New hack, 01 Oct 2005
    # We ALWAYS want to use the glue file, unless we have explicitly turned
    # this off in game.cfg - but that is always for debugging only.
    # So if the key is "glue", always set the value to "y".
    # This saves us having to remember to check that the value of "glue" is
    # set to "y" in game.cfg every time we do a release build.
    if ($tokens[0] eq "glue")
    {
        $tokens[1] = "y";
    }

    # Set config values in Engine.
    print "  Engine::Instance()->SetConfigValue(\"", 
		  $tokens[0],
		  "\", \"", 
		  $tokens[1], 
		  "\");\n";
  }
} 
close CONFIGFILE;

# Write the final bit at the end of the .cpp file.
# We want to close the function and the namespace.

print "}\n}\n\n";


######## End ########

