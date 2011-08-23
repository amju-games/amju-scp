/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: RulesNBall.h,v $
Revision 1.1.2.3  2007/06/10 08:58:03  jay
BallInPocket() is not told which pocket the ball hit. This info is lost
when a State change is used to flag ball/pocket collisions.

Revision 1.1.2.2  2006/08/14 17:50:25  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:40  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.3  2005/07/30 12:27:25  jay
Added virtual Clone() so undo stack can copy the current Rules object.

Revision 1.1.2.2  2005/06/22 21:44:42  jay
Ongoing fixes to N-ball rules

Revision 1.1.2.1  2005/06/19 21:37:41  jay
Move Rules variants to separate files

*/

#ifndef RULES_N_BALL_H_INCLUDED
#define RULES_N_BALL_H_INCLUDED

#include "Rules.h"

namespace Amju
{
// Rules for 6-ball, 9-ball, 15-ball etc. The number of balls is a loadable
// property.
class RulesNBall : public Rules
{
public:
  RulesNBall();

  virtual Rules* Clone();
  
  virtual const char* GetTypeName() const;
  virtual bool Load(File*);
  
// Override Rules functions
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

  // Call when all action on the table has stopped.
  virtual void BallsHaveStopped();

  // Get no of balls left which the current player must pot.
  // Defaults to counting all balls still in play (does NOT count the cue ball).
  // For UK pool, we will only count the balls of the colour for this player.
  virtual int GetNumBallsRemainingForPlayer();

protected:
  // Override this to test potted balls in case the game has just
  // been won or lost.
  // E.g. UK Pool: potted black before all other colours->lose game
  // N-Ball: potting highest ball without fouling->win game
  // Only called after at least one ball has been potted; return true
  // if game has now ended. Set the winner inside the subclassed function.
  virtual bool PotEndsGame();

  virtual void GivePlayerFoulAward();

#if defined(SCENE_EDITOR)
public:
  virtual bool Save(File*);
  virtual PropertyMap GetProperties() const;
  virtual void SetProperties(const PropertyMap&);
#endif

protected:
  int m_numBalls;
};
}

#endif

