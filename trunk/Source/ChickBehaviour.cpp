/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ChickBehaviour.cpp,v $
Revision 1.1.10.1  2005/06/05 00:25:41  Administrator
Hacks to get this to compile with const Orientations.

Revision 1.1  2004/09/08 15:42:34  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "ChickBehaviour.h"
#include "CharacterGameObject.h"
#include "HeightServer.h"
#include "Common.h"
#include "Level.h"
#include "Engine.h"
#include "Chick.h"
#include "Shed.h"
#include "Portal.h"
#include "SchAssert.h"

namespace Amju
{
float ChickBehaviour::s_far;
float ChickBehaviour::s_near;
float ChickBehaviour::s_farRotVel;
float ChickBehaviour::s_farVel;
float ChickBehaviour::s_nearVel;
float ChickBehaviour::s_minDist;
float ChickBehaviour::s_waitBetweenUpdates;

ChickBehaviour::ChickBehaviour()
{
  m_waitBetweenUpdates = s_waitBetweenUpdates;
  // Stagger the update time of each chick to avoid peaks.
  m_updateTime = m_waitBetweenUpdates * (float)(rand()) / (float)(RAND_MAX);
}

void ChickBehaviour::Init()
{
  s_far = atof(Engine::Instance()->GetConfigValue("chick_far").c_str());
  s_near = atof(Engine::Instance()->GetConfigValue("chick_near").c_str());
  s_farRotVel = atof(Engine::Instance()->GetConfigValue("chick_f_rvel").c_str());
  s_farVel = atof(Engine::Instance()->GetConfigValue("chick_f_vel").c_str());
  s_nearVel = atof(Engine::Instance()->GetConfigValue("chick_n_vel").c_str());
  s_minDist = atof(Engine::Instance()->GetConfigValue("chick_min_dist").c_str());
  s_waitBetweenUpdates = 
    atof(Engine::Instance()->GetConfigValue("chick_update_wait").c_str());
}

void ChickBehaviour::Update()
{
  // Don't update every frame, it won't scale.
  // TODO dynamically get time to wait before updating (i.e. update less often
  // if there are lots of chicks)
  m_updateTime += Engine::Instance()->GetDeltaTime();
  if (m_updateTime < m_waitBetweenUpdates)  
  {
    return;    
  }

  m_updateTime = 0;

  Assert(m_pCharacter);
  Assert(m_pTarget.GetPtr());
  // This function defines How Chicks behave.

  // Move towards the player if we are further away than some limit.
  // Get distance to player.
  float xdiff = m_pCharacter->GetOrientation()->GetX() - 
      m_pTarget->GetBoundingSphere()->GetCentre().x;

  float zdiff = m_pCharacter->GetOrientation()->GetZ() - 
      m_pTarget->GetBoundingSphere()->GetCentre().z;

  float dSquared = xdiff * xdiff + zdiff * zdiff;
  float d = sqrt(dSquared);

  if (d > s_far) 
  {
    // The chick is too far away from the player, and does not follow.
    // TODO wander behaviour, with periodic Decide state to check for nearness to player.
    m_pCharacter->SetForwardVel(s_farVel); 
    m_pCharacter->SetYRotateVel(s_farRotVel); 
  }
  else if (d > s_near) 
  {
    // Move towards the player.
    // TODO z = 0 !!
    float requiredRot = atan2(xdiff, zdiff);
    // Must convert to degs!
    float requiredDegs = requiredRot / pi * 180.0f;
    requiredDegs += 180.0f; // need this to fix z direction ? 
    m_pCharacter->SetYRotateVel(0);
    //m_pCharacter->GetOrientation()->SetYRot(requiredDegs);
    m_pCharacter->SetForwardVel(s_nearVel); 
  }
  else
  {
    // No need to move towards the player..
    m_pCharacter->SetForwardVel(0); // TODO CONFIG
  }
  
  // Move away from any Chick we are too close to.
  // Get all Chicks in the current room.
  Assert(m_pCharacter->GetLevel());
  int levelId = m_pCharacter->GetLevel()->GetId();
  int roomId = m_pCharacter->GetRoomId();
  // Get the game objects which are in the same room as this Chick.
  GameObjectMap& objs = Engine::Instance()->GetGameObjects(levelId, roomId);

  // Iterate through map of Game Objects. 
  // For each Chick (that isn't this one, duh) increase the distance
  // between us if it is less than some limit.
  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    GameObjectId gameObjId = it->first;
    PGameObject pGo = it->second;
    Chick* pChick = dynamic_cast<Chick*>(pGo.GetPtr()); // is it a Chick ?
    if (pChick)
    {
      if (pChick != m_pCharacter)
      {
        // Found another chick. Keep to the minimum distance.
        KeepMinDistance(pChick);
      }
      continue;
    }
/*
    Shed* pShed = dynamic_cast<Shed*>(pGo); // is it a Shed ?
    if (pShed)
    {
      // Check if we are within the Shed sphere.
      if (pShed->GetBoundingSphere()->Intersects(*(m_pCharacter->GetBoundingSphere())))
      {
        ((Chick*)m_pCharacter)->SetShed(pShed);
        m_pCharacter->SetState(NEAR_SHED);
      }
      continue;
    }

    Portal* pPortal = dynamic_cast<Portal*>(pGo); // is it a portal ?
    if (pPortal)
    {
      if (pPortal->GetBoundingSphere()->Intersects(*(m_pCharacter->GetBoundingSphere())))
      {
        // Go through the portal
        pPortal->HandleObjectCollision(m_pCharacter);
        continue;
      }
    }
*/
  }
}

void ChickBehaviour::KeepMinDistance(Chick* pChick)
{
  Assert(pChick);
  Assert(m_pCharacter);
  Assert(m_pCharacter->GetOrientation());
  Assert(pChick->GetBoundingSphere());

  float xdiff = m_pCharacter->GetOrientation()->GetX() - 
      pChick->GetBoundingSphere()->GetCentre().x;

  float zdiff = m_pCharacter->GetOrientation()->GetZ() - 
      pChick->GetBoundingSphere()->GetCentre().z;

  float dSquared = xdiff * xdiff + zdiff * zdiff;
  if (dSquared < SMALLEST)
  {
    // May end up here if multiple chicks collected..?
    return;
  }

  float d = sqrt(dSquared); 
  
  if (d < s_minDist) 
  {
    // This Chick is too close. We must move away from the Chick.
    // TODO This needs fixing: simply altering the coords causes bad flickering.
    // Instead we must work out which direction to face, and then move in that
    // direction, temporarily ignoring the player.
    // Well bizarrely this seems to be OK for Chicks, I suppose because they are
    // small and hop around anyway.

    // Get Vector to move in.
    xdiff /= d;
    zdiff /= d;

    float x = m_pCharacter->GetOrientation()->GetX();
    float z = m_pCharacter->GetOrientation()->GetZ();
    float dt = Engine::Instance()->GetDeltaTime();
    x += xdiff * dt * 0.5f; // TODO CONFIG
    z += zdiff * dt * 0.5f; // TODO CONFIG
    //m_pCharacter->GetOrientation()->SetX(x);
    //m_pCharacter->GetOrientation()->SetZ(z);
  }

}
}
