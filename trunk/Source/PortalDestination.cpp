/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: PortalDestination.cpp,v $
Revision 1.1.10.1  2005/05/08 08:02:29  jay
Fix GetOrientation - now returns const

Revision 1.1  2004/09/08 15:42:40  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "PortalDestination.h"

namespace Amju
{
void PortalDestination::Draw()
{
#if defined(_DEBUG)
  const_cast<BoundingSphere*>(GetBoundingSphere())->Draw();
#endif
}

const BoundingSphere* PortalDestination::GetBoundingSphere() const
{
  static BoundingSphere bs;
  bs.SetCentre(m_or.GetVertex());
  bs.SetRadius(1.0f); // just some notional radius
  return &bs;
}

void PortalDestination::CreateBoundingSphere(const Matrix& cm)
{
}

const char* PortalDestination::GetTypeName() const
{
  return "portaldest";
}

bool PortalDestination::Load(File* pf)
{
  return m_or.Load(pf);
}

void PortalDestination::SetOrientation(const Orientation& o)
{
  m_or = o;
}

const Orientation* PortalDestination::GetOrientation()
{
  return &m_or;
}

#if defined(SCENE_EDITOR)
bool PortalDestination::Save(File* pf)
{
  return m_or.Save(pf);
}

GameObject::PropertyMap PortalDestination::GetProperties() const
{
  // We only care about orientation - no other properties!
  PropertyMap pm;
  return pm;
}

void PortalDestination::SetProperties(const GameObject::PropertyMap& pm)
{
}

#endif
}

