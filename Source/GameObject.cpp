/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: GameObject.cpp,v $
Revision 1.1.10.4  2007/12/10 22:58:12  jay
Turn off some debug output

Revision 1.1.10.3  2007/06/23 11:43:15  Administrator
MSVC fix

Revision 1.1.10.2  2007/06/17 20:37:06  jay
POOL ONLINE: Add flag to GameObject to send state to server this frame.
Moved from PoolBall because other types may need to send their state, e.g. Rules

Revision 1.1.10.1  2005/05/08 08:03:41  jay
Fix GetOrientation - now returns const

Revision 1.1  2004/09/08 15:42:50  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "GameObject.h"
#include "File.h"
#include "CompositeFactory.h"
#include "Engine.h"
#include "Message.h"
#include "Fsm.h"
#include "StringUtils.h"

namespace Amju
{
GameObject::GameObject() :
  m_room(0),
  m_pLevel(0),
  m_isTranslucent(false),
  m_sendStateThisFrame(false)
{
  SetState(UNKNOWN);
}

GameObject::~GameObject()
{
}

bool GameObject::SendStateThisFrame() const
{
  return m_sendStateThisFrame;
}

void GameObject::SetSendState(bool send)
{
#ifdef SET_SEND_DEBUG
  if (send)
  {
    std::string s = "********** SET SEND STATE for object ";
    s += Amju::ToString(GetId());
    std::cout << s.c_str() << "\n";
  }
#endif

  m_sendStateThisFrame = send;
}

bool GameObject::Intersects(const GameObject& g) 
{
  // Return true if the bounding sphere of the other Game Object intersects the
  // bounding sphere of this one.
  const BoundingSphere* pBs = const_cast<GameObject&>(g).GetBoundingSphere();
  if (GetBoundingSphere() && pBs)
  {
    return GetBoundingSphere()->Intersects(*pBs);
  }
  return false;
}

void GameObject::DrawSphere()
{
  BoundingSphere* pBs = const_cast<BoundingSphere*>(GetBoundingSphere());
  if (pBs)
  {
    pBs->Draw();
  }
}

const Orientation* GameObject::GetOrientation()
{
  return 0;
}

VertexBase GameObject::GetPosition() const
{
  Assert(const_cast<GameObject*>(this)->GetOrientation());
  return const_cast<GameObject*>(this)->GetOrientation()->GetVertex();
}

bool GameObject::IsTranslucent() const
{
  return m_isTranslucent;
}

bool GameObject::Load(const std::string& filename)
{
  File f;
  if (!f.OpenRead(filename))
  {
    return false;
  }
  return Load(&f);
}

void GameObject::DrawEditSolid()
{
  static PSolidComponent pComp;
  if (!pComp.GetPtr())
  {
    pComp = SolidComponent::LoadSolid(Engine::Instance()->GetConfigValue("edit_solid"));
  }
  if (GetOrientation())
  {
    GetOrientation()->Draw();
  }
  if (pComp.GetPtr())
  {
    pComp->Draw();
  }
}
}

