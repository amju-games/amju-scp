/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: StringUtils.h,v $
Revision 1.1.10.2  2006/07/27 08:43:52  jay
Add ToHexString()

Revision 1.1.10.1  2005/05/09 05:54:20  jay
Added ToString(float) with parameter for no of decimal digits

Revision 1.1  2004/09/08 15:43:20  jay
Added to repository
  
*/

#if !defined(STRING_UTILS_H_INCLUDED)
#define STRING_UTILS_H_INCLUDED

#include <string>
#include <vector>

namespace Amju
{
// Helper: strips off any path info from a filename.
std::string StripPath(const std::string& s);
std::string StripPath(const std::string& s, const std::string& slashChar);

// Helper: trim spaces off beginning and end of string.
void Trim(std::string* pStr);

// Get the extension (letters following final dot) of a filename.
std::string GetFileExt(const std::string& filename);

// Get the filename _without_ the final dot and extension.
std::string GetFileNoExt(const std::string& f);

// Get only the path, i.e. remove the final filename from the string.
std::string GetFilePath(const std::string& filename);
std::string GetFilePath(const std::string& filename, const std::string& slashChar);

// Convert string to lower case.
std::string ToLower(const std::string& s);

// Remove character c from the string.
std::string Remove(const std::string& s, char c);

// Convert int to string
std::string ToString(int i);
std::string ToString(unsigned int i);

// Convert int to hex string
std::string ToHexString(unsigned int i);

// Convert float to string
std::string ToString(float f);
// This overload lets you set the number of digits after the decimal point.
std::string ToString(float f, int digits);

// Returns a string in which replaceThis is replaced with replaceWithThis.
// Known bug: replaceWithThis must NOT contain the string replaceThis.
// This condition causes an infinite loop, until the string is so big you 
// run out of memory.
std::string Replace(
  const std::string& s, 
  const std::string& replaceThis, 
  const std::string& replaceWithThis);

// Returns true if container string contains substr.
bool StringContains(const std::string& container, const std::string& substr);

// Split a string into a vector of substrings. A new substring is created
// before and after a special character in the text.
std::vector<std::string> Split(const std::string& text, char split_char);
}

#endif

