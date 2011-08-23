/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: LevelWorkerAddCrates.h,v $
Revision 1.1  2004/09/08 15:42:39  jay
Added to repository
  
*/

#if !defined(LEVEL_WORKER_ADD_CRATES_H_INCLUDED)
#define LEVEL_WORKER_ADD_CRATES_H_INCLUDED

#include "LevelWorkerAddSolidsBase.h"
#include "Orientation.h"
#include "BoundingBox.h"

namespace Amju
{
class LevelWorkerAddCrates : public LevelWorkerAddSolidsBase
{
public:
  static const char* TypeName;

  LevelWorkerAddCrates();
  virtual const char* GetTypeName();
  virtual bool AddFeature(Level*, CubeMap* );

protected:
};
}

#endif

