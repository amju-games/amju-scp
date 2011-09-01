/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStatePoolOutOfBounds.cpp,v $
Revision 1.1.2.2  2006/08/17 18:15:40  jay
Ongoing work for Online Pool

Revision 1.1.2.1  2006/08/14 17:50:14  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:27  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.8.1  2005/03/17 20:56:32  jay
Make GetBall() less specific

Revision 1.1  2004/09/08 15:43:04  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "EngineStatePoolOutOfBounds.h"
#include "Engine.h"
#include "PoolMisc.h"
#include "LevelServer.h"
#include "TextFactory.h"
#include "PoolCharacter.h"

namespace Amju
{
const char* EngineStatePoolOutOfBounds::Name = "golf_o_b"; 
// Register this State with the Engine.
static const bool registeredgolfsetupshot = Engine::Instance()->
  RegisterEngineState(EngineStatePoolOutOfBounds::Name, new EngineStatePoolOutOfBounds);

bool EngineStatePoolOutOfBounds::Load()
{
  if (!EngineStatePoolText::Load())
  {
    return false;
  }

  std::string text = "lost ball!"; // TODO intl8n
  m_pText = TextFactory::Instance()->Create(text);

  m_maxTime = GetEngine()->GetConfigFloat("golf_loser_time");

  return true;
}

void EngineStatePoolOutOfBounds::SetActive(bool active)
{
  m_pLevel = LevelServer::Instance()->GetCurrentLevel();

  if (active)
  {
    Engine::Instance()->PlayWav(Engine::Instance()->GetConfigValue("golf_wav_17"));

    Orientation o = *(GetActivePlayer()->GetOrientation());
    GetCamera()->SetPlayer(GetActivePlayer()); 
    GetCamera()->SetOrientation(o);
    GetCamera()->Reset();

    m_time = 0;
  }
}

void EngineStatePoolOutOfBounds::TimerExpired()
{
  if (GetRemainingPlayers() == 0)
  {
    // This should not be reached. There is at least one player alive, 
    // or we would not have reached this state.

std::cout << "Ending lost ball state - unexpectedly there are no players left ?! \n";
  }

  NextPlayer();
  ShowPlayerUp();

  s_flashBalls = true;
}

}

