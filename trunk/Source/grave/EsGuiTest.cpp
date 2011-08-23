
#include "EsGuiTest.h"
#include "Engine.h"

namespace Amju
{
const char* EsGuiTest::Name = "gui-test";

// Register this State with the Engine.
static const bool registered =
  Engine::Instance()->RegisterEngineState(EsGuiTest::Name, new EsGuiTest);

void EsGuiTest::DrawOverlays()
{
}
}

