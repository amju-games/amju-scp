/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: LevelWorkerAddWater.cpp,v $
Revision 1.1  2004/09/08 15:42:39  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "LevelWorkerAddWater.h"
#include "Level.h"
#include "Water.h"

namespace Amju
{
const char* LevelWorkerAddWater::TypeName = "water";
const char* LevelWorkerAddWater::GetTypeName() { return TypeName; }

bool LevelWorkerAddWater::Load(File* )
{
  // TODO
  return true;
}

#ifdef SCENE_EDITOR
bool LevelWorkerAddWater::Save(File* )
{
  return true;
}
#endif

bool LevelWorkerAddWater::AddFeature(Level* pLevel)
{
  Water* pWater = new Water;
  // TODO
  // Use this function so we keep track of which LevelWorker added which object.
  AddGameObject(pLevel, pWater);
  /*
  pLevel->SetCurrentRoomWaterHeight(pWater->GetHeight());
  */
  return true;
}

}

