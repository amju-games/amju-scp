/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: LevelWorkerAddSheds.cpp,v $
Revision 1.1  2004/09/08 15:42:39  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "LevelWorkerAddSheds.h"
#include "Shed.h"
#include "Level.h"
#include "CrocHome.h"
#include "File.h"
#include "SolidComposite.h"
#include "Engine.h"

namespace Amju
{
const char* LevelWorkerAddSheds::TypeName = "add_sheds";
const char* LevelWorkerAddSheds::GetTypeName() { return TypeName; }

bool LevelWorkerAddSheds::Load(File* pf)
{
  // Get the name for blocks to put under sheds when they are up in the air.
  if (!pf->GetDataLine(&m_blockName))
  {
    pf->ReportError("Expected block name for sheds.");
    return false;
  }
  PSolidComponent pBlock = SolidComponent::LoadSolid(m_blockName);
  if (!pBlock.GetPtr())
  {
    pf->ReportError("Bad block name for sheds.");
    return false;
  }
  return LevelWorkerAddSolidsBase::Load(pf);
}

bool LevelWorkerAddSheds::Save(File* )
{
  return false;
}

bool LevelWorkerAddSheds::AddFeature(Level* pLevel, CubeMap* pCubeMap)
{
  int numShedTypes = m_solidNames.size(); // num different types of sheds

  // Get the number of sheds to add to this level.
  // For each one, decide how high it should go.
  int numSheds = pLevel->GetId() + 2;
  int maxSheds = (int)Engine::Instance()->GetConfigFloat("max_sheds");

  //numSheds %= maxSheds;

  if (numSheds > maxSheds) 
  {
    numSheds = maxSheds;
  }

  float height = 2 * pLevel->GetId(); // TODO CONFIG ??
  float maxHeight = Engine::Instance()->GetConfigFloat("max_height");
  if (height > maxHeight)
  {
    height = maxHeight;
  }

  for (int i = 0; i < numSheds; i++)
  {
    int shedType = pLevel->GetId() % numShedTypes;
    std::string shedName = m_solidNames[shedType]; 
    Shed* pShed = new Shed;
    pShed->SetId(GetUniqueId());
    pShed->SetRoomId(0);
    pShed->SetLevel(pLevel);

    PSolidComponent pSolid = SolidComponent::LoadSolid(shedName); 
    pShed->SetSolid(pSolid); 
    BoundingBox bb;
    bb.SetFromSolid(pSolid.GetPtr());
    // Increase the bounding box to fit any block under the shed.
    bb.m_ymax += height;
    PSolidComponent pBlock = SolidComponent::LoadSolid(m_blockName);
    BoundingBox bbBlock;
    bbBlock.SetFromSolid(pBlock.GetPtr());
    bb.Union(bbBlock);

    Orientation o;
    if (!GetOrientation(pCubeMap, bb, pLevel->GetPlayAreaSize(), &o))
    {
      return false; // play area too small
    }

    o.SetY(o.GetY() + height);
    // Rotate a bit
    o.SetYRot((float)i * 90.0f); 
    pShed->SetOrientation(o); 
    // Use this function so we keep track of which LevelWorker added which object.
    AddGameObject(pLevel, pShed);

    // If the shed is up in the air, we also want to put a block under it.
    if (height > 0)
    {
      PSolidComponent pScene = pLevel->GetScene();
      SolidComposite* pComp = dynamic_cast<SolidComposite*>(pScene.GetPtr());
      if (pComp)
      {
        // Get block height
        float blockHeight = bbBlock.m_ymax - bbBlock.m_ymin;
        Orientation o1(o);
        // We want the _top_ of the block to be at y=height, so subtract boxheight.
        o1.SetY(height - blockHeight); 
        pBlock->SetOrientation(o1);
        // Make sure height server is set up, so we collide with the block.
        Matrix m;
        m.identity();
        o1.TransformMatrix(&m); 
        pBlock->StoreHeights(m);
        pComp->AddComponent(pBlock);
        // Redo bounding sphere tree to take block into account.
        m.identity();
        pComp->CreateBoundingSphere(m);
        // NB block is already added to cube map above.
      }
    }
    // Required to cast shadow on block.
    pShed->RefreshHeightServer();

    // Inside the shed put a 'Home' object, so we know when animals get there.
    CrocHome* pHome = new CrocHome;
    pHome->SetId(GetUniqueId());
    pHome->SetRoomId(0);
    BoundingSphere bs;
    bs.SetRadius(1.0f);
    VertexBase c(o.GetVertex());
    c.y += 1.0f;
    bs.SetCentre(c);
    pHome->SetSphere(bs);
    // Use this function so we keep track of which LevelWorker added which object.
    AddGameObject(pLevel, pHome);

    height += 2.0f; // TODO CONFIG
  }

  return true;
}
}

