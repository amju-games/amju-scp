/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: LevelWorkerAddBall.cpp,v $
Revision 1.1.10.2  2006/08/14 17:38:04  jay
Rename "Pool"

Revision 1.1.10.1  2006/07/26 21:25:17  jay
Use Pool/, not Pool/

Revision 1.1  2004/09/08 15:42:38  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "LevelWorkerAddBall.h"
#include "PoolBall.h"
#include "Controller.h"
#include "Level.h"
#include "Engine.h"
#include "BallController.h"

namespace Amju
{
const char* LevelWorkerAddBall::TypeName = "ball";
const char* LevelWorkerAddBall::GetTypeName() { return TypeName; }

LevelWorkerAddBall::LevelWorkerAddBall()
{
  m_infoString = "golf ball";
}

bool LevelWorkerAddBall::Load(File* pf)
{
  // Get Solid name for ball
  // Get ID for ball
  // Get start orientation
  // TODO
  return true;
}

#ifdef SCENE_EDITOR
bool LevelWorkerAddBall::Save(File* )
{
  return true;
}
#endif

bool LevelWorkerAddBall::AddFeature(Level* pLevel)
{

  PoolBall* pBall = new PoolBall;
//  pBall->SetGlobalGravity(2.0f); // TODO TEMP TEST

  static int ballId = 10; // TODO Get the ball ID from config
  pBall->SetId(ballId);

  pBall->SetLevel(pLevel);
  pBall->SetRoomId(pLevel->GetRoomId());

  pBall->Load(0);

  pBall->SetDeadStopAfterFall(false);
  pBall->SetController(new BallController);

  // Use this function so we keep track of which LevelWorker added which object.
  AddGameObject(pLevel, pBall);

  return true;
}
}

