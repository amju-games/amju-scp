/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Croc.cpp,v $
Revision 1.1  2004/09/08 15:42:35  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <iostream>
#include "Croc.h"
#include "Engine.h"
#include "EngineRunning.h"
#include "Message.h"
#include "Target.h"
#include "Level.h"
#include "CrocHome.h"
#include "GameObjectFactory.h"
#include "Character.h"
#include "SchAssert.h"

using namespace std;
namespace Amju
{
/*
// Register creation function with Game Object Factory
GameObject* CreateCroc() { return new Croc; }
static const bool isCrocRegistered = GameObjectFactory::Instance()->
  RegisterFactoryFunction("croc", CreateCroc);
*/

Croc::Croc()
{
  m_idleBehaviour.SetNpc(this);
  m_idleBehaviour.SetTarget(0);

  m_chaseBehaviour.Init();
  m_chaseBehaviour.SetNpc(this);
  m_chaseBehaviour.SetTarget(0);

  m_runHomeBehaviour.Init();
  m_runHomeBehaviour.SetNpc(this);
  m_runHomeBehaviour.SetTarget(0);

  m_frozenBehaviour.SetNpc(this);
  m_frozenBehaviour.SetTarget(0);

  m_testBehaviour.SetNpc(this);
  m_testBehaviour.SetTarget(0);

/*
  m_decideBehaviour.Init();
  m_decideBehaviour.SetNpc(this);
  m_decideBehaviour.SetTarget(0);
*/

  SetBehaviour(&m_idleBehaviour);
//  m_pCurrentBehaviour = &m_chaseBehaviour;
//  m_pCurrentBehaviour = &m_testBehaviour;

  m_health = 1.0f;

#if defined(SCENE_EDITOR)
  // default constructed Game Object must be saveable for Editor.
//  m_charName = "croc";
#endif

}

const char* Croc::GetTypeName() const
{
  return "croc";
}

void Croc::Draw()
{
  if (GetState() == FROZEN)
  {
    Engine::Instance()->PushColour(0.5f, 0.5f, 1.0f, 1.0f); // TODO CONFIG
  }
  
  Npc::Draw();

  if (GetState() == FROZEN)
  {
    Engine::Instance()->PopColour();
  }
}

void Croc::Dead()
{
  // This next call clears any pending messages.
  Npc::Dead();
  // Send message to self to regenerate at the Croc home.
  // Time delay must be longer than those in Npc::Dead()
  Engine::Instance()->SendGameMessage(new Message("npc rise from dead", 12, 
    GetId(), GetId(), RISE_FROM_DEAD));

  //FreeChicks();
}

void Croc::SetState(State newState)
{
  State prevState = GetState();

  if (prevState == newState)
  {
    // Hopefully prevent backlog of messages
    return;
  }

  // NB We must take care to discard messages which no longer apply to the current
  // State. Otherwise we get all kinds of hard-to-find bugs :-(
  // TODO wish I could think of a nicer way of doing this :-(

  if (newState == RISE_FROM_DEAD)
  {
    if (prevState != OUT_OF_PLAY)
    {
#if defined(_DEBUG)
      cout << "Croc discarding RISE_FROM_DEAD message" << endl;
#endif
      return;
    }

    Npc::SetState(newState); 

    m_health = 1.0f; // back to full health
    OnRoomEntry(); // back to start position etc
    SetState(UNKNOWN);
    SetActionIdle();
  }
  else if (newState == CHASE)
  {
    if (prevState != UNKNOWN && 
        prevState != RUN_HOME)
    {
#if defined(_DEBUG)
      cout << "Croc discarding CHASE message" << endl;
#endif
      return;
    }

#if defined(_DEBUG)
    cout << "Croc set to CHASE\n";
#endif

    Npc::SetState(newState); 

    SetBehaviour(&m_chaseBehaviour);
  }
  else if (newState == FROZEN)
  {
    if (prevState != UNKNOWN && 
        prevState != RUN_HOME &&
        prevState != CHASE) 
    {
#if defined(_DEBUG)
      cout << "Croc discarding FROZEN message" << endl;
#endif
      return;
    }

#if defined(_DEBUG)
    cout << "Croc set to FROZEN\n";
#endif

    Npc::SetState(newState); 

    SetBehaviour(&m_frozenBehaviour);
  }
  else if (newState == UNKNOWN)
  {
    if (prevState != RUN_HOME &&
        prevState != CHASE &&
        prevState != FROZEN && 
        prevState != RISE_FROM_DEAD) 
    {
#if defined(_DEBUG)
      cout << "Croc discarding UNKNOWN message" << endl;
#endif
      return;
    }

#if defined(_DEBUG)
    cout << "Croc set to UNKNOWN\n";
#endif
    Npc::SetState(newState); 
    SetBehaviour(&m_idleBehaviour);
  }
  else if (newState == RUN_HOME)
  {
    if (prevState != UNKNOWN &&
        prevState != CHASE /* && prevState != UNKNOWN */ ) 
    {
#if defined(_DEBUG)
      cout << "Croc discarding RUN_HOME message" << endl;
#endif
      return;
    }

#if defined(_DEBUG)
    cout << "Croc set to RUN_HOME\n";
#endif

    GameObject* pHome = GetHome();
    Assert(pHome);
    m_runHomeBehaviour.SetTarget(pHome);

    Npc::SetState(newState); 
    SetBehaviour(&m_runHomeBehaviour);
  }
  else if (newState == DEAD ||
           newState == FADING ||
           newState == OUT_OF_PLAY)
  {
    Npc::SetState(newState); 
  }
  else
  {
#if defined(_DEBUG)
    cout << "Croc: unhandled state change:" << newState << endl;
#endif
  }
}

void Croc::SetTarget(GameObject* pGo)
{
//  m_idleBehaviour.SetTarget(pGo);
  m_chaseBehaviour.SetTarget(pGo);
//  m_runHomeBehaviour.SetTarget(pGo);
}

void Croc::HandleObjectCollision(GameObject* pObj)
{
  // Check to see if we have collided with a chick. If so we snatch it!
  Chick* pChick = dynamic_cast<Chick*>(pObj);
  if (pChick && 
      pChick->GetState() != JUST_HATCHED && 
      pChick->GetState() != CAPTURED &&
      pChick->GetState() != OUT_OF_PLAY &&
      GetState() != FROZEN &&
      GetState() != DEAD &&
      GetState() != FADING && 
      GetState() != RISE_FROM_DEAD && 
      GetState() != OUT_OF_PLAY)
  {
    // Now the Croc should take the Chick away and eat it!
    pChick->Lose();

    // TODO eat chick animation

    // TODO Go to this state after Eat animation is over.
    // There may be another Chick close by. So we want to go to the Decision state
    // again to find the closest one.
    m_chaseBehaviour.ForceDecision();

/*
    // Set the Chick state to captured. Set the Croc state so it runs home.
    CaptureChick(pChick);
    pChick->SetCaptor(this);

    // Our behaviour should now change to running home. So the Target is now Home.
    // Each room with Crocs should have a Home for them.
    GameObject* pHome = GetHome();
    m_runHomeBehaviour.SetTarget(pHome);
    SetState(RUN_HOME);
*/
    return;
  }

/*
  CrocHome* pHome = dynamic_cast<CrocHome*>(pObj);
  if (pHome)
  {
    // Any Chicks are now dead. 
    // TODO Change the state to what ?
    KillChicks();
    SetState(UNKNOWN);
  }
*/
}

void Croc::HandlePlayerCollision(CharacterGameObject* pPlayer)
{
  if (GetState() == DEAD || GetState() == FADING)
  {
    return;
  }

  if (!pPlayer)
  {
    m_collideWithPlayer = false;
    return;
  }

  if (!m_collideWithPlayer)
  {
    m_collideWithPlayer = true;

    // Clear any pending messages
    Engine::Instance()->ClearMessages(GetId());
    // Set the NPC state to frozen, to give the player a chance to get away.
    SetState(FROZEN);
    // Send message to self, to reset state in a few seconds
    Engine::Instance()->SendGameMessage(new Message("npc collide", 1, // TODO CONFIG
      GetId(), GetId(), UNKNOWN));

    //FreeChicks(); // this croc drops any chicks carried
    //((Player*)pPlayer)->FreeChicks(); // player drops chicks too

    pPlayer->SetActionPain();
//    Engine::Instance()->GetGameState()->TakeDamage(m_playerDamage);
  }
}

GameObject* Croc::GetHome()
{
  // Search for Home objects in the current room. 
  // Pick the FURTHEST one. 

  float bestDist = 0; 

  GameObject* pResult = 0;

  // Get the game objects which are in the same room as the player.
  int levelId = m_pLevel->GetId();
  int roomId = m_pLevel->GetRoomId();
  GameObjectMap& objs = Engine::Instance()->GetGameObjects(levelId, roomId);
  // Iterate through map of Game Objects. 
  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    PGameObject pGo = it->second;
    CrocHome* pHome = dynamic_cast<CrocHome*>(pGo.GetPtr());
    if (pHome)
    {
      // It's a possible Home - get the distance between it and this Croc.
      float x = GetPosition().x - pHome->GetBoundingSphere()->GetCentre().x;
      float z = GetPosition().z - pHome->GetBoundingSphere()->GetCentre().z;
      float dist = x * x + z * z;
      // No point in taking sqrt.
      if (dist > bestDist)
      {
        bestDist = dist;
        pResult = pGo.GetPtr();
      }
    }
  }
  return pResult;
}

void Croc::KillChicks()
{
/*
  if (m_chicklist.size() == 0)
  {
    return;
  }

  // Set state of all Chicks carried to dead.
  for (ChickList::iterator it = m_chicklist.begin(); it != m_chicklist.end(); ++it)
  {
    Chick* pChick = *it;
    Assert(pChick);
    pChick->Lose();
  }

  m_chicklist.clear();
*/
}
}
