/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStatePoolShotInPlayDemo.cpp,v $
Revision 1.1.2.1  2006/08/14 17:50:17  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:29  jay
Moved Pool code from Pool/ to Pool/

Revision 1.2.6.4  2005/09/23 19:40:09  jay
Remove unused code

Revision 1.2.6.3  2005/09/02 18:16:57  jay
Remove dead code

Revision 1.2.6.2  2005/08/26 21:31:17  jay
Add mouse camera control

Revision 1.2.6.1  2005/04/01 06:01:28  jay
Pool Demo mode

Revision 1.2  2004/09/17 13:59:10  jay
Added demo mode

Revision 1.1  2004/09/08 15:43:05  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "EngineStatePoolShotInPlayDemo.h"
#include "EngineStatePoolSetUpShotDemo.h"
#include "EngineStateTitle.h"
#include "Engine.h"
#include "GameState.h"
#include "LevelServer.h"
#include "PowerGuage.h"
#include "Behaviour.h"
#include "PoolMisc.h"
#include "PoolCharacter.h"

namespace Amju
{
const char* EngineStatePoolShotInPlayDemo::Name = "golf_inplay_demo";

// Register this State with the Engine.
static const bool registeredgolfinplay = Engine::Instance()->
  RegisterEngineState(EngineStatePoolShotInPlayDemo::Name, 
  new EngineStatePoolShotInPlayDemo);

void EngineStatePoolShotInPlayDemo::MousePos(int x, int y)
{
  EngineStatePoolSetUpShotDemo::SMousePos(x, y);
  GetCamera()->MousePos(x, y);
}

void EngineStatePoolShotInPlayDemo::MouseButton(bool down, bool ctrl, bool shift)
{
  EngineStatePoolSetUpShotDemo::SMouseButton(down, ctrl, shift);
  GetCamera()->MouseButton(down, ctrl, shift);
}

void EngineStatePoolShotInPlayDemo::DrawOverlays()
{
  EngineStatePoolSetUpShotDemo::SDrawOverlays();
}

void EngineStatePoolShotInPlayDemo::SetActive(bool active)
{
  EngineStatePoolShotInPlay::SetActive(active);

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

