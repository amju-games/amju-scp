/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Target.cpp,v $
Revision 1.1.10.1  2005/05/08 08:03:45  jay
Fix GetOrientation - now returns const

Revision 1.1  2004/09/08 15:42:57  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "Target.h"
#include "File.h"

namespace Amju
{
Target::Target()
{
#if defined(SCENE_EDITOR)
  // TODO default constructed Game Object must be saveable for Editor.
#endif
}

void Target::Draw()
{
#if defined(_DEBUG)
  m_bsphere.Draw();
#endif
}

const BoundingSphere* Target::GetBoundingSphere() const
{
  return &m_bsphere;
}

void Target::CreateBoundingSphere(const Matrix& cm)
{
}

bool Target::Load(File* pf)
{
  return m_bsphere.Load(pf);
}

#if defined(SCENE_EDITOR)
bool Target::Save(File* pf)
{
  pf->WriteComment("// Target bounding sphere");
  return m_bsphere.Save(pf);
}

static const std::string RADIUS = "radius";

GameObject::PropertyMap Target::GetProperties() const
{
  PropertyMap pm;
  pm[RADIUS] = GameObject::Property(m_bsphere.GetRadius());
  return pm;
}

void Target::SetProperties(const GameObject::PropertyMap& pm)
{
  m_bsphere.SetRadius(const_cast<GameObject::PropertyMap&>(pm)[RADIUS].GetFloat());
}
#endif

const char* Target::GetTypeName() const
{
  return "target";
}

void Target::SetOrientation(const Orientation& o)
{
  m_or = o;
  m_bsphere.SetCentre(o.GetVertex());
}

const Orientation* Target::GetOrientation()
{
  m_or.SetVertex(m_bsphere.GetCentre());
  return &m_or;
}
}
