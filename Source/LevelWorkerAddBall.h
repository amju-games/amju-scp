/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: LevelWorkerAddBall.h,v $
Revision 1.1  2004/09/08 15:42:38  jay
Added to repository
  
*/

#if !defined(LEVEL_WORKER_ADD_BALL_H_INCLUDED)
#define LEVEL_WORKER_ADD_BALL_H_INCLUDED

#include "LevelWorker.h"

namespace Amju
{
class LevelWorkerAddBall : public LevelWorker
{
public:
  static const char* TypeName;

  LevelWorkerAddBall();
  virtual bool Load(File* );
#ifdef SCENE_EDITOR
  virtual bool Save(File* );
#endif
  virtual const char* GetTypeName();
  virtual bool AddFeature(Level*);
};
}

#endif

