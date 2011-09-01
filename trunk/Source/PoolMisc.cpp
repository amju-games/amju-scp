/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: PoolMisc.cpp,v $
Revision 1.1.2.6  2007/06/10 21:33:22  Administrator
Check for extra states BALL_IN_POCKET and BALL_OUT_OF_BOUNDS

Revision 1.1.2.5  2007/05/23 18:45:43  jay
Send all object changes in one Http request (POST), not individually.
 + Some tidying up.

Revision 1.1.2.4  2007/03/22 19:05:27  jay
Remove assert for no non-head-area

Revision 1.1.2.3  2007/03/16 08:50:41  jay
Online changes: each ball state change/collision is sent to the server, and
used in the replay to keep consistency between clients.

Revision 1.1.2.2  2007/03/11 20:32:07  Administrator
Pool online debug

Revision 1.1.2.1  2006/08/14 17:50:23  jay
Rename "Pool"

Revision 1.1.2.2  2006/08/07 20:42:50  jay
Online Pool - not working yet

Revision 1.1.2.1  2006/07/24 18:20:35  jay
Moved Pool code from Pool/ to Pool/

Revision 1.6.2.1.4.21  2005/11/06 09:40:56  jay
Forward decls in .h file, not includes.

Revision 1.6.2.1.4.20  2005/10/21 17:09:41  jay
Added Online flag functions

Revision 1.6.2.1.4.19  2005/09/29 19:07:55  jay
Added IsGameOver, SetIsGameOver and IsPoolBall funcs.
Now we can check if the game is still in progress; distinguish between
Pool Balls and subclassed objects.

Revision 1.6.2.1.4.18  2005/09/23 19:32:59  jay
Added NagMode functions

Revision 1.6.2.1.4.17  2005/09/14 19:44:03  jay
Added NumHumanPlayers()

Revision 1.6.2.1.4.16  2005/09/05 20:06:56  jay
Turn off debug output

Revision 1.6.2.1.4.15  2005/09/02 20:47:38  Administrator
Turn off reporting all objects if Rules cannot be found

Revision 1.6.2.1.4.14  2005/08/26 21:45:38  jay
No User Levels

Revision 1.6.2.1.4.13  2005/08/06 17:57:06  Administrator
Fix random demo room

Revision 1.6.2.1.4.12  2005/07/30 23:35:42  jay
Added debug output to investigate why levels not loading

Revision 1.6.2.1.4.11  2005/07/30 12:20:35  jay
Added GetBallRadius.

Revision 1.6.2.1.4.10  2005/06/19 21:35:51  jay
Comment change

Revision 1.6.2.1.4.9  2005/06/13 22:15:28  jay
Moved IsUserControlled() to here

Revision 1.6.2.1.4.8  2005/05/24 20:42:16  jay
More parameters for start game, and get ball object by ball number.

Revision 1.6.2.1.4.7  2005/05/15 17:17:48  jay
Player names capitalised - will need to change to allow user names.

Revision 1.6.2.1.4.6  2005/05/08 09:59:00  jay
Bug fix - lowest ball should not include the cue ball

Revision 1.6.2.1.4.5  2005/04/17 22:00:56  jay
Added GetNonHeadArea, for getting non-headstring area in current room

Revision 1.6.2.1.4.4  2005/04/03 16:30:38  jay
Minor tweaks

Revision 1.6.2.1.4.3  2005/04/01 05:55:13  jay
Added Demo mode functions

Revision 1.6.2.1.4.2  2005/03/20 20:13:27  jay
Ongoing work for Pool game

Revision 1.6.2.1.4.1  2005/03/17 20:55:24  jay
Added GetRules() for current room in level, and changed GetBall to return FMGO

Revision 1.6.2.1  2004/10/04 10:23:32  jay
Remove couts in release build

Revision 1.6  2004/09/25 21:12:40  Administrator
Remove duplicate line

Revision 1.5  2004/09/23 11:54:42  Administrator
Random stats at Game Start

Revision 1.4  2004/09/20 13:32:04  Administrator
You can set AI for players, for AI testing

Revision 1.3  2004/09/17 14:01:30  jay
Let virtual function handle ball in hole, so we can override for demo mode.

Revision 1.2  2004/09/15 09:25:01  Administrator
Changes to enable the Player Stats state. "Skill levels" of each player
are shown before each Hole starts.

Revision 1.1  2004/09/08 15:43:06  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <iostream>
#include "PoolMisc.h"
#include "Engine.h"
#include "EngineStateTimerText.h"
#include "EngineStatePoolShotInPlay.h"
#include "EngineStatePoolSetUpShot.h"
#include "GameState.h"
#include "FreeMovingSolidGameObject.h"
//#include "EngineStatePoolBallInHole.h"
#include "Level.h"
#include "LevelServer.h"
#include "PoolTeeBox.h"
#include "PoolHole.h"
#include "PoolCharacter.h"
#include "PoolCourseManager.h"
#include "LevelServer.h"
#include "EngineStateHiScore.h"
#include "EngineStateTitle.h"
#include "HiScoreTable.h"
#include "EngineStatePoolPlayerStats.h"
#include "PoolBall.h"
#include "NonHeadArea.h"
#include "PoolExplBall.h"
#include "PoolPuck.h"
#include "Rules.h"
#include "PoolOnline.h"

namespace Amju
{
bool InPlay(PGameObject p)
{
  return InPlay(p.GetPtr());
}

bool InPlay(GameObject* pGo)
{
  return 
    (pGo->GetState() != OUT_OF_PLAY &&
     pGo->GetState() != BALL_IN_POCKET &&
     pGo->GetState() != BALL_OUT_OF_BOUNDS); 
}

void StartNewHole()
{
  Engine::Instance()->OnNewRoom(); 

  Engine::Instance()->ChangeState(EngineStatePoolSetUpShot::Name, 
    Engine::IMMEDIATE);

//  Engine::Instance()->ChangeState(EngineStatePoolPlayerStats::Name, 
//    Engine::IMMEDIATE);
}

void NextPlayer()
{
  int count = 0;
  bool done = false;
  do
  {
    // Rotate simply moves the current index along, or back to zero.
    Engine::Instance()->GetGameState()->RotateToNextPlayer();
    PoolGameState::PlayerInfo* pPi = Engine::Instance()->GetGameState()->GetCurrentPlayerInfo(); 
    // Test if the player we have rotated to is still in the game.
    if (pPi->m_isPlaying && pPi->m_numBalls > 0) 
    {
#ifdef MISC_DEBUG
      std::cout << "Player up: " << pPi->m_id << " " << pPi->m_name.c_str() << " balls: " << pPi->m_numBalls << "\n";
#endif 
      done = true;
    }
    count++;
    if (count > 4)
    {
      // No players are playing - we should have caught this earlier!
      done = true;
      Assert(0 && "no next player..?");
    }
  } while (!done);
}

void ShowPlayerUp()
{
  Engine::Instance()->ChangeState(EngineStatePoolSetUpShot::Name, Engine::IMMEDIATE);
}

FreeMovingSolidGameObject* GetBall()
{
  // TODO sneaky call to LevelWorkerAddBall

  // Get the ID of the ball.
  int ballId = 10; // TODO TEMP TEST
 
  Assert(Engine::Instance()->GetGameObject(ballId).GetPtr());
 
  FreeMovingSolidGameObject* pBall = (FreeMovingSolidGameObject* )Engine::Instance()->GetGameObject(ballId).GetPtr();
  Assert(dynamic_cast<PoolBall*>(pBall));
  return pBall;
}

PoolTeeBox* GetTeeBox(Level* pLevel)
{
  // Get the game objects which are in the same room as the player.
  int levelId = pLevel->GetId();
  int roomId = pLevel->GetRoomId();
  GameObjectMap& objs = Engine::Instance()->GetGameObjects(levelId, roomId);
  // Iterate through map of Game Objects. 
  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    PGameObject pGo = it->second;

    PoolTeeBox* pTee = dynamic_cast<PoolTeeBox*>(pGo.GetPtr());
    if (pTee)
    {
      return pTee;
    }
  }
  return 0;
}

PoolHole* GetPoolHole(Level* pLevel)
{
  Assert(pLevel);

  // TODO Return the nearest hole to the current player, not just the
  // first one.

  // Get the game objects which are in the same room as the player.
  int levelId = pLevel->GetId();
  int roomId = pLevel->GetRoomId();
  GameObjectMap& objs = Engine::Instance()->GetGameObjects(levelId, roomId);
  // Iterate through map of Game Objects. 
  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    PGameObject pGo = it->second;

    PoolHole* pHole = dynamic_cast<PoolHole*>(pGo.GetPtr());
    if (pHole)
    {
      return pHole;
    }
  }
#ifdef MISC_DEBUG
  std::cout << "No golf hole for Level " << pLevel->GetId() << " room " << pLevel->GetRoomId() << "\n";
#endif

  return 0;
}

/*
void BallInHole()
{
  // Let the current EngineState handle this.
  // This lets us do something different for Demo mode.
  EngineStatePoolShotInPlay* p = dynamic_cast<EngineStatePoolShotInPlay*>(
    Engine::Instance()->GetEngineState().GetPtr());
  Assert(p);
  p->BallInHole();
}
*/

//CharacterGameObject* 
PoolCharacter* GetActivePlayer()
{
  int id = Engine::Instance()->GetGameState()->GetCurrentPlayerInfo()->m_id;
  GameObject* pGo = Engine::Instance()->GetGameObject(id).GetPtr();
  Assert(pGo);
  CharacterGameObject* pChar = dynamic_cast<CharacterGameObject* >(pGo);
  Assert(pChar);
  return dynamic_cast<PoolCharacter*>(pChar);
}

int GetRemainingPlayers()
{
  int numPlayers = Engine::Instance()->GetGameState()->GetNumberOfPlayers();
  int numRemaining = 0;
  for (int i = 0; i < numPlayers; i++)
  {
    PoolGameState::PlayerInfo* pPi = Engine::Instance()->GetGameState()->GetPlayerInfo(i);

    if (pPi->m_isPlaying && pPi->m_numBalls > 0)
    {
      numRemaining++;
    }
  }
  return numRemaining;
}

int GetStrokesLimit()
{
  return (int)Engine::Instance()->GetConfigFloat("golf_max_strokes"); 
  // TODO - per hole ?
}

float MakeRnd(float f)
{
  // Make a random float between 0 and f.
  float r = (float)rand() / (float)RAND_MAX * f;
  Assert(r >= 0);
  Assert(r <= f);
  return r;
}

int NumHumanPlayers()
{
  bool p1isAi = Engine::Instance()->GetGameState()->GetPlayerInfo(0)->m_isAi;
  bool p2isAi = Engine::Instance()->GetGameState()->GetPlayerInfo(1)->m_isAi;
  if (p1isAi && p2isAi)
  {
    return 0;
  }
  if (!p1isAi && !p2isAi)
  {
    return 2;
  }
  return 1;
}

void StartGame(int numPlayers, bool isFriendly, bool p1IsOnline, bool p2IsOnline)
{
  Assert(numPlayers == 0 || numPlayers == 1 || numPlayers == 2);

  // POOL changes: there are always two players, but one or both may be AI.

  // Create player one.
  Engine::Instance()->GetGameState()->ClearPlayers();
  Engine::Instance()->GetGameState()->SetIsFriendly(isFriendly);

  PoolGameState::PlayerInfo p1(1, "Amju", Orientation(), false);
  p1.m_score = 0;
  p1.m_isPlaying = true;
  p1.m_strokes = 0;
  p1.m_reset = true;
  p1.m_isAi = false;
  p1.m_isOnline = p1IsOnline;

  if (Engine::Instance()->GetConfigValue("player_1_is_ai") == "y")
  {
#ifdef MISC_DEBUG
    std::cout << "Setting player 1 to AI\n";
#endif
    p1.m_isAi = true;
  }
  if (numPlayers == 0)
  {
    p1.m_isAi = true;
  }

  p1.m_numBalls = (int)Engine::Instance()->GetConfigFloat("start_num_balls");

  // New for v.2.0 Player stats
  // Start stats are random  TODO weight differently
  // for different players ? Or from config file ?
  p1.m_stats.clear();
  p1.m_stats.push_back(0.2f + MakeRnd(0.5f)); // max shot power
  p1.m_stats.push_back(0.2f + MakeRnd(0.5f)); // drive accuracy
  p1.m_stats.push_back(0.25f + MakeRnd(0.5f)); // drive bias - 0.5 is dead on
  p1.m_stats.push_back(0.2f + MakeRnd(0.5f)); // putt accuracy
  p1.m_stats.push_back(0.2f + MakeRnd(0.5f)); // putt bias

  Engine::Instance()->GetGameState()->AddPlayerInfo(p1);

  // Create the second player.
  PoolGameState::PlayerInfo p2(2, "Marin", Orientation(), false);
  p2.m_score = 0;
  p2.m_isPlaying = true;
  p2.m_strokes = 0;
  p2.m_reset = true;
  p2.m_isAi = false;
  p2.m_isOnline = p1IsOnline;

  if (Engine::Instance()->GetConfigValue("player_2_is_ai") == "y")
  {
#ifdef MISC_DEBUG
    std::cout << "Setting player 2 to AI\n";
#endif
    p2.m_isAi = true;
  }
  // POOL: 2nd player is AI in 1-player game
  if (numPlayers < 2)
  {
    p2.m_isAi = true;
  }

  p2.m_numBalls = (int)Engine::Instance()->GetConfigFloat("start_num_balls");
  p2.m_stats.clear();
  p2.m_stats.push_back(0.2f + MakeRnd(0.5f)); // max shot power
  p2.m_stats.push_back(0.2f + MakeRnd(0.5f)); // drive accuracy
  p2.m_stats.push_back(0.25f + MakeRnd(0.5f)); // drive bias - 0.5 is dead on
  p2.m_stats.push_back(0.2f + MakeRnd(0.5f)); // putt accuracy
  p2.m_stats.push_back(0.2f + MakeRnd(0.5f)); // putt bias
  Engine::Instance()->GetGameState()->AddPlayerInfo(p2);

  // Set the current player OnNewRoom in EngineStatePoolSetUpShot.
  Engine::Instance()->GetGameState()->SetCurrentPlayer(0);

  SetIsGameOver(false);

#ifdef POOL_ONLINE
  // Reset the shot ID
  ThePoolOnlineManager::Instance()->SetShotId(0);
#endif
}


void StartCourse(int courseNum)
{
  SetIsGameOver(false);

  int currentCourseNum = -1;
  if (LevelServer::Instance()->GetCurrentLevel().GetPtr() )
  {
    currentCourseNum = LevelServer::Instance()->GetCurrentLevel()->GetId();
  }

#ifdef MISC_DEBUG
std::cout << "*** In StartCourse: current level: " << currentCourseNum 
  << " New: " << courseNum << "\n";
#endif

  // Only load up Level if necessary
  if (courseNum != currentCourseNum)
  {
    std::string levelFile = PoolCourseManager::Instance()->
      GetCourseFileName(courseNum);

#ifdef MISC_DEBUG
std::cout << "* START COURSE: Loading level " << courseNum 
  << ": " << levelFile.c_str() << "\n";
#endif

    bool isUser = false; //PoolCourseManager::Instance()->IsUser(courseNum);

    if (LevelServer::Instance()->Load(levelFile, isUser, courseNum).Failed())
    {
      // TODO report this error. Mark this file as bad ?
      std::string err = levelFile;
      err += ": Failed to load level data.";
      Engine::Instance()->ReportError(err);
      return;
    }
    LevelServer::Instance()->GetCurrentLevel()->SetId(courseNum);
  }
  LevelServer::Instance()->GetCurrentLevel()->SetRoomId(0);
  Engine::Instance()->StartGame(""); // NB param is unused

/*
  // POOL: Don't change state yet
  StartNewHole();
*/
}

void AddToScore(int s)
{
  Engine::Instance()->GetGameState()->GetCurrentPlayerInfo()->m_score += s;
  EngineStatePoolBase::CreateScore(); 
}

void CheckForHiScores()
{
  // If ANY of the players has got a high score, go to the high score
  // entry state. Else go to title state.

  bool highScoreExists = false;
  int numPlayers = Engine::Instance()->GetGameState()->GetNumberOfPlayers();
  for (int i = 0; i < numPlayers; i++)
  {
    int score = Engine::Instance()->GetGameState()->GetPlayerInfo(i)->m_score;
    if (HiScoreTable::Instance()->IsHighScore(score))
    {
#ifdef MISC_DEBUG
std::cout << "Score " << score << " IS a high score.\n";
#endif
      highScoreExists = true;
      break;
    }
  }

  if (highScoreExists)
  {
    Engine::Instance()->ChangeState(EngineStateHiScore::Name, Engine::IMMEDIATE);
  }
  else
  {
#ifdef MISC_DEBUG
std::cout << "No high scores.\n";
#endif

    Engine::Instance()->ChangeState(EngineStateTitle::Name, Engine::FADE);
  }
}

void AwardBonusBall()
{
  Engine::Instance()->GetGameState()->GetCurrentPlayerInfo()->m_numBalls++;
  EngineStatePoolBase::CreateBallsLeft();
  Engine::Instance()->PlayWav(
    Engine::Instance()->GetConfigValue("golf_wav_8"));
  EngineStatePoolBase::SetBallSize(500.0f); // TODO TEMP TEST
}

void ImproveStats(int player, float improvement)
{
  // improvement of 1.0 is best - higher numbers mean less improvement.

  std::vector<float>& stats = Engine::Instance()->GetGameState()->
    GetPlayerInfo(player)->m_stats;
  // Improve shot distance
  stats[0] += 0.1f; // TODO TEMP TEST
  if (stats[0] > 1.0f)
  {
    stats[0] = 1.0f;
  }

  // Improve accuracy
  stats[1] += 0.1f; // TODO TEMP TEST
  if (stats[1] > 1.0f)
  {
    stats[1] = 1.0f;
  }

  stats[3] += 0.1f; // TODO TEMP TEST
  if (stats[3] > 1.0f)
  {
    stats[3] = 1.0f;
  }

  // Improve bias - 0.5 is the ideal.
  float f = stats[2];
  f -= 0.5f;
  f *= 0.9f;
  f += 0.5f;
  stats[2] = f;

  f = stats[4];
  f -= 0.5f;
  f *= 0.9f;
  f += 0.5f;
  stats[4] = f;
}

SolidGameObject* GetNonHeadArea(Level* pLevel)
{
  Assert(pLevel);

  // Get the game objects which are in the same room as the player.
  int levelId = pLevel->GetId();
  int roomId = pLevel->GetRoomId();
  GameObjectMap& objs = Engine::Instance()->GetGameObjects(levelId, roomId);
  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    PGameObject pGo = it->second;

    NonHeadArea* p = dynamic_cast<NonHeadArea*>(pGo.GetPtr());
    if (p)
    {
      return p;
    }
  }
  //Assert(0 && "No Non-head area for this room/level");
  return 0;
}

Rules* GetRules(Level* pLevel)
{
  Assert(pLevel);

  // Get the game objects which are in the same room as the player.
  int levelId = pLevel->GetId();
  int roomId = pLevel->GetRoomId();
  GameObjectMap& objs = Engine::Instance()->GetGameObjects(levelId, roomId);
  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    PGameObject pGo = it->second;

    Rules* p = dynamic_cast<Rules*>(pGo.GetPtr());
    if (p)
    {
      return p;
    }
  }

#ifdef NO_RULES_DEBUG
std::cout << "GetRules(): No rules found. List of objects in Level: " 
  << levelId << " room: " << roomId << "\n";
  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    GameObjectId gameObjId = it->first;
    PGameObject pGo = it->second;
    std::cout << " Obj: " << gameObjId << " (" << pGo->GetTypeName() << ")\n";
  }
#endif

  Assert(0 && "No Rules for this room/level");
  return 0;
}

PoolBall* GetNumberBall(Level* pLevel, int number)
{
  Assert(pLevel);

  // Get the game objects which are in the same room as the player.
  int levelId = pLevel->GetId();
  int roomId = pLevel->GetRoomId();
  GameObjectMap& objs = Engine::Instance()->GetGameObjects(levelId, roomId);
  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    PGameObject pGo = it->second;
    PoolBall* p = dynamic_cast<PoolBall*>(pGo.GetPtr());
    if (p && p->GetNumber() == number)
    {
      return p;
    }
  }
  return 0;
}

PoolBall* GetLowestBall(Level* pLevel)
{
  int lowest = 9999; // Lowest no found so far
  PoolBall* pResult = 0;

  Assert(pLevel);

  // Get the game objects which are in the same room as the player.
  int levelId = pLevel->GetId();
  int roomId = pLevel->GetRoomId();
  GameObjectMap& objs = Engine::Instance()->GetGameObjects(levelId, roomId);
  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    PGameObject pGo = it->second;
    if (IsPoolBall(pGo.GetPtr()))
    {
      PoolBall* p = dynamic_cast<PoolBall*>(pGo.GetPtr());
      Assert(p);
      if (InPlay(p) && !IsCueBall(p))
      {
        if (p->GetNumber() < lowest)
        {
          lowest = p->GetNumber();
          pResult = p;
        }
      }
    }
  }
  return pResult;
}

bool IsCueBall(GameObject* p)
{
  //return (dynamic_cast<CueBall*>(p) != 0); 
  return (p->GetId() == 10); // TODO total HACK
}

bool IsPoolBall(GameObject* p)
{
  return (dynamic_cast<PoolBall*>(p)  &&  
          !dynamic_cast<PoolExplBall*>(p) &&
          !dynamic_cast<PoolPuck*>(p) ); 
}

void StartDemoMode()
{
  // Pick level 0-7, room 0-3
  int courseNum = rand() % 8;
  int roomId = rand() % 4;
  std::string levelFile = PoolCourseManager::Instance()->GetCourseFileName(courseNum)
;
  bool isUser = false;

#ifdef _DEBUG
std::cout << "Level for demo: " << levelFile.c_str() << "\n";
#endif

  if (LevelServer::Instance()->Load(levelFile, isUser, courseNum).Failed())
  {
#ifdef _DEBUG
std::cout << "Failed to load demo level!\n";
#endif
    Engine::Instance()->ChangeState(EngineStateTitle::Name, Engine::IMMEDIATE);
    return;
  }
  PLevel pLevel = LevelServer::Instance()->GetCurrentLevel();
  pLevel->SetRoomId(roomId);

  // This resets game objects when the player enters a room.
  //EngineRunningBase::SetPlayerRoom(roomId);

  // Create two player characters.
  // This function doesn't really start the game so should be renamed.
  // TODO choose characters at random
  StartGame(0); // no human player
  StartCourse(roomId); // Initialise room
}

bool IsDemoMode()
{
  bool isDemo = Engine::Instance()->GetGameState()->GetPlayerInfo(0)->m_isAi &&
    Engine::Instance()->GetGameState()->GetPlayerInfo(1)->m_isAi;
  return isDemo;
}

static bool gameIsOver = true;
bool IsGameOver()
{
#ifdef _DEBUG
std::cout << "Is Game Over ? " << gameIsOver << "\n";
#endif
  return gameIsOver; // file static
}

void SetIsGameOver(bool over)
{
#ifdef _DEBUG
std::cout << "Setting game is over flag: " << over << "\n";
#endif
  gameIsOver = over;
}

bool IsUserControlled()
{
  // NB Returns false for non-local online player
  return !(Engine::Instance()->GetGameState()->GetCurrentPlayerInfo()->m_isAi ||
           Engine::Instance()->GetGameState()->GetCurrentPlayerInfo()->m_isOnline);
}

float GetPoolBallRadius()
{
  static float r = 0;
  if (r == 0)
  {
    r = GetBall()->GetBoundingSphere()->GetRadius();
  }
  return r;
}

static bool isNagMode = false;
bool IsNagMode()
{
#ifdef NAG_DEBUG
std::cout << "Is Nag Mode: " << isNagMode << "\n";
#endif

  return isNagMode;
}

void SetNagMode(bool nag)
{
  isNagMode = nag;
}

static bool isOnline = false;
void SetIsOnlineGame(bool online)
{
  isOnline = online;
}

bool IsOnlineGame()
{
  return isOnline;
}
}

