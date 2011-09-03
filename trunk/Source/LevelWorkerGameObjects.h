/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: LevelWorkerGameObjects.h,v $
Revision 1.1  2004/09/08 15:42:39  jay
Added to repository
  
*/

#if !defined(LEVEL_WORKER_GAME_OBJECTS_H_INCLUDED)
#define LEVEL_WORKER_GAME_OBJECTS_H_INCLUDED

#include "LevelWorker.h"
#include "GameObject.h"

namespace Amju
{
// Loads One game object and adds it to the Level.
class LevelWorkerGameObjects : public LevelWorker
{
public:
  static const char* TypeName;

  virtual bool Load(File* );
#ifdef SCENE_EDITOR
  virtual bool Save(File* );
#endif

  virtual const char* GetTypeName();
  virtual bool AddFeature(Level*);
  virtual bool RemoveFeature(); 

  void SetGameObject(PPoolGameObject);

protected:
  PPoolGameObject m_pGo;
};
}

#endif

