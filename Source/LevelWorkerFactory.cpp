/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: LevelWorkerFactory.cpp,v $
Revision 1.1.8.1  2006/08/14 17:38:04  jay
Rename "Pool"

Revision 1.1  2004/09/08 15:42:39  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "LevelWorkerFactory.h"
#include "LevelWorkerAddRoom.h"
#include "LevelWorkerAddStupids.h"
//#include "LevelWorkerAddCrates.h"
#include "LevelWorkerAddTrees.h"
//#include "LevelWorkerAddSheds.h"
#include "LevelWorkerAddBonuses.h"
#include "LevelWorkerMusic.h"
// v. 2.0 features
//#include "LevelWorkerTerrain.h"
#include "LevelWorkerAddBall.h"
#include "LevelWorkerAddCharacters.h"
#include "LevelWorkerGameObjects.h"
#include "LevelWorkerFlowers.h"
#include "LevelWorkerName.h"
//#include "LevelWorkerBoundary.h"
//#include "LevelWorkerAddWater.h"

namespace Amju
{
LevelWorker* CreateLwAddRoom()
{
  return new LevelWorkerAddRoom;
}

LevelWorker* CreateLwAddStupids()
{
  return new LevelWorkerAddStupids;
}

//LevelWorker* CreateLwAddCrates()
//{
//  return new LevelWorkerAddCrates;
//}

LevelWorker* CreateLwAddTrees()
{
  return new LevelWorkerAddTrees;
}

//LevelWorker* CreateLwAddSheds()
//{
//  return new LevelWorkerAddSheds;
//}

LevelWorker* CreateLwAddBonuses()
{
  return new LevelWorkerAddBonuses;
}

LevelWorker* CreateLwMusic()
{
  return new LevelWorkerMusic;
}

//LevelWorker* CreateLwTerrain()
//{
//  return new LevelWorkerTerrain;
//}

LevelWorker* CreateLwBall()
{
  return new LevelWorkerAddBall;
}

LevelWorker* CreateLwChars()
{
  return new LevelWorkerAddCharacters;
}

LevelWorker* CreateLwGos()
{
  return new LevelWorkerGameObjects;
}

LevelWorker* CreateLwFlowers()
{
  return new LevelWorkerFlowers;
}

LevelWorker* CreateLwName()
{
  return new LevelWorkerName;
}

//LevelWorker* CreateLwBoundary()
//{
//  return new LevelWorkerBoundary;
//}

//LevelWorker* CreateLwAddWater()
//{
//  return new LevelWorkerAddWater;
//}

SingleLevelWorkerFactory::SingleLevelWorkerFactory()
{
  Add("add_room", CreateLwAddRoom);
  Add("add_stupids", CreateLwAddStupids);
  Add("add_trees", CreateLwAddTrees);
  Add("add_bonuses", CreateLwAddBonuses);
  Add("music", CreateLwMusic);
  Add("ball", CreateLwBall);
  Add("characters", CreateLwChars);
  Add("game_objects", CreateLwGos);
  Add("flowers", CreateLwFlowers);
  Add("name", CreateLwName);
}
}

