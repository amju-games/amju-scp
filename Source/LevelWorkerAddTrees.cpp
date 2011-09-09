/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: LevelWorkerAddTrees.cpp,v $
Revision 1.1  2004/09/08 15:42:39  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "LevelWorkerAddTrees.h"
#include "SortScene.h"
#include "Level.h"
#include "SolidComposite.h"
#include "File.h"
#include "HeightServer.h"
#include "SchAssert.h"

namespace Amju
{
const char* LevelWorkerAddTrees::TypeName = "add_trees";
const char* LevelWorkerAddTrees::GetTypeName() { return TypeName; }

LevelWorkerAddTrees::LevelWorkerAddTrees()
{
  m_infoString = "trees";
}

#ifdef SCENE_EDITOR
bool LevelWorkerAddTrees::Save(File* pf)
{
  // number of trees
  pf->WriteInteger(m_numTrees);

  // Save the tree leaves
  int numLeaves = m_leafNames.size();
  pf->WriteInteger(numLeaves);
  int i;
  for (i = 0; i < numLeaves; i++)
  {
    pf->Write(m_leafNames[i]);
  }

  // Save the tree trunks
  int numTrunks = m_solidNames.size();
  pf->WriteInteger(numTrunks);
  for (i = 0; i < numTrunks; i++)
  {
    pf->Write(m_solidNames[i]);
  }

  return true;
}
#endif

bool LevelWorkerAddTrees::Load(File* pf)
{
  // Load the number of trees
  if (!pf->GetInteger(&m_numTrees))
  {
    pf->ReportError("Expected number of trees.");
    return false;
  }

  // Load the leaves into m_solidNames. Then copy them to m_leafNames.
  // Then we can load the trunks into m_solidNames.
  if (!LevelWorkerAddSolidsBase::Load(pf))
  {
    pf->ReportError("Failed to load tree leaves.");
    return false;
  }
  m_leafNames = m_solidNames;
  m_solidNames.clear();

  // Load solid trunks
  if (!LevelWorkerAddSolidsBase::Load(pf))
  {
    pf->ReportError("Failed to load tree tunks.");
    return false;
  }

  return true;
}

bool LevelWorkerAddTrees::AddFeature(Level* pLevel)
{
  // Add the trunks to the Scene.
  PSolidComponent pScene = pLevel->GetScene();
  SolidComposite* pComp = dynamic_cast<SolidComposite*>(pScene.GetPtr());
  if (!pComp)
  {
    return false;
  }

  // Decide on the type of trunk to add
  int trunkType = (int)(Rnd() * (float)m_solidNames.size()); 
  const std::string& trunkName = m_solidNames[trunkType];

  // Add the leaves, as a SortScene game object.
  // Decide which kind of leaves to add
  int leafType = (int)(Rnd() * (float)m_leafNames.size());
  const std::string& leafName = m_leafNames[leafType]; 

  SortScene* pSs = new SortScene;
  pSs->SetId(GetUniqueId());

  for (int j = 0; j < m_numTrees; j++)
  {
    PSolidComponent pSolid = SolidComponent::LoadSolid(leafName);
    Assert(pSolid.GetPtr()); // checked in Load().

    // Add a trunk to the scene to match the leaves.
    PSolidComponent pTrunk = SolidComponent::LoadSolid(trunkName);
    Assert(pTrunk.GetPtr());

    // Get a valid orientation for the tree.
    // We don't want trees in water, so reject orientations where the 
    // y-value is below the water level for the current room.
    Orientation o;
    if (!GetOrientationAboveWater(pLevel, &o))
    {
      return false; // play area too small
    }

    // Rotate a bit
    o.SetYRot((float)j * 10.0f); // TODO CONFIG
    pSolid->SetOrientation(o);
    pSs->AddSolid(pSolid);

    pTrunk->SetOrientation(o);
    Matrix m;
    m.SetIdentity();
    o.TransformMatrix(&m);
    pTrunk->StoreHeights(m);
    pComp->AddComponent(pTrunk);
  }
  // Use this function so we keep track of which LevelWorker added which object.
  AddGameObject(pLevel, pSs);

  // Redo scene bounding sphere tree to take trunks into account.
  Matrix m;
  m.SetIdentity();
  pComp->CreateBoundingSphere(m);

  return true;
}

}

