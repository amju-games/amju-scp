/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: FreezeBonus.cpp,v $
Revision 1.1.2.3  2006/08/30 21:12:16  Administrator
Get Pool to build in MSVC

Revision 1.1.2.2  2006/08/14 17:50:19  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:33  jay
Moved Pool code from Pool/ to Pool/

Revision 1.2  2004/09/25 21:02:33  Administrator
SceneEd/MakeLevelBin fix - don't try to load "per.move"

Revision 1.1  2004/09/08 15:43:06  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "FreezeBonus.h"
#include "Engine.h"
#include "PoolMisc.h"
#include "Level.h"
#include "Stupid.h"

namespace Amju
{
FreezeBonus::FreezeBonus()
{
#if defined(SCENE_EDITOR)
  // default constructed Game Object must be saveable for Editor.
  std::string bonusSolid = "melon.move"; // TODO CONFIG
  m_pSolid = SolidComponent::LoadSolid(bonusSolid);
#endif
}

const char* FreezeBonus::GetTypeName() const
{
  return "freezebonus";
}

void FreezeBonus::HandleObjectCollision(PoolGameObject* pObj)
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

  // particle effect
  m_pExplosion->Reset(GetBoundingSphere()->GetCentre());

  Engine* e = Engine::Instance();
  // The bonus goes out of play, but we must allow a delay for the particle 
  // effect to be seen.
  e->SendGameMessage(new Message("bonus explode", 0, GetId(), GetId(), BONUS_EXPLODE));
  // TODO m_explodeTime
  e->SendGameMessage(new Message("bonus dead", 5, GetId(), GetId(), OUT_OF_PLAY));

  Engine::Instance()->PlayWav(Engine::Instance()->GetConfigValue("wav_freeze"));
  // For each Stupid in the current room, set state to frozen.
  // Send a timed message to unfreeze after some time delay.

  int levelId = m_pLevel->GetId();
  int roomId = m_pLevel->GetRoomId();
  GameObjectMap& objs = Engine::Instance()->GetGameObjects(levelId, roomId);
  // Iterate through map of Game Objects. 
  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    GameObjectId gameObjId = it->first;
    PPoolGameObject pGo = it->second;

    Stupid* pStupid = dynamic_cast<Stupid*>(pGo.GetPtr());
    if (pStupid && pStupid->GetState() == UNKNOWN)
    {
      // Send messages
      Engine::Instance()->SendGameMessage(new Message("npc freeze", 0, 
       GetId(), gameObjId, FROZEN));

      static const float FREEZE_TIME = Engine::Instance()->GetConfigFloat("freeze_time");

      Engine::Instance()->SendGameMessage(new Message("npc unfreeze", FREEZE_TIME, 
        GetId(), gameObjId, UNKNOWN));

    }
  }
  // Increase score
  AddToScore(50); // TODO CONFIG
}

}

