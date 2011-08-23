/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStateHiScore.cpp,v $
Revision 1.1.2.2  2006/08/14 17:50:11  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:30  jay
Moved Pool code from Pool/ to Pool/

Revision 1.2.2.1  2004/10/04 10:02:20  jay
#100 - bad logic in FindNextHiScore

Revision 1.2  2004/09/23 13:19:58  Administrator
Fixed infinite loop when more than one player

Revision 1.1  2004/09/08 15:43:05  jay
Added to repository
  
*/

// This source code originally written by JASON COLMAN 2000-2003. 
// You may use it as you wish, at your own risk!  jason@amju.com.

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "EngineStateHiScore.h"
#include "EngineStateWorldDemoHiScores.h"
#include "EngineStateTitle.h"
#include "Engine.h"
#include "File.h"
#include "TextWriter.h"
#include "HiScoreTable.h"
#include "TexturedQuad.h"
#include "GameState.h"
#include "TextFactory.h"
#include "GameState.h"
#include "PoolMisc.h"
#include "StringUtils.h"

namespace Amju
{
const char* EngineStateHiScore::Name = "enter_hi_score"; 
// Register this State with the Engine.
static const bool registeredtitle = Engine::Instance()->
  RegisterEngineState(EngineStateHiScore::Name, new EngineStateHiScore);

EngineStateHiScore::EngineStateHiScore()
{
  m_maxTime = 120.0f; 
  m_currentPlayer = 0;
}

void EngineStateHiScore::Draw()
{
  AmjuGL::PushMatrix();
  DrawBg();

  m_pCam->Update();
  m_pCam->Draw();

  m_lighting.DrawLight();

  m_or.Draw();

  AmjuGL::Translate(0, 1.5f, 0); // TODO TEMP TEST
  static const float z = Engine::Instance()->GetConfigFloat("hi_score_text_z2");
  AmjuGL::Translate(0, 0, z); 
  m_pGreetText->Draw();
  AmjuGL::Translate(0, -3.0f, 0);
  AmjuGL::PushMatrix();
  // Draw the text being edited:
  AmjuGL::Translate(-m_totalSize/2.0f, 0, -m_totalSize); 
  // Draw the text to the left of the cursor
  m_pComp->Draw();
  // Draw the text to the right of the cursor
  AmjuGL::Translate(m_leftSize + 0.5f, 0, 0);
  m_pCursor->Draw();
  m_pComp2->Draw();
  AmjuGL::PopMatrix();

  AmjuGL::PopMatrix();

  Engine::Instance()->GetTextWriter()->PrintCentre(12.0f, "Type your name then hit return.");
}

void EngineStateHiScore::DrawOverlays()
{
}

void EngineStateHiScore::Back()
{
}

void EngineStateHiScore::Finished()
{
  // This player is finished. Find out if there are any more high scores.
  // If no more, go on to the next state, otherwise restart this state.
  int score = Engine::Instance()->GetGameState()->GetPlayerInfo(m_currentPlayer)->m_score;

  HiScoreTable::Instance()->AddHiScore(score, m_text.c_str());

  // Check for any more high scores. 
  ++m_currentPlayer;
  if (FindNextHiScore())
  {
    // Clear name so next player can enter name.
    ResetText(); 
  }
  else
  {
#ifdef _DEBUG
std::cout << "Finished, and there are no more hi scores.\n";
#endif
    // We are done, write the hi score and go to hi score table state.
    HiScoreTable::Instance()->Save();
    TimerExpired();
  }
}

void EngineStateHiScore::OnKey(char key, bool down)
{
  EngineStateEnterText::OnKey(key, down);
}

void EngineStateHiScore::Red(bool down)
{
  // Allow spaces
  OnKey(' ', down);
}

bool EngineStateHiScore::Load()
{
  if (!EngineStateEnterText::Load())
  {
    return false;
  }
  return true;
}

void EngineStateHiScore::SetActive(bool active)
{
  EngineStateEnterText::SetActive(active);

  if (active)
  {
    // If no user input after 2 mins, go to title state anyway.
    m_maxTime = 120.0f;
    m_time = 0;
    m_playerName = "";

    m_currentPlayer = 0;
    if (!FindNextHiScore())
    {
#ifdef _DEBUG
std::cout << "Expected at least one high score.\n";
#endif
      TimerExpired();
    }

    std::string song = GetEngine()->GetConfigValue("golf_song_ball_in_hole");
    GetEngine()->PlaySong(song);
  }
}

bool EngineStateHiScore::FindNextHiScore()
{
  if (m_currentPlayer >= Engine::Instance()->GetGameState()->GetNumberOfPlayers())
  {
    return false;
  }

  int score = 0;
  // Find out which player has the high score we are showing.
  // Get the score.
  while (true) 
  {
    score = Engine::Instance()->GetGameState()->GetPlayerInfo(m_currentPlayer)->m_score;
    // is this a high score ?
    // If so, continue, otherwise find the next score.
    if (HiScoreTable::Instance()->IsHighScore(score))
    {
      // current player has high score.
      break;
    }
    m_currentPlayer++;
    if (m_currentPlayer >= Engine::Instance()->GetGameState()->GetNumberOfPlayers())
    {
      return false;
    }
  }
 
  std::string s = ToString(score);
  s += "$"; // new line
  s += "great score player ";
  s += ToString(m_currentPlayer + 1); // convert index to player ID
  s += "!";
  m_pGreetText = TextFactory::Instance()->CreateMultiline(s);

  return true;
}

void EngineStateHiScore::TimerExpired()
{
  GetEngine()->ChangeState(EngineStateWorldDemoHiScores::Name, Engine::IMMEDIATE);
}

}


