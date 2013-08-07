/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: FreeMovingGameObject.h,v $
Revision 1.1.8.5  2005/09/12 20:40:08  jay
All changes to m_prevOrientation should now go through SetPrevOrientation(),
so we can find things which change it more easily.
Replaced all the changes to m_prevOrientation with this call.
COMMENTED OUT the calls where only the height is changed - this obliterates
the previous (x, z) position, which for POOL is what we are interested in.
This has had the effect of making balls ricochet around like crazy, but
now capsule collision works.

Revision 1.1.8.4  2005/09/05 20:03:36  jay
Allow access to Previous position - used in POOL to get the first ball
we collide with.

Revision 1.1.8.3  2005/05/08 08:09:35  jay
Fix GetOrientation to return const.
New variable to store previous Orientation and speed. This lets us
backtrack to find exact collision point. Used in POOL.

Revision 1.1.8.2  2005/03/17 23:10:30  Administrator
MSVC compile error

Revision 1.1.8.1  2005/03/17 20:49:56  jay
Add SetForwardVector()

Revision 1.1  2004/09/08 15:42:49  jay
Added to repository
  
*/

#if !defined(FREE_MOVING_GAME_OBJECT_H_INCLUDED)
#define FREE_MOVING_GAME_OBJECT_H_INCLUDED

#include "VisibleGameObject.h"
#include "Orientation.h"
#include "SolidComponent.h"
#include "Controller.h"
//#include "Platform.h"
#include <RCPtr.h>

namespace Amju
{
class PlayerMessage;

class FreeMovingGameObject 
: public VisibleGameObject
{
public:

  FreeMovingGameObject();

  void SetController(Controller*);

  // Override default in VisibleGameObject.
  // We want to increase the size of the sphere as velocity increases,
  // with the sphere centre moving in the direction of movement.
  // This is so a moving object gets the heights of things it is travelling
  // towards, not just the things it is currently near to.
  virtual const BoundingSphere* GetHeightServerSphere();

  // Position
  // --------

  // Replace the entire orientation - this allows you to save the current
  // orientation, and to replace it if the new position is bad, say.
  virtual void SetOrientation(const Orientation& o);

  // Get the current orientation - designed to allow saving of current pos.
  virtual const Orientation* GetOrientation() { return &m_orientation; }
  
  Orientation GetPrevOrientation() const;

  // Forward movement/blockage/height drop
  // -------------------------------------

  // Move forward the specified number of units. The direction of movement
  // is the Y-axis rotation angle.
  void Forward(float units);

  // Get count of how many frames the NPC has been blocked, i.e. prevented from 
  // moving forwards.
  int GetBlocked() const;

  // Call to increment/reset blocked count. This is done internally, but
  // conceivably you may want to force it one way or the other.
  void SetBlocked(bool b);

  // Get float 0..1 indicating severity of drop ahead of the character.
  // 0 means no drop, (or uphill) 1 means a verical drop ahead.
  float DropAhead() const;


  // Velocity, acceleration
  // ----------------------
  // Set velocity, in units/sec.
  void SetForwardVel(float vel);
  float GetForwardVel() const { return m_forwardVel; }

  float GetMaxForwardVel() const;
  void SetMaxForwardVel(float maxvel);

  // Forward acceleration, in units/sec/sec.
  void SetForwardAccel(float accel) { m_forwardAccel = accel; }
  float GetForwardAccel() const { return m_forwardAccel; }

  // Get 2D vector the character is facing. This is set when SetYRotateVel()
  // is called. The vector has length 1.
  void GetForwardVector(float* x, float* z) const;
  void SetForwardVector(float x, float z);

  // Call to recalculate the x/z vector, if you change the Y-direction.
  void RecalcForwardVector();


  // Rotation about y-axis
  // ---------------------

  // Set Y-axis rotation speed, in degrees/sec.
  void SetYRotateVel(float degs);
  float GetYRotateVel() const { return m_yRotateVel; }


  // Rotate around Y axis by the given angle.
  // NB This is instantaneous.
  void RotateY(float degrees);

  // Turn by the specified angle. Turn velocity is an attribute of the object.
  //void TurnYDegrees(float degrees);
  // Turn velocity used by TurnYDegrees().
  //float GetTurnVel() const;
  //void SetTurnVel(float tv);


  // Platforms
  // ---------
  // I.e. Moving objects which affect the velocity of this one.

  // Set pointer to the Platform the character is on. If the character is not
  // on a platform, set to 0.
  //void SetActivePlatform(Platform* p);
  
  // Get platform Character is on, or 0.
  //Platform* GetActivePlatform() { return m_pActivePlatform; }


  // Gravity/falling functions
  // -------------------------

  // Acceleration due to gravity. Positive means ** DOWN **.
  static void SetGlobalGravity(float g);
  static float GetGlobalGravity() { return s_g; }

  // Get "local gravity", which is adjusted for resistance to gravity
  // for this object.
  float GetGravity() const;

  void RecalcHeight(float deltaTime); 

  void SetFalling(bool isFalling);

  // Returns true if body is in free fall. NB movement could be upwards
  // temporarily.
  bool IsFalling() { return m_isFalling; }

  // Called when falling body hits ground. Falling flag is still set, but
  // vertical velocity is flipped so body 'bounces' upwards.
  // Damping factor controls bounce: 1.0 means body bounces forever; 0 means 
  // body doesn't bounce at all.
  // When bounce is small, falling flag is reset.
  void BounceWithDamping(float damping);

  void SetGroundHeight(float f) { m_groundheight = f; }
 
  float GetGroundHeight() const { return m_groundheight; }

  // Vertical velocity; positive is ** DOWNWARDS **.
  void SetVerticalVel(float v) { m_verticalVel = v; }
  float GetVerticalVel() const { return m_verticalVel; }

  void Bounce();

  void SetBounceDampen(float bd) { m_bounceDampen = bd; }

  // React to hitting the ground after falling.
  virtual void FellFromHeight(float heightFallenFrom) {}

  void SetHeight(float y);
  float GetHeight() const;

  float GetHeightFallenFrom() const { return m_heightFallenFrom; }
  void SetHeightFallenFrom(float h) { m_heightFallenFrom = h; }

  // If true, object stops dead after it has fallen and has stopped bouncing.
  bool GetDeadStopAfterFall() const;
  void SetDeadStopAfterFall(bool b);

  // Heights
  // -------
  virtual const HeightServer* GetHeightServer() const;
  virtual void RefreshHeightServer();


  // Get the ground poly this object is on. May be zero.
  const HeightPoly* GetHeightPoly() const;


  // Recalculate position given current vel/accel etc.
  // -------------------------------------------------
  virtual void Recalculate();


  // Idle
  // ----

  // Return true if object is idle.
  bool IsIdle();
  // Get number of seconds object has been idle.
  float GetIdleTime();


  // Override for sound effects 
  // --------------------------
  virtual void OnBounceOffWall() {}
  virtual void OnBounceOnGround() {}

protected:
  // Respond to Messages containing PlayerCodes
  void ExecuteForwardVel(const PlayerMessage& m);
  void ExecuteRotateVel(const PlayerMessage& m);

  // Use controller to update position/vel etc.
  void ControlUpdateThis();

  void AdjustGravity();

private:
  // Acceleration due to gravity. Positive means ** DOWN **.
  static float s_g;

protected:
  // Local "gravity" - this is adjusted for buoyancy etc.
  // I.e. it is a combination of gravity and resistance to gravity.
  float m_g; 

  // Vertical velocity; positive is ** DOWNWARDS **.
  float m_verticalVel;

  bool m_isFalling;

  // Time spent falling so far.
  float m_fallingTime;

  // Height of ground under this body.
  float m_groundheight;

  // Affects how bouncy the body is when it hits the floor.
  float m_bounceDampen;


protected:

  // Used to position the character in space.
  Orientation m_orientation;

  // Character forward velocity.
  float m_forwardVel;

  // If true, the forward vel. should be zeroed as soon as we are no longer falling.
  bool m_stopWhenNotFalling;

  // Acceleration.
  float m_forwardAccel;

  // Degrees/sec rotation around Y axis.
  float m_yRotateVel;

  // 2D vector in x-z plane. This is the direction in which the character is
  // facing.
  double m_vectorX;
  double m_vectorZ;

  // POOL: Store previous values
  // This is so we can go back to state before a collision, to find
  // the exact collision point.
  float m_prevForwardVel;

//private:
  // Prevent changes to this unless through SetPrevOrientation()
  Orientation m_prevOrientation;
protected:
  void SetPrevOrientation(const Orientation&);

  float m_heightFallenFrom;

  // Count of how many frames this NPC has been stuck.
  int m_blockedCount;

  // Turning vel, degs/sec
  float m_turnVel;

  float m_maxForwardVel;


  // Collision/heights
  // -----------------

  // Point to the current ground height poly if possible - we must refresh when
  // the HeightServer is refreshed.
  const HeightPoly* m_pHeightPoly;

  // Store a HeightServer. This holds the static scene HS 
  // (i.e. VisibleGameObject::m_heightServer), plus HeightServers from Platforms.
  // Having a second HS means we don't have to recalculate everything when the
  // only thing that has changed is the position of (a) Platform(s); we just
  // recalculate the union of the static HS and the Platform HSs again.
  mutable HeightServer m_heightServerWithPlatforms;

  // The collision height server is made from this solid.
  // Characters need this because we want a really rough collision solid;
  // we don't want to use every triangle in the character's mesh!
  PSolidComponent m_pCollisionVol;


  // Platforms
  // ---------
  // Point to any platform this object is currently on.
  //Platform* m_pActivePlatform; 


  // Seconds for which character has been idle.
  float m_idleTime;

  // If true, object stops moving after a fall. This is useful for player 
  // characters. 
  bool m_deadStopAfterFall;


  // Controller
  // ----------
  // Controller: This controls the position/vel of this object.
  RCPtr<Controller> m_pController;

  // Bounding Sphere which stretches in direction of velocity, so 
  // HeightServer picks up things we are travelling towards.
  // NB maybe should be in FreeMoving bas class.
  BoundingSphere m_heightServerSphere;

};
}

#endif



