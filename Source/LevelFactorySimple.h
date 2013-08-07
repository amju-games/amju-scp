/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: LevelFactorySimple.h,v $
Revision 1.1.10.1  2005/09/17 22:39:21  Administrator
Fixed Object Delete in Scene Editor

Revision 1.1  2004/09/08 15:42:38  jay
Added to repository
  
*/

#if !defined(LEVEL_FACTORY_SIMPLE_H_INCLUDED)
#define LEVEL_FACTORY_SIMPLE_H_INCLUDED

#include <vector>
#include "LevelFactory.h"
#include "LevelWorker.h"

namespace Amju
{
class LevelFactorySimple : public LevelFactory
{
public:
  LevelFactorySimple();
  virtual ~LevelFactorySimple();
  virtual PLevel Create(int levelId);
  virtual LoadResult Load(File* );
#ifdef SCENE_EDITOR
  virtual bool Save(File* );
#endif
  void AddWorker(RCPtr<LevelWorker>);
  void DeleteWorker(RCPtr<LevelWorker>);

  // Override base class versions (which don't seem to be much use) 
  virtual void ClearWorkerForObject(int id);

  // Get the worker with the given type name.
  // There must be exactly one worker with this name.
  RCPtr<LevelWorker> GetWorker(const char* name);

  typedef std::vector<RCPtr<LevelWorker> > Workers;

protected:
  Workers m_workers;

  // Used in Load() to keep track of what has been loaded so far.
  // The idea is that Load() is called multiple times until it returns
  // OK or FAIL. 
  int m_loadCounter;

  // Number of workers to load - the first thing to read in Load().
  int m_numWorkers;

#if defined(SCENE_EDITOR)
public:
  Workers* GetWorkers() { return &m_workers; }
#endif
};
}

#endif

