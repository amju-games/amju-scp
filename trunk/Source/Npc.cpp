/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Npc.cpp,v $
Revision 1.1  2004/09/08 15:42:53  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <string>
#include "Npc.h"
#include "File.h"
#include "CharacterManager.h"
#include "Engine.h"
#include "GameState.h"
//#include "CharacterControl.h"
#include "Behaviour.h"
#include "Level.h"

using namespace std;

namespace Amju
{
extern void ReportError(const string& error);

Npc::Npc() :  
  m_playerDamage(0)
{
  m_collideWithPlayer = false;
  m_health = 10.0f; // TODO CONFIG
  m_fade = 1.0f;
  m_pCurrentBehaviour = 0;
  m_initialState = 0;

#if defined(SCENE_EDITOR)
  // default constructed Game Object must be saveable for Editor.
#endif
}

Npc::~Npc()
{
}

void Npc::SetBehaviour(Behaviour* pB) 
{ 
  m_pCurrentBehaviour = pB; 
  m_pCurrentBehaviour->OnActivated();
}

Behaviour* Npc::GetBehaviour()
{
  return m_pCurrentBehaviour;
}

#if defined(SCENE_EDITOR)
bool Npc::Save(File* pf)
{
  CharacterGameObject::Save(pf);

  pf->WriteComment("// NPC initial orientation");
  // NB We don't save m_initialOrientation!
  // That's because it's m_orientation that gets changed in the Editor.
  // We use m_initialOrientation only in the game, to remember the initial spot.
  m_orientation.Save(pf);
  
  pf->WriteComment("// NPC initial state");
  pf->WriteInteger(0); // TODO always UNKNOWN ?
  
  pf->WriteComment("// NPC player damage");
  pf->WriteFloat(m_playerDamage);
  
  pf->WriteComment("// NPC collision vol");
  SaveCollisionVolume(pf);
  
  pf->WriteComment("// NPC bounding spheres");
  SaveBoundingSpheres(pf);
  
  pf->WriteComment("// NPC shadow size");
  pf->WriteFloat(m_shadowSize);

  return true;
}

static const std::string CHAR_NAME= "character name";
static const std::string PLAYER_DAMAGE = "player damage";
static const std::string COLLISION_VOL = "collision volume";
static const std::string SHADOW_SIZE = "shadow size";

GameObject::PropertyMap Npc::GetProperties() const
{
  // TODO bounding spheres
  PropertyMap pm;

//  pm[CHAR_NAME] = GameObject::Property(m_charName);
  pm[PLAYER_DAMAGE] = GameObject::Property(m_playerDamage);
  pm[SHADOW_SIZE] = GameObject::Property(m_shadowSize);

  std::string collisionVolName = "block1.comp"; // TODO CONFIG PREFERENCES
  if (m_pCollisionVol.GetPtr())
  {
    collisionVolName = m_pCollisionVol->GetName();
  }
  pm[COLLISION_VOL] = GameObject::Property(collisionVolName);

  return pm;
}

void Npc::SetProperties(const GameObject::PropertyMap& pm)
{
  // TODO bounding spheres

//  m_charName = const_cast<GameObject::PropertyMap&>(pm)[CHAR_NAME].GetString();
/*
  m_pCharacter = CharacterManager::Instance()->GetCharacter(m_charName);
  if (!m_pCharacter.GetPtr())
  {
    ReportError(string("Bad character name: ") + m_charName);
  }
*/
  m_playerDamage = const_cast<GameObject::PropertyMap&>(pm)[PLAYER_DAMAGE].GetFloat();
  m_shadowSize = const_cast<GameObject::PropertyMap&>(pm)[SHADOW_SIZE].GetFloat();

  std::string collisionVolName = const_cast<GameObject::PropertyMap&>(pm)[COLLISION_VOL].GetString();
  m_pCollisionVol = SolidComponent::LoadSolid(collisionVolName);
  if (!m_pCollisionVol.GetPtr())
  {
    // ERROR
  }
}
#endif

bool Npc::Load(File* pf)
{
  if (!CharacterGameObject::Load(pf))
  {
    return false;
  }

  // Get Initial orientation
  Orientation o;
  if (!o.Load(pf))
  {
    pf->ReportError("Failed to load NPC orientation.");
    return false;
  }
  SetOrientation(o);
  // Save the initial orientation, so we can reset whenever player enters room.
  m_initialOrientation = o;

  // Get initial state
  if (!pf->GetInteger(&m_initialState))
  {
    pf->ReportError("Expected NPC initial state.");
    return false;
  }

  // Get player damage: this is the amount by which the player's health is
  // reduced if this NPC collides with the player.
  if (!pf->GetFloat(&m_playerDamage))
  {
    pf->ReportError("Expected NPC player damage.");
    return false;
  }

  if (!LoadCollisionVolume(pf))
  {
    return false;
  }

  if (!LoadBoundingSpheres(pf))
  {
    return false;
  }

  if (!pf->GetFloat(&m_shadowSize))
  {
    pf->ReportError("Expected shadow radius.");
    return false;
  }

  return true;
}

void Npc::OnRoomEntry()
{
  // TODO don't perform reset if player was only out of the room for a short time..?
  Engine::Instance()->ClearMessages(GetId());

  // Reset the orientation.
  SetOrientation(m_initialOrientation);

  // Set to initial state
  SetState((State)m_initialState);

  m_fade = 1.0f;
  SetBlocked(false);
}

void Npc::HandleObjectCollision(GameObject* pObj)
{
  // TODO This should be handled by the thing collided with.
  // We only need to handle Npc-Npc collisions here.
}

void Npc::Dead()
{
  // Delete any pending messages for this guy.
  Engine::Instance()->ClearMessages(GetId());

  // Change the model state to dead. 
  SetActionDead();
  SetState(DEAD);
  
  // Set fade to transparent.
  m_fade = 1.0f;
  // Send a message to fade to transparent in a short time.
  Engine::Instance()->SendGameMessage(new Message("npc dead fade", 5, 
    GetId(), GetId(), FADING));

  // Send a message to put us completely out of play in a short time.
  Engine::Instance()->SendGameMessage(new Message("npc dead oop", 10, 
    GetId(), GetId(), OUT_OF_PLAY));
}

void Npc::HandlePlayerCollision(CharacterGameObject* pPlayer)
{
  if (GetState() == DEAD ||
      GetState() == FADING ||
      GetState() == OUT_OF_PLAY ||
      GetState() == FROZEN) // TODO this may cuase player to never be harmed
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
    pPlayer->SetActionPain(); 

    // TODO Player and/or this NPC should recoil ?
    
    //Engine::Instance()->GetEngineState()->TakeDamage(m_playerDamage);
  }
}

void Npc::Draw()
{
  // TODO Fade to transparent if dead..
  if (GetState() == FADING)
  {
    m_fade -= Engine::Instance()->GetDeltaTime() * 0.2f; // TODO CONFIG
    if (m_fade < 0)
    {
      m_fade = 0;
    }
    Engine::Instance()->PushColour(1, 1, 1, m_fade);
  }
  if (GetState() == FROZEN)
  {
    Engine::Instance()->PushColour(0.5f, 0.5f, 1.0f, 1.0f);
  }
  CharacterGameObject::Draw();
  if (GetState() == FROZEN)
  {
    Engine::Instance()->PopColour();
  }

  if (GetState() == FADING)
  {
    Engine::Instance()->PopColour();
  }

#if defined(_DEBUG)
  if (GetBehaviour())
  {
    GetBehaviour()->Draw();
  }
#endif
}

void Npc::Update()
{
  // Check for collisions with other game objects - mainly we are concerned with
  // other NPCs and the player.
/*
  // Not required.. for golf we only care about ball collisions.
  CheckCharacterCollisions();
*/

  if (GetState() == DEAD || GetState() == FADING)
  {
    SetForwardVel(0);
    SetYRotateVel(0);
  }
  else if (m_pCurrentBehaviour)
  {
    m_pCurrentBehaviour->SetNpc(this);
    m_pCurrentBehaviour->Update();
  }

  if (GetState() != FADING)
  {
    CharacterGameObject::Update();
  }
}

void Npc::CheckCharacterCollisions()
{
  // Check if this NPC has collided with another character. If so, adjust so 
  // they don't occupy the same space.
  // Iterate over the game objects which are in the current level/room.
  int levelId = m_pLevel->GetId();
  int roomId = m_pLevel->GetRoomId();

  // Get the game objects which are in the same room as the player.
  GameObjectMap& objs = Engine::Instance()->GetGameObjects(levelId, roomId);
    
  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    PGameObject pGo = it->second;

    State s = pGo->GetState();
    //Engine::Instance()->GetEngineState()->GetState(gameObjId, &s);
    if (s != OUT_OF_PLAY)
    {
      PGameObject pGo = it->second;
      if ((GameObject*)this != pGo.GetPtr() && this->Intersects(*(pGo.GetPtr())))
      {
        // TODO we want one unambigous function which is called once for the
        // pair of objects which have collided.
        HandleObjectCollision(pGo.GetPtr());
        // Symmetrical call. 
        pGo->HandleObjectCollision(this);
      }
    }
  }
}

}

