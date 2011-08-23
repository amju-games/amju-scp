/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: PoolBonus.cpp,v $
Revision 1.1.2.3  2006/08/30 21:12:19  Administrator
Get Pool to build in MSVC

Revision 1.1.2.2  2006/08/14 17:50:22  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:37  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.6  2005/10/01 22:00:46  Administrator
Scene Ed: changed default Solid name in ctor

Revision 1.1.2.5  2005/09/29 19:10:29  jay
Set Shadow size

Revision 1.1.2.4  2005/09/25 20:15:18  Administrator
New WAV

Revision 1.1.2.3  2005/09/23 19:34:55  jay
Notify Rules when hit

Revision 1.1.2.2  2005/09/20 13:17:20  jay
Fix collision test

*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "PoolBonus.h"
#include "PoolHole.h"
#include "Engine.h"
#include "Level.h"
#include "PoolBall.h"
#include "PoolMisc.h"

namespace Amju
{
PoolBonus::PoolBonus()
{
#if defined(SCENE_EDITOR)
  // default constructed Game Object must be saveable for Editor.
  std::string bonusSolid = "melon.move"; // TODO CONFIG
  m_pSolid = SolidComponent::LoadSolid(bonusSolid);
#endif
}

const char* PoolBonus::GetTypeName() const
{
  return "pool-bonus";
}

void PoolBonus::HandleObjectCollision(GameObject* pObj)
{
  // If a ball hits this bonus, we explode, and create more heart bonuses
  // at each pocket.

  if (!pObj)
  {
    return;
  }
 
  if (GetState() != UNKNOWN)
  {
    // Don't react if in exploding state.
    return;
  }

  if (!dynamic_cast<PoolBall*>(pObj))
  {
    // Not a pool ball - we only react to collisions with balls.
    return;
  }

  GetRules(m_pLevel)->BallHitsObject((PoolBall*)pObj, this);

  // particle effect
  m_pExplosion->Reset(GetBoundingSphere()->GetCentre());

  Engine* e = Engine::Instance();
  // The bonus goes out of play, but we must allow a delay for the particle
  // effect to be seen.
  e->SendGameMessage(new Message("bonus explode", 0, GetId(), GetId(), BONUS_EXPLODE));
  // TODO m_explodeTime
  e->SendGameMessage(new Message("bonus dead", 5, GetId(), GetId(), OUT_OF_PLAY));

  Engine::Instance()->PlayWav("button112.wav");

  int levelId = m_pLevel->GetId();
  int roomId = m_pLevel->GetRoomId();
  GameObjectMap& objs = Engine::Instance()->GetGameObjects(levelId, roomId);
  // Iterate through map of Game Objects.
  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    //GameObjectId gameObjId = it->first;
    PGameObject pGo = it->second;
    PoolHole* p = dynamic_cast<PoolHole*>(pGo.GetPtr());
    if (p)
    {
      p->CreateExtraBonuses();
    }
  }
}

void PoolBonus::OnRoomEntry()
{
  Bonus::OnRoomEntry();
  // Shadow is tiny by default - TODO find out why.
  SetShadowSize(2.0f);
}

}


