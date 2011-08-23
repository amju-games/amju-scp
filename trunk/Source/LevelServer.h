/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: LevelServer.h,v $
Revision 1.1  2004/09/08 15:42:52  jay
Added to repository
  
*/

#if !defined(LEVEL_SERVER_H_INCLUDED)
#define LEVEL_SERVER_H_INCLUDED

#include <map>
#include <string>
#include "Level.h"
#include "Singleton.h"
#include "LevelFactorySimple.h"
#include "LoadResult.h"

namespace Amju
{
// Create Levels on the fly:
class LevelServeOnFly 
{
public:
  LevelServeOnFly();

  PLevel GetCurrentLevel();

  void Clear();

  // Create a new level with the given ID. This becomes the current level.
  bool Create(int id);

  // Rebuild current level, if the factory has had a worker changed.
  bool Rebuild();

#ifdef SCENE_EDITOR
  // Save the factory data for the current level.
  // NB Always saves to user directory.
  bool Save(const std::string& levelsFile);
#endif

  // Load the level data, which the factory uses to create the level.
  // Set flag to specify if level file is user-defined.
  // Give the ID for the level to be created.
  // Can return part-way through loading, in which case the LoadResult
  // will indicate that there is more data to come.
  LoadResult Load(const std::string& levelsFile, bool isUser, int id);

  // Get the factory which creates levels.
  SharedPtr<LevelFactorySimple> GetFactory();

protected:
  PLevel m_pLastLevelRequested;
  // The factory which creates Levels on the fly.
  SharedPtr<LevelFactorySimple> m_pFactory;

  // Store a number of the most recently used Levels. This is to improve
  // responsiveness - but obviously requires more memory.

  // Map filename to level
  typedef std::map<std::string, PLevel> LevelMap;
  LevelMap m_mruList;

};

typedef Singleton<LevelServeOnFly> LevelServer;

}
#endif

