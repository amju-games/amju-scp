/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: LevelWorkerTerrain.h,v $
Revision 1.1.10.1  2006/08/14 17:38:04  jay
Rename "Pool"

Revision 1.1  2004/09/08 15:42:40  jay
Added to repository
  
*/

#if !defined(LEVEL_WORKER_TERRAIN_H_INCLUDED)
#define LEVEL_WORKER_TERRAIN_H_INCLUDED

#include "LevelWorker.h"
//#include "Pool/TerrainGenPool.h"
#include "SolidComponent.h"

namespace Amju
{
class LevelWorkerTerrain : public LevelWorker
{
public:
  static const char* TypeName;

  virtual bool Load(File* ); 
#ifdef SCENE_EDITOR
  virtual bool Save(File* );
#endif

  virtual const char* GetTypeName();
  virtual bool AddFeature(Level*, CubeMap* ); 

  void SetTerrainPeakHeight(float h);
  float GetTerrainPeakHeight() const;

protected:
  PSolidComponent MakePyramidRect();
  PSolidComponent MakePyramid();

protected:
//  TerrainGenPool m_terrainGen;


};
}

#endif

