/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStatePoolBallInHole.cpp,v $
Revision 1.1.2.11  2007/07/21 23:42:35  Administrator
Store Online ID in player names: only request ID from server when uploading
stats for playing an online game for the first time.

Revision 1.1.2.10  2007/07/16 08:24:53  jay
Mark online game as finished: only when the final shot is replayed on the
second client. This is so the final shot is not immediately recognised as the
opponent quitting the game.

Revision 1.1.2.9  2007/07/08 19:49:59  Administrator
Only send player info when we save player stats - so we don't send info
for all players (bumping their last connection time)

Revision 1.1.2.8  2007/05/28 12:55:45  Administrator
Face table

Revision 1.1.2.7  2007/05/23 18:45:42  jay
Send all object changes in one Http request (POST), not individually.
 + Some tidying up.

Revision 1.1.2.6  2007/05/16 22:03:14  Administrator
Rotate camera when frame is over

Revision 1.1.2.5  2007/05/15 09:27:49  jay
Online: delete game from server when finished

Revision 1.1.2.4  2007/03/23 19:18:00  jay
Send online scores at end of state, so all hearts have been awarded.
Force birds eye view off when the frame ends.

Revision 1.1.2.3  2007/03/20 23:31:21  jay
Send Player info to server for online score table

Revision 1.1.2.2  2007/03/09 21:50:41  jay
Online game - ongoing fixes

Revision 1.1.2.1  2006/08/14 17:50:12  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:25  jay
Moved Pool code from Pool/ to Pool/

Revision 1.2.2.1.2.10  2005/09/30 12:59:31  jay
No earthquake on game over (unless bomb explodes)

Revision 1.2.2.1.2.9  2005/09/29 19:03:25  jay
Save Player Stats so we get any extra hearts awarded when game is over.

Revision 1.2.2.1.2.8  2005/09/25 20:16:47  Administrator
Show bonuses, so extra bonuses can accumulate

Revision 1.2.2.1.2.7  2005/09/16 20:04:50  jay
Remove unused code

Revision 1.2.2.1.2.6  2005/09/05 20:04:20  jay
Hide both players when we enter this state.

Revision 1.2.2.1.2.5  2005/08/26 21:16:01  jay
Changed comment

Revision 1.2.2.1.2.4  2005/08/07 08:12:50  Administrator
Added message for practice game

Revision 1.2.2.1.2.3  2005/08/06 20:22:46  Administrator
Show correct player name at start; show both players' balls.

Revision 1.2.2.1.2.2  2005/06/13 22:16:43  jay
Draw stats; but not if friendly game

Revision 1.2.2.1.2.1  2005/05/24 20:37:33  jay
Show player stats

Revision 1.2.2.1  2004/11/09 16:04:59  jay
Bonus points now depend on how many strokes it took to get the ball in the hole.

Revision 1.2  2004/09/15 09:22:27  Administrator
Remove dead code; improve winning player's stats

Revision 1.1  2004/09/08 15:43:04  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "EngineStatePoolBallInHole.h"
#include "EngineStatePoolSetUpShot.h"
#include "EngineStateThanksForPlaying.h"
#include "Engine.h"
#include "PoolGameState.h"
#include "LevelServer.h"
#include "PoolMisc.h"
#include "Trajectory.h"
#include "Controller.h"
#include "TextFactory.h"
#include "PoolCharacter.h"
#include "StringUtils.h"
#include "PoolCourseManager.h"
#include "TextWriter.h"
#include "TextColourizerColourList.h"
#include "PoolHole.h"
#include "EngineStateCongrats.h"
#include "EngineStatePoolCourseSelect.h"
#include "PoolPlayerStats.h"
#include "GuiButton.h"
#include "EngineStatePoolOnlineTable.h"
#include "PlayerNames.h"
#include "ThirdPersonCameraBase.h"

namespace Amju
{
extern void ReportError(const std::string&);

const char* EngineStatePoolBallInHole::Name = "ball_in_hole";

static const bool registered = Engine::Instance()->
  RegisterEngineState(EngineStatePoolBallInHole::Name, new EngineStatePoolBallInHole);

EngineStatePoolBallInHole::EngineStatePoolBallInHole()
{
  m_maxTime = 3.0f; // TODO CONFIG
  m_time = 0;
}

void EngineStatePoolBallInHole::OnNewGame()
{
}

void EngineStatePoolBallInHole::DrawStats(PlayerStats* pStats, float row)
{
  PoolPlayerStats* pPoolStats = dynamic_cast<PoolPlayerStats*>(pStats);
  if (pPoolStats)
  {
    int win, lose, draw;
    pPoolStats->GetStats(&win, &lose, &draw);
    std::string s = "Won: ";
    s += ToString(win);
    s += " Lost: ";
    s += ToString(lose);
    Engine::Instance()->GetTextWriter()->PrintCentre(row, s.c_str());
  }
}

void EngineStatePoolBallInHole::DrawOverlays()
{
  EngineStatePoolBase::DrawOverlays();

  DrawBonusesLeft();

  AmjuGL::Disable(AmjuGL::AMJU_DEPTH_READ);
  AmjuGL::PushMatrix();
  AmjuGL::Translate(m_bihLeft, m_bihTop, m_bihZ - 8.0f); // TODO TEMP TEST
  m_pText->Draw();
  AmjuGL::PopMatrix();
  AmjuGL::Enable(AmjuGL::AMJU_DEPTH_READ);

  // Draw played game stats
  // TODO Initialise once
  // Don't draw if friendly game
  if (Engine::Instance()->GetGameState()->IsFriendly())
  {
    // TODO INTL8N
    Engine::Instance()->GetTextWriter()->PrintCentre(9.0f, 
      "This was a practice game so stats are not affected.");
  }
  else
  {
    PlayerStats* pStats = 
      Engine::Instance()->GetGameState()->GetPlayerInfo(0)->GetPlayerStats();
    if (pStats)
    {
      std::string s = Engine::Instance()->GetGameState()->GetPlayerInfo(0)->m_name;
      Engine::Instance()->GetTextWriter()->PrintCentre(9.0f, s.c_str());
      DrawStats(pStats, 10.0f);
    }
    pStats = Engine::Instance()->GetGameState()->GetPlayerInfo(1)->GetPlayerStats();
    if (pStats)
    {
      std::string s = Engine::Instance()->GetGameState()->GetPlayerInfo(1)->m_name;
      Engine::Instance()->GetTextWriter()->PrintCentre(11.0f, s.c_str());
      DrawStats(pStats, 12.0f);
    }
  }
}

void EngineStatePoolBallInHole::Draw()
{
  EngineStatePoolText::Draw();
}

void EngineStatePoolBallInHole::SetActive(bool active)
{
  m_pLevel = LevelServer::Instance()->GetCurrentLevel();

  if (active)
  {
    // Don't do this here: do it in Rules, only if we replayed the final shot.
/*
#ifdef POOL_ONLINE
    if (IsOnlineGame())
    {
      ThePoolOnlineManager::Instance()->SendGameFinished();
    }
#endif
*/
    // Force birds eye view off - it may cause problems next game
    SetBirdsEye(false);

    SetIsGameOver(true);

    // Make camera rotate
    ((ThirdPersonCameraBase*)GetCamera().GetPtr())->SetRotateVel(10.0f, 0);
    GetCamera()->ZoomOut();

    // MAY19
    // Make player invisible, otherwise she appears on top of ball.
    // Do for both players or it looks weird. 
    // Keeping both players visible entails more work... TODO
    for (int i = 0; i < 2; i++)
    {
      int id = Engine::Instance()->GetGameState()->GetPlayerInfo(i)->m_id;
      GameObject* pGo = Engine::Instance()->GetGameObject(id).GetPtr();
      Assert(pGo);
      ((PoolCharacter*)pGo)->SetActionReady();
      ((PoolCharacter*)pGo)->SetAlpha(1.0f);

      pGo->SetState(UNKNOWN);
      static const Orientation Positions[2] = 
      {
        Orientation(40.0f, 0, 40.0f, 0, 225.0f, 0),
        Orientation(40.0f, 0, -40.0f, 0, -45.0f, 0)
      };
      pGo->SetOrientation(Positions[i]);
    }
    
    std::string text = Engine::Instance()->GetGameState()->GetCurrentPlayerInfo()->m_name;

    // Make sure player name is up to date.
    CreatePlayerName(text);

    text += " wins!";
    m_pText = TextFactory::Instance()->CreateMultiline(text);

    m_time = 0;
    m_expired = false;
    // Play sound effect of ball in hole
    Engine::Instance()->PlayWav(Engine::Instance()->GetConfigValue("golf_wav_9"));
    // "Yay!" acting
    Engine::Instance()->PlayWav(Engine::Instance()->GetConfigValue("golf_wav_12"));

    Engine::Instance()->GetGameState()->GetCurrentPlayerInfo()->m_isPlaying = false;
    // Play special music.
    std::string song = GetEngine()->GetConfigValue("golf_song_ball_in_hole");
    GetEngine()->PlaySong(song);

    // TODO CONFIG
//    Engine::Instance()->GetEarthquake().SetActive(5.0f, 20.0f);
    Engine::Instance()->SetLetterbox(true);
  }
  else
  {
    // Stop cam rotate!!
    ((ThirdPersonCameraBase*)GetCamera().GetPtr())->SetRotateVel(0, 0);

    Engine::Instance()->SetLetterbox(false);
  }
}

void EngineStatePoolBallInHole::TimerExpired()
{
#ifdef _DEBUG
std::cout << "TIMER EXP\n";
#endif

  if (m_expired)
  {
    // Already going to next Hole.
    return;
  }
  m_expired = true;
 
  // Save both player's stats. The idea is that a player may have had extra
  // bonuses awarded while this state was active.
  for (int i = 0; i < 2; i++)
  {
    PlayerStats* p = Engine::Instance()->GetGameState()->GetPlayerInfo(i)->
      GetPlayerStats();
    if (p)
    {
      if (!p->Save())
      {
#ifdef _DEBUG
std::cout << "FAILED to save player stats for " << i << "\n";
#endif
      }
    }
  }

#ifdef POOL_ONLINE
  if (IsOnlineGame())
  {
    // TODO Go to special state, "Would you like to play <opponent> again ?"
    // For now, go back to online table
    GetEngine()->ChangeState(EngineStatePoolOnlineTable::Name, Engine::IMMEDIATE);
  }
  else
  { 
    GetEngine()->ChangeState(EngineStatePoolCourseSelect::Name, Engine::IMMEDIATE);
  }
#else
  GetEngine()->ChangeState(EngineStatePoolCourseSelect::Name, Engine::IMMEDIATE);
#endif
}

void EngineStatePoolBallInHole::Update()
{
  static float  bonusTime = 0;
  bonusTime += GetEngine()->GetDeltaTime();

  // POOL: Show both players' balls
  if (bonusTime > 3.0f)
  {
    bonusTime = 0;
    Engine::Instance()->GetGameState()->RotateToNextPlayer();
    std::string name = Engine::Instance()->GetGameState()->GetCurrentPlayerInfo()->m_name;

    CreatePlayerName(name);
  }

  EngineStatePoolText::Update();
}

bool EngineStatePoolBallInHole::Load()
{
  if (!EngineStatePoolText::Load())
  {
    return false;
  }

  // Explosion effect - horribly pasted from Title state
  m_maxTime = 30;

  return true;
}

void EngineStatePoolBallInHole::MouseButton(bool down, bool, bool)
{
  if (!down && m_time > 3.0f)
  {
    TimerExpired();
  }
}
}

