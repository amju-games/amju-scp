/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStateTimerText.cpp,v $
Revision 1.2  2004/09/17 12:48:36  Administrator
Removed call to function which has gone away

Revision 1.1  2004/09/08 15:42:49  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "EngineStateTimerText.h"
#include "Engine.h"
#include "PoolGameState.h"

namespace Amju
{
const char* EngineStateTimerText::Name = "timer_text";
// Register this State with the Engine.
static const bool registeredthis = Engine::Instance()->
  RegisterEngineState(EngineStateTimerText::Name, new EngineStateTimerText);

EngineStateTimerText::EngineStateTimerText()
{
  m_maxTime = 3.0f;
  m_verticalPos = 0;
}

bool EngineStateTimerText::Load()
{
  std::string bg = GetEngine()->GetConfigValue("level_bg");
  if (!m_bg.Load(bg, ""))
  {
    return false;
  }
  return true;
}

void EngineStateTimerText::SetActive(bool active)
{
  if (active)
  {
    m_time = 0;
  }
}

void EngineStateTimerText::TimerExpired()
{
}

}

