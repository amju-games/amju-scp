/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: LevelWorker.h,v $
Revision 1.1  2004/09/08 15:42:38  jay
Added to repository
  
*/

#if !defined(LEVEL_FACTORY_WORKER_H_INCLUDED)
#define LEVEL_FACTORY_WORKER_H_INCLUDED

#include <string>
#include "RCPtr.h"
#include "BoundingBox.h"
#include "Orientation.h"
#include "GameObject.h"

namespace Amju
{
class File;
class Level;
class LevelFactory;

class LevelWorker : public RefCounted
{
public:
  LevelWorker();
  virtual ~LevelWorker() {}
  virtual bool Load(File* ) = 0;

#ifdef SCENE_EDITOR
  virtual bool Save(File* ) = 0;
#endif

  virtual const char* GetTypeName() = 0;

  // Subclasses override this to add their specific feature to the given level.
  virtual bool AddFeature(Level*) = 0;

  virtual bool RemoveFeature() { return false; } 

  void SetFactory(LevelFactory*);
  LevelFactory* GetFactory();

  std::string GetInfoString() const;

  // Set random number seed. This is set when a new level is created, so
  // level<n> is always the same.
  static void RndSeed(int);

  // Get a new unique ID.
  static int GetUniqueId();

protected:
  // Add the given game object to the given level.
  // Also keep track of which LevelWorker added the game object to the Level.
  // (The map is held by the LevelFactory which owns this LevelWorker.)
  void AddGameObject(Level* pLevel, PoolGameObject* pGo);

  // Choose a new random orientation, where the given bounding box doesn't
  // intersect anything else in the cube map.
  // absPlayAreaSize is the size of the play area in x-z on either side of the
  // origin.
  // Returns false if there is no space for the bounding box in the play area.
  bool GetOrientation(
    const BoundingBox& bb, 
    int absPlayAreaSize,
    Orientation* pResult);

  // This version of the function does not use a Cube Map to ensure that
  // objects do not intersect.
  bool GetOrientation(
    int absPlayAreaSize,
    Orientation* pResult);

  // As above, but checks for water and picks a spot above the water level.
  bool GetOrientationAboveWater(
    Level* pLevel,
    Orientation* pResult);

  // Return random float [0..1)
  // The results are the same over multiple platforms, so randomly created 
  // levels always look the same.
  static float Rnd();

private:
  static int s_seed;

#if defined(BUILT_IN_EDITOR)
  virtual bool Save(File* ) { return true; }
#endif

  LevelFactory* m_pFactory;

protected:
  std::string m_infoString;
};
}

#endif


