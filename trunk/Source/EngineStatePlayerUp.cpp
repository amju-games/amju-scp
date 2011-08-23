/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStatePlayerUp.cpp,v $
Revision 1.2  2004/09/15 09:15:37  Administrator
Minor fixes

Revision 1.1  2004/09/08 15:42:36  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "EngineStatePlayerUp.h"
#include "Engine.h"
#include "GameState.h"
#include "Text3dLeaf.h"

namespace Amju
{
const char* EngineStatePlayerUp::Name = "player_up";
// Register this State with the Engine.
static const bool registeredthis = Engine::Instance()->
  RegisterEngineState(EngineStatePlayerUp::Name, new EngineStatePlayerUp);

EngineStatePlayerUp::EngineStatePlayerUp()
{
  m_maxTime = 3.0f;
  m_verticalPos = 0;
}

bool EngineStatePlayerUp::Load()
{
  std::string bg = GetEngine()->GetConfigValue("level_bg");
  if (!m_bg.Load(bg, ""))
  {
    return false;
  }
  return true;
}

void EngineStatePlayerUp::SetActive(bool active)
{
  if (active)
  {
    m_time = 0;

    std::string text = 
      GetEngine()->GetGameState()->GetCurrentPlayerInfo()->m_name;
    text += " up";
    m_pComp = new Text3dLeaf(text.c_str());
  }
}

void EngineStatePlayerUp::TimerExpired()
{
}

}

