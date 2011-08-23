/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: PoolExplBall.h,v $
Revision 1.1.2.2  2006/08/14 17:50:23  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:38  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.4  2005/10/01 22:01:40  Administrator
Scene Ed: added paths to #includes for MSVC

Revision 1.1.2.3  2005/09/30 13:03:20  jay
Added new member, m_isDetonating; don't use DETONATING state. When in this
state collisions are not detected.
TODO find out why this is.

Revision 1.1.2.2  2005/09/29 19:11:25  jay
Flash out behaviour for exploding balls. There are still some quirks
with collision.

Revision 1.1.2.1  2005/09/23 19:59:42  jay
Add exploding balls for crazy pool

*/

#ifndef POOL_EXPL_BALL_H_INCLUDED
#define POOL_EXPL_BALL_H_INCLUDED

#include "PoolBall.h"
#include "BlastWaveEffect.h"
#include "ExplosionEffect.h"

namespace Amju
{
// Like PoolBalls but these explode when hit after a short time delay.
// The idea is that in Trick Shots you should pot the regular balls
// but avoid the exploding balls.
// NB Trick Shot Rules must exclude this kind of ball from the 
// "Valid Balls" list. This is handled by IsPoolBall() which excludes 
// this type explicitly.
class PoolExplBall : public PoolBall
{
public:
  virtual const char* GetTypeName() const;
  virtual void SetState(State newState);
  virtual void HandleObjectCollision(GameObject* pObj);
  virtual void Draw();
  virtual void CullDraw(Frustum*);
  virtual bool Load(File* pFile);
  virtual void Update();
  virtual void DrawShadow() const;
  virtual void OnRoomEntry();

protected:
  // Effect when block is hit
  BlastWaveEffect m_blastWave;

  ExplosionEffect m_explosion;

  // Timer for ticking sound
  float m_tickTimer;

  // Timer for flashing
  float m_flashTimer; // Time until we flip colour
  float m_detonateTimer; // Time we have been detonating. As this increases,
   // we flash faster.
  bool m_flashColour; // flips values to flash the Bomb.

  // The ID of the player who set off the bomb. This player loses if the
  // bomb explodes and the game is still in progress. If the game is
  // over, the bomb does not do anything. 
  int m_detonatePlayerId;

  // True if ball is flashing/ticking and about to explode.
  bool m_isDetonating;
};
}

#endif

