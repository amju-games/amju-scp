/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: LevelWorkerBoundary.cpp,v $
Revision 1.1  2004/09/08 15:42:39  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "LevelWorkerBoundary.h"
#include "Level.h"
#include "Room.h"
#include "File.h"
#include "SolidComponent.h"
#include "SolidComposite.h"
#include "ScaleSolid.h"

namespace Amju
{
const char* LevelWorkerBoundary::TypeName = "boundary";
const char* LevelWorkerBoundary::GetTypeName() { return TypeName; }

#ifdef SCENE_EDITOR
bool LevelWorkerBoundary::Save(File* pf)
{
  pf->Write(m_boundaryName);
  return true;
}
#endif

bool LevelWorkerBoundary::Load(File* pf)
{
  // Load the boundary solid
  if (!pf->GetDataLine(&m_boundaryName))
  {
    pf->ReportError("Expected boundary name.");
    return false;
  }
  PSolidComponent boundary = SolidComponent::LoadSolid(m_boundaryName);
  if (!boundary.GetPtr())
  {
    std::string err = "Bad boundary name: ";
    err += m_boundaryName;
    pf->ReportError(err);
    return false;
  }
  return true;
}

bool LevelWorkerBoundary::AddFeature(Level* pLevel, CubeMap* pCubeMap)
{
  PSolidComponent pScene = pLevel->GetScene();
  SolidComposite* pComp = dynamic_cast<SolidComposite*>(pScene.GetPtr());
  if (!pComp)
  {
    // No scene - can't add boundary
    return false;
  }

  // Get the play area size. The boundary is a square with 
  // length = 2 * play area size.
  int s = pLevel->GetPlayAreaSize();
  //s += 2; // so block is outside boundary, not on it.
  float len = s * 2; // + 2;

  // Height: initially choose halfway between 0 and the peak terrain height.
  float h = 0;//6.0f; // TODO CONFIG TEMP TEST

  Orientation o[4] = 
  {
    Orientation(s,  h, 0,  0, 270, 0),
    Orientation(-s, h, 0,  0, 90,  0),
    Orientation(0,  h, s,  0, 180, 0),
    Orientation(0,  h, -s, 0, 0,   0)
  };

  // Resize the boundary to the side length of the play area
  // -------------------------------------------------------
  // NB Because leaf data is only held once (in LeafData) we only need
  // to do the scaling once.
  // We are going to scale the boundary block in the x-axis.
  // First we must make sure the block is 1 unit wide in x.
  PSolidComponent pBlock = SolidComponent::LoadSolid(m_boundaryName);
  BoundingBox bb;
  bb.SetFromSolid(pBlock.GetPtr());
  float xExtent = bb.m_xmax - bb.m_xmin;
  // Scale the boundary block so x extent is 1 unit.
  ScaleRecursive(pBlock.GetPtr(), 1.0f / xExtent, 1.0f, 1.0f);
  // Now scale the boundary block to the length of the boundary.
  ScaleRecursive(pBlock.GetPtr(), len, 1.0f, 1.0f);

  for (int i = 0; i < 4; i++)
  {
    PSolidComponent pBlock = SolidComponent::LoadSolid(m_boundaryName);
    // NB This block will be ths same size as the one we resized above, 
    // because all leaf data with the same name is the same data.

    //Matrix m;
    //m.identity();
    //pBlock->RecalculateAbsoluteCoords(m);

    BoundingBox bb;
    bb.SetFromSolid(pBlock.GetPtr());
    float xExtent = bb.m_xmax - bb.m_xmin;

    //m.identity();
    //pBlock->RecalculateAbsoluteCoords(m);
    //bb.SetFromSolid(pBlock.GetPtr());
    //float xExtent2 = bb.m_xmax - bb.m_xmin;

    pBlock->SetOrientation(o[i]);
    Matrix m;
    m.identity();
    o[i].TransformMatrix(&m);
    // We want the heightserver so the ball will bounce off the boundary etc.
    pBlock->StoreHeights(m);

    pComp->AddComponent(pBlock);
  }

  // Redo bounding sphere tree to take boundary into account.
  Matrix m;
  m.identity();
  pComp->CreateBoundingSphere(m);

  return true;

}

}

