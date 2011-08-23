/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: RulesUk.h,v $
Revision 1.1.2.3  2007/06/10 08:58:04  jay
BallInPocket() is not told which pocket the ball hit. This info is lost
when a State change is used to flag ball/pocket collisions.

Revision 1.1.2.2  2006/08/14 17:50:25  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:40  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.7  2005/09/06 20:02:10  jay
Add virtual DecShotCount() and override for UK rules, so we never get
two shots on the black.

Revision 1.1.2.6  2005/08/06 17:56:40  Administrator
Allow AI to take free shots

Revision 1.1.2.5  2005/08/06 10:39:21  Administrator
Added Free Shot Counter

Revision 1.1.2.4  2005/08/04 19:39:55  jay
Fix Rules bugs for v.1.1

Revision 1.1.2.3  2005/07/30 12:27:25  jay
Added virtual Clone() so undo stack can copy the current Rules object.

Revision 1.1.2.2  2005/06/29 20:25:24  jay
Ongoing fixes to rules - removed one of two very similar functions.

Revision 1.1.2.1  2005/06/19 21:37:41  jay
Move Rules variants to separate files

*/

#ifndef RULES_UK_H_INCLUDED
#define RULES_UK_H_INCLUDED

#include "Rules.h"

namespace Amju
{
// Rules for UK pool
class RulesUk : public Rules
{
public:
  RulesUk();

  virtual Rules* Clone();

  virtual const char* GetTypeName() const;

// Override Rules functions
  // New game - this tells us to reset everything.
  virtual void OnRoomEntry(); 
  
  // These next virtual functions are for subclasses to override, to
  // produce a particular set of rules. 
  
  // Call when a shot begins. This can reset any flags, counters
  // etc that we need to determine if a shot is a foul or not.
  virtual void Reset();
  
  // Call when a ball goes in a pocket.
  // Check player colour: set if this is the first ball potted.
  // Also, pocketing the 8 ball may lose the game; pocketing the
  // highest-numbered ball may win the game, etc.
  virtual void BallInPocket(PoolBall* pBall);
  
  // Call when cue ball hits another ball
  virtual void CueBallHitsBall(PoolBall* pBall);

  // Call when a ball hits a rail
  virtual void BallHitsRail(PoolBall* pBall);

  // Call when all action on the table has stopped.
  virtual void BallsHaveStopped();

  // For UK pool, we will only count the balls of the colour for this player
  // plus the black.
  virtual int GetNumBallsRemainingForPlayer();

  // Return true if colours are set
  bool ColoursDecided() const;

  // If colours are decided, returns 1 or 2 for the player
  int GetPlayerColour(int player) const;

  // If colours are decided, returns the number of coloured balls
  // remaining for this player.
// NB Removed as numbers are cached, it's a bug minefield. Use
// GetNumBallsRemainingForPlayer().
//  int GetPlayerColoursRemaining(int player) const;

protected:
  // Override this to test potted balls in case the game has just
  // been won or lost.
  // E.g. UK Pool: potted black before all other colours->lose game
  // N-Ball: potting highest ball without fouling->win game
  // Only called after at least one ball has been potted; return true
  // if game has now ended. Set the winner inside the subclassed function.
  virtual bool PotEndsGame();

  // Called when previous player fouls, gives current player extra
  // shot, can place ball, etc.
  virtual void GivePlayerFoulAward();

  // Override this function so we can check if we are on the black.
  // If so we only get one shot.
  virtual void DecShotCounter();

  // Manage the Free Shot "flag" which is really a counter.
  void ResetFreeShot();
  void AwardFreeShot();
  void DecFreeShotCounter();

public:
  // Allow AI access to this function
  bool IsFreeShot() const;

protected:
  // For player 0 and 1, the colour of the ball to hit next for a
  // legal shot.
  int m_playerColour[2];
  // Set to true once the colours for the players are set - i.e. after
  // one ball is validly potted in one shot.
  bool m_coloursDecided;

  // Once colours are decided, this stores the number of colours 
  // remaining for each player.
  int m_playerColoursRemaining[2];

  // Free Shot is the first shot following a foul by the other player.
  // When it's a free shot you may pot a ball of the other player's
  // colour.
  // TODO Find out if it's ok to HIT a ball of the other player's colour 
  // first too.
  // NB This is a counter, not just a flag. If we use a flag there is
  // nowhere we can reset it and remember the state across shots.
  int m_isFreeShot;
};
}

#endif


