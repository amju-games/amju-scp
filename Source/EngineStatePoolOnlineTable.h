/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: EngineStatePoolOnlineTable.h,v $
Revision 1.1.2.16  2007/08/03 17:52:49  jay
DeleteUnusedGame added: store ID of new game, delete it if it ends up
not being used. Saves the server having to send all games involving the
current player.

Revision 1.1.2.15  2007/07/21 23:42:37  Administrator
Store Online ID in player names: only request ID from server when uploading
stats for playing an online game for the first time.

Revision 1.1.2.14  2007/07/18 22:17:43  jay
Improvements to Table/Wait/Accept/Decline states

Revision 1.1.2.13  2007/07/08 22:09:53  Administrator
Flag for when we are waiting for Accept from other player

Revision 1.1.2.12  2007/07/01 19:18:41  jay
Improve status reporting

Revision 1.1.2.11  2007/06/30 14:32:26  jay
Improve presentation

Revision 1.1.2.10  2007/05/26 21:04:39  jay
Fixes for Table state after one game, etc.

Revision 1.1.2.9  2007/05/23 18:45:42  jay
Send all object changes in one Http request (POST), not individually.
 + Some tidying up.

Revision 1.1.2.8  2007/05/12 21:46:36  Administrator
GUI improvements for POOL_ONLINE

Revision 1.1.2.7  2007/05/12 14:16:16  jay
Don't use table, use same code as ChoosePlayer state, so we can show stats

Revision 1.1.2.6  2007/03/09 21:50:41  jay
Online game - ongoing fixes

Revision 1.1.2.5  2006/08/26 21:28:36  jay
Ongoing changes for Online game; all Online features are off unless
POOL_ONLINE is defined

Revision 1.1.2.4  2006/08/14 17:50:14  jay
Rename "Pool"

Revision 1.1.2.3  2006/07/30 21:58:30  jay
Still broken, checking in for backup

Revision 1.1.2.2  2006/07/26 21:21:56  jay
Use new PoolOnline class

Revision 1.1.2.1  2006/07/24 18:20:32  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.2  2005/10/29 10:16:10  jay
Added more Online functionality - ongoing, not finished.

Revision 1.1.2.1  2005/10/21 17:44:03  jay
Not finished, the idea here is to show the "lobby" of games being played
 or waiting for a second player.

*/

#ifndef ENGINE_STATE_POOL_ONLINE_TABLE
#define ENGINE_STATE_POOL_ONLINE_TABLE

#ifdef POOL_ONLINE

#include "EngineStateText.h"
#include "OnlineReqManager.h"
#include "PoolOnline.h"
#include "GuiButton.h"

namespace Amju
{
class EngineStatePoolOnlineTable : public EngineStateText
{
public:
  static const char* Name;

  EngineStatePoolOnlineTable();

  // Set up an online game ready to play
  static void StartOnlineGame(int thisPlayerId, int opponentId, int chooseGame);

  virtual void SetActive(bool);
  virtual bool Load();
  virtual void Update();
  virtual void Draw();
  virtual void DrawOverlays();
  virtual void TimerExpired(); // Refresh table
  virtual void MouseButton(bool, bool, bool);

  // Create a game. We will be player 1; player 2 will be undefined until
  // another player joins the game. Then the state will change from 
  // Waiting to In Progress.
  void CreateGame();

  void RequestJoinGame(int gameId);

  void RequestWatchGame(int gameId);

  // Call to refresh table, as state of games, players etc change over time.
  // This is called by an OnlineReq when the request completes.
  void RefreshTable(
    const PoolOnline::Players& players,
    const PoolOnline::OnlineGames& games);  

  void SetScrollVel(float);

  // Set name of player who is waiting. This is used to set name in
  // Online Manager.
  // Also set ID if known, or -1. This saves looking up the ID if it is already known.
  void SetPlayerName(const std::string& name, int id);

  // Delete the game we created when we entered this state.
  // This is called if we join a different game.
  void DeleteUnusedGame();

protected:
  void RequestRefresh();
  void DrawOverlaysText();

protected:
  // Button to return to main menu
  PGuiElement m_pCancelButton;

  // Up/down buttons to scroll through players
  PGuiElement m_pScrollUpButton;
  PGuiElement m_pScrollDownButton;

  // Text etc for each button
  struct OnlineButton
  {
    SharedPtr<GuiButton> m_pButton;
    std::string m_name;
    std::string m_stats;
  };
  typedef std::vector<OnlineButton> Buttons;
  Buttons m_buttons;

  // Scroll vel for table of player names
  float m_yVel;
  // y-position of scrolling list
  float m_y;
  // y-position of final button: if on screen, we don't scroll down any further.
  float m_bottomY;

  std::string m_playerName;

  // True until we get a list of players from PoolOnline
  bool m_noPlayerInfoYet;

  // Number of games excluding games we don't care about (i.e. aleady
  //  in progress etc)
  int m_numGamesWaiting;
  // Num games in progress
  int m_numGamesInProgress;

  // True when we have clicked another player and are waiting for accept/decline
  bool m_weAreWaitingForAccept;

  // Player ID if known, or -1
  int m_playerId;

  // False if we have not yet created a new game for this player
  bool m_createdNewGame;

  // ID of game which is not used if player joins another game.
  // In this case we delete this game using this ID.
  int m_unusedGameId;
};
}

#endif // POOL_ONLINE

#endif


