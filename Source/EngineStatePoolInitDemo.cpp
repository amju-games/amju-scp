#include "EngineStatePoolInitDemo.h"
#include "EngineStatePoolSetUpShotDemo.h"
#include "Engine.h"
#include "PoolMisc.h"

namespace Amju
{
const char* EngineStatePoolInitDemo::Name = "init-demo";

// Register this State with the Engine.
static const bool registered = Engine::Instance()->
  RegisterEngineState(EngineStatePoolInitDemo::Name, new EngineStatePoolInitDemo);

void EngineStatePoolInitDemo::SetActive(bool active)
{
  if (active)
  {
    StartDemoMode();
    GetEngine()->ChangeState(EngineStatePoolSetUpShotDemo::Name, Engine::IMMEDIATE);
  }
}
}
