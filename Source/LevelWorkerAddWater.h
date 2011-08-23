/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: LevelWorkerAddWater.h,v $
Revision 1.1  2004/09/08 15:42:39  jay
Added to repository
  
*/

#if !defined(LEVEL_WORKER_ADD_WATER)
#define LEVEL_WORKER_ADD_WATER

#include "LevelWorker.h"

namespace Amju
{
class LevelWorkerAddWater : public LevelWorker
{
public:
  static const char* TypeName;

  virtual bool Load(File* );
#ifdef SCENE_EDITOR
  virtual bool Save(File* );
#endif
  virtual const char* GetTypeName();
  virtual bool AddFeature(Level*, CubeMap* );

protected:
};
}

#endif

