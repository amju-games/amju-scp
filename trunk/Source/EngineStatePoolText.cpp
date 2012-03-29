/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStatePoolText.cpp,v $
Revision 1.1.2.2  2007/06/10 21:33:18  Administrator
Check for extra states BALL_IN_POCKET and BALL_OUT_OF_BOUNDS

Revision 1.1.2.1  2006/08/14 17:50:18  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:30  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1  2004/09/08 15:43:05  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "EngineStatePoolText.h"
#include "Engine.h"
#include "PoolMisc.h"

namespace Amju
{
void EngineStatePoolText::Update()
{
  EngineStatePoolBase::Update();

  // Check the camera for collision with water
  int levelId = m_pLevel->GetId();
  int roomId = m_pLevel->GetRoomId();

  // Get the game objects which are in the same room as the player.
  GameObjectMap& objs = Engine::Instance()->GetGameObjects(levelId, roomId);
   
  Camera* pCam = GetCamera().GetPtr();
  Assert(pCam);
 
  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    PPoolGameObject pGo = it->second;

    if (InPlay(pGo))
    {
      PPoolGameObject pGo = it->second;
      // Check camera collision
      if (pCam->Intersects(*(pGo.GetPtr())))
      {
        pGo->HandleObjectCollision(pCam);
        pCam->HandleObjectCollision(pGo.GetPtr());
      }
    }
  }
}

bool EngineStatePoolText::Load()
{
  if (!EngineStatePoolBase::Load())
  {
    return false;
  }

  m_bihLeft = Engine::Instance()->GetConfigFloat("score_left");
  m_bihTop = Engine::Instance()->GetConfigFloat("score_top");
  m_bihZ = Engine::Instance()->GetConfigFloat("score_z");

  return true;
}

void EngineStatePoolText::DrawOverlays()
{
  // Clear the depth buffer - maybe takes too much time ?
  //glClear(AmjuGL::AMJU_DEPTH_READ);

  AmjuGL::Disable(AmjuGL::AMJU_DEPTH_READ);
  AmjuGL::PushMatrix();
  AmjuGL::Translate(m_bihLeft, m_bihTop, m_bihZ);
  m_pText->Draw();
  AmjuGL::PopMatrix();
  AmjuGL::Enable(AmjuGL::AMJU_DEPTH_READ);
}

}
