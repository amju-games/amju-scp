/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: PoolStupidBehaviour.cpp,v $
Revision 1.1.2.1  2006/08/14 17:50:24  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:35  jay
Moved Pool code from Pool/ to Pool/

Revision 1.2.6.2  2005/09/30 13:01:53  jay
Just added comments - see TODOs. There is something very strange going
on - we HAVE to set the forward vel of the NPCs every frame!?

Revision 1.2.6.1  2005/09/07 20:57:20  jay
Stop that crappy golf meandering

Revision 1.2  2004/09/22 12:05:09  Administrator
Stupid vel is configureable

Revision 1.1  2004/09/08 15:43:06  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "PoolStupidBehaviour.h"
//#include "CharacterGameObject.h"
#include "Stupid.h"
#include "Engine.h"
#include "PoolMisc.h"
#include "PoolCharacter.h"
#include "AngleCompare.h"

namespace Amju
{
PoolStupidBehaviour::PoolStupidBehaviour()
{
  // We don't look very far ahead at all - because we are stupid.
  // We look ahead (i.e. in forward direction) this far to see if we will
  // collide with any GameObjects. If so we change dir to avoid. 
  // POOL This looks shit.
  // TODO Turn gradually to avoid, don'f flip direction.
  //m_lookahead = 0.5f; // TODO CONFIG

  // TODO
  // Vel. depends on character type and level.
//  m_vel = 5.0f;

//  m_turnDir = 90.0f; // default; set in Stupid::Load

  m_time = 0;
  m_ringMin = 3.0f; // min distance from player
  m_ringMax = 10.0f; // max distance from player
  m_rotateVelMax = 30.0f;
}

void PoolStupidBehaviour::ReactToObstacle()
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

void PoolStupidBehaviour::Update()
{
  static const float stupidVel = Engine::Instance()->GetConfigFloat("stupid_vel");

  // This prevents each Stupid from having its own speed.
  // But we need it - otherwise birds (but not the other animals) are too
  // high and are stuck in the balls which are supposed to be above 
  // their heads. 
  // TODO Find out why we need to keep calling this!
  m_pCharacter->SetForwardVel(stupidVel); 

  m_time += Engine::Instance()->GetDeltaTime();
  if (false) //m_time > CHANGE_TIME)
  {
//std::cout << "Changing direction for animal " << m_pCharacter->GetId() << "\n";

    m_time = 0;

    // Get distance from player. If it is outside m_ringMax, aim towards the player.
    // If we are too close, aim away from the player.

    // TODO This won't work if different animals have different 'rings'.
    static const float ringMinSquared = m_ringMin * m_ringMin;
    static const float ringMaxSquared = m_ringMax * m_ringMax;
    const Orientation* pPlayerOr = GetActivePlayer()->GetOrientation();
    const Orientation* pAnimalOr = m_pCharacter->GetOrientation();

    float dx = pPlayerOr->GetX() - pAnimalOr->GetX();
    float dz = pPlayerOr->GetZ() - pAnimalOr->GetZ();

    // Square of distance between player and this character.
    float dsquared = dx * dx + dz * dz;

    float rv = 0;

    if ((dsquared > ringMaxSquared) && (rand() % 2) == 1)
    {
      rv = 270.0f;
    }
    else if (dsquared < ringMinSquared)
    {
      rv = 270.0f; 
    }
    else 
    {
      // Set a rotate vel
      rv = (float)rand() / (float)RAND_MAX * m_rotateVelMax; 
    }
    if (rand() % 2 == 1)
    {
      rv = -rv;
    }
    m_pCharacter->SetYRotateVel(rv);
  }

  // Hopefully AvoidObstacles will take case of turning when we hit a wall
  // (or another Character)
  //AvoidObstacles();

  if (IsBlocked())
  {
    // Too annoying when there are lots of Stupids
    // Engine::Instance()->PlayWav(Engine::Instance()->GetConfigValue("wav_hitswall"));
    ReactToObstacle();
  }
}

bool PoolStupidBehaviour::IsBlocked() const
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

float PoolStupidBehaviour::GetTurnDir() const
{
  float f = (float)rand() / (float)RAND_MAX * 360.0f;
  return f;

  //return ((Stupid*)m_pCharacter)->GetTurnDirection();
}

bool PoolStupidBehaviour::IsDropAhead() const
{
  // Same idea as camera, we detect when there is a drop in front of the 
  // character.
  float dropseverity = m_pCharacter->DropAhead();
  // If downward slope ahead is too steep, return true.
  return (dropseverity > 0.7f); // TODO CONFIG
}
}
