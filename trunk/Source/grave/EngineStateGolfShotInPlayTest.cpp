/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStateGolfShotInPlayTest.cpp,v $
Revision 1.1.2.2  2006/08/14 17:50:10  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:29  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1  2004/09/08 15:43:05  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "EngineStatePoolShotInPlayTest.h"
#include "EngineStateLevelDesign.h"
#include "Engine.h"
#include "LevelServer.h"
#include "PowerGuage.h"
#include "Behaviour.h"
#include "TextWriter.h"

namespace Amju
{
const char* EngineStatePoolShotInPlayTest::Name = "golf_inplay_test";

// Register this State with the Engine.
static const bool registeredgolfinplay = Engine::Instance()->
  RegisterEngineState(EngineStatePoolShotInPlayTest::Name, 
  new EngineStatePoolShotInPlayTest);

void EngineStatePoolShotInPlayTest::SetActive(bool active)
{
  EngineStatePoolShotInPlay::SetActive(active);
}

void EngineStatePoolShotInPlayTest::Red(bool)
{
  // Go back to Level Design State.
  GetEngine()->ChangeState(
    EngineStateLevelDesign::Name, Engine::FADE);
}

void EngineStatePoolShotInPlayTest::DrawOverlays()
{
  GetEngine()->GetTextWriter()->Print(0, 0, "Test mode");
}

}

