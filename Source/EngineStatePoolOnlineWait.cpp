/*
Amju Games source code (c) Copyright Jason Colman 2000-2006
$Log: EngineStatePoolOnlineWait.cpp,v $
Revision 1.1.2.15  2007/08/03 17:53:49  jay
Use OnlineGame state checking functions
DeleteUnusedGame moved to Table state

Revision 1.1.2.14  2007/08/02 08:48:46  jay
Replace hard-coded times with PoolOnline::Constants.
These can be updated from the server in future.

Revision 1.1.2.13  2007/07/28 12:55:20  Administrator
Don't crash if no opponent name - but show error msg at least for Beta

Revision 1.1.2.12  2007/07/25 07:52:31  jay
Fix so all game messages are picked up by the client before starting the
game. This fixes bug where client 1 clicked on Accept and Chose a game
type before client 2 registered the Accept message.

Revision 1.1.2.11  2007/07/19 17:29:10  jay
Use opponent name

Revision 1.1.2.10  2007/07/18 22:17:43  jay
Improvements to Table/Wait/Accept/Decline states

Revision 1.1.2.9  2007/06/30 14:36:22  jay
Improve presentation

Revision 1.1.2.8  2007/05/26 21:04:40  jay
Fixes for Table state after one game, etc.

Revision 1.1.2.7  2007/05/23 18:45:42  jay
Send all object changes in one Http request (POST), not individually.
 + Some tidying up.

Revision 1.1.2.6  2007/05/15 09:41:10  jay
Polls sever, goes to game chosen by opponent

Revision 1.1.2.5  2007/03/09 21:50:41  jay
Online game - ongoing fixes

Revision 1.1.2.4  2006/08/30 21:12:14  Administrator
Get Pool to build in MSVC

Revision 1.1.2.3  2006/08/26 21:28:36  jay
Ongoing changes for Online game; all Online features are off unless
POOL_ONLINE is defined

Revision 1.1.2.2  2006/08/14 17:50:14  jay
Rename "Pool"

Revision 1.1.2.1  2006/08/07 20:42:50  jay
Online Pool - not working yet

*/

#ifdef POOL_ONLINE

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "EngineStatePoolOnlineWait.h"
#include "Engine.h"
#include "TextWriter.h"
#include "PoolMisc.h"
#include "LevelServer.h"
#include "PoolBg.h"
#include "PoolGameState.h"
#include "PoolOnline.h"
#include "EngineStatePoolGameSelect.h"
#include "EsPoolDeclined.h"
#include "EngineStatePoolOnlineTable.h"

namespace Amju
{
const char* EngineStatePoolOnlineWait::Name = "pool-online-wait";
static const bool registered= Engine::Instance()->
  RegisterEngineState(EngineStatePoolOnlineWait::Name,
  new EngineStatePoolOnlineWait);

void ReportError(const std::string&);

void OnlineWaitCancel()
{
  // Set the state of the row in the game table so the opponent
  // knows we have canceled.
 
  ThePoolOnlineManager::Instance()->SendGameFinished();

  Engine::Instance()->ChangeState(EngineStatePoolGameSelect::Name,
    Engine::IMMEDIATE);
}

bool EngineStatePoolOnlineWait::Load()
{
  m_pCancelButton = new PoolGuiButton;
  if (!m_pCancelButton->Load("pool-online-table-cancel-button.txt"))
  {
    ReportError("Failed to load online table cancel button");
    return false;
  }
  m_pCancelButton->SetCommand(&OnlineWaitCancel);
  m_pCancelButton->SetSize(4.0f, 1.0f);
  m_pCancelButton->SetRelPos(14.5f, 10.0f); // TODO TEMP TEST

  if (!m_bg.Load(GetEngine()->GetConfigValue("file_bg"), ""))
  {
    ReportError("Failed to load BG for online wait state");
    return false;
  }

  return true;
}

void EngineStatePoolOnlineWait::Draw()
{
  AmjuGL::PushMatrix();
  DrawBg();
  
  m_pCam->Update();
  m_pCam->Draw(); 
  
  m_lighting.DrawLight();
 
  m_or.Draw();

  ThePoolBg::Instance()->Draw();
  AmjuGL::PopMatrix();
}

void EngineStatePoolOnlineWait::DrawOverlays()
{
  // TODO is opponent always player 0 if we are waiting ? 
  std::string opponent = ThePoolOnlineManager::Instance()->GetOpponentName(); 
  
  if (opponent.empty())
  {
    Engine::Instance()->GetTextWriter()->PrintCentre(6.0f, "Please wait... (unknown opponent name!)");
  }
  else
  {
    opponent[0] = toupper(opponent[0]);
  
    std::string s;
    if (m_waitState == 0)
    {
      s = "Please wait for a reply from " + opponent + "...";
    }  
    else if (m_waitState == 1)
    {
      s = "Please wait while " + opponent + " chooses the game...";
    }
    Engine::Instance()->GetTextWriter()->PrintCentre(6.0f, s.c_str());
  }

  m_pCancelButton->Draw();
}

void EngineStatePoolOnlineWait::Update()
{
  EngineStateText::Update();

  ThePoolOnlineManager::Instance()->Update();

  // Check online to see if opponent has set level/room yet.
  // When set, we can load the level and start.
  bool opponentHasChosen = m_chosenLevelId != -1 && m_chosenRoomId != -1;

  if (opponentHasChosen)
  {
#ifdef _DEBUG
std::cout << "WAIT STATE: Opponent has chosen game type, STARTING GAME!!\n";
#endif

    StartCourse(m_chosenLevelId);
    PLevel pLevel = LevelServer::Instance()->GetCurrentLevel();
    pLevel->SetRoomId(m_chosenRoomId);

    StartNewHole(); // TODO rename this
  }
}

void EngineStatePoolOnlineWait::SetActive(bool active)
{
  if (active)
  {
    m_time = 0;

    // Poll Game to check room and level
    m_maxTime = ThePoolOnlineManager::Instance()->GetConstant(PoolOnline::ACCEPT_DECLINE_POLL_PERIOD);

    m_chosenLevelId = -1;
    m_chosenRoomId = -1;
    m_waitState = 0;
  }
}

void EngineStatePoolOnlineWait::TimerExpired()
{
  m_time = 0;

  // TOOD We should set a callback or something for when the list changes.
  // We just poll for now.
  const PoolOnline::OnlineGame* game = ThePoolOnlineManager::Instance()->GetCurrentGame();

#ifdef _DEBUG
std::cout << "***CHECKING GAME: ";
if (game)
{
  std::cout <<  game->m_id;
}
else
{
  std::cout << "<gone away>";
}
std::cout << "\n";
#endif

  if (!game || game->IsDeleted())
  {
    // Opponent has canceled
    Engine::Instance()->ChangeState(EsPoolDeclined::Name, Engine::IMMEDIATE);
    return;
  }

#ifdef _DEBUG
  if (game)
  {
    std::cout << "Got current game info: id: " << game->m_id
      << " type: " << game->m_type
      << " state: " << game->m_state
      << "\n";
  }
  else
  {
    std::cout << "Can't get current game info!!! Looks like we will never get the game type!\n";
  }
#endif

  // Check if opponent has Accepted: start game, wait for opponent to choose
  // game type.
  if (game->IsInProgress())
  {
    // Delete the game which has us as player 0, waiting for an
    //  opponent to join.
    int thisPlayerId = ThePoolOnlineManager::Instance()->GetPlayerId();

    EngineStatePoolOnlineTable* et = dynamic_cast<EngineStatePoolOnlineTable*>(Engine::Instance()->GetEngineState(EngineStatePoolOnlineTable::Name).GetPtr());
    Assert(et);

    // Delete the new game we created when we entered the Table state.
    et->DeleteUnusedGame();

    et->StartOnlineGame(thisPlayerId, game->m_playerId[0], false /* opponent chooses */);
    m_waitState = 1;
  }

  // Make sure we have called StartOnlineGame before setting the level/room and
  // exiting this state.
  if (game && game->m_type != -1 && m_waitState == 1)
  {
    // Get level and room - type is 1000 * L + R
    int level = game->m_type / 1000; 
    int room =  game->m_type % 1000;

    SetGameTypeChosenByOpponent(level, room);
    // No need to poll again
    return;
  }  

  // Poll game DB row on server
  // We do this after checking the list, so we don't make an extra request we
  //  don't need. 
  ThePoolOnlineManager::Instance()->RequestCurrentGameStatus(); 
}

void EngineStatePoolOnlineWait::SetGameTypeChosenByOpponent(int lev, int room)
{
  m_chosenLevelId = lev;
  m_chosenRoomId = room;
}

void EngineStatePoolOnlineWait::MouseButton(bool down, bool ctrl, bool shift)
{
  m_pCancelButton->MouseButton(down, ctrl, shift);
}
}

#endif

