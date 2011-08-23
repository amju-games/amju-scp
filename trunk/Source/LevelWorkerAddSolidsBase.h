/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: LevelWorkerAddSolidsBase.h,v $
Revision 1.1  2004/09/08 15:42:39  jay
Added to repository
  
*/


#if !defined(LEVEL_WORKER_ADD_SOLIDS_BASE_H_INCLUDED)
#define LEVEL_WORKER_ADD_SOLIDS_BASE_H_INCLUDED

#include <string>
#include <vector>
#include "LevelWorker.h"

namespace Amju
{
// Base class for loading list of Solid names and verifying they load ok.
class LevelWorkerAddSolidsBase : public LevelWorker
{
public:
  virtual bool Load(File* );
#ifdef SCENE_EDITOR
  virtual bool Save(File* );
#endif

protected:
  std::vector<std::string> m_solidNames;

};
}

#endif

