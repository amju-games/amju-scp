/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ControlStyleDirectionalKeys.cpp,v $
Revision 1.1.8.1  2005/06/05 00:25:49  Administrator
Hacks to get this to compile with const Orientations.

Revision 1.1  2004/09/08 15:42:34  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "ControlStyleDirectionalKeys.h"
#include "Engine.h"
#include "Player.h"

namespace Amju
{
static const float NORTH = 0;
static const float SOUTH = 180;
static const float EAST = 270; // depends on orientation of fixed camera..
static const float WEST = 90;

ControlStyleDirectionalKeys::ControlStyleDirectionalKeys()
{
}

void ControlStyleDirectionalKeys::CommonKey(bool b)
{
  Engine::Instance()->GetPlayer()->ResetPieceCanRotate();

  if (Engine::Instance()->GetPlayer()->IsFalling())
  {
    return;
  }

  if (b)
  {
    // Start at walking speed. Start timer, so we break into a run after a delay.
    Engine::Instance()->GetPlayer()->SetForwardVel(m_walkForwardVel);
    m_isWalking = true;
    m_walkTime = 0; // we start running when this reaches m_maxWalkTime 
  }
  else
  {
    // Slow to stop.
    Engine::Instance()->GetPlayer()->SetForwardAccel(SLOW_TO_STOP);
    m_isWalking = false;
  }
}

void ControlStyleDirectionalKeys::PlusUp(bool b)
{
  CommonKey(b);
  //Engine::Instance()->GetPlayer()->GetOrientation()->SetYRot(NORTH);
}

void ControlStyleDirectionalKeys::PlusDown(bool b)
{
  CommonKey(b);
  //Engine::Instance()->GetPlayer()->GetOrientation()->SetYRot(SOUTH);
}

void ControlStyleDirectionalKeys::PlusLeft(bool b)
{
  CommonKey(b);
  //Engine::Instance()->GetPlayer()->GetOrientation()->SetYRot(WEST);
}

void ControlStyleDirectionalKeys::PlusRight(bool b)
{
  CommonKey(b);
  //Engine::Instance()->GetPlayer()->GetOrientation()->SetYRot(EAST);
}

}


