/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStateGolfSetUpShotTest.cpp,v $
Revision 1.1.2.2  2006/08/14 17:50:10  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:28  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1  2004/09/08 15:43:05  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "EngineStatePoolSetUpShotTest.h"
#include "EngineStateLevelDesign.h"
#include "EngineStatePoolShotInPlayTest.h"
#include "Engine.h"
#include "GameState.h"
#include "Level.h"
#include "Behaviour.h"
#include "PowerGuage.h" // needed because header only has fwd decl.
#include "TextWriter.h"

namespace Amju
{
const char* EngineStatePoolSetUpShotTest::Name = "golf_setup_test";

// Register this State with the Engine.
static const bool registeredgolfinplay = Engine::Instance()->
  RegisterEngineState(EngineStatePoolSetUpShotTest::Name, 
  new EngineStatePoolSetUpShotTest);

void EngineStatePoolSetUpShotTest::SetActive(bool active)
{
  // Set up a single non-AI player character
  if (active)
  {
    GetEngine()->GetGameState()->ClearPlayers();

    GameState::PlayerInfo p1(1, "amju", Orientation(), false);
    p1.m_score = 0;
    p1.m_isPlaying = true;
    p1.m_strokes = 0;
    p1.m_reset = true;
    p1.m_isAi = false;
    GetEngine()->GetGameState()->AddPlayerInfo(p1);
    GetEngine()->GetGameState()->SetCurrentPlayer(0);
  }
  EngineStatePoolSetUpShot::SetActive(active);
}

void EngineStatePoolSetUpShotTest::Red(bool)
{
  // Go back to Level Design State.
  GetEngine()->ChangeState(
    EngineStateLevelDesign::Name, Engine::FADE);
}

void EngineStatePoolSetUpShotTest::DrawOverlays()
{
  GetEngine()->GetTextWriter()->Print(0, 0, "Test mode");
}

void EngineStatePoolSetUpShotTest::TakeShotNow(
  float yRot, float verticalVel, float horVel, float accel)
{
  TakeShotNowImpl(yRot, verticalVel, horVel, accel);

  Engine::Instance()->ChangeState(
    EngineStatePoolShotInPlayTest::Name, Engine::IMMEDIATE);
}
}

