/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ControlStyleMovePlayerBase.cpp,v $
Revision 1.1  2004/09/08 15:42:35  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "ControlStyleMovePlayerBase.h"
#include "Engine.h"
#include "PieceMover.h"
#include "EnginePaused.h"
#include "Takeable.h"
#include "AiNotifier.h"
#include "Player.h"
#include "EngineRunning.h"

#include "SchAssert.h"

namespace Amju
{
// TODO CONFIG
// Acceleration when player stops. Negative because we want to decelerate.
const float ControlStyleMovePlayerBase::SLOW_TO_STOP = -20.0f; 

ControlStyleMovePlayerBase::ControlStyleMovePlayerBase()
{
  Reset();
}

void ControlStyleMovePlayerBase::Reset()
{
  m_flippedX = false;
  m_flippedY = false;
  m_shuffle = 5.0f;

  // TODO TEMP TEST - currently in EngineRunning too!
  m_maxForwardVel = Engine::Instance()->GetConfigFloat("player_fwd_vel_run");
  m_walkForwardVel = Engine::Instance()->GetConfigFloat("player_fwd_vel_walk");
  m_currentMaxForwardVel = m_maxForwardVel;
  m_playerAccel = Engine::Instance()->GetConfigFloat("player_walk_accel");
  m_playerRotateVel = Engine::Instance()->GetConfigFloat("player_rot_vel");

  ControlStyle::Reset();
}

void ControlStyleMovePlayerBase::RotatePlayer(float degsSec)
{
  Engine::Instance()->GetPlayer()->SetYRotateVel(degsSec);
}

void ControlStyleMovePlayerBase::FlipPlayer(float degs)
{
  if (Engine::Instance()->GetPlayer()->IsFalling())
  {
    return;
  }

  const Orientation* pO = Engine::Instance()->GetPlayer()->GetOrientation();
  float yRot = pO->GetYRot();
  yRot += degs;
////  pO->SetYRot(yRot);
  Engine::Instance()->GetPlayer()->SetOrientation(*pO);
  // TODO problem here: flipping may stick the player b.sphere into a wall!
  //ExtractPlayerFromWall();

  m_flippedX = true;
}


}

