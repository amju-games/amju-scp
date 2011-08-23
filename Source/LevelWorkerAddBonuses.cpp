/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: LevelWorkerAddBonuses.cpp,v $
Revision 1.1  2004/09/08 15:42:38  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "LevelWorkerAddBonuses.h"
#include "Bonus.h"
#include "Bean.h"
#include "Level.h"
#include "Engine.h"

namespace Amju
{
const char* LevelWorkerAddBonuses::TypeName = "add_bonuses";
const char* LevelWorkerAddBonuses::GetTypeName() { return TypeName; }

LevelWorkerAddBonuses::LevelWorkerAddBonuses()
{
  m_infoString = "bonus";
}

bool LevelWorkerAddBonuses::AddFeature(Level* pLevel, CubeMap* pCubeMap)
{
  // Extra Time bonuses
  // ------------------
  const std::string& timeBonusName = m_solidNames[0]; 

  int num = 6; // TODO
  for (int j = 0; j < num; j++)
  {
    PSolidComponent pSolid = SolidComponent::LoadSolid(timeBonusName);
    Assert(pSolid.GetPtr()); // checked in Load().

    Bonus* pBonus = new Bonus;
    pBonus->SetLevel(pLevel);
    pBonus->SetId(GetUniqueId());
    pBonus->SetSolid(pSolid);

    BoundingBox bb(0, 1.0f, 0, 1.0f, 0, 1.0f); // unit cube
    Orientation o;
    if (!GetOrientation(pCubeMap, bb, pLevel->GetPlayAreaSize(), &o))
    {
      return false; // play area too small
    }
    // Set each bonus higher up...?
    o.SetY(o.GetY() + 1.0f + 2.0f * (float)(j / 2)); // TODO TEMP TEST TODO CONFIG
    pBonus->SetOrientation(o);
    // Required to cast shadows correctly.
    // NB we will need to recalc. shadows when any crates are moved.
    pBonus->RefreshHeightServer();
    pBonus->SetShadowSize(Engine::Instance()->GetConfigFloat("bonus_shadow_size")); 

    // Use this function so we keep track of which LevelWorker added which object.
    AddGameObject(pLevel, pBonus);
  }
/*
  // Turn direction bonuses
  // ----------------------
  const std::string& dirBonusName = m_solidNames[1]; 

  for (int k = 0; k < num; k++)
  {
    PSolidComponent pSolid = SolidComponent::LoadSolid(dirBonusName);
    Assert(pSolid.GetPtr()); // checked in Load().

    Bean* pBean= new Bean;
    pBean->SetLevel(pLevel);
    pBean->SetId(GetUniqueId());
    pBean->SetSolid(pSolid);

    BoundingBox bb(0, 1.0f, 0, 1.0f, 0, 1.0f); // unit cube
    Orientation o;
    if (!GetOrientation(pCubeMap, bb, pLevel->GetPlayAreaSize(), &o))
    {
      return false; // play area too small
    }
    // Set each bonus higher up...?
    o.SetY(o.GetY() + 1.0f + 2.0f * (float)(k / 2)); // TODO TEMP TEST TODO CONFIG
    pBean->SetOrientation(o);
    // Required to cast shadows correctly.
    // NB we will need to recalc. shadows when any crates are moved.
    pBean->RefreshHeightServer();
    pBean->SetShadowSize(Engine::Instance()->GetConfigFloat("bonus_shadow_size")); 

    // Use this function so we keep track of which LevelWorker added which object.
    AddGameObject(pLevel, pBean);
  }
 */
  return true;
}
}
