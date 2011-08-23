/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: FileSize.cpp,v $
Revision 1.1  2004/09/08 15:43:18  jay
Added to repository
  
*/

// This source code originally written by JASON COLMAN 2000-2003. 
// You may use it as you wish, at your own risk!  jason@amju.com.

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <string>
#include "FileSize.h"
#include "File.h"

namespace Amju
{
unsigned int FileSize(const char* filename)
{
  std::string fullpath = File::GetRoot() + filename;

  struct stat buf;
  if (stat(fullpath.c_str(), &buf) == -1)
  {
    // Error
    return 0;
  }

  unsigned int s = buf.st_size;

  return s;
}
}


