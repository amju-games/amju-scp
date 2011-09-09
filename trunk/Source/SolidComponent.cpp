/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: SolidComponent.cpp,v $
Revision 1.1  2004/09/08 15:42:56  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <map>
#include <string>
#include "SolidComponent.h"
#include "SolidComposite.h"
#include "SolidLeaf.h"
#include "CompositeFactory.h"
#include "File.h"
#include "Engine.h"
#include "Mgc/MgcSegment3.h"
#include "Mgc/MgcVector3.h"

namespace Amju
{
SolidComponent::SolidComponent() 
#if defined(SCENE_EDITOR)
  : m_isDirty(false)
#endif
{
}

SolidComponent::~SolidComponent()
{
}

PSolidComponent SolidComponent::LoadSolid(const std::string& name)
{
  // Cache composites. If we get a request for a composite we have already
  // loaded, return a clone of the cached composite. 
  // NB For Editing, cloning doesn't seem to work :-( 
#if (0) //!defined(SCENE_EDITOR)
  static std::map<std::string, PSolidComponent> cache;

  if (cache.find(name) != cache.end())
  {
    PSolidComponent& pComp = cache[name];
    return pComp->Clone();  
  }
#endif

  PSolidComponent pc = CompositeFactory::Instance()->Create(name);
  if (!pc.GetPtr())
  {
    std::string error = "Bad component type: " + name;
    Engine::Instance()->ReportError(error);
    return 0;
  }

  if (!pc->Load(name))
  {
    std::string error = "Failed to load component: " + name;
    Engine::Instance()->ReportError(error);
    return 0;
  }

#if (0) //!defined(SCENE_EDITOR)
  // Add the component to the cache if it's of a suitable type.
  if (dynamic_cast<SolidComposite*>(pc.GetPtr()))
  {
    cache[name] = pc;
  }
#endif

  return pc;
}

bool SolidComponent::LineIntersects(
  const Vec3f& v1, 
  const Vec3f& v2,
  float r) const
{
  Mgc::Segment3 seg;
  seg.Origin() = Mgc::Vector3(v1.x, v1.y, v1.z);
  seg.Direction() = Mgc::Vector3(v2.x - v1.x, v2.y - v1.y, v2.z - v1.z);
  return LineIntersects(seg, r);
}
}

