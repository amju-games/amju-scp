/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: StringUtils.cpp,v $
Revision 1.1.10.4  2009/07/22 12:44:26  jay
Make ASCP freeware, so disable Registration code.

Revision 1.1.10.3  2007/12/10 22:54:31  jay
Float to string: if value is zero, just return "0"

Revision 1.1.10.2  2006/07/27 08:43:52  jay
Add ToHexString()

Revision 1.1.10.1  2005/05/09 05:54:20  jay
Added ToString(float) with parameter for no of decimal digits

Revision 1.1  2004/09/08 15:43:20  jay
Added to repository
  
*/

#include <algorithm> // remove()
#include <iostream>
#include "StringUtils.h"
#include "SchAssert.h"

namespace Amju
{
bool StringContains(const std::string& container, const std::string& substr)
{
  unsigned int f = container.find(substr);
  return (f != std::string::npos);
}

std::string StripPath(const std::string& s, const std::string& slashChar)
{
  std::string ret = s;
  unsigned int f = ret.find_last_of(slashChar);
  if (f != std::string::npos)
  {
    ret = ret.substr(f + 1);
  }
  return ret;
}

std::string StripPath(const std::string& s)
{
  // Return string, but minus any path info. 
  // To do this, we search from the end of the string for the path separator
  // character. If we find this character, we strip it and anything before it.
/*
#if defined(WIN32)
  const std::string slash = "\\"; 
#else
  const std::string slash = "/"; 
#endif
  return StripPath(s, slash);
*/
  // Strip backslash and forward slash characters.
  return StripPath(StripPath(s, "/"), "\\");
}

std::string GetFilePath(const std::string& filename)
{
#if defined(WIN32)
  const std::string slash = "\\"; 
#else
  const std::string slash = "/"; 
#endif
  return GetFilePath(filename, slash);
}

std::string GetFilePath(const std::string& s, const std::string& slashChar)
{
  std::string ret = s;
  unsigned int f = ret.find_last_of(slashChar);
  if (f != std::string::npos)
  {
    ret = ret.substr(0, f);
  }
  return ret;
}

void Trim(std::string* ps)
{
  Assert(ps);
  while (!ps->empty() && ps->at(0) == ' ')
  {
    *ps = ps->substr(1);
  }
  while (!ps->empty() && ps->at(ps->size() - 1) == ' ')
  {
    *ps = ps->substr(0, ps->size() - 1);
  }
}

std::string GetFileExt(const std::string& f)
{
  unsigned int pos = f.rfind(".");

  if (pos != std::string::npos)
  {
    return f.substr(pos + 1);
  }
  return "";
}

std::string GetFileNoExt(const std::string& f)
{
  // Return all up to but not including the final dot.
  unsigned int pos = f.rfind(".");

  if (pos != std::string::npos)
  {
    return f.substr(0, pos);
  }
  return f;
}

std::string ToLower(const std::string& s)
{
  std::string result;
  for (unsigned int i = 0; i < s.size(); i++)
  {
#if defined(MACOSX) || defined(MINGW)
    result += tolower(int(s[i]));
#else
    result += tolower(unsigned char(s[i]));
#endif
  }
  return result;
}

std::string Remove(const std::string& s, char c)
{
  std::string r(s);
  r.erase(std::remove(r.begin(), r.end(), c), r.end());
  return r;
}

std::string ToHexString(unsigned int i)
{
  char buf[50];
  sprintf(buf, "%x", i);
  return buf;
}

std::string ToString(unsigned int i)
{
  char buf[50];
  sprintf(buf, "%u", i);
  return buf;
}

std::string ToString(int i)
{
  char buf[50];
  sprintf(buf, "%d", i);
  return buf;
}

std::string ToString(float f)
{
  if (f == 0)
  {
    return "0";
  }

  char buf[50];
  sprintf(buf, "%f", f);
  return buf;
}

std::string ToString(float f, int digits)
{
  if (f == 0)
  {
    return "0";
  }

  char buf[50];
  std::string s = "%." + ToString(digits) + "f";
  sprintf(buf, s.c_str(), f);
  return buf;
}

std::string Replace(
  const std::string& s, 
  const std::string& replaceThis, 
  const std::string& replaceWithThis)
{
  std::string r(s);
  int j;
  int length = replaceThis.length();

  while ((j = r.find(replaceThis)) != std::string::npos)
  {
    r.replace(j, length, replaceWithThis);
  }
  return r;
}

std::vector<std::string> Split(const std::string& cs, char c)
{
  std::vector<std::string> r;
  std::string s(cs);
  while (true) 
  {
    int i = s.find(c);
    if (i == std::string::npos)
    {
      // No special character found, so push back the entire string and finish.
      r.push_back(s);
#ifdef _DEBUG
      std::cout << "Split: final string: " << s.c_str() << "\n";
#endif
      break; 
    }    
    else
    {
      // Found the special character. 
      // Push back whatever is before the character, then work on the remainder
      // of the string.
#ifdef _DEBUG
      std::cout << "Split: string: " << s.substr(0, i).c_str() << " rem: " << s.substr(i + 1).c_str() << "\n";
#endif 
      r.push_back(s.substr(0, i));
      s = s.substr(i + 1);
    }
  }
  return r;
}
}

