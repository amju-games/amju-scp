/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: PoolMisc.h,v $
Revision 1.1.2.3  2007/06/10 21:33:23  Administrator
Check for extra states BALL_IN_POCKET and BALL_OUT_OF_BOUNDS

Revision 1.1.2.2  2007/05/23 18:45:43  jay
Send all object changes in one Http request (POST), not individually.
 + Some tidying up.

Revision 1.1.2.1  2006/08/14 17:50:23  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:35  jay
Moved Pool code from Pool/ to Pool/

Revision 1.3.6.13  2005/11/06 13:33:31  jay
Re-add the includes removed in the last revision, because, DUH, IT BREAKS
ALL THE CLIENT CODE relying on the includes. So not a very good idea.

Revision 1.3.6.12  2005/11/06 09:40:57  jay
Forward decls in .h file, not includes.

Revision 1.3.6.11  2005/10/21 17:09:41  jay
Added Online flag functions

Revision 1.3.6.10  2005/09/29 19:07:55  jay
Added IsGameOver, SetIsGameOver and IsPoolBall funcs.
Now we can check if the game is still in progress; distinguish between
Pool Balls and subclassed objects.

Revision 1.3.6.9  2005/09/23 19:32:59  jay
Added NagMode functions

Revision 1.3.6.8  2005/09/14 19:44:03  jay
Added NumHumanPlayers()

Revision 1.3.6.7  2005/07/30 12:20:35  jay
Added GetBallRadius.

Revision 1.3.6.6  2005/06/13 22:15:28  jay
Moved IsUserControlled() to here

Revision 1.3.6.5  2005/05/24 20:42:16  jay
More parameters for start game, and get ball object by ball number.

Revision 1.3.6.4  2005/04/17 22:00:56  jay
Added GetNonHeadArea, for getting non-headstring area in current room

Revision 1.3.6.3  2005/04/01 05:55:13  jay
Added Demo mode functions

Revision 1.3.6.2  2005/03/20 20:13:27  jay
Ongoing work for Pool game

Revision 1.3.6.1  2005/03/17 20:55:24  jay
Added GetRules() for current room in level, and changed GetBall to return FMGO

Revision 1.3  2004/09/21 15:03:19  Administrator
Minor tidying

Revision 1.2  2004/09/15 09:25:01  Administrator
Changes to enable the Player Stats state. "Skill levels" of each player
are shown before each Hole starts.

Revision 1.1  2004/09/08 15:43:06  jay
Added to repository
  
*/

#if !defined(GOLF_MISC_H_INCLUDED)
#define GOLF_MISC_H_INCLUDED

#include <string>
#include "PoolBall.h"
#include "Rules.h"

namespace Amju
{
class FreeMovingSolidGameObject;
class PoolTeeBox;
class PoolHole;
class Level;
class CharacterGameObject;
class PoolCharacter;
class SolidGameObject;

// Convenience functions to check if a ball is in play, i.e. still on the table.
bool InPlay(PPoolGameObject p);
bool InPlay(PoolGameObject* pGo);

// Call to start playing a hole. I.e. at game start, and
// then whenever the players start a new hole.
void StartNewHole();

void ShowPlayerUp();

void NextPlayer();

void TakeShotNow(float yRot, float verticalVel, float horVel, float accel);

// Get the golf ball. There's only one in play at a time
// (unless we want special crazy courses with more than 1 ball)
FreeMovingSolidGameObject* GetBall();

// Called when currently active player gets the ball in the hole.
//void BallInHole();

// Get the start position for the Hole.
PoolTeeBox* GetTeeBox(Level*);

// Get the hole we are aiming for.
// TODO As we want to allow multiple holes, this function should
// return the hole nearest to the current player.
PoolHole* GetPoolHole(Level*);

// Get the currently active character (may be user- or ai-controlled)
PoolCharacter* GetActivePlayer();

// Get number of players who still have to get the ball in the 
// current hole.
int GetRemainingPlayers();

// Get max no of strokes allowed for current hole.
int GetStrokesLimit();

void StartCourse(int courseNum); // call whenever a course is selected

// call once when game is to start
// numPlayers must be 0 (i.e. Demo) 1 or 2.
// If friendly game, stats are not recorded.
// Either player may be online if 2 players.
void StartGame(int numPlayers, bool isFriendly = true,
  bool p1IsOnline = false, bool p2IsOnline = false); 

// Add points to current player's score
void AddToScore(int);

// Either go to title state or enter hi score state for
// each player with a high score.
void CheckForHiScores(); 

// Player gets a bonus ball
void AwardBonusBall();

void ImproveStats(int player, float improvement);

// Get the Rules for the current room in the given level.
Rules* GetRules(Level*);

// Get the lowest numbered ball in play on the table in the current
// room of the given level.
PoolBall* GetLowestBall(Level*);

// Return true if the given ball is the cue ball.
bool IsCueBall(PoolGameObject*);

// Returns true if given obejct is a PoolBall or CueBall.
// This is useful because special "crazy" objects derive from PoolBall,
// but should not be treated as valid balls.
bool IsPoolBall(PoolGameObject*);

// Set up demo level and start 2 AIs playing.
void StartDemoMode();

// Returns true if both players are AIs.
bool IsDemoMode();

// Returns true if game is over; i.e. we are in title/demo,
// game select, or BallInHole/TrickShotOver states.
// Returns false if a game is in progress, even if paused.
bool IsGameOver();

// This is called by the GameOver state when a game ends.
void SetIsGameOver(bool isOver);

// Returns true if in Nag Mode: we play a demo game but this is to
// nag the player to buy.
bool IsNagMode();
void SetNagMode(bool nag);

// Get the object representing the 'not-behind-the-headstring'
// area of the table.
SolidGameObject* GetNonHeadArea(Level*);

// Return a ball in the current room in the given level, having the
// given number. There may be more than one, in which case this
// function returns the ball with the lowest ID. I.e. only likely to
// be useful if numbers are unique in the room.
// Returns 0 if there is no ball with the given number.
PoolBall* GetNumberBall(Level* pLevel, int number);

// Is current player controlled by user, or AI (or online player) ?
bool IsUserControlled();

// Always use this function to get the radius of a pool ball.
float GetPoolBallRadius();

// Get number of humans playing
int NumHumanPlayers();

void SetIsOnlineGame(bool online);
bool IsOnlineGame();

}

#endif

