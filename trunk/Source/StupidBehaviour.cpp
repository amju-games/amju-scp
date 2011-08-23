/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: StupidBehaviour.cpp,v $
Revision 1.1  2004/09/08 15:42:41  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "StupidBehaviour.h"
//#include "CharacterGameObject.h"
#include "Stupid.h"
#include "Engine.h"

namespace Amju
{
StupidBehaviour::StupidBehaviour()
{
  // We don't look very far ahead at all - because we are stupid.
  m_lookahead = 0.5f; // TODO CONFIG

  // TODO
  // Vel. depends on character type and level.
//  m_vel = 5.0f;

//  m_turnDir = 90.0f; // default; set in Stupid::Load
}

void StupidBehaviour::ReactToObstacle()
{
  // Turn to face new direction.
  // TODO smoothly turn to face new direction
  m_pCharacter->RotateY(GetTurnDir());

  // If the new direction is blocked, try the opposite direction.
  // Otherwise Stupids will be excessively stupid, and backtrack rather than
  // going in the available new direction.
  // (If both directions are blocked, the call in the next frame will cause
  // backtracking.)
  if (IsBlocked())
  {
    m_pCharacter->RotateY(180.0f); // go in the opposite direction.
  }
}

void StupidBehaviour::Update()
{
  static float stupidVel = Engine::Instance()->GetConfigFloat("stupid_vel");

  m_pCharacter->SetForwardVel(5.0f); // TODO TEMP TEST

/*
  // Hopefully AvoidObstacles will take case of turning when we hit a wall
  // (or another Character)
  AvoidObstacles();
*/

  if (IsBlocked())
  {
    // Too annoying when there are lots of Stupids
    // Engine::Instance()->PlayWav(Engine::Instance()->GetConfigValue("wav_hitswall"));
    ReactToObstacle();
  }
}

bool StupidBehaviour::IsBlocked() const
{
  bool b = m_pCharacter->GetBlocked() > 5; // TODO CONFIG
  if (b)
  {
    m_pCharacter->SetBlocked(false); // reset counter
  }

  // If there isn't a wall in the way, check for a drop instead.
  // We want to avoid jumping off.
  if (!b)
  {
    b = IsDropAhead();
  }
  return b;
}

float StupidBehaviour::GetTurnDir() const
{
  return ((Stupid*)m_pCharacter)->GetTurnDirection();
}

bool StupidBehaviour::IsDropAhead() const
{
  // Same idea as camera, we detect when there is a drop in front of the 
  // character.
  float dropseverity = m_pCharacter->DropAhead();
  // If downward slope ahead is too steep, return true.
  return (dropseverity > 0.7f); // TODO CONFIG
}
}
