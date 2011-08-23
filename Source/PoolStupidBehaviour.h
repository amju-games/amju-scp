/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: PoolStupidBehaviour.h,v $
Revision 1.1.2.1  2006/08/14 17:50:24  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:35  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1  2004/09/08 15:43:06  jay
Added to repository
  
*/

#if !defined(GOLF_STUPID_BEHAVIOUR_H_INCLUDED)
#define GOLF_STUPID_BEHAVIOUR_H_INCLUDED

#include "AvoidBehaviourBase.h"

namespace Amju
{
// This behaviour makes character stupidly walk around.
// We derive from SeekBehaviour so we can avoid obstacles.
// This is a slight improvement on StupidBehaviour, where the
// chanracters would just fall off the world. We want to
// move around within a ring region around the player,
// and we DON'T want to fall off any edges!
class PoolStupidBehaviour : public AvoidBehaviourBase
{
public:
  PoolStupidBehaviour();

  virtual void Update();

protected:
  virtual void ReactToObstacle();

  float GetTurnDir() const;
 
  bool IsBlocked() const;

  bool IsDropAhead() const;

protected:
  // Change rotate vel after a time.
  float m_time;

  // Min and max distance from player.
  float m_ringMin;
  float m_ringMax;
 
  // Max rotate vel. 
  float m_rotateVelMax;
};
}

#endif
