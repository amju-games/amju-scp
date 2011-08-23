/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: LevelFactory.h,v $
Revision 1.1.10.1  2005/09/17 22:39:19  Administrator
Fixed Object Delete in Scene Editor

Revision 1.1  2004/09/08 15:42:38  jay
Added to repository
  
*/

#if !defined(LEVEL_FACTORY_H_INCLUDED)
#define LEVEL_FACTORY_H_INCLUDED

#include <map>
#include "SharedPtr.h"
#include "Level.h"
#include "LevelWorker.h"
#include "LoadResult.h"

namespace Amju
{
class File;

// Interface for Level-Creation classes.
// We want to input a level ID, and get a whole level out of the other
// end, without having to design it in detail.
class LevelFactory : public Shareable
{
public:
  virtual ~LevelFactory() {}
  // Create a level given a level ID.
  // This function creates the Level on the heap in a SharedPtr.
  virtual PLevel Create(int levelId) = 0;
  // Load the factory data. This data tells us what things to add to 
  // the level.
  virtual LoadResult Load(File* ) = 0;

#ifdef SCENE_EDITOR
  // Save the factory data.
  virtual bool Save(File* ) = 0;
#endif

  virtual void SetWorkerForObject(SharedPtr<LevelWorker>, int id);
  virtual void ClearWorkerForObject(int id);

protected:
  typedef std::map<int, SharedPtr<LevelWorker> > IdToWorkerMap;
  IdToWorkerMap m_idToWorker;
};
}

#endif

