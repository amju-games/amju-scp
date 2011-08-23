/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: LevelWorkerAddCrates.cpp,v $
Revision 1.1  2004/09/08 15:42:39  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "LevelWorkerAddCrates.h"
#include "File.h"
#include "Level.h"
#include "SolidComponent.h"
#include "TakeableCrate.h"
#include "BoundingBox.h"
#include "CubeMap.h"
#include "Engine.h"

namespace Amju
{
const char* LevelWorkerAddCrates::TypeName = "add_crates";
const char* LevelWorkerAddCrates::GetTypeName() { return TypeName; }

LevelWorkerAddCrates::LevelWorkerAddCrates()
{
  m_infoString = "crate";
}

bool LevelWorkerAddCrates::AddFeature(Level* pLevel, CubeMap* pCubeMap)
{
  // TODO could use Buildings as lists of crates and their orientations.

  // For each crate type
  for (int i = 0; i < m_solidNames.size(); i++)
  {
    const std::string& crateName = m_solidNames[i]; 

    // Choose number of crates of this type, given the level.
    int num = 6; // TODO
    for (int j = 0; j < num; j++)
    {
      PSolidComponent pSolid = SolidComponent::LoadSolid(crateName);
      Assert(pSolid.GetPtr()); // checked in Load().

      // For each crate, get orientation.
      TakeableCrate* pCrate = new TakeableCrate;
      pCrate->SetId(GetUniqueId());
      pCrate->SetLevel(pLevel);
      pCrate->SetSolid(pSolid);
      // Set default orientation so bounding box etc is set up.
      pCrate->SetOrientation(Orientation());
      BoundingBox bb = pCrate->Takeable::GetCollisionVol()->GetBoundingBox();
      bb.SetFromSolid(pSolid.GetPtr());
      Orientation o;
      if (!GetOrientation(pCubeMap, bb, pLevel->GetPlayAreaSize(), &o))
      {
        return false; // play area too small
      }        
 
      pCrate->SetOrientation(o);
      pCrate->SetStartOrientation(o);
      pCrate->SetShadowSize(Engine::Instance()->GetConfigFloat("block_shadow_size"));
      pCrate->CreateShadow();
      // Use this function so we keep track of which LevelWorker added which object.
      AddGameObject(pLevel, pCrate);
    }
  }

  return true;
}
}

