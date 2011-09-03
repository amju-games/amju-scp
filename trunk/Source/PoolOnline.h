/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: PoolOnline.h,v $
Revision 1.1.2.30  2007/12/13 09:32:21  jay
Add constant for single-player refresh period

Revision 1.1.2.29  2007/08/03 17:51:31  jay
OnlineGame: use state checking functions, not raw values.
DeleteUnusedGame removed, now done in Table state, which remembers the unused
 game ID.

Revision 1.1.2.28  2007/08/02 08:48:47  jay
Replace hard-coded times with PoolOnline::Constants.
These can be updated from the server in future.

Revision 1.1.2.27  2007/08/02 08:16:07  jay
Change to how we get the list of games from server.
Check how long since both players in a game have connected. Discard a
game if the disconnect time is too long.
NB This is not done on the server so queries run faster.

Revision 1.1.2.26  2007/07/21 23:42:41  Administrator
Store Online ID in player names: only request ID from server when uploading
stats for playing an online game for the first time.

Revision 1.1.2.25  2007/07/19 17:28:13  jay
Convenience function to get opponent name

Revision 1.1.2.24  2007/07/12 20:31:41  jay
Return seconds since opponent last connected

Revision 1.1.2.23  2007/07/08 22:09:00  Administrator
Return value from XML parsing functions; log http reply on error

Revision 1.1.2.22  2007/07/05 21:13:13  jay
Use signed rather than unsigned for time since last connect

Revision 1.1.2.21  2007/07/01 19:15:52  jay
Handle duplicated names

Revision 1.1.2.20  2007/06/28 14:36:35  jay
Check if opponent has quit; function to reset disconnect time; Getters for
 server info

Revision 1.1.2.19  2007/06/23 11:28:57  jay
Store online opponent ID, so we can check for the opponent going away.

Revision 1.1.2.18  2007/06/21 18:16:38  jay
Store last frame number in shot data received from client. This is so we can
 check that all frames have been played before ending the shot

Revision 1.1.2.17  2007/06/05 08:16:14  jay
Use new XML parser

Revision 1.1.2.16  2007/05/26 21:04:40  jay
Fixes for Table state after one game, etc.

Revision 1.1.2.15  2007/05/23 18:45:43  jay
Send all object changes in one Http request (POST), not individually.
 + Some tidying up.

Revision 1.1.2.14  2007/05/15 09:32:17  jay
Get current game; send accept/decline messages to server

Revision 1.1.2.13  2007/05/12 14:15:48  jay
Add won/lost/hearts to Online Player struct

Revision 1.1.2.12  2007/04/28 11:41:20  Administrator
Get object states for new shot: this was broken because we were getting
the states before updating the shot ID, and so we were getting the state
changes for the previous shot! Hopefully this is now fixed, but we
must check in before testing, to run on another client

Revision 1.1.2.11  2007/03/16 08:50:41  jay
Online changes: each ball state change/collision is sent to the server, and
used in the replay to keep consistency between clients.

Revision 1.1.2.10  2007/03/12 22:08:18  jay
New columns for game_object table, so we can set an object's state at any
frame during a shot.

Revision 1.1.2.9  2007/03/10 12:58:10  jay
Pool Online - delete game when finished

Revision 1.1.2.8  2007/03/09 21:50:42  jay
Online game - ongoing fixes

Revision 1.1.2.7  2006/08/26 21:28:37  jay
Ongoing changes for Online game; all Online features are off unless
POOL_ONLINE is defined

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

Revision 1.1.2.2  2005/10/29 10:16:10  jay
Added more Online functionality - ongoing, not finished.

Revision 1.1.2.1  2005/10/21 17:45:01  jay
Not finished; wrap communication with server for online games.

*/

#ifndef POOL_ONLINE_H_INCLUDED
#define POOL_ONLINE_H_INCLUDED

// No online code unless POOL_ONLINE is defined
#ifdef POOL_ONLINE

#include <map>
#include <string>
#include <vector>
#include "Singleton.h"
#include "Mutex.h"
#include "States.h"
#include "Vertex.h"
#include "Xml/XmlNodeInterface.h"
#include "HttpClient.h"
#include "OnlineReqManager.h"
#include "GameObject.h"

namespace Amju
{
class Level;

class PoolOnline
{
public:

  // List of different tasks we ask the server to do.
  // 
  // GET: get the info back from the server as response to Http request.
  // SEND: post message to server DB, no response is expected.
  //   This can be in response to a request from another client, 
  //   or in the case of Chat, just sent off to a client.
  enum Task
  {
    GET_PLAYER_LIST,
    SEND_CHAT,
    GET_CHAT,
    GET_GAME_LIST,
    GET_GAME_STATUS,         
    GET_OBJECT_STATUS_LIST, 
    SEND_SHOT_INFO,
    SEND_OBJECT_STATUS,
    SEND_GAME_STATUS,
    REGISTER_PLAYER,
    REQUEST_JOIN_GAME,
    I_AM_ALIVE,
    CREATE_NEW_GAME,
    GET_SHOT_INFO,
    DELETE_GAME
  };

  PoolOnline();
  ~PoolOnline();

  // List of constants: these can be downloaded from server to get 
  // updated values depending on server performance, etc.
  enum Constant
  {
    TABLE_REFRESH_PERIOD,
    NEW_SHOT_POLL_PERIOD,
    ACCEPT_DECLINE_POLL_PERIOD,
    CHECK_DISCONNECT_POLL_PERIOD,
    DISCONNECT_TIME_LIMIT,
    DISCARD_GAME_WAITING_TIME,
    DISCARD_GAME_IN_PROGRESS_TIME,
    SINGLE_PLAYER_REFRESH_PERIOD,

    NUM_CONSTANTS
  };
  enum { MAX_CONSTANTS_PLUS_SPARE = 16 };

  float GetConstant(Constant);

  // Reset the state of any existing game.
  // Player ID may change, game ID must be changed, etc.
  void Reset();

  // Call from Table state once we have a player ID
  void CreateNewGame();

  // Call while waiting for opponent to play to check that she is still 
  // playing.
  bool OpponentHasDisconnected();

  // Returns number of secs since opponent last connected to server.
  int OpponentDisconnectTime();

  bool OpponentHasQuit();

  // If player decides not to quit, call this to reset the disconnect timer.
  // Otherwise we will get a disconnect every frame. 
  void ResetOpponentDisconnectTime();

  void SetLastStatusMsg(const std::string& msg);
  const std::string& GetLastStatusMsg() const;

  void Update();
  int GetPlayerId() const;
 
  // Call when we start playing an online opponent
  void SetOpponentId(int id);
  int GetOpponentId() const;

  // Convenience function, looks up opponent ID in players to get name
  std::string GetOpponentName() const;

  void SetGameId(int gameId);
  int GetGameId() const;

  void SetThisIsPlayer1(bool);
  bool IsThisPlayer1() const;

  bool IsTakingShot() const;

  // Get/set the current frame number
  int GetFrameNumber() const;
  void SetFrameNumber(int f);

  // If playing a shot received from the server, this is the max frame
  // number for which we have object state info.
  int GetFinalFrameNumberSent() const;

  // Get the current shot ID. This is incremented each shot in the current
  // game.
  int GetShotId() const;
  void SetShotId(int shotId); // TODO We should only increment in this class

  void RequestGamesListRefresh();

  void RequestJoinGame(int gameId);
  
  void DeleteGame(int gameId);

  // Send request to check current game. This is used if we are waiting
  // for the opponent to choose a game type.
  void RequestCurrentGameStatus();

  struct OnlineGameObject
  {
    OnlineGameObject(
      int frame,
      int id,
      State state,
      const VertexBase& v,
      const VertexBase& vel) 
      : 
      m_frame(frame), m_id(id), m_state(state), m_vertex(v), m_vel(vel) {}

    std::string ToString() const;

    // Frame number at which the object m_id has this state.
    int m_frame;
    int m_id;
    State m_state;
    VertexBase m_vertex;
    VertexBase m_vel;
  };
  typedef std::vector<OnlineGameObject> OnlineGameObjects;
  
  struct ShotInfo
  {
    ShotInfo(
      float yRot, 
      float verticalVel, 
      float horVel, 
      float accel, 
      float english, 
      float drawRoll,
      float cueX,
      float cueY,
      float cueZ
    ) : 
      m_yRot(yRot), m_verticalVel(verticalVel), m_horVel(horVel),
      m_accel(accel), m_english(english), m_drawRoll(drawRoll),
      m_cueX(cueX), m_cueY(cueY), m_cueZ(cueZ)
    {
    } 
    void SetShotId(int shotId) { m_shotId = shotId; }
    void SetPlayerId(int playerId) { m_playerId = playerId; }

    // Less-than, for sort
    bool operator<(const ShotInfo& s) const { return m_shotId < s.m_shotId; }

    float m_yRot;
    float m_verticalVel;
    float m_horVel; 
    float m_accel; 
    float m_english; 
    float m_drawRoll;
    int m_shotId; // ID for the shot - next in sequence
    int m_playerId; // Server-generated ID of player who took the shot
    float m_cueX; // position of cue ball at start of shot
    float m_cueY;
    float m_cueZ;
  };

  // Send Shot info when player takes a shot.
  void SendShotInfo(const ShotInfo&);

  // Request latest shot info for the current game.
  void RequestShotInfo();

  // Is there a new shot with ID greater than the ID given ?
  // If there is a new shot, increments the shot ID and gets object info 
  //  for the new shot.
  bool IsNewShot(int lastShotId);

  // Get the latest shot taken for the current game.
  // Should only be called after IsNewShot() returns true.
  ShotInfo GetLastShotInfo() const;

  // Set the URL of the server, including directory where server script
  // lives, but not including the script name.
  // Usually expected to be: www.amju.com/cgi-bin
  void SetServerUrl(const std::string& url);

  // Set the name of the local player.
  // This is sent to the server when we poll for other players, so other
  // players get to know about this one.
  // Also set the player ID if known. This is so we don't have to look it up
  //  if we already know the ID.
  // Normally if ID is not known, we want to request an ID from the server.
  // Turn this off by setting requestIdIfNotSet false.
  void SetPlayerName(const std::string& playerName, int playerId, bool requestIdIfNotSet);
  const std::string& GetPlayerName() const;

  struct OnlinePlayer
  {
    // Default ctor so we can have OnlinePlayers in a map
    OnlinePlayer() : m_id(-1), m_lastTime(0) {}

    OnlinePlayer(
      int id, 
      const std::string& name, 
      const std::string ipaddr, 
      int lastTime,
      int won, int lost, int hearts) : 
      m_id(id), m_name(name), m_ipaddr(ipaddr), m_lastTime(lastTime),
      m_won(won), m_lost(lost), m_hearts(hearts), m_nameExtra(0)  {}

    int m_id;
    std::string m_name;
    std::string m_ipaddr;
    // Number of seconds since this player last contacted the server.
    int m_lastTime;
    int m_won;
    int m_lost;
    int m_hearts;
    // Extra number for duplicated names. 
    // If 0, there is no duplicated name.
    // If 1+, show this number in brackets after name.
    int m_nameExtra;
  };
  // Map of ID to player
  typedef std::map<int, OnlinePlayer> Players;
 
  // Get the players currently (i.e. at last poll) online
  const Players& GetPlayersOnline();

  // Get the state of all objects for the current online game
  // I.e. the game this user is playing or watching
  const OnlineGameObjects& GetObjectsOnline();

  // Send state of this object to server
  bool SetOnlineObjectState(GameObject* pObj);

  // Send state of all objects in level to server
  void SetOnlineObjectStates(Level* pLevel);

  // Send request to server to get list of all objects states, and the
  // frame when the state changed.
  // current level.
  void GetOnlineObjectStates();

  // If we are taking a shot, send any updated object states to the server.
  // If we are replaying a shot, set objects with a state change this frame.
  void UpdateOrSendObjectStatesThisFrame();

  // If true, we are waiting for object states from the server.
  // In this case, we should not update any objects, as the update will be
  // overwritten by the server data.
  bool IsWaitingForObjectStates() const;

  // Set the above flag: only reset by the object requesting server data.
  void SetIsWaitingForObjectStates(bool);

  struct OnlineGame
  {
    OnlineGame(
      int id, 
      int player1Id,
      int player2Id, 
      int state,
      int type) : m_id(id), m_state(state), m_type(type)
    {
      m_playerId[0] = player1Id;
      m_playerId[1] = player2Id;
    }
    // If true, the Game has only one player and is 
    // waiting for someone else to join.
    bool IsWaiting() const { return m_state == 0; }

    // If true, game has 2 players but player 1 has not yet
    // accepted or declined player2.
    bool IsWaitingForAccept() const { return m_state == 5; } 

    bool IsDeleted() const { return m_state == 6; }

    bool IsInProgress() const { return m_state == 1; }

    int m_id;
    int m_playerId[2];
    int m_state;
    int m_type;  
  };
  typedef std::vector<OnlineGame> OnlineGames;
  
  // Get list of games currently active
  const OnlineGames& GetGames();

  // Update current game ID on server with chosen level and room
  void SendGameSelected(int levelId, int roomId);

  // Update the server, the current game is finished - won/lost or abandoned.
  void SendGameFinished();

  // Call when player1 accepts or declines a join request
  void SendGameJoinAccepted();
  void SendGameJoinDeclined();

  // Get the game we are currently playing or joining, or 0 if none.
  const OnlineGame* GetCurrentGame() const;
 
//protected:

  bool SetObjectsStateFromXml(PXml);

  bool SetPlayersStateFromXml(PXml);
  
  bool SetGamesStateFromXml(PXml);

  void RequestPlayerId();
  bool SetPlayerIdFromXml(PXml);

  bool SetShotInfoFromXml(PXml);

protected:
  std::string GetBaseUrl(Task task);
  // Allow the other Online class to create URL to register player.
  //  Sigh.. this is because we have broken Online requests into 2 classes
  friend class OnlinePlayerProfile;

  void SendObjectStateChangesThisFrame();

  void UpdateObjectStateChangesThisFrame();

  void UpdateObject(const OnlineGameObject&);

  // Count number of occurrences of name in Player Names
  int CountName(const std::string& name);

  int GetPlayerConnectTime(int playerId) const;

protected:
  std::string m_serverUrl;

  // Name of player. We use this once to register the name with the
  // server, then we use player ID.
  std::string m_playerName;
  // -1 until set by registering the player name with the server.
  // Valid IDs are >= 0
  int m_playerId;

  // List of players online. 
  Players m_players;  
  
  // List of games
  OnlineGames m_games;
 
  // List of object state changes which occur this frame.
  // This is used to adjust objects as we play the frame.
  OnlineGameObjects m_objects;

  // All object state/velocity changes, and their positions when they change.
  // This data is sent to the server when we send the shot info.
  OnlineGameObjects m_objectChangesToSend;

  // List of shots for the current game, in order taken.
  typedef std::vector<ShotInfo> Shots;
  Shots m_shots;

  // ID of game we are playing or watching. -1 means not set yet.
  int m_gameId;

  // Holds on to Http requests until they complete.
  OnlineReqManager m_onlineReqManager;

  bool m_thisClientIsPlayer1;

  // If true, we are expecting to get back Object states from the server.
  bool m_isWaitingForObjectStates;

  // Store the last error or information message, e.g. Can't connect to
  //  server, etc. Show this in the current Engine State.
  std::string m_lastStatusMsg;

  // Current shot number in game
  int m_shotId;

  // Current frame in shot
  int m_frameId;

  // Max frame number of online game object messages received: we need to
  // know this so we don't stop the shot too early.
  int m_finalFrameNumberSent;

  // ID of opponent, or -1 if no opponent has been set (not playing a game)
  int m_opponentId;

  // Constants, set in ctor but can be updated by querying server
  float m_constants[MAX_CONSTANTS_PLUS_SPARE];
};

typedef Singleton<PoolOnline> ThePoolOnlineManager;
}

#endif // POOL_ONLINE

#endif // include guard

