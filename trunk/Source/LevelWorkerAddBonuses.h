/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: LevelWorkerAddBonuses.h,v $
Revision 1.1  2004/09/08 15:42:38  jay
Added to repository
  
*/

#if !defined(LEVEL_WORKER_ADD_BONUSES)
#define LEVEL_WORKER_ADD_BONUSES

#include "LevelWorkerAddSolidsBase.h"

namespace Amju
{
class LevelWorkerAddBonuses : public LevelWorkerAddSolidsBase
{
public:
  static const char* TypeName;

  LevelWorkerAddBonuses();
  virtual const char* GetTypeName();
  virtual bool AddFeature(Level*);
};
}

#endif



