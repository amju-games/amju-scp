/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: FightBehaviour.cpp,v $
Revision 1.1.10.1  2005/06/05 00:25:59  Administrator
Hacks to get this to compile with const Orientations.

Revision 1.1  2004/09/08 15:42:38  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "FightBehaviour.h"
#include "CharacterGameObject.h"
#include "HeightServer.h"
#include "Common.h"
#include "Engine.h"
#include "EngineRunning.h"
#include "SchAssert.h"

namespace Amju
{
//float FightBehaviour::s_farDistance;

FightBehaviour::FightBehaviour()
{
  m_shootTimer = 0;
}

void FightBehaviour::Init()
{
  //s_farDistance = atof(Engine::Instance()->GetConfigValue("croc_far_dist").c_str());
}

void FightBehaviour::Update()
{
  // Point in direction of target. We don't change the angle directly, but change
  // the rotate vel.
  float yrot = m_pCharacter->GetOrientation()->GetYRot(); // NPC angle

  float xdiff = m_pCharacter->GetOrientation()->GetX() - 
      m_pTarget->GetBoundingSphere()->GetCentre().x;

  float zdiff = m_pCharacter->GetOrientation()->GetZ() - 
      m_pTarget->GetBoundingSphere()->GetCentre().z;

  float dSquared = xdiff * xdiff + zdiff * zdiff;
  float d = sqrt(dSquared);

  if (d > 4.0f) // TODO CONFIG
  {
    // Get angle we _should_ be facing in to chase target.
    // Say hello to arctan..
    // TODO z = 0 !!
    float requiredRot = atan2(xdiff, zdiff);
    // Must convert to degs!
    float requiredDegs = requiredRot / pi * 180.0f;
    requiredDegs += 180.0f; // need this to fix z direction ? 

    // Get the diff between character direction and target direction.
    float targetDir = requiredDegs;
    Assert(targetDir >= 0);
    Assert(targetDir < 360.0f);
    float charDir = m_pCharacter->GetOrientation()->GetYRot();
    charDir += 3600.0f;
    charDir = charDir - 360.0f * (float)(int)(charDir/360);

    // If within 5 degrees of the right direction, run towards target.
    if (fabs(charDir - targetDir) < 5.0f) // TODO CONFIG
    {
      //m_pCharacter->GetOrientation()->SetYRot(requiredDegs);
      m_pCharacter->SetForwardVel(4.0f); // TODO CONFIG
      // Don't alter course
     m_pCharacter->SetYRotateVel(0); 
    }
    else
    {
      // Rotate to face target.
      m_pCharacter->SetForwardVel(1.0f); // TODO CONFIG
      // Set Rotate speed so we will eventually be facing the player.
      m_pCharacter->SetYRotateVel(290); // TODO CONFIG 
    }
  }
  else // closer than 4.0
  {
    // Shoot a missile at the target.
    // TODO aim at target
    m_shootTimer += Engine::Instance()->GetDeltaTime();
    if (m_shootTimer > 0.5f) // TODO CONFIG
    {
      m_shootTimer = 0;
/*
      ((EngineRunning*)(Engine::Instance()->GetEngineState(EngineRunning::Name).GetPtr()))->
        FireMissile(m_pCharacter);
*/
    }

    m_pCharacter->SetForwardVel(0.2f); // TODO CONFIG

  }
}
}

