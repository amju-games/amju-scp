/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: LevelWorkerFlowers.h,v $
Revision 1.1  2004/09/08 15:42:39  jay
Added to repository
  
*/

#if !defined(LEVEL_WORKER_FLOWERS_H_INCLUDED)
#define LEVEL_WORKER_FLOWERS_H_INCLUDED

#include "LevelWorker.h"

namespace Amju
{
class LevelWorkerFlowers : public LevelWorker
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
  std::string m_flowerSolidName;
  int m_numFlowers;
  int m_numBunches;
  float m_radius;
};
}

#endif

