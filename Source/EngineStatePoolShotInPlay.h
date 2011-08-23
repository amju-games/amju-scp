/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStatePoolShotInPlay.h,v $
Revision 1.1.2.4  2007/05/23 18:45:42  jay
Send all object changes in one Http request (POST), not individually.
 + Some tidying up.

Revision 1.1.2.3  2007/03/16 08:50:40  jay
Online changes: each ball state change/collision is sent to the server, and
used in the replay to keep consistency between clients.

Revision 1.1.2.2  2006/08/17 18:15:41  jay
Ongoing work for Online Pool

Revision 1.1.2.1  2006/08/14 17:50:17  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:29  jay
Moved Pool code from Pool/ to Pool/

Revision 1.2.6.6  2005/09/23 19:39:11  jay
Add include for convenience

Revision 1.2.6.5  2005/08/26 21:29:59  jay
Add mouse camera control

Revision 1.2.6.4  2005/06/19 21:30:30  jay
Changes to ball collision code, to try to eliminate intersecting balls.

Revision 1.2.6.3  2005/04/21 17:35:29  jay
Show big text

Revision 1.2.6.2  2005/04/11 21:21:24  jay
Try to extract any intersecting balls before ending this state.

Revision 1.2.6.1  2005/03/17 20:58:12  jay
Initial changes from Pool to Pool

Revision 1.2  2004/09/17 14:00:38  jay
Added virtual BallInHole() so we can override in Demo mode

Revision 1.1  2004/09/08 15:43:05  jay
Added to repository
  
*/

#if !defined(ENGINE_STATE_GOLF_SHOT_IN_PLAY_H_INCLUDED)
#define ENGINE_STATE_GOLF_SHOT_IN_PLAY_H_INCLUDED

#include <utility>
#include <vector>
#include "EngineStatePoolBase.h"
#include "ExplosionEffect.h"
#include "PoolBall.h"
#include "Level.h"

namespace Amju
{
class EngineStatePoolShotInPlay : public EngineStatePoolBase
{
public:
  static const char* Name;

  virtual void Draw();
  virtual void DrawOverlays();

  virtual void SetActive(bool active);
  virtual bool Load();
  virtual void Clear();
  virtual void Update();
  virtual void DoCameraWork();

  // Control camera with mouse 
  virtual void MousePos(int x, int y);
  virtual void MouseButton(bool down, bool ctrl, bool shift);

  void SetPlayerOrientation(const Orientation& o);

  // Called when collision detection code finds that ball and Hole
  // intersect. Overridden in Demo subclass.
//  virtual void BallInHole();

protected:
  virtual void DrawPlayer();

  // Called when balls have stopped moving: deal with online stuff.
  void OnlineBallsStopped();

  void ResetSendStateFlags();

protected:
  // All collision testing, including special ball collision code.
  // Returns true if any balls collide.
  bool CollisionTest();

  // Check if any Ball has collided with another object. 
  // Returns true if there is any collision.
  // Sets up list of pairs of PoolBalls which have collided.
  typedef std::vector<std::pair<PoolBall*, PoolBall*> > BallPairs;
  bool BallCollisionTest(BallPairs* pBallPairs);

  // Handle ball collisions - rewritten to avoid intersecting balls
  // hopefully.
  // Returns true if any balls have collided (i.e. right now they
  // intersect - but won't after this call, right ?!)
  bool BallCollisions();

  // Handle collisions between all pairs of objects where one or
  // both object is not a ball. I.e. ball-pocket collisions, but
  // we may handle other types here in future.
  void NonBallPairTest();

  // Returns true if balls have stopped moving - but we may wait longer for
  // extracting.
  bool BallsHaveStopped();

  // Returns true if ball has gone OB.
  bool IsOutOfBounds();

  // Called if ball goes OB
  void OutOfBoundsReset();

  bool IsUnderwater();

protected:
  // True if changing to another state.
  bool m_isFading;

  // Player position; we need to remember this as when the ball moves we lose
  // this information.
  Orientation m_playerOr;

  // Particle effect as club hits ball
  ExplosionEffect m_explosion;

  // Big text when something happens, like a pot or foul.
  PSolidComponent m_pBigText;

  // If true the balls have stopped and we should go to the next state.
  // However, we must wait until we get the final ball positions from the
  // server.
  bool m_onlineReadyToFinish;
};
}

#endif

