/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: LevelWorkerAddTrees.h,v $
Revision 1.1  2004/09/08 15:42:39  jay
Added to repository
  
*/

#if !defined(LEVEL_WORKER_ADD_TREES_H_INCLUDED)
#define LEVEL_WORKER_ADD_TREES_H_INCLUDED

#include "LevelWorkerAddSolidsBase.h"

namespace Amju
{
class LevelWorkerAddTrees : public LevelWorkerAddSolidsBase
{
public:
  static const char* TypeName;

  LevelWorkerAddTrees();
  virtual bool Load(File* );
#ifdef SCENE_EDITOR
  virtual bool Save(File* );
#endif
  virtual const char* GetTypeName();
  virtual bool AddFeature(Level*);

protected:
  // We have one list of solids for the trunks, and another for
  // the leaves. This is so we can mix and match leaves and trunks.
  std::vector<std::string> m_leafNames;

  int m_numTrees;
};
}

#endif

