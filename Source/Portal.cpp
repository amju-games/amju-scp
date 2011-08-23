/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Portal.cpp,v $
Revision 1.1.10.1  2005/05/08 08:02:29  jay
Fix GetOrientation - now returns const

Revision 1.1  2004/09/08 15:42:40  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "Portal.h"
#include "PortalDestination.h"
#include "File.h"
#include "Engine.h"
#include "Level.h"
#include "Character.h"
#include "EngineRunning.h"
#include "GameObjectFactory.h"

namespace Amju
{
/*
// Register creation function with Game Object Factory
GameObject* CreatePortal() { return new Portal; }
static const bool isPortalRegistered = GameObjectFactory::Instance()->
  RegisterFactoryFunction("portal", CreatePortal);
*/

Portal::Portal() 
{
#if defined(SCENE_EDITOR)
  // default constructed Game Object must be saveable for Editor.
  m_destId = 0; // this should be a valid ID for a PortalDestination
#endif
}

void Portal::Draw()
{
#if defined(_DEBUG)
  // Just draw the bounding sphere (only if spheres switched on)
  m_bsphere.Draw(); 
#endif
}

bool Portal::Load(File* pf)
{
  // Get b.sphere
  if (!m_bsphere.Load(pf))
  {
    return false;
  }

  // Get new orientation for player when Portal is entered
  if (!pf->GetInteger(&m_destId))
  {
    pf->ReportError("Expected ID for portal destination.");
    return false;
  }

  // We can't test m_destId here, in case it hasn't been loaded yet:-(

  return true;
}

#if defined(SCENE_EDITOR)
bool Portal::Save(File* pf)
{
  pf->WriteComment("// Portal b. sphere");
  m_bsphere.Save(pf);

  pf->WriteComment("// Portal destination ID");
  pf->WriteInteger(m_destId);

  return true; 
}

static const std::string RADIUS = "radius";
static const std::string DEST_ID = "destination object ID";

GameObject::PropertyMap Portal::GetProperties() const
{
  // TODO orientation
  PropertyMap pm;
  pm[RADIUS] = GameObject::Property(m_bsphere.GetRadius());
  pm[DEST_ID] = GameObject::Property(m_destId);
  return pm;
}

void Portal::SetProperties(const GameObject::PropertyMap& pm)
{
  // TODO orientation
  m_bsphere.SetRadius(const_cast<GameObject::PropertyMap&>(pm)[RADIUS].GetFloat());
  m_destId = const_cast<GameObject::PropertyMap&>(pm)[DEST_ID].GetInt();
}
#endif

const char* Portal::GetTypeName() const
{
  return "portal";
}

void Portal::HandlePlayerCollision(CharacterGameObject* pPlayer)
{
  if (!pPlayer)
  {
    return;
  }

  // Player has entered portal. 
  Engine::Instance()->PlayWav(Engine::Instance()->GetConfigValue("portal"));

  // Set new orientation. This happens even if the new room is the same as the
  // old room, so there is the possibility of 'teleporting' around in one room.

  PGameObject pDest = Engine::Instance()->GetGameObject(m_destId);
  
  Assert(dynamic_cast<PortalDestination*>(pDest.GetPtr()));
  
  pPlayer->SetOrientation(*(pDest->GetOrientation()));
  
  EngineRunning* er = (EngineRunning*)Engine::Instance()->
    GetEngineState(EngineRunning::Name).GetPtr();
  // Set new camera orientation
  er->GetCamera()->SetOrientation(*(pDest->GetOrientation()));
  // Set new room

  m_pLevel->SetRoomId(pDest->GetRoomId());
  //er->SetPlayerRoom(pDest->GetRoomId());
  Engine::Instance()->OnNewRoom();

}

void Portal::HandleObjectCollision(GameObject* pObj)
{
  // NPCs may also use portals. The code is slightly different to the
  // player code because the camera doesn't follow the NPC.
  if (!pObj)
  {
    return;
  }
  Engine::Instance()->PlayWav(Engine::Instance()->GetConfigValue("portal"));

  PGameObject pDest = Engine::Instance()->GetGameObject(m_destId);
  Assert(dynamic_cast<PortalDestination*>(pDest.GetPtr()));
  pObj->SetOrientation(*(pDest->GetOrientation()));
  // Set new room for object; remove from list of items in old room
  int newRoomId = pDest->GetRoomId();  
  Engine::Instance()->GameObjectChangeRoom(pObj->GetId(), newRoomId);
}

void Portal::SetOrientation(const Orientation& o)
{
  m_bsphere.SetCentre(o.GetVertex());
}

const Orientation* Portal::GetOrientation()
{
  static Orientation o;
  o.SetX(m_bsphere.x());
  o.SetY(m_bsphere.y());
  o.SetZ(m_bsphere.z());
  return &o;
}
}

