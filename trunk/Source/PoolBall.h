/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: PoolBall.h,v $
Revision 1.1.2.6  2007/06/17 20:39:17  jay
Move 'send state' flag from here to GameObject

Revision 1.1.2.5  2007/06/10 08:56:18  jay
Override SetState() - this is where Rules are notified of balls in pocket
 or out of bounds. This should fix consistency bugs where collisions
 are different on the two clients

Revision 1.1.2.4  2007/03/16 08:50:40  jay
Online changes: each ball state change/collision is sent to the server, and
used in the replay to keep consistency between clients.

Revision 1.1.2.3  2007/03/10 23:39:04  Administrator
Override CueBall::SetOrientation() for ShotLOg

Revision 1.1.2.2  2006/08/14 17:50:21  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:37  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.15  2006/04/13 18:19:19  jay
Fix collision volume

Revision 1.1.2.14  2005/09/29 19:08:48  jay
Slightly rearrange collision handling code so subclasses can do accuarate
physics.

Revision 1.1.2.13  2005/09/02 18:18:30  jay
Make sure GetExactCollisionCoords returns the first point of contact,
not the 'exit' point.

Revision 1.1.2.12  2005/08/26 21:53:28  jay
Added Draw/Roll

Revision 1.1.2.11  2005/06/29 20:22:53  jay
Experiement with new collision response algo. Not much good :-(

Revision 1.1.2.10  2005/06/19 21:34:08  jay
GetExact collision coords without affecting ball positions;
Store all balls we intersect so we move away from all of them

Revision 1.1.2.9  2005/05/15 17:19:08  jay
Add GetTq()

Revision 1.1.2.8  2005/05/08 09:57:04  jay
Added capsule collision test - but disabled until capsules are accurate.
Added code to get exact positions of colliding balls.

Revision 1.1.2.7  2005/04/21 17:34:34  jay
Override RefreshHeightServer to ignore NonHeadArea.

Revision 1.1.2.6  2005/04/17 21:59:24  jay
Added english/spin

Revision 1.1.2.5  2005/04/17 17:25:38  Administrator
Changes for Scene Editor

Revision 1.1.2.4  2005/04/11 21:23:20  jay
Move away from another ball if we intersect it.

Revision 1.1.2.3  2005/04/01 05:57:49  jay
Now uses a quaternion to show natural roll. This fixes problems with
gimbal lock.

Revision 1.1.2.2  2005/03/19 17:22:57  jay
Add Scene Ed code - saving, properties.

Revision 1.1.2.1  2005/03/17 20:53:40  jay
New classes for Pool game

*/

#ifndef AMJU_POOL_BALL_H_INCLUDED
#define AMJU_POOL_BALL_H_INCLUDED

#include "FreeMovingSolidGameObject.h"
#include "Quaternion.h"
#include "TexturedQuad.h"
#include "SoundFxManager.h"

namespace Amju
{
class PoolBall : public FreeMovingSolidGameObject
{
public:
  PoolBall();

  virtual const char* GetTypeName() const;

  virtual void SetState(State newState);
  virtual void Draw();
  virtual void Update();
  virtual bool Load(File* pFile);
  virtual void OnRoomEntry();
  // Override this so we can tell when a moving ball is outside the view frustum.
  virtual void CullDraw(Frustum*);

  virtual void HandleObjectCollision(GameObject* pObj);

  // Lose some momentum when we bounce off a wall. 
  // Also sound FX.
  virtual void OnBounceOffWall();
  virtual void OnBounceOnGround();

  // Override this (from FreeMovingGameObject) so we can discount the collision
  // vol of the Non-Head(string) Area. It's only used when the user is allowed to 
  // place the ball.
  virtual void RefreshHeightServer();

  // Need to override this because PoolBall coords are handled differently
  // to normal FreeMovingSolidGameObjects
  virtual void RecalcCollisionVol();

  // Override Intersects so we can check the capsule makde by a moving 
  // ball. 
  virtual bool Intersects(const GameObject& );

  // Override FreeMovingGameObject version because we need better
  // accuracy.
  virtual void Recalculate();

  // Override so we can use super-accurate coords
  virtual void SetOrientation(const Orientation&);
  virtual const Orientation* GetOrientation();

  // Get number - e.g. 1 - 15. Could be anything though.
  // Used by Rule for the current room to decide if shot/pot
  // was legal etc.
  int GetNumber() const;

  // Set y-axis spin vel.
  void SetEnglishVel(float degsPerSec);

  // Set forward/back spin on ball. This is in addition to the natural 
  // angular vel - i.e. 0 gives "natural roll".
  void SetDrawRollVel(float degsPerSec);
  // Get the angular vel due to top/back spin, degs/sec 
  float GetDrawRollVel() const;

  TexturedQuad* GetTq();

  // Find the exact coords of the two balls when they collide.
  // Sets the balls positions to these coords if they exist:
  // the balls may not collide, in which case the ball positions
  // are not changed.
  static void GetExactCollisionCoords(PoolBall* p1, PoolBall* p2);

  // Get the time of collision and positions, but do not set the
  // ball positions.
  static void GetExactCollisionCoords(
    const PoolBall* p1, 
    const PoolBall* p2,
    float* pCollisionTime,
    VertexBase* v1, 
    VertexBase* v2);

  // Set the position at the given time after the last known good
  // time.
  void SetPositionAtTime(float t);

  // Called to set the change in forward vector due to top/back spin.
  // The change is applied after a short pause, not immediately.
  void ApplyDrawRoll(float dx, float dz, float vel);

protected:
  // Used by GetExactCollisionCoords to limit the max time, and so
  // distance travelled, of the moving ball. This is to prevent finding
  // the 'exit' point of the moving ball.
  static float GetTmaxLimit(const PoolBall*, const PoolBall*);

  // Called from HandleObjectCollision when other object is also a ball.
  void HandleBallCollision(PoolBall*);

  // Called by above func, symmetrically deals with both balls.
  void HandleBallCollision(PoolBall*, PoolBall*);

public:
#if defined(SCENE_EDITOR)
  virtual bool Save(File*);
  virtual PropertyMap GetProperties() const;
  virtual void SetProperties(const PropertyMap&);
  std::string m_tqBmp, m_tqAlpha;
#endif

protected:
  // True if (cue) ball is sliding rather than rolling.
  // After some time roll takes over.
  bool m_isSliding;
  // Distance which this ball has slid so far
//  float m_slideDist;
  // Max slide distance before natural roll takes over.
//  static float s_maxSlideDist;

  // Forward/draw roll vel (degs/sec).
  float m_rollVel;
  // Handle draw/roll
  // If true, we are pausing before applying Draw or roll to the ball's path
  // following a collision.
  bool m_rollPause;
  // Time we have paused so far
  float m_rollTime;
  // The change to the ball's forward vector which we apply after the pause.
  float m_rollDx, m_rollDz;
  // The change to the ball's speed which we apply after the pause.
  float m_rollApplyVel;

  // Current roll angle.
  float m_rollAngle; 
  // Side/English spin vel. This causes a curve in the ball's path
  // which we model using the Turn Velocity.
  float m_englishVel;
  // Current english angle
  float m_englishAngle;
  // Deceleration - the english spin must stop when the ball stops
  // moving, right ?
  float m_englishAccel;

  Orientation m_startOr;

  // For some pool games, each ball has a separate number.
  // For UK pool, these numbers could indicate red/yellow - i.e. they
  // don't have to be unique.
  int m_number;

  // Rotation: we need to use a quaternion to store the roll/side
  // rotation, or we get gimbal lock.
  Quaternion m_quat;

  // 2D quad to show this ball on HUD etc.
  TexturedQuad m_tq;

  // Velocity added to ball vel to avoid intersections with balls or walls.
  // The idea is that if the dt between frames is too great, the ball's
  // vel will have decayed to zero by the next frame, and so we won't get
  // an opportunity to fix intersections. This extra vel is not decelerated
  // so will still be good after any time delay.
  // Normally this vel is zero, but gets set positive when there is a
  // collision.
  float m_intersectVel;

  // Remember all balls we are intersecting. Move away from them,
  // ignoring whatever the forward vector is.
  std::vector<PoolBall*> m_intersectBalls;

  // Super accurate (x, z) coord, so we don't drift as the ball travels.
  double m_x;
  double m_z;
};

// Cue ball doesn't do anything different, but having it as a separate
// class may make things easier (RTTI).
class CueBall : public PoolBall
{
public:
  CueBall();

  virtual bool Load(File* pFile);

  // Override for debugging (ShotLog)
  virtual void SetOrientation(const Orientation&);
};
}

#endif


