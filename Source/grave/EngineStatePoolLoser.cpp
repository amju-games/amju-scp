/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStatePoolLoser.cpp,v $
Revision 1.1.2.1  2006/08/14 17:50:14  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:27  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1  2004/09/08 15:43:04  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "EngineStatePoolLoser.h"
#include "TextFactory.h"
#include "Engine.h"
#include "LevelServer.h"
#include "GameState.h"
#include "PoolMisc.h"
#include "EngineStatePoolScoreboard.h"
#include "PoolCharacter.h"
#include "EngineStateTitle.h"
#include "EngineStateHiScore.h"
#include "HiScoreTable.h"
#include "StringUtils.h"

namespace Amju
{
const char* EngineStatePoolLoser::Name = "golf_loser";
static const bool registered = Engine::Instance()->
  RegisterEngineState(EngineStatePoolLoser::Name, new EngineStatePoolLoser);

bool EngineStatePoolLoser::Load()
{
  if (!EngineStatePoolText::Load())
  {
    return false;
  }

  m_maxTime = GetEngine()->GetConfigFloat("golf_loser_time");

  return true;
}

void EngineStatePoolLoser::SetActive(bool active)
{
  m_pLevel = LevelServer::Instance()->GetCurrentLevel();
  if (active)
  {
    GetCamera()->SetWater(0);
    m_isFading = false;
    Engine::Instance()->PlayWav(Engine::Instance()->GetConfigValue("golf_wav_17"));

    Orientation o = *(GetActivePlayer()->GetOrientation());
    GetCamera()->SetPlayer(GetActivePlayer()); 
    GetCamera()->SetOrientation(o);
    GetCamera()->Reset();

    m_time = 0;
  
    std::string text = "game over";
    // If this is a two-player game, and there is one player left, we should
    // say "game over player 1" or "game over player 2" 
    // Or perhaps use the character names ?

    if (GetRemainingPlayers() > 0)
    {
      // Find out which player it's Game Over for.
      int player = Engine::Instance()->GetGameState()->GetCurrentPlayerInfo()->m_id;
      text += "$player";
      text += ToString(player); 
    }

    m_pText = TextFactory::Instance()->CreateMultiline(text);
  }
}

void EngineStatePoolLoser::TimerExpired()
{
  if (m_isFading)
  {
    return;
  }
  m_isFading = true;
  if (GetRemainingPlayers() == 0)
  {
    // Either go to title state or enter hi score state for
    // each player with a high score.
    CheckForHiScores(); // In PoolMisc
    return;
  }

  // v.1.2: There may be another player still alive, so we may reach this point.
  NextPlayer();
  ShowPlayerUp();
}
}

