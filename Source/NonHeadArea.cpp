/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: NonHeadArea.cpp,v $
Revision 1.1.2.3  2007/07/15 21:59:39  Administrator
MSVC warning

Revision 1.1.2.2  2006/08/14 17:50:21  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:36  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.3  2005/06/04 22:34:13  jay
Don't draw walls unless turned on with #define

Revision 1.1.2.2  2005/04/18 22:39:59  Administrator
Scene Ed fixes

Revision 1.1.2.1  2005/04/17 22:00:06  jay
Added not-behind-headstring object for placing cue ball

*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "NonHeadArea.h"
#include "File.h"

namespace Amju
{
GameObject* CreateNonHeadArea()
{
  return new NonHeadArea;
}

NonHeadArea::NonHeadArea()
{
#ifdef SCENE_EDITOR
  // default constructed Game Object must be saveable for Editor.
  std::string bonusSolid = "cube2.comp"; // TODO CONFIG
  m_pSolid = SolidComponent::LoadSolid(bonusSolid);
#endif
}

const char* NonHeadArea::GetTypeName() const
{
  return "nonheadarea";
}

bool NonHeadArea::Load(File* pf)
{
  // Load the Solid
  std::string name;
  if (!pf->GetDataLine(&name))
  {
    pf->ReportError("Expected Solid name.");
    return false;
  }
  PSolidComponent pSolid = SolidComponent::LoadSolid(name);
  if (!pSolid.GetPtr())
  {
    pf->ReportError("Failed to load Takeable Solid.");
    return false;
  }
  // Load the orientation/position
  if (!pSolid->LoadOrientation(pf))
  {
    pf->ReportError("Failed to load Takeable orientation.");
    return false;
  }

  SetSolid(pSolid);
  return true;
}

void NonHeadArea::Draw()
{
#ifdef NON_HEAD_DEBUG
  m_collisionVolume.DrawWalls();
#endif
}

#if defined(SCENE_EDITOR)
bool NonHeadArea::Save(File* pf)
{
  return SolidGameObject::Save(pf);
}

static const char* COMP_FILE = "solid";
GameObject::PropertyMap NonHeadArea::GetProperties() const
{
  // TODO
  PropertyMap pm;   
  pm[COMP_FILE] = m_pSolid->GetName();
  return pm;
}
 
void NonHeadArea::SetProperties(const PropertyMap& pm)
{
  std::string compFile =
    const_cast<GameObject::PropertyMap&>(pm)[COMP_FILE].GetString();

  PSolidComponent pComp = SolidComponent::LoadSolid(compFile);
  if (pComp.GetPtr())
  {
    m_pSolid = pComp;
  }
}
#endif

}

