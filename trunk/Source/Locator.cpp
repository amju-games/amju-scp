/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Locator.cpp,v $
Revision 1.1.10.1  2005/05/08 08:03:42  jay
Fix GetOrientation - now returns const

Revision 1.1  2004/09/08 15:42:52  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "Locator.h"
#include "File.h"
#include "Vertex.h"
#include "Engine.h"
#include "CharacterGameObject.h"
#include "GameObjectFactory.h"

namespace Amju
{
/*
// Register creation function with Game Object Factory
GameObject* CreateLocator() { return new Locator; }
static const bool isLocatorRegistered = GameObjectFactory::Instance()->
  RegisterFactoryFunction("locator", CreateLocator);
*/
Locator::Locator() : 
  m_reactId(-1)
{
#if defined(SCENE_EDITOR)
  // TODO default constructed Game Object must be saveable for Editor.
#endif
}

void Locator::Draw()
{
  m_bsphere.Draw(); 
}

void Locator::SendMessages()
{
  m_messages.SendMessages();
}

bool Locator::Load(File* pf)
{
  // Get Reaction Object ID
  if (!pf->GetInteger(&m_reactId))
  {
    pf->ReportError("Expected locator reaction-object ID.");
    return false;
  }
  // Get b.sphere
  if (!m_bsphere.Load(pf))
  {
    return false;
  }

  // Now load the messages which will be sent when the activation object hits.
  return m_messages.Load(pf, GetId());  
}

#if defined(SCENE_EDITOR)
bool Locator::Save(File* pf)
{
  pf->WriteComment("// Locator: ID of object we react to, then sphere");
  pf->WriteInteger(m_reactId);
  m_bsphere.Save(pf);

  pf->WriteComment("// Locator: message list");
  m_messages.Save(pf);

  return true; 
}

static const std::string RADIUS = "radius";
static const std::string REACT_ID = "react ID";

GameObject::PropertyMap Locator::GetProperties() const
{
  PropertyMap pm;
  pm[RADIUS] = GameObject::Property(m_bsphere.GetRadius());
  pm[REACT_ID] = GameObject::Property(m_reactId);

  // TODO message list

  return pm;
}

void Locator::SetProperties(const GameObject::PropertyMap& pm)
{
  m_bsphere.SetRadius(const_cast<GameObject::PropertyMap&>(pm)[RADIUS].GetFloat());
  m_reactId = (int)const_cast<GameObject::PropertyMap&>(pm)[RADIUS].GetFloat();

  // TODO message list
}
#endif

const char* Locator::GetTypeName() const
{
  return "locator";
}

void Locator::HandlePlayerCollision(CharacterGameObject* pPlayer)
{
  HandleObjectCollision(pPlayer);
}

void Locator::HandleObjectCollision(GameObject* pObj)
{
  if (GetState() == OUT_OF_PLAY)
  {
    return;
  }

  // Is the object we collided with the one special object which activates 
  // this locator ?
  if (pObj->GetId() == m_reactId)
  {
    SetState(OUT_OF_PLAY); // must reset with message

    // Yes - send activation messages.
    SendMessages();
  }
}

void Locator::SetOrientation(const Orientation& o)
{
  m_bsphere.SetCentre(o.GetVertex());
}

const Orientation* Locator::GetOrientation()
{
  static Orientation s_o;
  s_o.SetX(m_bsphere.x());
  s_o.SetY(m_bsphere.y());
  s_o.SetZ(m_bsphere.z());
  return &s_o;
}

}

