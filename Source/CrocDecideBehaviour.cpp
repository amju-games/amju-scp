/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: CrocDecideBehaviour.cpp,v $
Revision 1.1  2004/09/08 15:42:35  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "CrocDecideBehaviour.h"
#include "CharacterGameObject.h"
#include "Common.h"
#include "Level.h"
#include "Engine.h"
#include "Chick.h"
#include "Croc.h"
#include "SchAssert.h"

namespace Amju
{
float CrocDecideBehaviour::s_idleChaseDistance;
float CrocDecideBehaviour::s_runHomeChaseDistance;

CrocDecideBehaviour::CrocDecideBehaviour() : m_period(2.0f) /* some default */, m_timer(0)
{
}

void CrocDecideBehaviour::SetUpdatePeriod(float period)
{
  m_period = period;
}

void CrocDecideBehaviour::Init()
{
  s_idleChaseDistance = atof(Engine::Instance()->GetConfigValue("croc_idle_chase_dist").c_str());
  s_runHomeChaseDistance = atof(Engine::Instance()->GetConfigValue("croc_run_home_chase_dist").c_str());
}


void CrocDecideBehaviour::ForceUpdate()
{
  m_timer = m_period;   // force a decision now by making the timer expire.
  Update();
}

void CrocDecideBehaviour::Update()
{
  m_timer += Engine::Instance()->GetDeltaTime();
  if (m_timer < m_period)
  {
    return;
  }
  m_timer = 0;

  // Chase a chick if there is one close enough.
  // Iterate through the Chicks in this room. Find the one that is closest. 
  // If it is closer than some limit, chase it.
  Assert(m_pCharacter);
  Assert(m_pCharacter->GetLevel());

  int levelId = m_pCharacter->GetLevel()->GetId();
  int roomId = m_pCharacter->GetRoomId();
  // Get the game objects which are in the same room as this NPC.
  GameObjectMap& objs = Engine::Instance()->GetGameObjects(levelId, roomId);

  // Iterate through map of Game Objects. 
  // For each Chick check the distance. If we are closer than some distance,
  // chase the chick.
  // The chase distance depends on what we're already doing. If we're just
  // idle, the distance is greater than if we are going home.
  float chaseDistance = s_idleChaseDistance;
  if (m_pCharacter->GetState() == RUN_HOME)
  {
    chaseDistance = s_runHomeChaseDistance;
  }

  const int id = m_pCharacter->GetId(); // used when sending messages

  bool foundChick = false;
  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    GameObjectId gameObjId = it->first;
    PGameObject pGo = it->second;
    Chick* pChick = dynamic_cast<Chick*>(pGo.GetPtr());
    if (!pChick)
    {
      continue;
    }

    // Don't bother with this chick unless it's in normal (UNKNOWN) state.
    // Also consider chasing it if the player has captured it..
    // TODO This is no good, another NPC could have captured it. 
    // Could be interesting.
    if (!(pChick->GetState() == UNKNOWN || pChick->GetState() == CAPTURED))
    {
      continue;
    }

    Assert(m_pCharacter->GetOrientation());
    Assert(pChick->GetBoundingSphere());

    float xdiff = m_pCharacter->GetOrientation()->GetX() - 
      pChick->GetBoundingSphere()->GetCentre().x;

    float zdiff = m_pCharacter->GetOrientation()->GetZ() - 
      pChick->GetBoundingSphere()->GetCentre().z;

    float dSquared = xdiff * xdiff + zdiff * zdiff;
    float d = sqrt(dSquared); 
    if (d < chaseDistance) 
    {
      // TODO Test for unobstructed view.
      // Chase this Chick. 
      ((Npc*)m_pCharacter)->SetTarget(pChick);
      // There may be a closer chick. So reduce the chase distance and keep searching.
      chaseDistance = d;
      foundChick = true;
    }
  }

  if (foundChick)
  {
    // Send a message to the Croc to change state.
    Engine::Instance()->SendGameMessage(new Message("start chasing", 0, id, id, CHASE));
  }
  else
  {
    // No chick to chase, so run home.
    // The idea is that Crocs are always on the move, rather than standing 
    // around like lemons.
    Engine::Instance()->SendGameMessage(new Message("idle", 0, id, id, RUN_HOME));
    //Engine::Instance()->SendGameMessage(new Message("idle", 0, id, id, UNKNOWN));
  }
}
}
