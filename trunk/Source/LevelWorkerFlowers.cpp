/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: LevelWorkerFlowers.cpp,v $
Revision 1.1  2004/09/08 15:42:39  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "LevelWorkerFlowers.h"
#include "Level.h"
#include "File.h"
#include "SolidComposite.h"
#include "Engine.h"

namespace Amju
{
extern void ReportError(const std::string&);

const char* LevelWorkerFlowers::TypeName = "flowers";
const char* LevelWorkerFlowers::GetTypeName() { return TypeName; }

#ifdef SCENE_EDITOR
bool LevelWorkerFlowers::Save(File* )
{
  return true;
}
#endif

bool LevelWorkerFlowers::Load(File* pf)
{
  if (!pf->GetDataLine(&m_flowerSolidName))
  {
    pf->ReportError("Expected solid name for flowers.");
    return false;
  }
  // Check that the name is valid and the solid loads.
  PSolidComponent pBlock = SolidComponent::LoadSolid(m_flowerSolidName);
  if (!pBlock.GetPtr())
  {
    pf->ReportError("Bad solid name for flowers.");
    return false;
  }
  if (!pf->GetInteger(&m_numFlowers))
  {
    pf->ReportError("Expected number of flowers in each bunch.");
    return false;
  }
  if (!pf->GetInteger(&m_numBunches))
  {
    pf->ReportError("Expected number of bunches of flowers.");
    return false;
  }
  if (!pf->GetFloat(&m_radius))
  {
    pf->ReportError("Expected radius for flower bunch.");
    return false;
  }

  return true;
}

bool LevelWorkerFlowers::AddFeature(Level* pLevel)
{
  PSolidComponent pScene = pLevel->GetScene();
  SolidComposite* pComp = dynamic_cast<SolidComposite*>(pScene.GetPtr());
  if (!pComp)
  {
    ReportError("No scene set for level ?");
    return false;
  }

  for (int i = 0; i < m_numBunches; i++)
  {
    RCPtr<SolidComposite> pBunch = new SolidComposite;

    // Get a random position for this bunch.
    Orientation o;
    // DON'T use the CubeMap.
    // Reduce area by radius, so we don't try to plant flowers in the air!
    if (!GetOrientation(pLevel->GetPlayAreaSize() - (int)m_radius, &o))
    {
      Assert(0);
      return false; // play area too small
    }

    for (int j = 0; j < m_numFlowers; j++)
    {
      PSolidComponent pSolid = SolidComponent::LoadSolid(m_flowerSolidName);
      Assert(pSolid.GetPtr()); // checked in Load().

      Orientation o1(o);
      o1.SetX(o1.GetX() + (Rnd() - 0.5f) * m_radius);
      o1.SetZ(o1.GetZ() + (Rnd() - 0.5f) * m_radius);

      // Get height of ground
      // Make a bounding sphere at the (x, z) coord of the flower. 
      // The y-coord/radius will have to be a guess which will ensure the
      // sphere will enclose the ground poly at (x, z).
      // Get a HeightServer containing the height polys which intersect 
      //  the sphere, then get the height.
      float y = 0;
      float r = 10.0f; // TODO CONFIG
      BoundingSphere bs(Vec3f(o1.GetX(), y, o1.GetZ()), r);
      HeightServer hs;
      pLevel->GetHeightServer(&hs, bs);
      if (!hs.GetHeight(o1.GetX(), o1.GetZ(), &y))
      {
        // Sphere not big enough, terrain not built, etc.
        Assert(0);
        return false;
      }
      o1.SetY(y);

      // Rotate a bit
      o1.SetYRot((float)j * 10.0f); // TODO CONFIG
      pSolid->SetOrientation(o1);
/*
      Matrix m;
      m.SetIdentity();
      o1.TransformMatrix(&m);
      pSolid->StoreHeights(m);
*/
      pBunch->AddComponent(pSolid);
    }
    pComp->AddComponent(pBunch.GetPtr());
  }

  return true;
}
}
