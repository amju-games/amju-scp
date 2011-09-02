/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: BonusBall.cpp,v $
Revision 1.1.2.2  2006/08/14 17:50:07  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:25  jay
Moved Pool code from Pool/ to Pool/

Revision 1.2  2004/09/25 21:02:33  Administrator
SceneEd/MakeLevelBin fix - don't try to load "per.move"

Revision 1.1  2004/09/08 15:43:04  jay
Added to repository
  
*/

#include "BonusBall.h"
#include "Engine.h"
#include "PoolGameState.h"
#include "PoolMisc.h"
#include "EngineStatePoolBase.h"

namespace Amju
{
BonusBall::BonusBall()
{
#if defined(SCENE_EDITOR)
  // default constructed Game Object must be saveable for Editor.
  std::string bonusSolid = "melon.move"; // TODO CONFIG
  m_pSolid = SolidComponent::LoadSolid(bonusSolid);
#endif
}

const char* BonusBall::GetTypeName() const
{
  return "bonusball";
}

void BonusBall::HandleObjectCollision(GameObject* pObj)
{
  if (!pObj)
  {
    return;
  }
  if (GetState() != UNKNOWN)
  {
    // Don't react if in exploding state.
    return; 
  }

  Engine* e = Engine::Instance();
  // The bonus goes out of play, but we must allow a delay for the particle 
  // effect to be seen.
  e->SendGameMessage(new Message("bonus explode", 0, GetId(), GetId(), BONUS_EXPLODE));
  // TODO m_explodeTime
  e->SendGameMessage(new Message("bonus dead", 5, GetId(), GetId(), OUT_OF_PLAY));

  // particle effect
  m_pExplosion->Reset(GetBoundingSphere()->GetCentre());

  // Increase score
  AddToScore(m_bonusPoints); // PoolMisc

  AwardBonusBall();
}
}
