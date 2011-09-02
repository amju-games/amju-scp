/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: EngineStatePoolOnlineTable.cpp,v $
Revision 1.1.2.41  2007/08/04 11:08:05  Administrator
Fix smaller font size

Revision 1.1.2.40  2007/08/04 10:37:52  Administrator
Make font smaller for VS lines

Revision 1.1.2.39  2007/08/03 17:52:49  jay
DeleteUnusedGame added: store ID of new game, delete it if it ends up
not being used. Saves the server having to send all games involving the
current player.

Revision 1.1.2.38  2007/08/02 09:15:05  jay
Play wav when number of players increases, and when invited to play

Revision 1.1.2.37  2007/08/02 08:48:46  jay
Replace hard-coded times with PoolOnline::Constants.
These can be updated from the server in future.

Revision 1.1.2.36  2007/07/30 06:10:06  jay
Wait before refreshing

Revision 1.1.2.35  2007/07/28 12:54:02  Administrator
Check state is 10 before going to Accept/Decline state

Revision 1.1.2.34  2007/07/27 08:43:02  jay
Debug-only code

Revision 1.1.2.33  2007/07/25 07:50:57  jay
Remove call to StartOnlineGame which is no longer used - it's called from
 Wait state.

Revision 1.1.2.32  2007/07/21 23:42:37  Administrator
Store Online ID in player names: only request ID from server when uploading
stats for playing an online game for the first time.

Revision 1.1.2.31  2007/07/19 17:28:54  jay
Set opponent ID as soon as it is known, so we can display name.

Revision 1.1.2.30  2007/07/18 22:17:43  jay
Improvements to Table/Wait/Accept/Decline states

Revision 1.1.2.29  2007/07/12 20:25:41  jay
Disable all player buttons if waiting for accept/decline

Revision 1.1.2.28  2007/07/08 22:09:52  Administrator
Flag for when we are waiting for Accept from other player

Revision 1.1.2.27  2007/07/08 19:50:26  Administrator
Fix numbers of players waiting/games in progress

Revision 1.1.2.26  2007/07/07 08:17:47  jay
Try to centre stats text

Revision 1.1.2.25  2007/07/05 21:14:07  jay
Show time since last connect for each player

Revision 1.1.2.24  2007/07/03 23:10:49  Administrator
Fix number of players waiting

Revision 1.1.2.23  2007/07/01 20:51:55  jay
Show duplicated names with a heart + number at the end of the name

Revision 1.1.2.22  2007/07/01 20:28:38  jay
If we cancel, make sure Online game flag is reset (breaks demo mode)

Revision 1.1.2.21  2007/07/01 19:18:41  jay
Improve status reporting

Revision 1.1.2.20  2007/06/30 14:32:26  jay
Improve presentation

Revision 1.1.2.19  2007/06/28 16:31:46  Administrator
Fix up/down scrolling

Revision 1.1.2.18  2007/06/23 11:29:36  jay
Set opponent ID in PoolOnline manager so we can check he is still there.
Capitalise opponent name

Revision 1.1.2.17  2007/05/26 23:15:38  Administrator
Reset visible table

Revision 1.1.2.16  2007/05/26 21:04:39  jay
Fixes for Table state after one game, etc.

Revision 1.1.2.15  2007/05/16 22:08:14  Administrator
Set up PlayerInfo for online game so we can see/add to hearts -- NOT TESTED

Revision 1.1.2.14  2007/05/15 09:32:58  jay
Added more states: accept/decline and game select

Revision 1.1.2.13  2007/05/12 21:46:25  Administrator
GUI improvements for POOL_ONLINE

Revision 1.1.2.12  2007/05/12 14:16:16  jay
Don't use table, use same code as ChoosePlayer state, so we can show stats

Revision 1.1.2.11  2007/03/10 12:58:09  jay
Pool Online - delete game when finished

Revision 1.1.2.10  2007/03/09 21:50:41  jay
Online game - ongoing fixes

Revision 1.1.2.9  2006/08/30 21:12:14  Administrator
Get Pool to build in MSVC

Revision 1.1.2.8  2006/08/30 14:51:18  jay
Added BG

Revision 1.1.2.7  2006/08/26 21:28:36  jay
Ongoing changes for Online game; all Online features are off unless
POOL_ONLINE is defined

Revision 1.1.2.6  2006/08/14 17:50:14  jay
Rename "Pool"

Revision 1.1.2.5  2006/08/12 07:43:08  jay
Ongoing fixes for Online Pool: currently trying to get both clients to always
be consistent.

Revision 1.1.2.4  2006/08/07 20:42:50  jay
Online Pool - not working yet

Revision 1.1.2.3  2006/07/30 21:58:30  jay
Still broken, checking in for backup

Revision 1.1.2.2  2006/07/26 21:21:56  jay
Use new PoolOnline class

Revision 1.1.2.1  2006/07/24 18:20:32  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.2  2005/10/29 10:16:09  jay
Added more Online functionality - ongoing, not finished.

Revision 1.1.2.1  2005/10/21 17:44:03  jay
Not finished, the idea here is to show the "lobby" of games being played
 or waiting for a second player.

*/

#ifdef POOL_ONLINE

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "EngineStatePoolOnlineTable.h"
#include "EngineStatePoolOnlineWait.h"
#include "Engine.h"
#include "PoolGameState.h"
#include "PoolOnline.h"
#include "TextWriter.h"
#include "StringUtils.h"
#include "GuiText.h"
#include "GuiTextButton.h"
#include "EngineStatePoolGameSelect.h"
#include "PoolMisc.h"
#include "EngineStatePoolCourseSelect.h"
#include "PoolPlayerStats.h"
#include "PoolBg.h"
#include "EsPoolOnlineAccept.h"
#include "EsPoolOnlineGameSelect.h"
#include "TimePeriod.h"

namespace Amju
{
const char* EngineStatePoolOnlineTable::Name = "pool-online-table";
static const bool registered= Engine::Instance()->
  RegisterEngineState(EngineStatePoolOnlineTable::Name, 
  new EngineStatePoolOnlineTable);

extern void ReportError(const std::string&);

void OnScrollUpButton()
{
  static const float scrollvel = Engine::Instance()->GetConfigFloat(
    "help_scroll_vel");

  ((EngineStatePoolOnlineTable*)
    Engine::Instance()->GetEngineState().GetPtr())->
    SetScrollVel(scrollvel); 
}

void OnScrollDownButton()
{
  static const float scrollvel = Engine::Instance()->GetConfigFloat(
    "help_scroll_vel");

  ((EngineStatePoolOnlineTable*)
    Engine::Instance()->GetEngineState().GetPtr())->
    SetScrollVel(-scrollvel); 
}

// Class for Join button commands
class OnlineJoinCommand : public GuiCommand
{
public:
  OnlineJoinCommand(int id) : m_id(id) {}
  virtual bool Do()
  {
std::cout << "We want to join game " << m_id << ", requesting....\n";

    ((EngineStatePoolOnlineTable*)
      Engine::Instance()->GetEngineState(EngineStatePoolOnlineTable::Name
      ).GetPtr())->RequestJoinGame(m_id);
    
    return false; // no undo
  }
private:
  int m_id;
};

class OnlineWatchCommand : public GuiCommand
{
public:
  OnlineWatchCommand(int id) : m_id(id) {}
  virtual bool Do()
  {
    ((EngineStatePoolOnlineTable*)
      Engine::Instance()->GetEngineState(EngineStatePoolOnlineTable::Name
      ).GetPtr())->RequestWatchGame(m_id);
    
    return false; // no undo
  }
private:
  int m_id;
};

void OnlineCancel()
{
  SetIsOnlineGame(false);
  ThePoolOnlineManager::Instance()->Reset();

  Engine::Instance()->ChangeState(EngineStatePoolGameSelect::Name,
    Engine::IMMEDIATE);
}

EngineStatePoolOnlineTable::EngineStatePoolOnlineTable()
{
  m_unusedGameId = -1;
}

void EngineStatePoolOnlineTable::DeleteUnusedGame()
{
  if (m_unusedGameId != -1)
  {
    ThePoolOnlineManager::Instance()->DeleteGame(m_unusedGameId);
    m_unusedGameId = -1;
  }
}

void EngineStatePoolOnlineTable::SetScrollVel(float v)
{
  m_yVel = v;
}

void EngineStatePoolOnlineTable::SetPlayerName(const std::string& name, int playerId)
{
  Assert(!name.empty());
  // NB ID can be -1 => request from server

  m_playerName = name;
  m_playerId = playerId;
}

void EngineStatePoolOnlineTable::SetActive(bool active)
{
  if (active)
  {
    // We may not have already deleted any unused game.
    DeleteUnusedGame(); 

    m_createdNewGame = false;
    m_weAreWaitingForAccept = false;
    m_noPlayerInfoYet = true;
    m_numGamesWaiting = 0;
    m_numGamesInProgress = 0;

    m_yVel = 0;
    m_y = 0;
    m_bottomY = 0;

    // Reset visible table
    m_buttons.clear();
    m_y = 0;
    m_yVel = 0;

    // Reset ID etc. This forces us to get ID from server and create new
    //  row in game table.
    ThePoolOnlineManager::Instance()->Reset();

    ThePoolOnlineManager::Instance()->SetPlayerName(m_playerName, m_playerId, true /* get ID if not set */);
    // The above call requests ID from server if needed.
    // so no need for: ThePoolOnlineManager::Instance()->RequestPlayerId();
    
#ifdef _DEBUG
std::cout << "ONLINE TABLE STATE... Register player with server!\n"
  << "Player name: " << m_playerName.c_str() << "\n";
#endif

    m_maxTime = ThePoolOnlineManager::Instance()->
      GetConstant(PoolOnline::TABLE_REFRESH_PERIOD);

    m_time = m_maxTime * 0.5f; // wait half of period before refreshing
  }
}

void EngineStatePoolOnlineTable::RequestRefresh()
{
  if (ThePoolOnlineManager::Instance()->GetPlayerId() == -1)
  {
#ifdef _DEBUG
std::cout << "OnlineTable: player ID is -1, so don't refresh\n";
#endif
    // Waiting to get ID back from server.
    return;
  }
#ifdef _DEBUG
std::cout << "OnlineTable: player ID is "
  << ThePoolOnlineManager::Instance()->GetPlayerId()
  << " so DO refresh\n";
#endif

  ThePoolOnlineManager::Instance()->RequestGamesListRefresh();
}

void EngineStatePoolOnlineTable::StartOnlineGame(int thisPlayerId, int opponentId, int chooseGame)
{
#ifdef _DEBUG
std::cout << "FOUND A PARTNER, START GAME!\n";
std::cout << "This player ID: " << thisPlayerId <<"\n";
std::cout << "Opponent ID:    " << opponentId << "\n";
#endif

  ThePoolOnlineManager::Instance()->SetOpponentId(opponentId);

  // Set this so we know who starts, and who polls for shot info.
  ThePoolOnlineManager::Instance()->SetThisIsPlayer1(chooseGame != 0);

  StartGame(2, false /* friendly */, false /* p1 online */, true /* p2 online */);

  // Set online player names
  const PoolOnline::Players& players = ThePoolOnlineManager::Instance()->
    GetPlayersOnline();
  std::string thisName = const_cast<PoolOnline::Players&>(players)[thisPlayerId].m_name;
  std::string opponentName = const_cast<PoolOnline::Players&>(players)[opponentId].m_name;
  opponentName[0] = toupper(opponentName[0]);

  if (chooseGame)
  {
    Engine::Instance()->GetEngineState()->GetPlayerInfo(0)->m_name = thisName;
    Engine::Instance()->GetEngineState()->GetPlayerInfo(0)->m_isAi = false;
    Engine::Instance()->GetEngineState()->GetPlayerInfo(0)->m_isOnline = false;

    Engine::Instance()->GetEngineState()->GetPlayerInfo(1)->m_name = opponentName;
    Engine::Instance()->GetEngineState()->GetPlayerInfo(1)->m_isAi = false;
    Engine::Instance()->GetEngineState()->GetPlayerInfo(1)->m_isOnline = true;

    PoolPlayerStats::SetPlayerStats(thisName, 0); // fixes hearts 
  }
  else
  {
    Engine::Instance()->GetEngineState()->GetPlayerInfo(1)->m_name = thisName;
    Engine::Instance()->GetEngineState()->GetPlayerInfo(1)->m_isAi = false;
    Engine::Instance()->GetEngineState()->GetPlayerInfo(1)->m_isOnline = false;

    Engine::Instance()->GetEngineState()->GetPlayerInfo(0)->m_name = opponentName;
    Engine::Instance()->GetEngineState()->GetPlayerInfo(0)->m_isAi = false;
    Engine::Instance()->GetEngineState()->GetPlayerInfo(0)->m_isOnline = true;

    PoolPlayerStats::SetPlayerStats(thisName, 1); 
  }

  // If we initiated the game, we can now choose the level/room.
  // If we joined the game, the opponent chooses the level/room.
  if (chooseGame)
  {
    // We go to "Course" select screen and choose a game type.
    // NB Only a subset of levels should be avaiable for online 2-player
    // TODO
    Engine::Instance()->ChangeState(
      EsPoolOnlineGameSelect::Name,
      Engine::IMMEDIATE);

    Engine::Instance()->GetEngineState()->SetCurrentPlayer(0); // this player starts
  }
  else
  {
    // Waiting state: wait for opponent to set the room and level
    Engine::Instance()->ChangeState(
      EngineStatePoolOnlineWait::Name,
      Engine::IMMEDIATE);

    Engine::Instance()->GetEngineState()->SetCurrentPlayer(0); 
  }
}

void EngineStatePoolOnlineTable::RefreshTable(
  const PoolOnline::Players& players,
  const PoolOnline::OnlineGames& games)
{
  int thisPlayerId = ThePoolOnlineManager::Instance()->GetPlayerId();
#ifdef _DEBUG
std::cout << "This player ID: " << thisPlayerId << "\n";
#endif

  if (thisPlayerId == -1)
  {
    ThePoolOnlineManager::Instance()->RequestPlayerId();
    return;
  }

  Assert(thisPlayerId > -1);
 
  m_noPlayerInfoYet = false;

  // Play sound if number of waiters increases
  int prevNumGamesWaiting = m_numGamesWaiting;

  m_numGamesWaiting = 0;
  m_numGamesInProgress = 0;

  if (games.empty())
  {
#ifdef _DEBUG
std::cout << "No games, so nothing to display :-(\n";
#endif

  }
  else
  {
    // If there are any active games, there must be players, right ?
    Assert(!players.empty());
  }
  m_buttons.clear();

  int gameNum = 1;
  int buttonNum = 0;
  for (
    PoolOnline::OnlineGames::const_iterator it = games.begin();
    it != games.end();
    ++it)
  {
    const PoolOnline::OnlineGame& g = *it; 
    std::string s;
    s += "Game ID: ";
    s += ToString(g.m_id);
#ifdef _DEBUG
std::cout << "**** Game " << gameNum << ": " << s.c_str() << "  ";
#endif
    gameNum++;

    // Create a BUTTON, add to table
    std::string text;
    std::string statText;

    const PoolOnline::OnlinePlayer& p1 = 
      const_cast<PoolOnline::Players&>(players)[g.m_playerId[0]];

    const PoolOnline::OnlinePlayer& p2 = 
      const_cast<PoolOnline::Players&>(players)[g.m_playerId[1]];

    std::string p1name = p1.m_name;
    std::string p2name = p2.m_name; 

    if (p1name.empty())
    {
      if (p2.m_nameExtra > 0)
      {
        // No brackets in font - just using one to make a heart
        p2name += " ) " + ToString(p2.m_nameExtra); 
      }

      text = p2name;
      statText = "won " + ToString(p2.m_won) + " lost " + ToString(p2.m_lost) + " \" " + ToString(p2.m_hearts); 

      // Add time since last connection to stat text
      statText += " waiting ";
      statText += TimePeriod(p2.m_lastTime).ToString();
    } 
    else if (p2name.empty())
    {
      if (p1.m_nameExtra > 0)
      {
        p1name += " ) " + ToString(p1.m_nameExtra);
      }

      text = p1name;
      statText = "won " + ToString(p1.m_won) + " lost " + ToString(p1.m_lost) + " \" " + ToString(p1.m_hearts); 

#ifdef POOL_ONLINE_BETA_TEST
      // Add time since last connection to stat text
      statText += " waiting ";
      statText += TimePeriod(p1.m_lastTime).ToString();
#endif
    }
    else
    {
      // Neither name is empty
      text = p1name + " vs " + p2name;
    }

#ifdef _DEBUG
std::cout << "P1: id: " << g.m_playerId[0] << ": "  << p1name.c_str() << "  ";
std::cout << "P2: id: " << g.m_playerId[1] << ": " << p2name.c_str() << "  ";
#endif

    if (g.m_playerId[0] == thisPlayerId && g.m_playerId[1] == -1)
    {
      // This is us, waiting for someone to join. Don't display.
      // Remember this game ID, because if we join another game we will
      // want to delete this unused game.
      m_unusedGameId = g.m_id;

      // Don't create button for this game
      continue;
    }
    else if (g.m_playerId[0] == thisPlayerId && g.IsWaitingForAccept() /* && g.m_playerId[1] != -1 */)
    {
      // Player2 has requested to join this game. We now accept or decline.
      ThePoolOnlineManager::Instance()->SetGameId(g.m_id);
      ((EsPoolOnlineAccept*) Engine::Instance()->GetEngineState(EsPoolOnlineAccept::Name).GetPtr())->
        Set(thisPlayerId, g.m_playerId[1], p2name, g.m_id);
      Engine::Instance()->ChangeState(EsPoolOnlineAccept::Name, Engine::IMMEDIATE);

      // Play a wav - we have been invited to play!
      Engine::Instance()->PlayWav("bonus_points.wav");

      return;
    }
    else if (g.m_playerId[1] == thisPlayerId && g.m_playerId[0] != -1)
    {
#ifdef _DEBUG
std::cout << "Found partner! We are player 2\n";
#endif
      ThePoolOnlineManager::Instance()->SetGameId(g.m_id);
    
      Engine::Instance()->ChangeState(EngineStatePoolOnlineWait::Name,
        Engine::IMMEDIATE);
      return;
    }
/*
// This suppresses an opponent :-(
    else if (g.m_playerId[0] != -1 &&
             g.m_playerId[0] != thisPlayerId &&
             g.m_playerId[1] != -1 &&
             g.m_playerId[1] != thisPlayerId)
    {
      // Two players, neither of whom is us - ignore
      ++m_numGamesInProgress;
      continue;
    }
*/

    SharedPtr<GuiButton> pButton = new GuiButton;
    pButton->GuiElement::Load("pool-player-button.txt");
    // If the game has 2 players and is in progress, we can Watch.
    // If there is only one player, the game is waiting, and we can Join.
    // But the state could change, and a waiting game could even be joined by
    // someone else after we clicked on "join". So we must attempt
    // to join, then check if we were successful.
    if (g.IsWaiting())
    {
#ifdef _DEBUG
std::cout << " ..JOIN?";
#endif
      pButton->SetCommand(new OnlineJoinCommand(g.m_id));

      ++m_numGamesWaiting;
    }
    else
    {
#ifdef _DEBUG
std::cout << " ..WATCH?";
#endif
      //text += "Playing.. click to watch";
      //pButton->SetCommand(new OnlineWatchCommand(g.m_id));
      // TODO v.2.1 allow player to watch a game between 2 other players
      pButton->SetEnabled(false);

      ++m_numGamesInProgress;
    }
    pButton->SetSize(12.0f, 2.0f); // TODO CONFIG
    static const float LEFT = 6.0f; // TODO CONFIG
    float top = 2.5f + 2.2f * (float)buttonNum;
    ++buttonNum;
    pButton->SetRelPos(top, LEFT); // top,left
    m_bottomY = top; // store lowest position

    OnlineButton b;
    b.m_name = text;
    b.m_stats = statText;
    b.m_pButton = pButton;
    m_buttons.push_back(b);

#ifdef _DEBUG
std::cout << "\n";
#endif
  }

  // When we click a button, disable all buttons. 
  // When we get a confirmation that the button press has been registered
  // by the server, go to the Waiting state.
  if (m_weAreWaitingForAccept)
  {
    for (int i = 0; i < (int)m_buttons.size(); i++)
    {
      m_buttons[i].m_pButton->SetEnabled(false);
    }
  }

  // Play a sound if there are now more players waiting than before
  if (prevNumGamesWaiting < m_numGamesWaiting)
  {
    Engine::Instance()->PlayWav("bonus_points.wav");
  }
}

bool EngineStatePoolOnlineTable::Load()
{
  m_pCancelButton = new GuiButton;
  if (!m_pCancelButton->Load("pool-online-table-cancel-button.txt"))
  {
    ReportError("Failed to load online table cancel button");
    return false;
  }
  m_pCancelButton->SetCommand(&OnlineCancel);
  m_pCancelButton->SetSize(4.0f, 1.0f);
  m_pCancelButton->SetRelPos(14.5f, 10.0f); // TODO TEMP TEST

  if (!m_bg.Load(GetEngine()->GetConfigValue("file_bg"), ""))
  {
    ReportError("Failed to load BG for table state");
    return false;
  }

  m_pScrollUpButton = new GuiButton;
  if (!m_pScrollUpButton->Load("pool-up-button.txt"))
  {
    ReportError("Help state: failed to load scroll up button.");
    return false;
  }
  m_pScrollUpButton->SetSize(2.0f, 2.0f);
  m_pScrollUpButton->SetRelPos(1.0f, 20.0f); // TODO TEMP TEST
  m_pScrollUpButton->SetCommand(&OnScrollUpButton);
  
  m_pScrollDownButton = new GuiButton;
  if (!m_pScrollDownButton->Load("pool-down-button.txt"))
  {
    ReportError("Help state: failed to load scroll down button.");
    return false;
  }
  m_pScrollDownButton->SetSize(2.0f, 2.0f);
  m_pScrollDownButton->SetRelPos(13.0f, 20.0f); // TODO TEMP TEST
  m_pScrollDownButton->SetCommand(&OnScrollDownButton);

  return true;
}

void EngineStatePoolOnlineTable::DrawOverlaysText()
{
  if (m_noPlayerInfoYet)
  {
    Engine::Instance()->GetTextWriter()->PrintCentre(6.0f, 
      "Connecting to server, please wait...");
  }
  else if (m_weAreWaitingForAccept)
  {
    Engine::Instance()->GetTextWriter()->PrintCentre(0,
      "Please wait for a reply...");
  }
  else 
  {
    if (m_numGamesWaiting == 0)
    {
      Engine::Instance()->GetTextWriter()->PrintCentre(6.0f, 
        "Sorry, there seems to be no-one waiting to play right now.");

      // Plural/singular
      std::string s;
      if (m_numGamesInProgress == 0)
      {
        s = "(There are no games in progress)";
      }
      else if (m_numGamesInProgress == 1)
      {
        s = "(There is one game in progress)";
      }
      else
      {
        s = "(There are ";
        s += ToString(m_numGamesInProgress);
        s += " games in progress)";
      }
      Engine::Instance()->GetTextWriter()->PrintCentre(7.0f, s.c_str());
    }
    else
    {
      Engine::Instance()->GetTextWriter()->PrintCentre(0,
        "Please click on a player to invite...");

      // Show number of games somewhere
      std::string s = ToString(m_numGamesWaiting);
      s += " player";
      if (m_numGamesWaiting != 1)
      {
        s += "s";
      }
      s += " waiting, ";
      s += ToString(m_numGamesInProgress);
      s += " game";
      if (m_numGamesInProgress != 1)
      {
        s += "s";
      }
      s += " in progress";
      Engine::Instance()->GetTextWriter()->PrintCentre(1.0f, s.c_str());
    }
  }
  
  Engine::Instance()->GetTextWriter()->PrintCentre(15.5f, 
    ThePoolOnlineManager::Instance()->GetLastStatusMsg().c_str());
}

void EngineStatePoolOnlineTable::DrawOverlays()
{
  Font* pFont = TheFontManager::Instance()->GetFont("cheri-1.0");
  const float size = pFont->GetSize(); 
  for (int i = 0; i < (int)m_buttons.size(); i++)
  {
    const OnlineButton& pb = m_buttons[i];

    float x = 0, y = 0; 
    pb.m_pButton->GetRelPos(&y, &x); // sigh, (top, left)
    y += m_y;

    // Don't draw at top/bottom of screen
    if (y > 12.0f) // TODO CONFIG
    {
      break;
    }

    if (y < 1.5f)
    {
      continue;
    }

    pb.m_pButton->SetRelPos(y, x); 

    pb.m_pButton->Draw();

    float scale = 1.0f;
    if (pb.m_pButton->IsMouseOver() || pb.m_pButton->IsSelected())
    {
      scale *= 1.1f;
    }
    float e = 0;
    if (pb.m_pButton->IsSelected())  
    {
      e = 0.1f;
    }

    // Make "player1 vs player2" text smaller
    float smaller = 1.0f;
    if (pb.m_stats.empty())
    {
      smaller = 0.8f; // TODO CONFIG
    }

    pFont->SetSize(size * scale * smaller);
    float centre = 12.0f - 2.5f * pFont->GetTextWidth(pb.m_name.c_str());
    pFont->PrintNoBlend(centre + e, y + 0.3f + e, pb.m_name.c_str());

    // Draw stats etc string
    pFont->SetSize(size * scale * 0.45f); // small so time etc all fits
    float centre2 = 12.0f - 1.4f * pFont->GetTextWidth(pb.m_stats.c_str());
    pFont->PrintNoBlend(centre2 + e, y + 0.8f + e, pb.m_stats.c_str());

    y -= m_y;
    pb.m_pButton->SetRelPos(y, x);
  }

  pFont->SetSize(size);

  m_pCancelButton->Draw();
  m_pScrollUpButton->Draw();
  m_pScrollDownButton->Draw();

  DrawOverlaysText();
}

void EngineStatePoolOnlineTable::Draw()
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

bool operator==(const PoolOnline::OnlineGame& og, int id)
{
  return id == og.m_id;
}

void EngineStatePoolOnlineTable::RequestJoinGame(int gameId)
{
#ifdef _DEBUG
std::cout << "Request to join game " << gameId << "\n";
#endif

  // Send request to join. 
  ThePoolOnlineManager::Instance()->RequestJoinGame(gameId);

  // Set opponent ID, so we can display messages with the opponent name.
  const PoolOnline::OnlineGames& games = ThePoolOnlineManager::Instance()->GetGames();
  PoolOnline::OnlineGames::const_iterator it = std::find(games.begin(), games.end(), gameId);
  if (it != games.end())
  {
    const PoolOnline::OnlineGame& og = *it;
    int opponentId = og.m_playerId[0];
    ThePoolOnlineManager::Instance()->SetOpponentId(opponentId);
  }

  m_weAreWaitingForAccept = true;

  // Immediately disable all buttons
  for (int i = 0; i < (int)m_buttons.size(); i++)
  {
    m_buttons[i].m_pButton->SetEnabled(false);
  }
}

void EngineStatePoolOnlineTable::RequestWatchGame(int gameId)
{
#ifdef _DEBUG
std::cout << "Request to watch game " << gameId << "\n";
#endif

}

void EngineStatePoolOnlineTable::Update()
{
  EngineStateText::Update();

  // TODO This should be called by Engine, or EngineState, so it's
  // always updated.
  ThePoolOnlineManager::Instance()->Update();

  // If we got a player ID, create a new empty game
  // But only do this once!
  if (!m_createdNewGame && 
      ThePoolOnlineManager::Instance()->GetPlayerId() != -1)
  {
    ThePoolOnlineManager::Instance()->CreateNewGame();
    m_createdNewGame = true;
  }

  // decelerate scrolling
  static const float accel = Engine::Instance()->GetConfigFloat(
    "help_scroll_accel");
  float dt = Engine::Instance()->GetDeltaTime();

#ifdef SCROLL_HELP_DEBUG
std::cout << "m_y: " << m_y << "  vel: " << m_yVel << "\n";
#endif

  m_y += dt * m_yVel;

  // Don't scroll up past top button 
  if (m_y >= 0)
  {
    m_y = 0;
    m_yVel = 0;
    m_pScrollUpButton->SetEnabled(false);
  }
  else
  {
    m_pScrollUpButton->SetEnabled(true);
  }

  // Don't scroll down past bottom button
  const float BUTTONS_ON_SCREEN = 5.0f * 2.2f;
  float minY = BUTTONS_ON_SCREEN - m_bottomY; // TODO TEMP TEST
  if (minY < 0)
  {
    if (m_y <= minY)
    {
      m_y = minY;
      m_yVel = 0;
      m_pScrollDownButton->SetEnabled(false);
    }
    else
    {
      m_pScrollDownButton->SetEnabled(true);
    }
  }
  else
  {
    m_pScrollDownButton->SetEnabled(false);
  }

  // Decelerate
  if (m_yVel > 0)
  {
    m_yVel -= dt * accel;
    if (m_yVel < 0)
    {
      m_yVel = 0;
    }
  }
  else if (m_yVel < 0)
  {
    m_yVel += dt * accel;
    if (m_yVel > 0)
    {
      m_yVel = 0;
    }
  }
}

void EngineStatePoolOnlineTable::TimerExpired()
{
  RequestRefresh();
  m_time = 0;
}

void EngineStatePoolOnlineTable::MouseButton(bool down, bool ctrl, bool shift)
{
  m_pCancelButton->MouseButton(down, ctrl, shift);
  m_pScrollUpButton->MouseButton(down, false, false);
  m_pScrollDownButton->MouseButton(down, false, false);

  for (int i = 0; i < (int)m_buttons.size(); i++)
  {
    OnlineButton& pb = m_buttons[i];
    pb.m_pButton->MouseButton(down, ctrl, shift);
  }
}
}

#endif

