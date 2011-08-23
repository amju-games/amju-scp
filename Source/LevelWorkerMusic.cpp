/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: LevelWorkerMusic.cpp,v $
Revision 1.1  2004/09/08 15:42:39  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "LevelWorkerMusic.h"
#include "File.h"
#include "Level.h"

namespace Amju
{
const char* LevelWorkerMusic::TypeName = "music";
const char* LevelWorkerMusic::GetTypeName() { return TypeName; }

#ifdef SCENE_EDITOR
bool LevelWorkerMusic::Save(File* )
{
  return false;
}
#endif

bool LevelWorkerMusic::Load(File* pf)
{
  int numSongs = 0;
  if (!pf->GetInteger(&numSongs))
  {
    pf->ReportError("Expected number of songs.");
    return false;
  }
  // Get each song file name. Open each file to mark it as used.
  for (int i = 0; i < numSongs; i++)
  {
    std::string songName;
    if (!pf->GetDataLine(&songName))
    {
      pf->ReportError("Expected song name.");
      return false;
    }
    File songFile(false); // => no version info
    if (!songFile.OpenRead(songName, File::BINARY))
    {
      std::string err = "Failed to open song file ";
      err += songName;
      pf->ReportError(err);
      return false;
    }
    m_songNames.push_back(songName);
  }
  return true;
}

bool LevelWorkerMusic::AddFeature(Level* pLevel, CubeMap* )
{
  int id = pLevel->GetId();
  id %= m_songNames.size();
  std::string song = m_songNames[id];
  pLevel->SetMusicFilename(song);
  return true;
}
}


