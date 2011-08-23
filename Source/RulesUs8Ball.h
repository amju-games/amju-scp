/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: RulesUs8Ball.h,v $
Revision 1.1.2.3  2007/06/10 08:58:04  jay
BallInPocket() is not told which pocket the ball hit. This info is lost
when a State change is used to flag ball/pocket collisions.

Revision 1.1.2.2  2006/08/14 17:50:25  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:41  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.4  2005/07/30 12:27:25  jay
Added virtual Clone() so undo stack can copy the current Rules object.

Revision 1.1.2.3  2005/06/29 20:25:24  jay
Ongoing fixes to rules - removed one of two very similar functions.

Revision 1.1.2.2  2005/06/22 21:45:38  jay
Implemented Us 8 Ball rules

Revision 1.1.2.1  2005/06/19 21:37:42  jay
Move Rules variants to separate files

*/

#ifndef RULES_8_BALL_H_INCLUDED
#define RULES_8_BALL_H_INCLUDED

#include "Rules.h"

namespace Amju
{
// Very similar to UK 8 ball.
// The differences are:
// - all the balls have different numbers, so we can't rely on the ball
//   number identifying a player's ball directly. Instead we must
//   identify spots (< 8) or stripes (> 8)
// - foul results NOT in an extra shot for the next player, but the next
//   player may place the cue ball ANYWHERE, not just behind the
//   headstring.
//
// NB We are not deriving from RulesUk so we don't have the problem
// of "brittle" code - fixing a bug in the base class breaks the derived
// one. MAYBE we can derive from RulesUK later ??
class RulesUs8Ball : public Rules
{
public:
  enum SpotsOrStripes { SPOTS, STRIPES };

  RulesUs8Ball();

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

  // For UK pool, we will only count the balls of the colour for this player.
  virtual int GetNumBallsRemainingForPlayer();

  // Return true if colours are set
  bool ColoursDecided() const;

  // If colours are decided, returns SPOTS or STRIPES for the player
  SpotsOrStripes GetPlayerColour(int player) const;
  
  // If colours are decided, returns the number of coloured balls
  // remaining for this player. This includes the 8 ball.
  // If colours not decided, it currently returns 8. TODO is this ok ?
//  int GetPlayerColoursRemaining(int player) const;

protected:
  // Override this to test potted balls in case the game has just
  // been won or lost.
  // E.g. UK Pool: potted black before all other colours->lose game
  // N-Ball: potting highest ball without fouling->win game
  // Only called after at least one ball has been potted; return true
  // if game has now ended. Set the winner inside the subclassed function.
  virtual bool PotEndsGame();

  virtual void GivePlayerFoulAward();

protected:
  // For player 0 and 1, the colour of the ball to hit next for a
  // legal shot.
  SpotsOrStripes m_playerColour[2];

  // Set to true once the colours for the players are set - i.e. after
  // one ball is validly potted in one shot.
  bool m_coloursDecided;

  // Once colours are decided, this stores the number of colours
  // remaining for each player.
  int m_playerColoursRemaining[2];
};
}

#endif

