/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: LevelFinder.cpp,v $
Revision 1.1  2004/09/08 15:42:38  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "LevelFinder.h"
#include "Directory.h"
#include "File.h"

namespace Amju
{
LevelFinder::LevelFileList LevelFinder::GetLevelFiles()
{
  // Get user directory and level extension
  

  LevelFileList files;

  std::vector<std::string> filenames;

  // Get predefined levels
  // Use Glue File if it is enabled.
  Dir(File::GetRoot(), &filenames, true); 
  // Copy matching filenames to files result.
  std::vector<std::string>::iterator it;
  for (it = filenames.begin(); it != filenames.end(); ++it)
  {
    files.push_back(LevelFile(*it, false));
  }

  // Get User levels

  return files;
}

}
