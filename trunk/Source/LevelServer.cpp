/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: LevelServer.cpp,v $
Revision 1.1.10.2  2007/03/06 20:57:07  jay
Turn off debug out

Revision 1.1.10.1  2005/07/30 23:36:11  jay
Added debug output for Loading

Revision 1.1  2004/09/08 15:42:52  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "LevelServer.h"
#include "File.h"
#include "LevelFactorySimple.h"
#include "Engine.h"
#include "StringUtils.h"
#include "ExtraDir.h"
#include "SchAssert.h"

using namespace std;

namespace Amju
{
LevelServeOnFly::LevelServeOnFly()
{
  m_pFactory = new LevelFactorySimple;
}

PLevel LevelServeOnFly::GetCurrentLevel()
{
  return m_pLastLevelRequested;
}

void LevelServeOnFly::Clear()
{
  m_pLastLevelRequested = 0;
}

RCPtr<LevelFactorySimple> LevelServeOnFly::GetFactory()
{
  return m_pFactory;
}

bool LevelServeOnFly::Create(int id)
{
  // Get highest level ID, use the next ID for the new level.^M
  Level* pLevel = new Level;
  pLevel->SetId(id); 
  m_pLastLevelRequested = pLevel;
  m_pFactory = new LevelFactorySimple;
  return true; 
}

LoadResult LevelServeOnFly::Load(const std::string& levelFile, bool isUser, int id)
{
#ifdef LEVEL_LOAD_DEBUG
std::cout << "LevelServer: " << levelFile.c_str() << " isUser: " << isUser
  << " ID: " << id << "\n";
#endif

  if (m_mruList.find(levelFile) != m_mruList.end())
  {
#ifdef LEVEL_LOAD_DEBUG
std::cout << "LevelServer: " << levelFile.c_str() << " is cached!\n";
#endif

    // The level exists in the MRU list.
    m_pLastLevelRequested = m_mruList[levelFile];
    m_pLastLevelRequested->RefreshObjects();  
    return true;
  }

  // Store the last filename used. If this function is called with the same
  // filename again, we should resume loading rather than load a new file.
  bool resume = false; 
  static std::string prevname;
  if (prevname == levelFile)
  {
    // We are resuming a load rather than starting a new load.
    resume = true;
  }
  prevname = levelFile;

  static RCPtr<File> pf;

  if (!resume)
  {
    // If the level is user defined, don't use the glue file, or the standard
    // directory root. Instead, prepend the user directory.
    if (isUser)
    {
      pf = new File(true /* has version */, File::STD /* no glue */);
      std::string filename = GetExtraDir();

      filename += StripPath(levelFile);
      if (!pf->OpenRead(filename, false /* not binary */, false /* no root */))
      {
        pf->ReportError("Couldn't open user-defined level factory file.");
        return false;
      }
    }
    else
    {
#ifdef BINARY_LEVEL_FILES
      pf = new File;
      // Attempt to open binary file first
      std::string binfile = StripPath(levelFile);
      // If binfile does not end with 'b', append it.
      if (binfile[binfile.size() -1] != 'b')
      {
        binfile += "b";
      }

      if (pf->OpenRead(binfile, true))
      {
#ifdef LEVEL_LOAD_DEBUG
        pf->ReportError("Opened binary level file.");
#endif
      }
      else
#endif // BINARY_LEVEL_FILES
      {
        pf = new File;
        // Remove 'b' from end of level file if it exists
        std::string notbinfile(levelFile);
        StripPath(notbinfile);
        if (notbinfile[notbinfile.size() - 1] == 'b')
        {
          notbinfile = notbinfile.substr(0, notbinfile.size() - 2);
        }
        if (!pf->OpenRead(StripPath(levelFile)))
        {
          pf->ReportError("Couldn't open level factory file.");
          return false;
        }
#ifdef LEVEL_LOAD_DEBUG
std::cout << "LevelServer: Opened level file " << notbinfile.c_str() << "\n";
#endif
      }
    }
  }

  if (!resume)
  {
    m_pFactory = new LevelFactorySimple;
  }

  LoadResult r = m_pFactory->Load(pf.GetPtr());
  if (r.Failed())
  {
    return false;
  }
  else if (r.HasMore())
  {
    return LoadResult::MORE;
  }
  // Loaded ok.

  Engine::Instance()->ClearGameObjects();

  // Create the level - we assume that creation is much faster than loading, and
  // don't bother with a progress bar.
  m_pLastLevelRequested = m_pFactory->Create(id);

  // Now it's safe to add this to the MRU list, so if this file is requested
  // again we just look up the Level data in memory.
  // Store in MRU list.
  // TODO Throw out oldest item if necessary.
  m_mruList[levelFile] = m_pLastLevelRequested;

/*
  // Load any Game Objects
  if (!m_pLastLevelRequested->LoadGameObjects(&f))
  {
    return false;
  }
*/
  return true;
}

bool LevelServeOnFly::Rebuild()
{
  if (!m_pFactory.GetPtr())
  {
    return false;
  }
  if (!m_pLastLevelRequested.GetPtr())
  {
    return false;
  }
  // We dump the old level and create a new one.
  // Make sure we don't lose any info!
  std::string name = m_pLastLevelRequested->GetLevelName();
  m_pLastLevelRequested = m_pFactory->Create(m_pLastLevelRequested->GetId());

  m_pLastLevelRequested->SetLevelName(name);

  return true;
}

#ifdef SCENE_EDITOR
bool LevelServeOnFly::Save(const std::string& levelFile)
{
  std::string fullname;
  //fullname += GetExtraDir();
/*
Engine::Instance()->GetConfigValue("extra_dir");
  // TODO MACOSX
  // Get full path
  // TODO TEMP TEST
  fullname = "/Users/jay/pet/" + fullname;
*/

  //fullname += "/";
  fullname += levelFile;

  File f;
  if (!f.OpenWrite(
    fullname, 
    File::CURRENT_VERSION, 
    false, // not binary 
    false  // don't use root, i.e. this file is going to a non-standard directory.
  ))
  {
    f.ReportError("Couldn't open level factory file for saving.");
    return false;
  }
  
  if (!m_pFactory->Save(&f))
  {
    return false;
  }

  // Save binary version
  File binfile;
  std::string binfilename = fullname;
  binfilename += "b";
  if (!binfile.OpenWrite(binfilename, File::CURRENT_VERSION, true, false))
  {
    binfile.ReportError("Failed to open binary level file for saving.");
    return false;
  }

  if (!m_pFactory->Save(&binfile))
  {
    return false;
  }

  // Save all Game Objects in the level ?
  // This causes problems, because all game objects would be saved, 
  // including ones that are created by Factory Workers.
  // Also if the Factory Worker isn't saved, then its parameters
  // can't be tweaked any more.
  // So it's better to have a Factory Worker for every Game Object,
  // and to NOT save the objects independently of the Factory Worker
  // which made them.
/*
  if (!m_pLastLevelRequested->SaveGameObjects(&f))
  {
    return false;
  }
*/
  return true;
}
#endif
}

