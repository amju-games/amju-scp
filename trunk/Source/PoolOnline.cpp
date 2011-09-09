/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: PoolOnline.cpp,v $
Revision 1.1.2.59  2007/12/15 17:46:55  jay
Decrease poll period for single-player game

Revision 1.1.2.58  2007/12/13 09:32:21  jay
Add constant for single-player refresh period

Revision 1.1.2.57  2007/12/12 10:23:52  jay
When we get an updated list of games and players, only update the Lobby
Table if we are in Table EngineState

Revision 1.1.2.56  2007/08/03 17:51:31  jay
OnlineGame: use state checking functions, not raw values.
DeleteUnusedGame removed, now done in Table state, which remembers the unused
 game ID.

Revision 1.1.2.55  2007/08/02 09:15:21  jay
_DEBUG tweaking

Revision 1.1.2.54  2007/08/02 08:54:19  jay
Turn off http error logging for release

Revision 1.1.2.53  2007/08/02 08:48:47  jay
Replace hard-coded times with PoolOnline::Constants.
These can be updated from the server in future.

Revision 1.1.2.52  2007/08/02 08:16:07  jay
Change to how we get the list of games from server.
Check how long since both players in a game have connected. Discard a
game if the disconnect time is too long.
NB This is not done on the server so queries run faster.

Revision 1.1.2.51  2007/07/28 12:51:43  jay
If a game is declined, set its state to 6. Right ?

Revision 1.1.2.50  2007/07/27 08:42:34  jay
Debug-only code

Revision 1.1.2.49  2007/07/25 07:53:12  jay
Fix bad opponent name - sometimes the opponent name is not known, if the
player has gone away.

Revision 1.1.2.48  2007/07/22 12:43:25  jay
Turn off debug output in release mode

Revision 1.1.2.47  2007/07/21 23:42:40  Administrator
Store Online ID in player names: only request ID from server when uploading
stats for playing an online game for the first time.

Revision 1.1.2.46  2007/07/19 17:28:13  jay
Convenience function to get opponent name

Revision 1.1.2.45  2007/07/18 22:14:13  jay
Increase number of concurrent SendGameType reqs, so we can always send a
Quit state

Revision 1.1.2.44  2007/07/12 20:31:41  jay
Return seconds since opponent last connected

Revision 1.1.2.43  2007/07/08 22:08:59  Administrator
Return value from XML parsing functions; log http reply on error

Revision 1.1.2.42  2007/07/07 08:18:05  jay
Add Request type to logging

Revision 1.1.2.41  2007/07/05 22:33:39  Administrator
For now, log Http failures to file even in release mode

Revision 1.1.2.40  2007/07/05 21:12:38  jay
Log http failures to log file

Revision 1.1.2.39  2007/07/01 19:15:52  jay
Handle duplicated names

Revision 1.1.2.38  2007/06/28 16:31:24  Administrator
Fix type

Revision 1.1.2.37  2007/06/28 15:07:51  Administrator
Fix poll time bug

Revision 1.1.2.36  2007/06/28 14:36:35  jay
Check if opponent has quit; function to reset disconnect time; Getters for
 server info

Revision 1.1.2.35  2007/06/23 11:28:57  jay
Store online opponent ID, so we can check for the opponent going away.

Revision 1.1.2.34  2007/06/21 18:16:38  jay
Store last frame number in shot data received from client. This is so we can
 check that all frames have been played before ending the shot

Revision 1.1.2.33  2007/06/19 08:27:01  jay
Send final state message for Rules as well as PoolBalls

Revision 1.1.2.32  2007/06/17 21:13:21  jay
Send/receive Rules states in shot info for consistency; check for bad XML

Revision 1.1.2.31  2007/06/11 09:35:21  jay
Assert is no longer true

Revision 1.1.2.30  2007/06/10 21:33:24  Administrator
Check for extra states BALL_IN_POCKET and BALL_OUT_OF_BOUNDS

Revision 1.1.2.29  2007/06/05 08:16:14  jay
Use new XML parser

Revision 1.1.2.28  2007/05/26 21:04:40  jay
Fixes for Table state after one game, etc.

Revision 1.1.2.27  2007/05/23 18:45:43  jay
Send all object changes in one Http request (POST), not individually.
 + Some tidying up.

Revision 1.1.2.26  2007/05/16 22:02:41  Administrator
Added error strings

Revision 1.1.2.25  2007/05/16 22:02:44  jay
Update game state/type should always specify both values

Revision 1.1.2.24  2007/05/15 12:29:51  jay
Tidy up/add some #ifdef _DEBUGs

Revision 1.1.2.23  2007/05/15 09:32:17  jay
Get current game; send accept/decline messages to server

Revision 1.1.2.22  2007/05/12 21:49:50  Administrator
GUI improvements for POOL_ONLINE

Revision 1.1.2.21  2007/05/12 14:15:48  jay
Add won/lost/hearts to Online Player struct

Revision 1.1.2.20  2007/04/28 20:43:35  Administrator
Previous attempt did not like incremented shot ID

Revision 1.1.2.19  2007/04/28 20:29:05  Administrator
Fix: IsNewShot() is repeatedly called and so we cannot update shot ID there

Revision 1.1.2.18  2007/04/28 11:41:19  Administrator
Get object states for new shot: this was broken because we were getting
the states before updating the shot ID, and so we were getting the state
changes for the previous shot! Hopefully this is now fixed, but we
must check in before testing, to run on another client

Revision 1.1.2.17  2007/03/22 22:47:34  Administrator
Turn off debug

Revision 1.1.2.16  2007/03/16 08:50:41  jay
Online changes: each ball state change/collision is sent to the server, and
used in the replay to keep consistency between clients.

Revision 1.1.2.15  2007/03/12 22:53:11  Administrator
Shot Log

Revision 1.1.2.14  2007/03/12 22:08:18  jay
New columns for game_object table, so we can set an object's state at any
frame during a shot.

Revision 1.1.2.13  2007/03/10 21:50:53  jay
Compare verts; ShotLog

Revision 1.1.2.12  2007/03/10 12:58:09  jay
Pool Online - delete game when finished

Revision 1.1.2.11  2007/03/10 09:56:05  Administrator
Online: consistency check

Revision 1.1.2.10  2007/03/09 21:50:42  jay
Online game - ongoing fixes

Revision 1.1.2.9  2006/08/30 21:12:20  Administrator
Get Pool to build in MSVC

Revision 1.1.2.8  2006/08/26 21:28:37  jay
Ongoing changes for Online game; all Online features are off unless
POOL_ONLINE is defined

Revision 1.1.2.7  2006/08/17 18:15:42  jay
Ongoing work for Online Pool

Revision 1.1.2.6  2006/08/14 17:50:23  jay
Rename "Pool"

Revision 1.1.2.5  2006/08/12 07:43:08  jay
Ongoing fixes for Online Pool: currently trying to get both clients to always
be consistent.

Revision 1.1.2.4  2006/08/07 20:42:51  jay
Online Pool - not working yet

Revision 1.1.2.3  2006/07/30 21:58:30  jay
Still broken, checking in for backup

Revision 1.1.2.2  2006/07/26 21:21:25  jay
Ongoing changes to fix Online stuff

Revision 1.1.2.1  2006/07/24 18:20:38  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.3  2006/03/17 18:24:18  jay
CURL disabled for now

Revision 1.1.2.2  2005/10/29 10:16:10  jay
Added more Online functionality - ongoing, not finished.

Revision 1.1.2.1  2005/10/21 17:45:01  jay
Not finished; wrap communication with server for online games.

*/

#ifdef POOL_ONLINE

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <iostream>
#include "PoolOnline.h"
#include "HttpClient.h"
#include "StringUtils.h"
#include "GameObject.h"
#include "Orientation.h"
#include "EngineStatePoolOnlineTable.h"
#include "Engine.h"
#include "OnlineReqTestable.h"
#include "AmjuTime.h"
#include "Level.h"
#include "PoolBall.h"
#include "ShotLog.h"
#include "PoolMisc.h"
#include "LevelServer.h"
#include "StringUtils.h"
#include "LogFile.h"
#include "TimePeriod.h"
#include "PlayerNames.h"

//#define POOL_ONLINE_DEBUG

// Define this to use test data, not really connect to server
// Not needed any more because we can run the server script locally
// But good to test threading issues!
//#define OFFLINE_TEST

//#define LOG_HTTP_ERRORS


#ifdef LOG_HTTP_ERRORS
#define LOG(str) Log(str)
#else
#define LOG(str) // str
#endif

namespace Amju
{
// TODO increment this if the server must treat two client versions 
// differently.
static const int CLIENT_VERSION = 1;

// NB Each code release should have a corresponding server script version.
// This is to try to avoid breaking backward compatibility and make
// maintenance easier etc.
const std::string SCRIPT = "ascp-online-v.2.1.pl";

// We may send several of this kind of request in quick succession
static const int MAX_SEND_GAME_TYPE_REQS = 4;

void Log(const std::string& str)
{
  LogFile log; 

  std::string filename = "http-err-";
  filename += ThePoolOnlineManager::Instance()->GetPlayerName();
  filename += ".txt";
  log.SetName(filename);
  log.Log(str);
}

bool SafeStrCmp(const char* s1, const char* s2)
{
  if (!s1 || !s2)
  {
    return false;
  }
  return (strcmp(s1, s2) == 0);
}

// Define base class for all requests:
// For offline debugging, use Testable base class.
#ifdef OFFLINE_TEST
class PoolOnlineBase : public OnlineReqTestable
{
public:
  PoolOnlineBase(const std::string& url, const char* debugName) :
    OnlineReqTestable(debugName) /* discard url */
  {
  }
};
#else
class PoolOnlineBase : public OnlineReq
{
public:
  PoolOnlineBase(const std::string& url, const char* debugName) :
    OnlineReq(url, HttpClient::GET, debugName)
  {
  }
};
#endif

// Class for sending Requests to server for Pool game.
// TODO Add a handler member, rather than a function, to decouple game 
// code from this class.
class PoolOnlineReq : public PoolOnlineBase
{
public:
  PoolOnlineReq(
    const std::string& url, const char* debugName) :
    PoolOnlineBase(url, debugName)
  {
  }
    
  virtual void HandleResult()
  {
    if (!GetResult().GetSuccess())
    {
      LOG(GetResult().GetErrorString() + ": PoolOnlineReq");
      return; // TODO Error msg
    }
    // TODO We don't need to parse the result, do we ?
#ifdef _DEBUG
std::cout << "PoolOnlineReq finished: " << GetName() << "\n";
#endif
  }
};

// Class for sending shot info, including state changes when a ball
// changes direction or state. This list of data could overrun the length
// of a URL, so we must use POST - otherwise, we could cimply use 
// PoolOnlineReq.
class PoolSendShotReq : public PoolOnlineReq
{
public:
  PoolSendShotReq(const std::string url) :
    PoolOnlineReq(url, "send-shot-info")
  {
    m_method = HttpClient::POST;
  }
};

// Class for sending Join Game request to server. 
class JoinGameReq : public PoolOnlineBase
{   
public:
  JoinGameReq(
    const std::string& url) :
    PoolOnlineBase(url, "join-game-req")
  {
      ThePoolOnlineManager::Instance()->SetLastStatusMsg("Sending request to join game...");
  }
    
  virtual void HandleResult()
  {
    // If this request completes successfully we still don't know that
    // we joined the game ok, because another player may have beaten us to it.
    if (GetResult().GetSuccess())
    {
      // We don't need to parse the result. We just need to check the list of
      // games again to see if we successfully joined.
      ThePoolOnlineManager::Instance()->RequestGamesListRefresh();
    }
    else
    {
      // TODO Better error msg
      LOG(GetResult().GetErrorString() + ": JoinGameReq");
      ThePoolOnlineManager::Instance()->SetLastStatusMsg("Can't talk to server");
    }
  }
};

// Class for sending request for list of current games.
class ListGamesReq : public PoolOnlineBase
{ 
public:
  ListGamesReq(
    const std::string& url) :
    PoolOnlineBase(url, "list-games-req")
  {
      ThePoolOnlineManager::Instance()->SetLastStatusMsg("Refreshing games list...");
  }

  virtual void HandleResult()
  {
    if (!GetResult().GetSuccess())
    {
      LOG(GetResult().GetErrorString() + ": ListGamesReq");
      ThePoolOnlineManager::Instance()->SetLastStatusMsg("Couldn't get games list from server");
      return; 
    }
#ifdef _DEBUG
std::cout << "LIST GAMES REQ: Returned XML: " << GetResult().GetString().c_str() << "\n\n";
#endif
    PXml xml = ParseXml(GetResult().GetString().c_str());

    // Get the list of players first, then discard games where player time
    // is out of range.
    if (!ThePoolOnlineManager::Instance()->SetPlayersStateFromXml(xml))
    {
      LOG(GetResult().GetString() + ": SetPlayersStateFromXml failed");
      return;
    }
   
    if (!ThePoolOnlineManager::Instance()->SetGamesStateFromXml(xml))
    {
      LOG(GetResult().GetString() + ": SetGamesStateFromXml failed");
      return;
    }

    EngineStatePoolOnlineTable* espolt = dynamic_cast<EngineStatePoolOnlineTable*>(Engine::Instance()->GetEngineState().GetPtr());
    if (espolt)
    {
      // We are in table mode, so update the table
      espolt->RefreshTable(
        ThePoolOnlineManager::Instance()->GetPlayersOnline(),
        ThePoolOnlineManager::Instance()->GetGames() );
    }

    ThePoolOnlineManager::Instance()->SetLastStatusMsg("Got games list...");
  }
};

// Check current game to see if the type has been set by the opponent
class CheckGameStatusReq : public PoolOnlineBase
{
public:
  CheckGameStatusReq(const std::string& url) : PoolOnlineBase(url, "check-status")
  {
    // Message overwrites "waiting for.." message
    //ThePoolOnlineManager::Instance()->SetLastStatusMsg("Checking game...");
  }

  virtual void HandleResult()
  {
    if (!GetResult().GetSuccess())
    {
      LOG(GetResult().GetErrorString() + ": CheckGameStatusReq");
      ThePoolOnlineManager::Instance()->SetLastStatusMsg("Couldn't get games list from server");
      return; 
    }
#ifdef _DEBUG
std::cout << "GAME STATUS REQ: Returned XML: " << GetResult().GetString().c_str() << "\n\n";
#endif
    PXml xml = ParseXml(GetResult().GetString().c_str());
 
    if (!ThePoolOnlineManager::Instance()->SetGamesStateFromXml(xml))
    {
      LOG(GetResult().GetString() + ": SetGamesStateFromXml failed");
    }
  }
};


class GetShotInfoReq : public PoolOnlineBase
{
public:
  GetShotInfoReq(
    const std::string& url) :
    PoolOnlineBase(url, "get-shot-info-req")
  {
  }

  virtual void HandleResult()
  {
    if (!GetResult().GetSuccess())
    {
      LOG(GetResult().GetErrorString() + ": GetShotInfoReq");
      ThePoolOnlineManager::Instance()->SetLastStatusMsg("Can't get shot info from server.");
      return; // TODO Error msg
    }
    PXml xml = ParseXml(GetResult().GetString().c_str());

    if (!ThePoolOnlineManager::Instance()->SetShotInfoFromXml(xml))
    {
      LOG(GetResult().GetString() + ": SetShotInfoFromXml failed");
    }
  }
};

// TODO May not be necessary - requesting games also returns list
//  of players
class ListPlayersReq : public PoolOnlineBase
{
public:
  ListPlayersReq(
    const std::string& url) :
    PoolOnlineBase(url, "list-players-req")
  {
  }

  virtual void HandleResult()
  {
    if (!GetResult().GetSuccess())
    {
      LOG(GetResult().GetErrorString() +": ListPlayersReq");
      return; // TODO Error msg
    }
    PXml xml = ParseXml(GetResult().GetString().c_str());

    if (!ThePoolOnlineManager::Instance()->SetPlayersStateFromXml(xml))
    {
      LOG(GetResult().GetString() +": SetPlayersStateFromXml failed");
    }
  }
};

// Class for registering player ID and getting a unique ID from server
class PlayerIdReq : public PoolOnlineBase
{
public:
  PlayerIdReq(
    const std::string& url) :
    PoolOnlineBase(url, "player-id-req")
  {
      ThePoolOnlineManager::Instance()->SetLastStatusMsg("Getting ID...");
  }

  virtual void HandleResult()
  {
    if (!GetResult().GetSuccess())
    {
      LOG(GetResult().GetErrorString() + ": PlayerIdReq");
      ThePoolOnlineManager::Instance()->SetLastStatusMsg("Can't connect to the Amju Super Cool Pool Server.");
      return; 
    }
    PXml xml = ParseXml(GetResult().GetString().c_str());

    if (!ThePoolOnlineManager::Instance()->SetPlayerIdFromXml(xml))
    {
      LOG(GetResult().GetString() + ": SetPlayerIdFromXml failed");
    }
  }
};
  
class ObjectSetStateReq : public PoolOnlineBase
{
public:
  ObjectSetStateReq(const std::string& url) :
    PoolOnlineBase(url, "object-set-state-req")
  {
  }

  virtual void HandleResult()
  {
    if (!GetResult().GetSuccess())
    {
      LOG(GetResult().GetErrorString() + ": ObjhectSetStateReq");
      ThePoolOnlineManager::Instance()->SetLastStatusMsg("Can't send ball positions to server.");
      return; // TODO Error msg
    }
//std::cout << "OBJECT SET STATE REQ: Got result: " << GetResult().GetString().c_str() << "\n";
  }
};

// Class for requesting state of every object in the current level/room
class ObjectReq : public PoolOnlineBase
{
public:
  ObjectReq(const std::string& url) :
    PoolOnlineBase(url, "object-req")
  {
#ifdef _DEBUG
std::cout << "OBJECT REQ: Url: '" << url << "'\n";
#endif
  }

  virtual void HandleResult()
  {
    if (!GetResult().GetSuccess())
    {
      LOG(GetResult().GetErrorString() + ": ObjectReq");
      return; // TODO Error msg
    }
//std::cout << "OBJECT REQ: Got result: " << GetResult().GetString().c_str() << "\n";

    PXml xml = ParseXml(GetResult().GetString().c_str());

    // Update the state of each object
    if (!ThePoolOnlineManager::Instance()->SetObjectsStateFromXml(xml))
    {
      LOG(GetResult().GetString() + ": SetObjectsStateFromXml failed");
    }

    // It is now safe to update objects again
    ThePoolOnlineManager::Instance()->SetIsWaitingForObjectStates(false);
  }
};

class SendGameTypeReq : public PoolOnlineBase
{
public:
  SendGameTypeReq(const std::string& url) : PoolOnlineBase(url, "send-game-type")
  {
  }

  virtual void HandleResult()
  {
    if (!GetResult().GetSuccess())
    {
      LOG(GetResult().GetErrorString() + ": SendGameTypeReq");
      ThePoolOnlineManager::Instance()->SetLastStatusMsg("Couldn't update game");
      return; 
    }
    // Nothing to do here ?
    //ThePoolOnlineManager::Instance()->SetLastStatusMsg("Updated game OK");
  }
};

class DeleteGameReq : public PoolOnlineBase
{
public:
  DeleteGameReq(const std::string& url) : PoolOnlineBase(url, "delete-game")
  {
  }
  
  virtual void HandleResult()
  {
    if (!GetResult().GetSuccess())
    {
      LOG(GetResult().GetErrorString() + ": DeleteGameReq");
      ThePoolOnlineManager::Instance()->SetLastStatusMsg(
        "Couldn't delete game");
      return; 
    }
  }
};

PoolOnline::PoolOnline()
{
  m_gameId = -1; // i.e. undefined
  m_playerId = -1; // undefined
  m_thisClientIsPlayer1 = false; // MUST be set - see below
  SetIsWaitingForObjectStates(false);

  // Set default values for constants
  m_constants[TABLE_REFRESH_PERIOD] = 5.0f;
  m_constants[NEW_SHOT_POLL_PERIOD] = 7.0f;
  m_constants[ACCEPT_DECLINE_POLL_PERIOD] = 5.0f;
  m_constants[CHECK_DISCONNECT_POLL_PERIOD] = 10.0f;
  m_constants[DISCONNECT_TIME_LIMIT] = 60.0f;
  m_constants[DISCARD_GAME_WAITING_TIME] = 10.0f;
  m_constants[DISCARD_GAME_IN_PROGRESS_TIME] = 120.0f;
  m_constants[SINGLE_PLAYER_REFRESH_PERIOD] = 3.0f;
}

PoolOnline::~PoolOnline()
{
#ifdef _DEBUG
std::cout << "PoolOnline dtor - sending game finished\n";
#endif

  SendGameFinished();
}

float PoolOnline::GetConstant(PoolOnline::Constant c)
{
  Assert(c < NUM_CONSTANTS);
  return m_constants[(int)c];
}

int PoolOnline::GetGameId() const
{
  return m_gameId;
}

int PoolOnline::GetOpponentId() const
{
  return m_opponentId;
}

std::string PoolOnline::GetOpponentName() const
{
  Players::const_iterator it = m_players.find(GetOpponentId());
  if (it == m_players.end())
  {
    return "your opponent"; 
  }

  return it->second.m_name;
}

bool PoolOnline::OpponentHasQuit()
{
  static float pollTimer = 0;
  pollTimer += Engine::Instance()->GetDeltaTime();
  if (pollTimer < GetConstant(CHECK_DISCONNECT_POLL_PERIOD))
  {
    return false;
  }

  pollTimer = 0;

#ifdef _DEBUG
std::cout << "Polling for opponent Quit (not disconnect).....\n";
#endif

  RequestCurrentGameStatus();

  // Check current game state. If set to 6, opponent has quit.
  const OnlineGame* pGame = GetCurrentGame();

  if (!pGame)
  {
#ifdef _DEBUG
std::cout << "Opponent has quit: no current game!\n";
#endif
    return true;
  }
  if (pGame->IsDeleted())
  {
#ifdef _DEBUG
std::cout << "Opponent has quit: current game has been deleted!\n";
#endif
    return true;
  }

#ifdef _DEBUG
std::cout << "Opponent has not quit\n";
#endif

  return false;
}

void PoolOnline::ResetOpponentDisconnectTime()
{
  Players::iterator it = m_players.find(m_opponentId);
  if (it == m_players.end())
  {
#ifdef _DEBUG
    std::cout << "Reset opponent disconnect time: opponent has gone away!\n";
#endif
    return;
  }
  it->second.m_lastTime = 0; 
}

bool PoolOnline::OpponentHasDisconnected()
{
  Assert(m_opponentId != -1);

  static float pollTimer = 0;
  pollTimer += Engine::Instance()->GetDeltaTime();
  if (pollTimer > GetConstant(CHECK_DISCONNECT_POLL_PERIOD))
  {
    pollTimer = 0;

#ifdef DISCONNECT_DEBUG
std::cout << "Polling for opponent disconnect.....\n";
#endif

    std::string url = GetBaseUrl(PoolOnline::GET_PLAYER_LIST);
    // Don't get all players - we only need to get the status of our opponent.
    // Also we update our own time.
    url += "&opponent_id=";
    url += ToString(m_opponentId);

#ifdef DISCONNECT_DEBUG
std::cout << "DISCONNECT URL: " << url.c_str() << "\n";
#endif

    PoolOnlineBase* pReq = new ListPlayersReq(url);
    m_onlineReqManager.AddReq(pReq);
  }

  // Get last time opponent connected: if within some time limit, the
  //  opponent is still connected.
  int disconnectTime = OpponentDisconnectTime();

  if (disconnectTime < GetConstant(DISCONNECT_TIME_LIMIT))
  {
#ifdef DISCONNECT_DEBUG
std::cout << "Poll for disconnect: opponent " << m_opponentId 
  << " refreshed " << disconnectTime 
  << " secs ago, so has not disconnected.\n";
#endif

    return false;
  }

#ifdef DISCONNECT_DEBUG
std::cout << "Poll for disconnect: opponent " << m_opponentId 
  << " has not refreshed for " << disconnectTime 
  << " secs, so HAS DISCONNECTED.\n";
#endif

  return true;
}

int PoolOnline::OpponentDisconnectTime()
{
  Assert(m_opponentId != -1);

  Players::iterator it = m_players.find(m_opponentId);
  if (it == m_players.end())
  {
    // Player no longer in table.. is this likely to happen ??
#ifdef _DEBUG
std::cout << "Poll for disconnect: opponent " << m_opponentId 
  << " not in player list, so HAS DISCONNECTED.\n";
#endif

    return 99999; // TODO TEMP TEST
  }

  const OnlinePlayer& p = it->second; 
  if ((int)(p.m_lastTime) < 0)
  {
    std::cout << "Refresh time is < 0!\n";
  }

  return p.m_lastTime;
}

void PoolOnline::Reset()
{
  SetLastStatusMsg("*Reset Online Manager*");

  m_playerId = -1;
  m_gameId = -1;
  m_players.clear();
  m_games.clear();
  m_objects.clear();
  m_objectChangesToSend.clear();
  m_shots.clear();
  m_isWaitingForObjectStates = false;
  m_shotId = -1;
  m_frameId = -1;
  m_finalFrameNumberSent = 0;
  m_opponentId = -1;
}

int PoolOnline::GetFrameNumber() const
{
  return m_frameId;
}

void PoolOnline::SetFrameNumber(int f)
{
  m_frameId = f;
}
   
int PoolOnline::GetFinalFrameNumberSent() const
{
  return m_finalFrameNumberSent;
}
 
int PoolOnline::GetShotId() const
{
  return m_shotId;
}

void PoolOnline::SetShotId(int shotId)
{
  m_shotId = shotId;
}

void PoolOnline::SetLastStatusMsg(const std::string& msg)
{
  m_lastStatusMsg = msg;
}

const std::string& PoolOnline::GetLastStatusMsg() const
{
  return m_lastStatusMsg;
}

void PoolOnline::SetThisIsPlayer1(bool b)
{
  m_thisClientIsPlayer1 = b;
}

bool PoolOnline::IsThisPlayer1() const
{
   return m_thisClientIsPlayer1;
}

bool PoolOnline::IsTakingShot() const
{
  return IsUserControlled(); 
}

int PoolOnline::GetPlayerId() const
{
  return m_playerId;
}

void PoolOnline::SetOpponentId(int id)
{
  m_opponentId = id;
}

void PoolOnline::SetGameId(int gameId)
{
#ifdef _DEBUG
std::cout << "Setting Game ID: " << gameId << "\n";
#endif

  m_gameId = gameId;
}

void PoolOnline::Update()
{
#ifdef SEND_I_AM_ALIVE
  // Periodically send a message telling the server that we are alive
  if (m_onlineReqManager.CountAllReqs() == 0)
  {
    // No messages queued, so send a message 
    std::string url = ThePoolOnlineManager::Instance()->GetBaseUrl(
      PoolOnline::I_AM_ALIVE);

    PoolOnlineBase* pReq = new PoolOnlineReq(url, "i-am-alive");
    m_onlineReqManager.AddReq(pReq);
  }
#endif

  m_onlineReqManager.Update();
}

void PoolOnline::SendGameFinished()
{
  Assert(m_playerId != -1); // this should have been sorted by now!

  // Set the state of the game to 6.
  // The opponent checks for this value. If found, he should delete the game.

   std::string url = GetBaseUrl(PoolOnline::SEND_GAME_STATUS);
   url += "&game_id=";
   url += ToString(m_gameId);
   url += "&state=6";
   url += "&type=0"; // dummy - server script needs a value for this

#ifdef _DEBUG
std::cout << "SendGameFinished URL: " << url.c_str() << "\n";
#endif
 
   PoolOnlineBase* pReq = new SendGameTypeReq(url);
   m_onlineReqManager.AddReq(pReq, MAX_SEND_GAME_TYPE_REQS);

  // Don't do this, it doesn't give the opponent a chance to detect that
  // the game is over. (Well, he could check that the row has gone...?)
  // DeleteGame(m_gameId);  

  Reset();
}

void PoolOnline::SendGameSelected(int levelId, int roomId)
{
  // Update Game with level and room.
  Assert(m_playerId != -1); // this should have been sorted by now!
  std::string url = GetBaseUrl(PoolOnline::SEND_GAME_STATUS);
  url += "&game_id=";
  url += ToString(m_gameId);
  url += "&type=";
  // Game selection is encoded as 1000 * level + room
  url += ToString(levelId * 1000 + roomId); 
  url += "&state=1"; // server script needs this value

  PoolOnlineBase* pReq = new SendGameTypeReq(url);

  // Allow lots (10) of this kind of request - this is to make sure we
  // don't discard this one. If the previous request reaches the server _after_
  // this one, it should still work - this updates the game_type, while
  // previous requests have changed the state.
  m_onlineReqManager.AddReq(pReq, 10);
}

void PoolOnline::SendGameJoinAccepted()
{
  Assert(m_playerId != -1); // this should have been sorted by now!
  std::string url = GetBaseUrl(PoolOnline::SEND_GAME_STATUS);
  url += "&game_id=";
  url += ToString(m_gameId);
  url += "&state=1";
  url += "&type=-1";

  PoolOnlineBase* pReq = new SendGameTypeReq(url);

  m_onlineReqManager.AddReq(pReq);
}

void PoolOnline::SendGameJoinDeclined()
{
  // TODO Mark declined game ?
  Assert(m_playerId != -1); // this should have been sorted by now!
  std::string url = GetBaseUrl(PoolOnline::SEND_GAME_STATUS);
  url += "&game_id=";
  url += ToString(m_gameId);
  // Mark the game as deleted. Right ?
  url += "&state=6";
  url += "&type=-1";

  PoolOnlineBase* pReq = new SendGameTypeReq(url);

  m_onlineReqManager.AddReq(pReq);
}

void PoolOnline::DeleteGame(int gameId)
{
  // Explicit task to delete row from game table, and cascade delete
  // all other data for the game.

  std::string url = GetBaseUrl(PoolOnline::DELETE_GAME);
  url += "&game_id=";
  url += ToString(gameId);

  PoolOnlineBase* pReq = new DeleteGameReq(url);
  m_onlineReqManager.AddReq(pReq);
}

void PoolOnline::RequestCurrentGameStatus()
{
  // Check there is a current game, i.e. a Join request was successful.

  Assert(m_playerId != -1); // this should have been sorted by now!

  // Just get the game we are interested in
  // TODO Check for game_id in server script
  std::string url = GetBaseUrl(PoolOnline::GET_GAME_LIST);
  url += "&game_id=";
  url += ToString(m_gameId);

  PoolOnlineBase* pReq = new CheckGameStatusReq(url);
  m_onlineReqManager.AddReq(pReq);
}

void PoolOnline::RequestJoinGame(int gameId)
{
  Assert(m_playerId != -1);

  std::string url = GetBaseUrl(PoolOnline::REQUEST_JOIN_GAME);
  url += "&game_id=";
  url += ToString(gameId);

  PoolOnlineBase* pReq = new JoinGameReq(url);

  m_onlineReqManager.AddReq(pReq);
}

void PoolOnline::RequestGamesListRefresh()
{
  Assert(m_playerId != -1);

  std::string url = ThePoolOnlineManager::Instance()->GetBaseUrl(
    PoolOnline::GET_GAME_LIST);

  PoolOnlineBase* pReq = new ListGamesReq(url);

  m_onlineReqManager.AddReq(pReq);
}

void PoolOnline::SendShotInfo(const PoolOnline::ShotInfo& s)
{
  Assert(m_gameId > -1);

  std::string url = ThePoolOnlineManager::Instance()->GetBaseUrl(
    PoolOnline::SEND_SHOT_INFO);
  url += "&gameid=";
  url += ToString(m_gameId);

  // NB Shot ID is not auto-generated on server -- we need to know it
  // so we can send object updates.
  // Update to this: all shot info is send in one request so we could go
  // back to auto-incremented ID..?

  url += "&shot_id=";
  url += ToString(GetShotId());

  url += "&y_rot=";
  url += ToString(s.m_yRot);

  url += "&h_vel=";
  url += ToString(s.m_horVel);

  url += "&v_vel=";
  url += ToString(s.m_verticalVel);

  url += "&english=";
  url += ToString(s.m_english);
  
  url += "&topspin=";
  url += ToString(s.m_drawRoll);

  url += "&cuex=";
  url += ToString(s.m_cueX);
  url += "&cuey=";
  url += ToString(s.m_cueY);
  url += "&cuez=";
  url += ToString(s.m_cueZ);

  // Lots of data, need to send this as POST, not GET

  // Send all state changes: first send the number of changes.
  // Then for each change, send each member.
  // Each param needs a unique ID... alternatively we could make one big
  // param in XML syntax..?
  int numChanges = m_objectChangesToSend.size();
  url += "&numChanges=";
  url += ToString(numChanges);
  for (int i = 0; i < numChanges; i++)
  {
    const OnlineGameObject& ogo = m_objectChangesToSend[i];

    url += "&frame_";
    url += ToString(i);
    url += "=";
    url += ToString(ogo.m_frame); 

    url += "&id_";
    url += ToString(i);
    url += "=";
    url += ToString(ogo.m_id); 
    
    url += "&state_";
    url += ToString(i);
    url += "=";
    url += ToString(ogo.m_state); 
    
    url += "&x_";
    url += ToString(i);
    url += "=";
    url += ToString(ogo.m_vertex.x); 
    
    url += "&y_";
    url += ToString(i);
    url += "=";
    url += ToString(ogo.m_vertex.y); 
    
    url += "&z_";
    url += ToString(i);
    url += "=";
    url += ToString(ogo.m_vertex.z); 
    
    url += "&velx_";
    url += ToString(i);
    url += "=";
    url += ToString(ogo.m_vel.x); 
    
    url += "&vely_";
    url += ToString(i);
    url += "=";
    url += ToString(ogo.m_vel.y); 
    
    url += "&velz_";
    url += ToString(i);
    url += "=";
    url += ToString(ogo.m_vel.z); 
  }
#ifdef _DEBUG
std::cout << "**Send shot info: url: " << url << "\n";
#endif

  PoolOnlineBase* pReq = new PoolSendShotReq(url);

  m_onlineReqManager.AddReq(pReq);

#ifdef _DEBUG
std::cout << "CLEARING m_objectChangesToSend\n";
#endif
  m_objectChangesToSend.clear();
}

void PoolOnline::RequestShotInfo()
{
  Assert(m_gameId > -1);

  std::string url = ThePoolOnlineManager::Instance()->GetBaseUrl(
    PoolOnline::GET_SHOT_INFO);
  url += "&game_id=";
  url += ToString(m_gameId);
  url += "&last_shot_id=";
  url += ToString(m_shotId);

  PoolOnlineBase* pReq = new GetShotInfoReq(url);

  m_onlineReqManager.AddReq(pReq);
}

bool PoolOnline::IsNewShot(int shotId) 
{
  if (m_shots.empty())
  {
    return false;
  }

  if (m_shots.rbegin()->m_shotId <= shotId)
  {
    return false;
  }
  if (m_shots.rbegin()->m_playerId == m_playerId)
  {
    // If latest shot was taken by me, we don't want to know about it.
    return false;
  }
 
  return true;
}

PoolOnline::ShotInfo PoolOnline::GetLastShotInfo() const
{
  // Don't get the very last shot, in case the other client has taken
  // more than one. Get the next shot in sequence.
  // Ok to search as this is only called once, when we are taking the shot.
  //  ...TODO right ???

  Assert(!m_shots.empty());

  // Find the shot with the lowest ID which is > m_shotId,
  // which should be m_shotId + 1
  for (unsigned int i = 0; i < m_shots.size(); i++)
  {
    if (m_shots[i].m_shotId == m_shotId + 1)
    {
      return m_shots[i];
    }
  }
#ifdef _DEBUG
  std::cout << "Expected to find a shot ID 1 greater than m_shotId\n";
#endif
  Assert(0);
  return *(m_shots.rbegin());
}

bool PoolOnline::SetShotInfoFromXml(PXml xml)
{
  PXml p = xml.getChildNode(0).getChildNode(1);

  if (SafeStrCmp(p.getName(), "shots"))
  {
    m_shots.clear();

    // Iterate over children - each child is a player.
    // TODO Check num children
    for (int i = 0; i < p.nChildNode(); i++) 
    {
      PXml pc = p.getChildNode(i);

      Assert(pc.getChildNode(0).getText());
      int shotId = atoi(pc.getChildNode(0).getText());
      int playerId = atoi(pc.getChildNode(1).getText());
      float yRot = atof(pc.getChildNode(2).getText());
      float hVel = atof(pc.getChildNode(3).getText());
      float vVel = atof(pc.getChildNode(4).getText());
      // NB  no acc!
      float eng  = atof(pc.getChildNode(5).getText());
      float draw = atof(pc.getChildNode(6).getText());
      // Cue ball start position
      float cueX = atof(pc.getChildNode(7).getText());
      float cueY = atof(pc.getChildNode(8).getText());
      float cueZ = atof(pc.getChildNode(9).getText());

      // Acceleration is fixed on all clients
      static const float acc = Engine::Instance()->GetConfigFloat(
        "golf_ball_accel");

#ifdef POOL_ONLINE_DEBUG
std::cout << "Found a Shot! cueX: " << cueX
  << " cueY: " << cueY
  << " cueZ: " << cueZ << "\n";
#endif

      ShotInfo s(yRot, vVel, hVel, acc, eng, draw, cueX, cueY, cueZ);
      s.SetShotId(shotId);
      s.SetPlayerId(playerId);
      m_shots.push_back(s); // NB below we order by ID so latest shot is at back
    }
    // Sort the shots so the latest shot is last.
    // (Not guaranteed by DB - we could do it on the server..?)
    std::sort(m_shots.begin(), m_shots.end());
    return true;
  }
  return false;
}

std::string PoolOnline::GetBaseUrl(Task task)
{
  std::string url = m_serverUrl + SCRIPT;
  switch (task)
  {
  case GET_PLAYER_LIST:
    url += "?task=1";
    break;

  case SEND_CHAT:
    url += "?task=5";
    break;

  case GET_CHAT:
    url += "?task=6";
    break;

  case GET_GAME_LIST:
    url += "?task=7";
    break;

  case GET_GAME_STATUS:
    url += "?task=9";
    break;

  case GET_OBJECT_STATUS_LIST:
    url += "?task=10";
    break;

  case SEND_SHOT_INFO:
    url += "?task=11";
    break;

  case SEND_OBJECT_STATUS:
    url += "?task=12";
    break;

  case SEND_GAME_STATUS:
    url += "?task=13";
    break;

  case REGISTER_PLAYER:
    url += "?task=15";
    break;

  case REQUEST_JOIN_GAME:
    url += "?task=3";
    break;

  case I_AM_ALIVE:
    url += "?task=4";
    break;

  case CREATE_NEW_GAME:
    url += "?task=2";
    break;

  case GET_SHOT_INFO:
    url += "?task=16";
    break;

  case DELETE_GAME:
    url += "?task=18";
    break;

  default:
#ifdef _DEBUG
std::cout << "Unexpected task enum\n";
#endif
    Assert(0);
  }  

  // Add client version. This lets the server send back data which the client
  // expects, if the format changes.
  url += "&client_ver=";
  url += ToString(CLIENT_VERSION);

  if (m_playerId > -1) 
  {
    // Player ID has been looked up ok.
    url += "&player=";
    url += ToString(m_playerId);
  }

  return url;
}

void PoolOnline::SetServerUrl(const std::string& url)
{
  m_serverUrl = url;
}

void PoolOnline::SetPlayerName(const std::string& playerName, int playerId, bool requestIdIfNotSet)
{
  Assert(!playerName.empty());

  m_playerName = playerName;
  m_playerId = playerId;

  if (m_playerId == -1 && requestIdIfNotSet)
  {
    // Register this player with the server and get the player ID.
    // Until this request completes we can't send any other requests to the
    // server because they all require a player ID.
    RequestPlayerId();
  }
}

const std::string& PoolOnline::GetPlayerName() const
{
  return m_playerName;
}

void PoolOnline::RequestPlayerId()
{
#ifdef _DEBUG
std::cout << "RequestPlayerId!!!\n";
#endif

  std::string url = GetBaseUrl(REGISTER_PLAYER);
  url += "&playername="; 
  url += m_playerName; // dodgy characters are handled in HttpClient
  url += "&won=";
  url += ToString(0); // TODO
  url += "&lost=";
  url += ToString(0); // TODO
  url += "&hearts=";
  url += ToString(0); // TODO

  PoolOnlineBase* pReq = new PlayerIdReq(url);
  m_onlineReqManager.AddReq(pReq);
}

void PoolOnline::CreateNewGame()
{
  // Create a new empty game, with this player waiting.
  std::string url = GetBaseUrl(CREATE_NEW_GAME);
  PoolOnlineBase* pReq = new PoolOnlineReq(url, "new-game-req");
  m_onlineReqManager.AddReq(pReq);
}

bool PoolOnline::SetPlayerIdFromXml(PXml xml)
{
  PXml p = xml.getChildNode(0).getChildNode(1);

  if (SafeStrCmp(p.getName(), "player_id"))
  {
#ifdef _DEBUG
std::cout << "Found xml node \"player_id\"...\n";
#endif
    Assert(p.getText());
    int id = atoi(p.getText());
#ifdef POOL_ONLINE_DEBUG
std::cout << "Setting player ID: " << id << "\n";
#endif
    m_playerId = id;

    SetLastStatusMsg("Got your ID: " + ToString(id));

    // Set ID in PlayerNames
    ThePlayerNames::Instance()->SetId(m_playerName, m_playerId);

    return true;
  }
  // TODO Error msg
  ThePoolOnlineManager::Instance()->SetLastStatusMsg("Failed to parse ID");
  return false;
}

const PoolOnline::Players& PoolOnline::GetPlayersOnline()
{
  return m_players;
}

const PoolOnline::OnlineGameObjects& PoolOnline::GetObjectsOnline()
{
  return m_objects;
}

const PoolOnline::OnlineGames& PoolOnline::GetGames()
{
  return m_games;
}

int PoolOnline::CountName(const std::string& name)
{
  int count = 0;
  for (Players::iterator it = m_players.begin(); it != m_players.end(); ++it)
  {
    if (it->second.m_name == name)
    {
      ++count;
    }  
  }
  return count;
}

bool PoolOnline::SetPlayersStateFromXml(PXml xml)
{
  // Child(0) is <ascp>, child(0, 1) is <ascp><players>
  // Iterate over children until we find "players" element
  PXml r1 = xml.getChildNode(0);
  // We must search for "players" node because the XML returned depends on the query
  for (int i = 0; i < r1.nChildNode(); i++)
  {
    PXml child = r1.getChildNode(i);
    if (SafeStrCmp(child.getName(), "players"))
    {
      // Get a list of all players online.
      m_players.clear();

      // Iterate over children - each child is a player.
      for (int j = 0; j < child.nChildNode(); j++) 
      {
        PXml pc = child.getChildNode(j);

        int playerId = atoi(pc.getChildNode(0).getText());
        std::string name = pc.getChildNode(1).getText(); 
        std::string ipaddr = pc.getChildNode(2).getText(); 
        int lastTime = atoi(pc.getChildNode(3).getText()); 
        int won = atoi(pc.getChildNode(4).getText()); 
        int lost = atoi(pc.getChildNode(5).getText()); 
        int hearts = atoi(pc.getChildNode(6).getText()); 

        OnlinePlayer player(playerId, name, ipaddr, lastTime, won, lost, hearts);
        int count = CountName(name);
        player.m_nameExtra = count; 

#ifdef _DEBUG
std::cout << "GOT PLAYER: ID: " << playerId 
<< " name: " << name.c_str() 
<< " IP: " << ipaddr.c_str() 
<< " lastTime: " << TimePeriod(lastTime).ToString().c_str() << " secs "
<< "(" << lastTime << ")"
<< "\n";
#endif

        m_players[playerId] = player;
      }
      return true;
    }
  }
  
  return false;
}

void PoolOnline::GetOnlineObjectStates()
{
  Assert(m_gameId > -1);

  // Don't update any object when this flag is set: the updates
  // will be overwritten when this server request completes.
  SetIsWaitingForObjectStates(true);

  std::string url = GetBaseUrl(GET_OBJECT_STATUS_LIST);
  url += "&game_id="; 
  url += ToString(m_gameId);
  url += "&shot_id=";
  url += ToString(GetShotId() + 1); // fix shot ID, it has not been incremented yet
  
  PoolOnlineBase* pReq = new ObjectReq(url);
  m_onlineReqManager.AddReq(pReq);
  // When this request completes, we will update all objects with the
  // data returned from the server, and finally reset the IsWaiting.. flag.
}

bool PoolOnline::IsWaitingForObjectStates() const
{
  return m_isWaitingForObjectStates;
}

void PoolOnline::SetIsWaitingForObjectStates(bool b)
{
  m_isWaitingForObjectStates = b;
}

bool VertexCompare(const Vec3f& v1, const Vec3f& v2)
{
  float dx = fabs(v1.x - v2.x);
  float dy = fabs(v1.y - v2.y);
  float dz = fabs(v1.z - v2.z);

  static const float MAX_DIFF = 0.001f;
  if (dx < MAX_DIFF && dy < MAX_DIFF && dz < MAX_DIFF)
  {
    return true;
  }
  else
  {
    char buf[500];
    sprintf(buf, "v1.x: %f v1.y: %f v1.z: %f   v2.x: %f v2.y: %f v2.z: %f",
      v1.x, v1.y, v1.z, v2.x, v2.y, v2.z);

    std::string s = "Consistency check failed: ";
    s += buf;
    ShotLog(buf);

    return false; 
  }
}

bool PoolOnline::SetObjectsStateFromXml(PXml xml)
{
  // Child (0, 1) is <ascp><objects>
  PXml p = xml.getChildNode(0).getChildNode(1);
  if (SafeStrCmp(p.getName(), "objects"))
  {
    m_objects.clear();
    m_finalFrameNumberSent = 0;

    int i; // msvc fix
    // Iterate over children - each child is an object.
    for (i = 0; i < p.nChildNode(); i++) 
    {
      PXml pc = p.getChildNode(i);

      int frame = atoi(pc.getChildNode(0).getText());
      int objId = atoi(pc.getChildNode(1).getText());
      int state = atoi(pc.getChildNode(2).getText());
      float x = atof(pc.getChildNode(3).getText());
      float y = atof(pc.getChildNode(4).getText());
      float z = atof(pc.getChildNode(5).getText());
      float vel_x = atof(pc.getChildNode(6).getText());
      float vel_y = atof(pc.getChildNode(7).getText());
      float vel_z = atof(pc.getChildNode(8).getText());
      
      OnlineGameObject ogo(
        frame, objId, State(state), 
        Vec3f(x, y, z), 
        Vec3f(vel_x, vel_y, vel_z));

      m_objects.push_back(ogo);

      // Store max frame number
      if (frame > m_finalFrameNumberSent)
      {
        m_finalFrameNumberSent = frame;
      }
    }

#ifdef _DEBUG
std::cout << "....there are " << m_objects.size() << " objects state changes for this shot. Final frame: " << m_finalFrameNumberSent << "\n";

    ShotLog("Got list of object state changes.");

    for (unsigned i = 0; i < m_objects.size(); i++)
    {
      const OnlineGameObject& obj = m_objects[i];

      // Show the list in ShotLog
      ShotLog(obj.ToString());
    }
#endif

    // Reset flag so we can now play the shot.
    SetIsWaitingForObjectStates(false);
    return true;
  }

#ifdef _DEBUG
std::cout << "XML PARSE Error! Expected <ascp><objects>..???\n";
#endif

  return false;
}

std::string PoolOnline::OnlineGameObject::ToString() const
{
  std::string s;
  s += "Frame: ";
  s += Amju::ToString(m_frame);
  s += " Object ID: ";
  s += Amju::ToString(m_id);

  // TODO

  return s;
}


void PoolOnline::SetOnlineObjectStates(Level* pLevel)
{
#ifdef _DEBUG
std::cout << "SetOnlineObjectStates: pLevel: " << pLevel << "\n";
#endif

  int levelId = pLevel->GetId();
  int roomId = pLevel->GetRoomId();
  GameObjectMap& objs = Engine::Instance()->GetGameObjects(levelId, roomId);
  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    GameObject* pGo = it->second.GetPtr();
    Assert(pGo);
    // Send states for Pool Balls, Rules 
    // TODO Other types also need to be sent ??
    if (dynamic_cast<PoolBall*>(pGo)
        // TODO We don't need to send final Rules state: if a foul has not
        //  been triggered, there is no point. ?????
        // But if foul is only detected when balls have stopped, we DO need
        //  this!
         || dynamic_cast<Rules*>(pGo))
    {
#ifdef _DEBUG
std::cout << "  Sending state for object (1): " << pGo->GetId() << "\n";
#endif

      SetOnlineObjectState(pGo);
    }
  }
}

bool PoolOnline::SetOnlineObjectState(GameObject* pObj)
{
  Assert(m_gameId > -1);
      
  PoolBall* pBall = dynamic_cast<PoolBall*>(pObj);
  if (pBall)
  {
    // Don't send changes immediately - store them until the shot is
    // over. This is to reduce socket connections/threads/DB connections.
    Vec3f vel; 
    pBall->GetForwardVector(&vel.x, &vel.z);
    vel *= pBall->GetForwardVel();
      
    OnlineGameObject ogo(
      m_frameId, 
      pBall->GetId(), 
      pBall->GetState(), 
      pBall->GetPosition(),
      vel);

    m_objectChangesToSend.push_back(ogo);

#ifdef _DEBUG
std::cout << "Added a change... Size of m_objectChangesToSend is now: "
  << m_objectChangesToSend.size() << "\n";
#endif

    return true;
  }
  else
  {
    // Not a ball - just send the State
    OnlineGameObject ogo(
      m_frameId, 
      pObj->GetId(), 
      pObj->GetState(), 
      Vec3f(),
      Vec3f() );

    m_objectChangesToSend.push_back(ogo);
    
#ifdef _DEBUG
std::cout << "Added a change for Non-ball object " << pObj->GetId()
  << " Size of m_objectChangesToSend is now: "
  << m_objectChangesToSend.size() << "\n";
#endif

    return true;
  }

  return false;
}

void PoolOnline::UpdateOrSendObjectStatesThisFrame()
{
  if (IsTakingShot())
  {
    // Send state changes for objects which have been flagged
    SendObjectStateChangesThisFrame();
  }
  else
  {
    // Update objects which have a state change this frame
    UpdateObjectStateChangesThisFrame();
  }
}

void PoolOnline::SendObjectStateChangesThisFrame()
{
  // Iterate over objects. Send state for those objects which have been
  // flagged.
#ifdef _DEBUG
  std::vector<int> ids; // TEST
#endif

  Level* pLevel = LevelServer::Instance()->GetCurrentLevel().GetPtr();
  int levelId = pLevel->GetId();
  int roomId = pLevel->GetRoomId();
  GameObjectMap& objs = Engine::Instance()->GetGameObjects(levelId, roomId);
  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    PPoolGameObject pGo = it->second;
    Assert(pGo.GetPtr());
    if (pGo->SendStateThisFrame())
    {
#ifdef _DEBUG
std::cout << "  Sending state for object (2): " << pGo->GetId() << "\n";
      ids.push_back(pGo->GetId());
#endif
      SetOnlineObjectState(pGo.GetPtr());
    }
  }

#ifdef _DEBUG
  if (ids.size() > 0)
  {
    std::string s = "Sending object states: Num: ";
    s += ToString((int)ids.size());
    for (unsigned int i = 0; i < ids.size(); i++)
    {
      s += ": ";
      s += ToString(ids[i]);
    }
    std::cout << s.c_str() << "\n";
    ShotLog(s);
  }
#endif
}

void PoolOnline::UpdateObject(const PoolOnline::OnlineGameObject& obj)
{
#ifdef _DEBUG
std::cout << "Setting game object with state from server. Frame: "
  << obj.m_frame
  << ":\n";
#endif

      PPoolGameObject pGo = Engine::Instance()->GetGameObject(obj.m_id);
      if (!pGo.GetPtr())
      {
#ifdef _DEBUG
std::cout << "Error! Object ID from server: " << obj.m_id << " does not exist\n";
#endif
        Assert(0);
        // TODO Error message
        return; 
      }

#ifdef _DEBUG
      // Compare local state and state from server
      // This may not be true, as ball in pocket/off table are states
      //Assert(pGo->GetState() == obj.m_state);
      if (pGo->GetOrientation() &&
          !VertexCompare(pGo->GetPosition(), obj.m_vertex))
      {
        std::string s = "..object ID " + ToString(pGo->GetId()) + ": " + pGo->GetTypeName();
        PoolBall* pBall = dynamic_cast<PoolBall*>(pGo.GetPtr());
        if (pBall)
        {
          s += " ball num: " + ToString(pBall->GetNumber());
        }
        ShotLog(s);
      }
#endif

      pGo->SetState(obj.m_state);
#ifdef _DEBUG
std::cout << "  OBJECT: " << obj.m_id << ": Set state: " << obj.m_state 
  << "  : Set vertex: "
  << ToString(obj.m_vertex).c_str() << "\n";
#endif

      if (pGo->GetOrientation())
      {
        Orientation o = *(pGo->GetOrientation());
        o.SetVertex(obj.m_vertex);
        pGo->SetOrientation(o);

        ShotLog("place cue ball 30");
      }
}

void PoolOnline::UpdateObjectStateChangesThisFrame()
{
  // Iterate over state change list. If the frame number for an element
  // matches the current frame, update the object.
#ifdef UPDATE_DEBUG
std::cout << "Update any objects with downloaded states this frame ?\n"
  "This is frame: " << GetFrameNumber() 
  << "\n";
#endif

  // NB Iterate over all object state changes - so no need for them to be
  //  ordered.
  for (unsigned int i = 0; i < m_objects.size(); i++)
  {
    OnlineGameObject& og = m_objects[i];
    if (og.m_frame == GetFrameNumber())
    {
#ifdef _DEBUG
      std::cout << "Update object: " << og.ToString().c_str() << "\n";
      ShotLog(og.ToString());
#endif

      UpdateObject(og);
    }
  }
}

int PoolOnline::GetPlayerConnectTime(int playerId) const
{
  Players::const_iterator it = m_players.find(playerId);
  if (it == m_players.end())
  {
    // Player not found!
    return 999999; // so game will be discarded
  }
  int t = it->second.m_lastTime;
  return t;
}

#define MIN(a, b) (a < b) ? a : b

bool PoolOnline::SetGamesStateFromXml(PXml xml)
{
  m_games.clear();

  // Child (0, 1) is <ascp><games>
  PXml p = xml.getChildNode(0).getChildNode(1);

  if (SafeStrCmp(p.getName(), "games"))
  {
    // Iterate over children - each child is a game.
    for (int i = 0; i < p.nChildNode(); i++)
    {
      PXml pc = p.getChildNode(i);

      int gameId = atoi(pc.getChildNode(0).getText());
      int player1Id = atoi(pc.getChildNode(1).getText());
      int player2Id = atoi(pc.getChildNode(2).getText());
      int state = atoi(pc.getChildNode(3).getText());
      int type = atoi(pc.getChildNode(4).getText());

      // Check how long since each player connected.
      int player1secs = GetPlayerConnectTime(player1Id);
      int player2secs = GetPlayerConnectTime(player2Id);
      // Get the player who has been disconnected for least time.
      int disconnectTime = MIN(player1secs, player2secs);
      // Depending on game type and diconnect time, we may discard the game.
      if (disconnectTime > GetConstant(DISCARD_GAME_WAITING_TIME) && state == 0)
      {
#ifdef _DEBUG
std::cout << "Game ID: " << gameId << " state 10, min player disconnect time: "
  << disconnectTime
  << " so discarding this game\n";
#endif

        // Discard this game
        continue;
      }
      else if (disconnectTime > GetConstant(DISCARD_GAME_IN_PROGRESS_TIME))
      {
#ifdef _DEBUG
std::cout << "Game ID: " << gameId << " state: " << state
  << ", min player disconnect time: "
  << disconnectTime
  << " so discarding this game\n";
#endif

        continue;
      }

      OnlineGame og(gameId, player1Id, player2Id, state, type);

#ifdef _DEBUG
std::cout << "Found a game! ID: " << gameId << "\n";
#endif

      m_games.push_back(og);
    }
#ifdef _DEBUG
{
  std::cout << " ...found " << m_games.size() << " games: ";
  for (unsigned int i = 0; i < m_games.size(); i++)
  {
    std::cout << " ID:" << m_games[i].m_id << " ";
  }
  std::cout << "\n";
}
#endif

    return true;
  }

#ifdef _DEBUG
std::cout << "FOUND NO GAMES :-(\n";
#endif

  return false;
}

const PoolOnline::OnlineGame* PoolOnline::GetCurrentGame() const
{
#ifdef _DEBUG
std::cout << "GetCurrentGame: m_gameId: " << m_gameId << "... ";
#endif

  for (unsigned int i = 0; i < m_games.size(); i++)
  {
#ifdef _DEBUG
std::cout << " " << m_games[i].m_id;
#endif

    if (m_games[i].m_id == m_gameId)
    {
#ifdef _DEBUG
std::cout << "\n";
#endif

      return &m_games[i];
    }
  }

#ifdef _DEBUG
std::cout << " --> NOT FOUND!\n";
#endif
  return 0;
}
}

#endif // POOL_ONLINE
