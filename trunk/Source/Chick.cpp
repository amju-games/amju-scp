/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Chick.cpp,v $
Revision 1.1  2004/09/08 15:42:34  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <string>
#include "Chick.h"
#include "Engine.h"
#include "Shed.h"
#include "Egg.h"
#include "Player.h"
#include "GameState.h"
#include "CrocHome.h" // TODO TEMP TEST
#include "Level.h"
#include "SchAssert.h"

using namespace std;
namespace Amju
{
Chick::Chick()
{
  m_playerDamage = 0;
  m_shadowSize = atof(Engine::Instance()->GetConfigValue("chick_shadow").c_str());

  // Size of Chick bounding sphere
  m_radius = 0.3f;
  m_sphereYOffset = 0.6f; // distance from ground to sphere centre
  m_eggId = 0; // set when this Chick is created in Egg hatch

  m_shedBehaviour.SetNpc(this);
}

void Chick::DrawShadow() const
{
  if (GetState() == CAPTURED)
  {
    return;
  }
  Npc::DrawShadow();
}

void Chick::Hatch()
{
  // Decide what kind of Chick we are.
  // Set Behaviour and Character Skin accordingly.
  // Different kinds of Chicks have different flocking characteristics.
  // (This could be done by setting behavour parameters rather than creating
  //  different Behaviour objects..??)
}

PGameObject Chick::FindShed()
{
  // Return the nearest Shed in the current level/room if one exists.
  // TODO use Collision/Interaction manager to do this.
  float bestDist = 99999.0f; 

  GameObject* pResult = 0;

  // Get the game objects which are in the same room as the player.
  int levelId = m_pLevel->GetId();
  int roomId = m_pLevel->GetRoomId();
  GameObjectMap& objs = Engine::Instance()->GetGameObjects(levelId, roomId);
  // Iterate through map of Game Objects. 
  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    PGameObject pGo = it->second;

    // TODO using CrocHome!!! We should use Shed, or ChickHome!
    CrocHome* pHome = dynamic_cast<CrocHome*>(pGo.GetPtr());
    if (pHome)
    {
      // It's a possible Home - get the distance 
      float x = GetPosition().x - pHome->GetBoundingSphere()->GetCentre().x;
      float z = GetPosition().z - pHome->GetBoundingSphere()->GetCentre().z;
      float dist = x * x + z * z;
      // No point in taking sqrt.
      if (dist < bestDist)
      {
        bestDist = dist;
        pResult = pGo.GetPtr();
      }
    }
  }
  return pResult;

}

void Chick::SetState(State s)
{
  if (GetState() == s)
  {
    // Hopefully prevent backlog of messages
    return;
  }

  Npc::SetState(s);

  if (s == JUST_HATCHED)
  {
    Hatch();
  }
  else if (s == OUT_OF_PLAY)
  {
    // Once this chick is eaten or saved, we can regenerate the Egg it hatched out of.
    Assert((Engine::Instance()->GetGameObject(m_eggId)).GetPtr()); 
    Assert(dynamic_cast<Egg*>(Engine::Instance()->GetGameObject(m_eggId).GetPtr()));

    bool canRegenerate = 
      ((Egg*)(Engine::Instance()->GetGameObject(m_eggId).GetPtr()))->CanRegenerate();

    if (canRegenerate) 
    {   
      // TODO (random) time delay ?
      // Send message to egg to regenerate.
      Engine::Instance()->SendGameMessage(new Message("egg regenerate", 10, // TODO CONFIG
        GetId(), m_eggId, UNKNOWN));
    }
  }
/*
  else if (s == NEAR_SHED)
  {
    Assert(m_pShed);
    // Initially the target is the bottom of the Shed's ramp.
    //GameObject* pRamp = m_pShed->GetRamp();
    m_shedBehaviour.SetTarget(m_pShed);
    SetBehaviour(&m_shedBehaviour);
  }
  else if (s == ON_RAMP)
  {
    // Now we want to head for the inside of the Shed.
    Assert(m_pShed);
    //GameObject* pInside = m_pShed->GetInside();
    m_shedBehaviour.SetTarget(m_pShed);
    SetBehaviour(&m_shedBehaviour);
  }
  else if (s == CAPTURED)
  {
    // Chick has been caught by a bad NPC. The NPC carries the Chick, unless the 
    // player rescues it!
    SetBehaviour(&m_capturedBehaviour);
  }
*/
  else
  {
    // Decide what to do
    Decide();
  }
  // TODO DEAD behaviour ?
}

void Chick::Decide()
{
  PGameObject pShed = FindShed();
  if (pShed.GetPtr())
  {
    m_shedBehaviour.SetTarget(pShed);
    if (m_shedBehaviour.CreatePath() == SearchOk)
    {
      SetBehaviour(&m_shedBehaviour);
      return;
    }
  }
  SetBehaviour(&m_normalBehaviour);
  m_normalBehaviour.SetTarget(Engine::Instance()->GetPlayer());
}

void Chick::OnRoomEntry()
{
}

void Chick::Update()
{
/*
  if (GetState() == CAPTURED)
  {
    // If captured, Chick has no control over movement.
    // So don't call base class Update(), it will do strange things.
    GetBehaviour()->SetNpc(this);
    GetBehaviour()->Update();
    CheckCharacterCollisions();
    return;
  }
*/

  Npc::Update();
}

void Chick::Draw()
{
  // Draw translucent if captured ?
  /*
  if (GetState() == CAPTURED)
  {
    Engine::Instance()->PushColour(1.0f, 1.0f, 1.0f, 0.5f);
  }
  */
  Npc::Draw();
  /*
  if (GetState() == CAPTURED)
  {
    Engine::Instance()->PopColour(); 
  }
  */
}

void Chick::HandlePlayerCollision(CharacterGameObject* pPlayer)
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
    // Player collects the chick.
    //pPlayer->HandleObjectCollision(this);
    // Collect the chick.
    //SetState(OUT_OF_PLAY);
    // TODO sound effect
  }
}

void Chick::HandleObjectCollision(GameObject* pObj)
{
/*
  Shed* pShed = dynamic_cast<Shed*>(pObj);
  if (pShed)
  {
    pShed->HandleObjectCollision(this);
  }
*/
}

/*
void Chick::SetCaptor(GameObject* pCaptor)
{
  // Captured behaviour treats its Target as the thing which has captured the chick.
  m_capturedBehaviour.SetTarget(pCaptor);
}

GameObject* Chick::GetCaptor()
{
  if (GetState() != CAPTURED)
  {
    return 0;
  }
  return m_capturedBehaviour.GetTarget();
}

void Chick::SetCaptureHeight(float h)
{
  m_capturedBehaviour.SetHeight(h);
}
*/

void Chick::Win()
{
  // Player saves/Wins this Chick.
  // Chick Game Object goes out of play. Score is increased.
  // We should do a really good effect too.
  Engine::Instance()->PlayWav(Engine::Instance()->GetConfigValue("wav_chick_safe"));

/*
  Engine::Instance()->SendGameMessage(new Message("chick safe!", 
      0, // time delay
      GetId(),
      GetId(),
      OUT_OF_PLAY));
*/
  SetState(OUT_OF_PLAY);

  // Get points for saving chick
  //Engine::Instance()->GetGameState()->AddScore(100); // TODO CONFIG

  // Clear the saved chick from the Player chick list
  //Engine::Instance()->GetPlayer()->ClearChick(this);

  // Decrement number of chicks left to collect 
//  int numChicks = Engine::Instance()->GetGameState()->GetNumChicks();
//  numChicks--;
//  Assert(numChicks >= 0); // calling this function multiple times for one chick ?
//  Engine::Instance()->GetGameState()->SetNumChicks(numChicks);

  // If numChicks is zero, the level is finished!
  /*
  if (numChicks == 0)
  {
    Engine::Instance()->ClearMessages(0); // clear pending player messages
    Engine::Instance()->GetPlayer()->SetYRotateVel(0);
    Engine::Instance()->GetPlayer()->SetForwardAccel(0);
    Engine::Instance()->GetPlayer()->SetForwardVel(0);
    // TODO a celebratory end-of-level animation

    Engine::Instance()->LevelCompleted();
  }
  */
}

void Chick::Lose()
{
  // This chick is killed, so player loses this one.
  // Play a wav. 
  string deadChickWav = Engine::Instance()->GetConfigValue("wav_dead_chick");
  Engine::Instance()->PlayWav(deadChickWav);

  // Send message rather than directly setting Chick state ?

#if defined(_DEBUG)
/*
  cout << "Chick (ID " << GetId() << ") is DEAD!" << endl;
*/
#endif

  SetState(OUT_OF_PLAY); // NB not DEAD, leads to problems :-(

  // Penalize the player:
  // Increase the number of chicks which must be collected for the current level.
//  int numChicks = Engine::Instance()->GetGameState()->GetNumChicks();
//  numChicks++; // TODO CONFIG
//  Engine::Instance()->GetGameState()->SetNumChicks(numChicks);
}

void Chick::RefreshHeightServer()
{
  // We want chicks to follow paths along things we have built with
  // takeables. So we must do things the slow way.
  Npc::RefreshHeightServer();

/*
  m_pHeightPoly = 0; // TODO
  VisibleGameObject::RefreshHeightServer(&m_heightServerWithPlatforms);

  // We deliberately ignore Game Objects for speed. This means that Chicks will
  // pass through Game Objects, e.g. crates, SortScenes, etc.
*/
}

}

