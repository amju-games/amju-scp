/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: Rules.h,v $
Revision 1.1.2.6  2007/06/21 18:15:24  jay
Move check for not hitting anything from BallsHaveStopped out to its own
 function. This is so we can check for this condition without moving the
 state on to the next player.

Revision 1.1.2.5  2007/06/17 20:38:40  jay
Route all calls to ThisPlayerFouls via a SetState message, so we can send
this state online

Revision 1.1.2.4  2007/06/10 21:33:26  Administrator
Check for extra states BALL_IN_POCKET and BALL_OUT_OF_BOUNDS

Revision 1.1.2.3  2007/06/10 08:58:03  jay
BallInPocket() is not told which pocket the ball hit. This info is lost
when a State change is used to flag ball/pocket collisions.

Revision 1.1.2.2  2006/08/14 17:50:25  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:40  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.24  2005/09/29 19:12:25  jay
Allow external events to end the game.

Revision 1.1.2.23  2005/09/23 19:35:22  jay
Add BallHitsObject func

Revision 1.1.2.22  2005/09/16 20:05:47  jay
Changes so derived Trick Shot rules can work.

Revision 1.1.2.21  2005/09/13 23:09:27  jay
Added COMBO counters so we can reward multiple pots etc.

Revision 1.1.2.20  2005/09/06 20:02:09  jay
Add virtual DecShotCount() and override for UK rules, so we never get
two shots on the black.

Revision 1.1.2.19  2005/08/04 19:39:55  jay
Fix Rules bugs for v.1.1

Revision 1.1.2.18  2005/07/30 12:27:25  jay
Added virtual Clone() so undo stack can copy the current Rules object.

Revision 1.1.2.17  2005/07/05 09:39:50  jay
Added help text

Revision 1.1.2.16  2005/06/29 20:27:17  jay
Ongoing fixes to Rules.

Revision 1.1.2.15  2005/06/24 05:38:28  Administrator
Fixes to get Scene Editor to compile

Revision 1.1.2.14  2005/06/22 21:44:14  jay
Added overrideable function to award player when opponent fouls.

Revision 1.1.2.13  2005/06/19 21:36:17  jay
Move subclasses to separate files

Revision 1.1.2.12  2005/06/13 22:13:05  jay
Draw() draws AI lines

Revision 1.1.2.11  2005/05/24 20:45:26  jay
Additions for UK Pool

Revision 1.1.2.10  2005/05/08 09:57:55  jay
Ongoing improvements - check for first ball hit by cue ball

Revision 1.1.2.9  2005/04/21 17:33:51  jay
Add small, explanatory text and big text

Revision 1.1.2.8  2005/04/17 17:25:39  Administrator
Changes for Scene Editor

Revision 1.1.2.7  2005/04/11 21:24:35  jay
Distinguish between letting the user place the ball behind the headstring
and letting the user place the ball anywhere.

Revision 1.1.2.6  2005/04/03 16:31:25  jay
Added common function for setting balls out of play.

Revision 1.1.2.5  2005/04/01 06:03:14  jay
Added 'shot is break' flag
Added Behaviours to Rules: The Rules object creates the AI behaviour
corresponding to the Rules.

Revision 1.1.2.4  2005/03/20 20:13:28  jay
Ongoing work for Pool game

Revision 1.1.2.3  2005/03/19 17:22:58  jay
Add Scene Ed code - saving, properties.

Revision 1.1.2.2  2005/03/17 23:11:25  Administrator
Get Scene Ed to build

Revision 1.1.2.1  2005/03/17 20:53:40  jay
New classes for Pool game

*/

#ifndef AMJU_RULES_H_INCLUDED
#define AMJU_RULES_H_INCLUDED

#include "GameObject.h"
#include "PoolBehaviour.h"
#include "HelpText.h"

namespace Amju
{
class PoolBall;
class PoolHole;

// Base class for Rules for a poll game.
// There should be one Rules object per Room, where each room has a different
// style of pool being played in it. 
// This base class implements a sensible set of rules, which can be 
// overridden in subclasses to get more specific rules.
class Rules : public PoolGameObject
{
public:
  // Useful function for setting a pool ball to stop moving.
  // We DON'T set the state to OUT_OF_PLAY -- if this function is called
  //  the ball must be in either BALL_IN_POCKET or BALL_OUT_OF_BOUNDS.
  // We keep this state so we can send it to server for POOL_ONLINE
  static void SetBallOutOfPlay(PoolBall*);
  // Get current player name
  static std::string GetName();

public:
  Rules();

  // Create a copy of this object on the heap.
  // This is used to save the current rules state.
  // We must copy onto the heap, as we don;t know how big a subclassed
  // object will be.
  virtual Rules* Clone() = 0;

  // GameObject overrides
  virtual void SetState(State newState);
  virtual void Draw();
  virtual const BoundingSphere* GetBoundingSphere() const;
  virtual void CreateBoundingSphere(const Matrix& cm);
  virtual const char* GetTypeName() const;
  virtual void Update();

  // Default impl loads help text
  virtual bool Load(File*); 

  // New game - this tells us to reset everything.
  virtual void OnRoomEntry();

  // These next virtual functions are for subclasses to override, to
  // produce a particular set of rules. 

  // Call when a shot begins. This can reset any flags, counters
  // etc that we need to determine if a shot is a foul or not.
  virtual void Reset();

  // Call when a ball goes in a pocket.
  // Basic implementation: check if the ball is the cue ball - if 
  // so it's a foul. Subclasses should check if the current player
  // is allowed to pot this ball, is the pocket allowed, etc.
  // Also, pocketing the 8 ball may lose the game; pocketing the 
  // highest-numbered ball may win the game, etc.
  virtual void BallInPocket(PoolBall* pBall);

  // Call when cue ball hits another ball
  virtual void CueBallHitsBall(PoolBall* pBall);

  // Call when a ball hits a rail
  virtual void BallHitsRail(PoolBall* pBall); 

  // Override this in subclasses for special objects - this is intended
  // for "crazy" pool which has other objects, not just balls.
  virtual void BallHitsObject(PoolBall* pBall, PoolGameObject* pObj) {}

  // Call when all action on the table has stopped.
  virtual void BallsHaveStopped();

  // Get no of balls left which the current player must pot.
  // Defaults to counting all balls still in play (does NOT count the cue ball).
  // For UK pool, we will only count the balls of the colour for this player.
  virtual int GetNumBallsRemainingForPlayer();

  // This function can be called if an external event forces a player
  // to win/lose. E.g. a player sets off a bomb - when it explodes, the
  // player who set it off loses.
  // Give the ID of the winner. When the current shot is over, the game 
  // will be over, according to this ID. I.e. it has precedence over 
  // a winning/losing shot.
  void SetGameOverForExtReason(int winnerId, const std::string& text);

  // Set Foul flag if no ball has been hit.
  // TODO Make virtual if we come across a game type where this is legal.
  void CheckForNoBallHit();

protected:
  // Override this to test potted balls in case the game has just
  // been won or lost.
  // E.g. UK Pool: potted black before all other colours->lose game
  // N-Ball: potting highest ball without fouling->win game
  // Only called after at least one ball has been potted; return true
  // if game has now ended. Set the winner inside the subclassed function.
  // NB Now may be called when shot is foul; and possibly no ball potted ?
  virtual bool PotEndsGame() { return false; }

  // Called when previous player fouls, gives current player extra
  // shot, can place ball, etc.
  virtual void GivePlayerFoulAward() = 0;

  // Decrement no of shots left for player.
  // Overrride in UK pool so you only get one shot on the black.
  virtual void DecShotCounter();

public:
  // We may be able to tell that a shot is a foul, before all the balls
  // have stopped moving. In this case the 'foul flag' will be set, and
  // can be checked here.
  bool IsShotFoul() const;

  // Get text explaining why the shot was e.g. a foul.
  std::string GetExplanation() const;

  // Called when a ball (not out of play) falls off the table.
  void BallOutOfBounds(PoolBall*);

  // Returns true if the cue ball should be reset to the "Tee" position.
  bool ResetCueBall() const;

  // Get the AI behaviour for these rules.
  PoolBehaviour* GetBehaviour();

  bool ShotIsBreak() const;

  bool PlayerMayPlaceCueBall() const;
  bool PlayerMayPlaceCueBallBehindHeadstring() const;

  // Get big text, e.g. "foul"
  std::string GetBigText() const;
  // Get explanatory text, e.g. if a foul this is to explain why it's a foul.
  std::string GetExplText() const; 

  // This is a full page of text explaining rules.
  HelpText* GetHelpText();

protected:
  // Subclasses call these functions when they decide that a foul
  // has occurred, etc.
  void GoToNextPlayer();

  // Override for trick shot Rules, where there isn't a Winner/Loser
  virtual void ThisPlayerWins();
  virtual void ThisPlayerLoses();

  // Show text to player explaining why they have lost, fouled etc.
  void ShowText(const std::string&); 
  // Call to give a player points, if applicable
  void ThisPlayerGetsPoints(int points);
  // Call to give the ball to the current player; i.e., they potted it
  // legally.
  void ThisPlayerGetsBall(PoolBall* pBall);

  // Called when player fouls: set state or wait for state message
  void ThisPlayerFouls();

  // Called when FOUL message received
  void ThisPlayerFouls2();

  void NextPlayerGetsExtraShot();
  bool GotValidPot();
  bool FoulLosesGame();

  // Called by all subclasses' Load(File*) - how can we enforce this ? 
  bool LoadHelpText();

  void UpdateStats();
  void UpdateStats(int id, bool win, const std::string& opponentName);

protected:
  // Just to keep things simple, pretend Rules live in a sphere.
  BoundingSphere m_bsphere;

  // Set once we know the current shot is a foul; reset in Reset().
  bool m_isFoul;
 
  // Set if player got a pot. This will usually mean this player player
  // gets another go, unless they also fouled.
  bool m_gotValidPot;

  // Big text, e.g. "Foul"
  std::string m_bigText;
  // Explanation text, to tell player why shot was a foul etc.
  std::string m_text;

  // Set this flag if fouling this shot loses the game.
  bool m_foulLosesGame;

  // Game Object ID of pocket which nominated ball must go in.
  int m_nominatedPocketId;

  // Game Object ID of nominated ball - cue ball must hit this ball first
  // if nominated ? Or, it must go in (nominated) pocket ?
  int m_nominatedBallId;

  // True if the cue ball should be reset - i.e. it has been potted or 
  // knocked off the table.
  bool m_resetCueBall;

  RCPtr<PoolBehaviour> m_pBehaviour;

  // True if this shot is a break (i.e. first shot on entering room,
  // or rules say that balls have been re-racked).
  bool m_shotIsBreak;

  // Place ball flags - NB these are not reset in Reset, but are reset
  // when the next shot is taken.
  bool m_placeBall; // next player can place ball anywhere
  bool m_placeBallHeadstring; // next player can place ball anywhere behind headstring

  // Counter of shots left for this player. Normally set to 1 when player
  // changes, or 2 if previous player fouled. Decremented each shot,
  // incremented on valid pot. When it hits zero it's the next player's turn.
  int m_shotsRemaining;

  // ID of first object ball to be hit by cue ball.
  // Starts off at -1, which is an invalid ID. If no object ball is hit, 
  // this variable will still be set to -1 when balls stop.
  // Otherwise the ID is of the first ball the cue ball hit.
  int m_firstBallId;

  // Balls potted this shot
  std::vector<PoolBall*> m_ballsPottedThisShot;

  HelpText m_helpText;
  // File name for loading help text - set in ctor of derived classes.
  std::string m_helpfilename;

  // COMBOs
  // Record number of balls potted this visit. 
  // This is so we can reward multiple pots in the same visit.
  int m_numBallsPottedThisVisit;

  // Remember the Number of the last ball potted - this is also for COMBOs.
  // We can reward the player for potting balls of consecutive numbers.
  // (Initially US 8 ball only but maybe in other Rules subclasses too.)
  // NB We need to remember this for each player.
  int m_lastBallPotted[2];

  // ID of winner, set for some external reason.
  // If SetGameOverForExtReason is called, this var holds the winner ID.
  // This forces the game to be over, with the winner set 
  // according to this ID. I.e. it has precedence over a winning/losing
  // shot.
  int m_externalWinnerId;
  // Explanation for win/lose game set by an external event.
  std::string m_externalText;

public:
#if defined(SCENE_EDITOR)
  virtual bool Save(File*);
  virtual PropertyMap GetProperties() const;
  virtual void SetProperties(const PropertyMap&);
#endif
};
}

#endif


