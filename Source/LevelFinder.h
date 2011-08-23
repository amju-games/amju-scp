/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: LevelFinder.h,v $
Revision 1.1  2004/09/08 15:42:38  jay
Added to repository
  
*/

#if !defined(LEVEL_FINDER_H_INCLUDED)
#define LEVEL_FINDER_H_INCLUDED

#include <string>
#include <vector>

namespace Amju
{
// LevelFinder finds level files.
// These can be predefined or user-created/downloaded.
// Predefined files are in the Data directory. Either Glue or
// standard files may be used.
// User levels are in "Extras" directory (name to be decided).
class LevelFinder
{
public:
  struct LevelFile
  {
    LevelFile(const std::string& f, bool user=true) : 
      m_filename(f), m_isUser(user) {}

    std::string m_filename;
    bool m_isUser;
  };

  typedef std::vector<LevelFile> LevelFileList;

  LevelFileList GetLevelFiles();
};
}

#endif

