/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: SortScene.cpp,v $
Revision 1.1.10.1  2007/07/15 21:07:05  Administrator
POOL only: sort scene has NO collision vol!

Revision 1.1  2004/09/08 15:42:57  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "SortScene.h"
#include "File.h"
#include "Engine.h"
#include "SolidComposite.h"
#include "AttribDecorator.h"
#include <algorithm>
#include <string>

namespace Amju
{
SortScene::SortScene()
{
  m_isTranslucent = true;

#if defined(SCENE_EDITOR)
  // TODO default constructed Game Object must be saveable for Editor.
#endif
}

typedef CompVector NodeStack;

float DistanceSquared(const Vec3f& v1, const Vec3f& v2)
{
  const float dx = (v1.x - v2.x);
  //const float dy = (v1.y - v2.y);
  const float dz = (v1.z - v2.z);
  //return dx * dx + dy * dy + dz * dz;

  // For trees, tall leaf billboards can result in the wrong drawing order.
  // So we ignore heights.
  return dx * dx + dz * dz;
}

bool SortScene::SceneSorter::operator()(
  const PSolidComponent& p1, 
  const PSolidComponent& p2) const
{
  const float d1sq = DistanceSquared(p1->GetBoundingSphere()->GetCentre(), m_v);
  const float d2sq = DistanceSquared(p2->GetBoundingSphere()->GetCentre(), m_v);
  return d1sq > d2sq;
}

void SortScene::Resort()
{
  // Get camera pos. Resort vector if it has changed.
  PCamera pCam = Engine::Instance()->GetCamera();
  if (!pCam.GetPtr())
  {
    return;
  }

  Vec3f eyePos = pCam->GetOrientation()->GetVertex();

  if (!(m_lastEyePos == eyePos)) // TODO op!= 
  {
    // Sort vector.
    SceneSorter sorter(eyePos);
    std::sort(m_solids.begin(), m_solids.end(), sorter);

    m_lastEyePos = eyePos;
  }
}

void SortScene::Draw()
{
  Resort();
  // Draw elements in order. This draws from the furthest away to the nearest.
  for (CompVector::iterator it = m_solids.begin(); it != m_solids.end(); ++it)
  {
    (*it)->Draw();
  }
}

void SortScene::Update()
{
}

const BoundingSphere* SortScene::GetBoundingSphere() const
{
  return &m_bsphere;
}

void SortScene::CreateBoundingSphere(const Matrix& cm)
{
}

#if defined(SCENE_EDITOR)
bool SortScene::Save(File* pf)
{
  // Write number of Solids.
  pf->WriteComment("// Sort Scene: number of solids");
  pf->WriteInteger(m_unexpandedNames.size());

  // For each solid, write its filename.
  for (unsigned int i = 0; i < m_unexpandedNames.size(); i++)
  {
    pf->Write(m_unexpandedNames[i]);
  }
  
  return true;
}

static const std::string SOLID_NAME = "solid name";

GameObject::PropertyMap SortScene::GetProperties() const
{
  // TODO only one solid name is allowed here.
  PropertyMap pm;
  if (m_unexpandedNames.empty())
  {
    // Need to insert a default value, so client knows the type of the Variable.
    pm[SOLID_NAME] = GameObject::Property(std::string("block1.comp")); // TODO CONFIG PREFERENCES
  }
  else
  {
    pm[SOLID_NAME] = GameObject::Property(m_unexpandedNames[0]);
  }

  return pm;
}

void SortScene::SetProperties(const GameObject::PropertyMap& pm)
{
  m_unexpandedNames.clear();
  m_unexpandedNames.push_back(const_cast<GameObject::PropertyMap&>(pm)[SOLID_NAME].GetString());
}
#endif

const char* SortScene::GetTypeName() const
{
  return "sortscene";
}

void SortScene::CullDraw(Frustum* pFrustum)
{
  Resort();
  // Draw elements in order. This draws from the furthest away to the nearest.
  for (CompVector::iterator it = m_solids.begin(); it != m_solids.end(); ++it)
  {
    (*it)->CullDraw(*pFrustum);
  }
}

void AddNonCompositeToVector(PSolidComponent pComp, CompVector& vec, NodeStack stack)
{
  // We are going to make a list of composites, where every node has exactly one child.
  // There is one node per element in stack.
  // Each node has the orientation of the corresponding stack element.
  // The leaf node is pComp.
  // Once we have created this list, add the root node to vec.

  SharedPtr<SolidComposite> pRoot = new SolidComposite; // extra root node
  SharedPtr<SolidComposite> pCurrent = pRoot;
  for (NodeStack::iterator it = stack.begin(); it != stack.end(); ++it)
  {
    // It would be nice to use Clone() here, but it's not that simple :-(
    // We only want to 'clone' one path from the root to a leaf node, not
    // the whole tree.
    SharedPtr<SolidComposite> pChild;
    // If the Composite is in fact a decorator, Clone the attribute which
    // is decorated.
    if (dynamic_cast<AttribDecorator*>(it->GetPtr()))
    {
      AttribDecorator* pAttribChild = new AttribDecorator;
      PAttrib pAttrib = ((AttribDecorator*)(it->GetPtr()))->GetAttrib();
      pAttribChild->SetAttrib(pAttrib->Clone());
      pChild = pAttribChild;
    }
    else
    {
      pChild = new SolidComposite;
    }
    pChild->SetOrientation(*((*it)->GetOrientation()));
    pCurrent->AddComponent(pChild.GetPtr());
    pCurrent = pChild;
  }
  pCurrent->AddComponent(pComp);

  Matrix SetIdentityMatrix;
  SetIdentityMatrix.SetIdentity();
  pRoot->CreateBoundingSphere(SetIdentityMatrix);

  Matrix m;
  m.SetIdentity();
  pRoot->StoreHeights(m);

  vec.push_back(pRoot.GetPtr());
}

void AddCompositeToVector(SolidComposite* pComposite, CompVector& vec, NodeStack stack)
{
  // Do a depth first search of the tree.
  // When we find a leaf (a non-composite) we want to add it to the vector,
  // but with all the parent node transformations incorporated.
  // Otherwise leaves would all appear at the origin.

  stack.push_back(pComposite);

  int numChildren = pComposite->GetNumChildren();
  for (int i = 0; i < numChildren; i++)
  {
    PSolidComponent pChild = pComposite->GetChild(i);
    SolidComposite* pCompChild = dynamic_cast<SolidComposite*>(pChild.GetPtr());
    if (pCompChild)
    {
      // Child of this composite is also a composite - so recurse.
      AddCompositeToVector(pCompChild, vec, stack);
    }
    else
    {
      // Add this non-composite to the vector, vec. But we must incorporate all the
      // transformations in stack.
      AddNonCompositeToVector(pChild, vec, stack);
    }
  }
}

// Recursively add Components to a CompVector.
// Normally we simply add the component. The recursive part is when we have a
// Composite. In this case we want to add each path from root to leaf
// separately.
// This is so that the children of a composite can be sorted by distance 
// from the camera.
void AddSolidToVector(PSolidComponent pSolid, CompVector& vec)
{
  SolidComposite* pComposite = dynamic_cast<SolidComposite*>(pSolid.GetPtr());
  if (pComposite)
  {
    // Stack of nodes, initially empty (function is recursive)
    NodeStack stack;
    AddCompositeToVector(pComposite, vec, stack);
  }
  else
  {
    // Not a composite - simply add it.
    vec.push_back(pSolid);
  }
}

bool SortScene::Load(File* pf)
{
  // Get number of Solids.
  int numSolids = 0;
  if (!pf->GetInteger(&numSolids))
  {
    pf->ReportError("Expected number of solids.");
    return false;
  }

  // Load each solid, add to vector.
  for (int i = 0; i < numSolids; i++)
  { 
    std::string solidName;
    if (!pf->GetDataLine(&solidName))
    {
      pf->ReportError("Expected solid name.");
      return false;
    }
    // Load the solid, calculate absolute coords for bounding sphere location.
    PSolidComponent pSolid = SolidComponent::LoadSolid(solidName);
    if (!pSolid.GetPtr())
    {
      pf->ReportError("Bad solid name in sortscene list.");
      return false;
    }

    // For saving, store the solid name before we expand out all paths from
    // the root to each leaf.
    m_unexpandedNames.push_back(solidName);

    AddSolid(pSolid);
  }
  return true;
}

void SortScene::AddSolid(PSolidComponent pSolid)
{
  Matrix SetIdentityMatrix;
  SetIdentityMatrix.SetIdentity();
  pSolid->CreateBoundingSphere(SetIdentityMatrix);

  // Bounding sphere should be created by merging all the child spheres.
  m_bsphere += *(pSolid->GetBoundingSphere());

  Matrix m;
  m.SetIdentity();
  pSolid->GetOrientation()->TransformMatrix(&m); 
  pSolid->StoreHeights(m);

  // Add to collision volume now - i.e. RecalcCollisionVol() is a no-op.
  // POOL: we DON'T need the trees to have collision volumes!!
  //BoundingSphere bs(Vec3f(0, 0, 0), 1000); // big B.S.
  //pSolid->AddHeights(&m_collisionVolume, bs);

  //m_solids.push_back(pSolid);
  AddSolidToVector(pSolid, m_solids);
}

void SortScene::RecalcCollisionVol()
{
  // We never move, so no need to do anything.
}

}
