/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStatePoolShowShotDemo.cpp,v $
Revision 1.1.2.1  2006/08/14 17:50:18  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:30  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.6.4  2005/09/23 19:42:16  jay
Remove unnecessary code

Revision 1.1.6.3  2005/08/26 21:32:26  jay
Add mouse camera control

Revision 1.1.6.2  2005/04/17 22:04:06  jay
Added english and draw/roll parameters to shot

Revision 1.1.6.1  2005/04/01 06:01:53  jay
Pool Demo mode

Revision 1.1  2004/09/17 13:59:11  jay
Added demo mode

*/

#include "EngineStatePoolShowShotDemo.h"
#include "EngineStatePoolShotInPlayDemo.h"
#include "EngineStatePoolSetUpShotDemo.h"
#include "Engine.h"
#include "GameState.h"
#include "Level.h"
#include "PoolChangeState.h"

namespace Amju
{
const char* EngineStatePoolShowShotDemo::Name = "golf_show_shot_demo";
// Register this State with the Engine.
static const bool registeredgolfshowshot = Engine::Instance()->
  RegisterEngineState(EngineStatePoolShowShotDemo::Name, 
  new EngineStatePoolShowShotDemo);

void EngineStatePoolShowShotDemo::TimerExpired()
{
  // TODO TEMP TEST try base class version, it should work now we are
  // handling State changes more sensibly
  EngineStatePoolShowShot::TimerExpired();
  return;

/*
  const GameState::PlayerInfo::PoolStroke& gs =
  GetEngine()->GetGameState()->GetCurrentPlayerInfo()->m_golfStroke;

  EngineStatePoolSetUpShot::TakeShotNowImpl(gs.m_yRot, gs.m_vertVel, gs.m_horVel, gs.m_accel,
    gs.m_english, gs.m_drawRoll);

  ChangeStateToShotInPlay(); 
*/
}

void EngineStatePoolShowShotDemo::DrawOverlays()
{
  EngineStatePoolSetUpShotDemo::SDrawOverlays();
}

void EngineStatePoolShowShotDemo::MousePos(int x, int y)
{
  EngineStatePoolSetUpShotDemo::SMousePos(x, y);
  GetCamera()->MousePos(x, y);
}

void EngineStatePoolShowShotDemo::MouseButton(bool down, bool ctrl, bool shift)
{
  EngineStatePoolSetUpShotDemo::SMouseButton(down, ctrl, shift);
  GetCamera()->MouseButton(down, ctrl, shift);
}

void EngineStatePoolShowShotDemo::SetActive(bool active)
{
  EngineStatePoolShowShot::SetActive(active);

  if (active)
  {
    GetEngine()->SetClearColour(1.0f, 0, 1.0f);
    GetEngine()->SetLetterbox(true);
  }
  else
  {
    GetEngine()->SetClearColour(0, 0, 0);
    GetEngine()->SetLetterbox(false);
  }
}
}



