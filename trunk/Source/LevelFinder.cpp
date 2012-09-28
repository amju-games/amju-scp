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

  DirEnts dirents;  

  // Get predefined levels
  // Use Glue File if it is enabled.
  Dir(File::GetRoot(), &dirents, true); 
  // Copy matching filenames to files result.
  DirEnts::iterator it;
  for (it = dirents.begin(); it != dirents.end(); ++it)
  {
    DirEnt& de = *it;
    if (!de.m_isDir)
    {
      files.push_back(LevelFile(de.m_name, false));
    }
  }

  // Get User levels

  return files;
}

}
