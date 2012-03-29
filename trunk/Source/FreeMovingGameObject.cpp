/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: FreeMovingGameObject.cpp,v $
Revision 1.2.6.7  2005/09/13 23:06:21  jay
Added bounce debug output

Revision 1.2.6.6  2005/09/12 20:40:08  jay
All changes to m_prevOrientation should now go through SetPrevOrientation(),
so we can find things which change it more easily.
Replaced all the changes to m_prevOrientation with this call.
COMMENTED OUT the calls where only the height is changed - this obliterates
the previous (x, z) position, which for POOL is what we are interested in.
This has had the effect of making balls ricochet around like crazy, but
now capsule collision works.

Revision 1.2.6.5  2005/09/08 20:00:33  jay
Whitespace

Revision 1.2.6.4  2005/09/05 20:03:35  jay
Allow access to Previous position - used in POOL to get the first ball
we collide with.

Revision 1.2.6.3  2005/05/08 08:09:34  jay
Fix GetOrientation to return const.
New variable to store previous Orientation and speed. This lets us
backtrack to find exact collision point. Used in POOL.

Revision 1.2.6.2  2005/04/21 17:36:34  jay
Tidy up

Revision 1.2.6.1  2005/03/17 20:49:55  jay
Add SetForwardVector()

Revision 1.2  2004/09/17 13:45:22  jay
Fix so SetOrientation() also sets the bounding sphere

Revision 1.1  2004/09/08 15:42:49  jay
Added to repository
  
*/


#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "FreeMovingGameObject.h"
#include "Orientation.h"
#include <algorithm>
#include <functional> // mem_fun
#include <math.h>
#include <iostream>
#include "CharacterGameObject.h"
#include "CharacterManager.h"
#include "Character.h"
#include "Engine.h"
#include "Level.h"
#include "PoolGameState.h"
#include "PlayerCodes.h"
#include "PlayerMessage.h"
#include "Matrix.h"
#include "Player.h"
#include "RotateHelper.h"
#include "Controller.h"
#include "AngleCompare.h"
#include "SchAssert.h"

namespace Amju
{
// Acceleration due to gravity. Positive is downwards.
float FreeMovingGameObject::s_g = 3.0f;

void FreeMovingGameObject::SetGlobalGravity(float g) 
{ 
  s_g = g; 
}

FreeMovingGameObject::FreeMovingGameObject()
{
  m_g = s_g;
  m_verticalVel = 0;
  m_isFalling = false;
  m_fallingTime = 0;
  m_groundheight = 0;
  m_bounceDampen = 0.1f;
  m_forwardVel = 0;
  m_prevForwardVel = 0;
  m_forwardAccel = 0;
  m_yRotateVel = 0;
  m_vectorX = 0;
  m_vectorZ = 0;
  //m_pActivePlatform = 0;
  m_pHeightPoly = 0;
  m_heightFallenFrom = 0;
  m_stopWhenNotFalling = false;
  SetBlocked(false);
  m_turnVel = 90.0f; // Default.
  m_maxForwardVel = 10.0f;  // Default.
  m_idleTime = 0;
  m_deadStopAfterFall = true;
}

float FreeMovingGameObject::GetGravity() const
{
  return m_g;
}

void FreeMovingGameObject::SetController(Controller* p)
{
  m_pController = p;
}

Orientation FreeMovingGameObject::GetPrevOrientation() const
{
  return m_prevOrientation;
}

void FreeMovingGameObject::SetPrevOrientation(const Orientation& o)
{
  m_prevOrientation = o;
}

void FreeMovingGameObject::SetOrientation(const Orientation& o)
{
  // As we explicitly set the Orientation, forget about the old one ?
  // TODO
  if (o == m_orientation)
  {
    return;
  }

// TODO TEMP TEST - fix cue ball capsule bug ???
//  m_prevOrientation = m_orientation;
  SetPrevOrientation(o);

  m_orientation = o; 

  // 16/9/2004 - setting orientation didn't change the b.sphere!
  // This caused hard-to-find bug - this should hopefully fix other
  // bizarre behaviour.
  Matrix m;
  m.SetIdentity();
  CreateBoundingSphere(m);

  RecalcForwardVector();
}

const HeightPoly* FreeMovingGameObject::GetHeightPoly() const
{
  return m_pHeightPoly;
}

void FreeMovingGameObject::AdjustGravity()
{
  // Ordinarily, local grav == global grav.
  m_g = s_g;
}

const BoundingSphere* FreeMovingGameObject::GetHeightServerSphere()
{
  // Increase sphere size with speed.
  // Centre of sphere should be nearer to direction we are moving in.
  m_heightServerSphere = *(GetBoundingSphere());

  // Don't bother if speed is below some threshold.
  if (m_verticalVel > 1.0f)
  {
    // Get speed. Using pythag. this is speed ^ 2.
    float speedSq = m_verticalVel * m_verticalVel + m_forwardVel * m_forwardVel;

    // Multiply sphere size by speedSq * some constant
    float r = m_heightServerSphere.GetRadius() * sqrt(speedSq);
    m_heightServerSphere.SetRadius(r);
  
    // Get sphere centre 
    // Move centre in direction of movement by distance proportional to speed.
  }

  return &m_heightServerSphere; 
}

void FreeMovingGameObject::RecalcHeight(float deltaTime)
{
  if (!m_isFalling)
  {
    // No change in vertical position.
    return;
  }

  // Adjust local gravity to take buoyancy etc into account.
  AdjustGravity();

  m_fallingTime += deltaTime;
  m_verticalVel += deltaTime * GetGravity();
  float deltaHeight = deltaTime * m_verticalVel;

  // Subtract delta H because on the Y axis, up is positive.
  if (deltaHeight == 0)
  {
    return;
  }
  // POOL HACK
  //m_prevOrientation = m_orientation;
  // WILD 3
  //SetPrevOrientation(m_orientation);

  m_orientation.SetY(m_orientation.GetY() - deltaHeight);
}

void FreeMovingGameObject::SetFalling(bool isFalling)
{
  if (isFalling == m_isFalling)
  {
    // No change in state.
    return;
  }
  m_isFalling = isFalling;
  m_fallingTime = 0;
  m_verticalVel = 0;
}

void FreeMovingGameObject::BounceWithDamping(float damping)
{
  static const float BOUNCE_STOP = 
    Engine::Instance()->GetConfigFloat("bounce_stop");
 
  Assert(damping >= 0);
  Assert(damping <= 1.0f);

  // Flip vertical velocity upwards.
  if (fabs(m_verticalVel) > BOUNCE_STOP)
  {
    m_verticalVel = m_verticalVel * -damping;
    m_fallingTime = 0;

#ifdef BOUNCE_DEBUG
std::cout << "BOUNCE: Object " << GetId() << " IS STILL bouncing.\n";
#endif

  }
  else
  {
    m_verticalVel = 0;
    SetFalling(false);

#ifdef BOUNCE_DEBUG
std::cout << "BOUNCE: Object " << GetId() << " has stopped bouncing.\n";
#endif

  }
}

void FreeMovingGameObject::Bounce()
{
  BounceWithDamping(m_bounceDampen); 
  OnBounceOnGround();
}

void FreeMovingGameObject::RotateY(float degrees)
{
  // Can't rotate while falling (bug no 3)
  if (IsFalling())
  {
    return;
  }

  if (degrees == 0)
  {
    return;
  }

  //m_prevOrientation = m_orientation;
  // WILD 2: try to fix wild ricochets
  //SetPrevOrientation(m_orientation);
  // Position didn't change, so don't set prev orientation

  float yRotate = m_orientation.GetYRot();
  yRotate += degrees; 
  m_orientation.SetYRot(yRotate);

  RecalcForwardVector();
}

void FreeMovingGameObject::RecalcForwardVector()
{
  float yRotate = m_orientation.GetYRot();
  // Make a 2D unit vector in the x-z plane, pointing in the direction the 
  // character is facing. This saves us doing sin/cos every frame.
  m_vectorX = sin((double)yRotate / 180.0 * pi);
  m_vectorZ = cos((double)yRotate / 180.0 * pi);
}

void FreeMovingGameObject::Forward(float units)
{
  if (units == 0)
  {
    return;
  }

  float x = m_orientation.GetX();
  float z = m_orientation.GetZ();
  const float y = m_orientation.GetY();

  x += (float)((double)units * m_vectorX);
  z += (float)((double)units * m_vectorZ);

  //m_prevOrientation = m_orientation;
  SetPrevOrientation(m_orientation);
  m_orientation.SetX(x);
  m_orientation.SetZ(z);

  // Get the new Y-value if it exists on the current HeightPoly. 
  if (m_pHeightPoly)
  {
    float newY = m_pHeightPoly->GetY(x, z);
    if (newY < y + 0.9f && newY > y - 0.9f)
    {
      // New height seems to be valid.
      m_orientation.SetY(newY);
    }
  }
  else
  {
    // Try to get a valid HeightPoly. Go for the highest poly within range 
    // of current player height.
    m_pHeightPoly = GetHeightServer()->GetHeightPoly(x, y + 0.9f, y - 0.9f, z);
  }
}

void FreeMovingGameObject::GetForwardVector(float* x, float* z) const
{
  *x = (float)m_vectorX;
  *z = (float)m_vectorZ;
}

void FreeMovingGameObject::SetForwardVector(float x, float z)
{
  m_vectorX = x;
  m_vectorZ = z;

  // Update orientation y-rot, otherwise RecalcForwardVector()
  // will wipe out the vector we just set.
  float yrot = RadToDeg(atan2(x, z));
  Orientation o = *(GetOrientation());
  o.SetYRot(yrot);
  SetOrientation(o);
}

void FreeMovingGameObject::SetForwardVel(float vel) 
{ 
  // NB Can't rely on messages because currently this class has no GetId()
  
  //  PlayerMessage m("character fwd vel", 0, GetId(), GetId(), PLAYER_FWD_VEL, vel);
  //  ExecuteForwardVel(m);

  // If falling, forward vel. should not be under user control.
  // If user can still rotate, we must not alter the forward vector while
  // falling.
  if (IsFalling())// && vel == 0) j.c. test 2/8/2002
  {
    m_stopWhenNotFalling = GetDeadStopAfterFall();
    return;
  }
  // As we explicitly set a new vel, forget about the old one ?
  // TODO
  m_prevForwardVel = vel; // m_forwardVel;
  m_forwardVel = vel; 

/*
  PMessage m = new PlayerMessage(
    "character fwd vel", 0, GetId(), GetId(), PLAYER_FWD_VEL, vel);
  Engine::Instance()->SendGameMessage(m);
*/
}

void FreeMovingGameObject::ExecuteForwardVel(const PlayerMessage& m)
{
  float vel = m.GetArg();

  // If falling, forward vel. should not be under user control.
  // If user can still rotate, we must not alter the forward vector while
  // falling.
  if (IsFalling())// && vel == 0) j.c. test 2/8/2002
  {
    m_stopWhenNotFalling = GetDeadStopAfterFall();
    return;
  }
  m_prevForwardVel = m_forwardVel;
  m_forwardVel = vel; 

  // TODO
  // Check the vel against the max vel for this character; clamp if necessary.
}

void FreeMovingGameObject::SetYRotateVel(float degs)
{
  m_yRotateVel = degs;

  //PlayerMessage m("character rotate vel", 0, GetId(), GetId(), PLAYER_ROTATE_VEL, degs);
  //ExecuteRotateVel(m);

/*
  PMessage m = new PlayerMessage(
    "character rotate vel", 0, GetId(), GetId(), PLAYER_ROTATE_VEL, degs);
  Engine::Instance()->SendGameMessage(m);
*/
}

void FreeMovingGameObject::ExecuteRotateVel(const PlayerMessage& m)
{
  m_yRotateVel = m.GetArg(); 
}

int FreeMovingGameObject::GetBlocked() const
{
  return m_blockedCount;
}

void FreeMovingGameObject::SetBlocked(bool b)
{
  if (b)
  {

#if defined(_DEBUG)
//    std::cout << "NPC is blocked, id=" << GetId() << "\n";
#endif

    m_blockedCount++;
  }
  else
  {
    m_blockedCount = 0;
  }
}

/*
void FreeMovingGameObject::TurnYDegrees(float degrees)
{
  // TODO this should turn with turn velocity as set below until the
  // specified angle is reached.

  RotateY(degrees); // instantaneous :-(
}

float FreeMovingGameObject::GetTurnVel() const
{
  return m_turnVel;
}

void FreeMovingGameObject::SetTurnVel(float tv)
{
  m_turnVel = tv;
}
*/

float FreeMovingGameObject::GetMaxForwardVel() const
{
  return m_maxForwardVel;
}

void FreeMovingGameObject::SetMaxForwardVel(float maxvel)
{
  m_maxForwardVel = maxvel;
}

float FreeMovingGameObject::DropAhead() const
{
  // Get severity of drop ahead. 0 means no drop, 1 means vertical drop.

  // Get ground height in front of character.
  // First work out where 'in front' is - get the character direction vector.
  float xvec, zvec;
  GetForwardVector(&xvec, &zvec);
 
  // Multiply vector to get look-ahead distance.
  xvec *= 2.0f;
  zvec *= 2.0f;

  float xfront = m_orientation.GetX() + xvec;
  float zfront = m_orientation.GetZ() + zvec;
  // Get the height: the character has a height server.
  const HeightServer* pHs = GetHeightServer();
  float h = HeightServer::BadHeight;
  // severity of drop ahead: 0 means there is none; 1 means vertical drop ahead.
  float dropseverity = 0;
  if (pHs->GetHeight(xfront, zfront, &h))
  {
    if (h < GetHeight()) 
    {
      dropseverity = GetHeight() - h;
      if (dropseverity > 1.0f)
      {
        dropseverity = 1.0f;
      }
    }
  }
  else
  {
    // Can't get height, indicating a vertical drop.
    dropseverity = 1.0f;
  }
  return dropseverity;
}

void FreeMovingGameObject::SetHeight(float y)
{
  if (y == m_orientation.GetY())
  {
    return;
  }

  // POOL HACK: don't set prev orientation, we lose prev (x, z).
  //m_prevOrientation = m_orientation;
  //SetPrevOrientation(m_orientation);

  m_orientation.SetY(y);
}

float FreeMovingGameObject::GetHeight() const
{
  return m_orientation.GetY();
}

const HeightServer* FreeMovingGameObject::GetHeightServer() const
{
  return &m_heightServerWithPlatforms;
}

void FreeMovingGameObject::Recalculate()
{
  float deltaTime = Engine::Instance()->GetDeltaTime();

  // Don't (de-)accelerate if not on the ground (?)
  if (!IsFalling())// || m_verticalVel < 1.0f)  // TODO CONFIG
  {
    m_prevForwardVel = m_forwardVel;
    m_forwardVel += m_forwardAccel * deltaTime;
  }

  if (IsFalling())
  {
    m_stopWhenNotFalling = GetDeadStopAfterFall();
  }

  if (m_stopWhenNotFalling && !IsFalling())
  {
    m_prevForwardVel = m_forwardVel;
    m_forwardVel = 0;
    m_forwardAccel = 0;
    m_yRotateVel = 0;
    m_stopWhenNotFalling = false;
  }

  if (m_forwardVel < -SMALLEST && m_forwardAccel < -SMALLEST)
  {
    // We're going backwards, and our accelerataion will make us go back faster,
    // which is no good, so reset.
    m_prevForwardVel = m_forwardVel;
    m_forwardVel = 0;
    m_forwardAccel = 0;
  }
  else if (m_forwardVel < -SMALLEST && m_forwardAccel > SMALLEST)
  {
    // We are going backwards, but are accelerating forwards - ok.
  }
  else if (m_forwardVel > SMALLEST && m_forwardAccel > SMALLEST)
  {
    // We will accelerate forwards faster and faster - no good.
    // TODO max speed
//    m_forwardVel = 0;
//    m_forwardAccel = 0;
  }
  else if (m_forwardVel > SMALLEST && m_forwardAccel < -SMALLEST)
  {
    // Moving forward, decelerating - ok.
  }

  float units = m_forwardVel * deltaTime;
  Forward(units);

  float degs = m_yRotateVel * deltaTime;
  RotateY(degs);

  // Calculate new height if we're falling.
  RecalcHeight(deltaTime); 
}

bool FreeMovingGameObject::IsIdle()
{
  // TODO forward accel. or rotation accel. ?
  bool idle = (m_forwardVel < 0.01f);

  if (!idle)
  {
    m_idleTime = 0;
  }
  return idle;
}

float FreeMovingGameObject::GetIdleTime()
{
  if (!IsIdle())
  {
    m_idleTime = 0;
  }
  else
  {
    m_idleTime += Engine::Instance()->GetDeltaTime();
  }
  return m_idleTime;
}

bool FreeMovingGameObject::GetDeadStopAfterFall() const
{
  return m_deadStopAfterFall;
}

void FreeMovingGameObject::SetDeadStopAfterFall(bool b)
{
  m_deadStopAfterFall = b;
}

void FreeMovingGameObject::ControlUpdateThis()
{
  if (m_pController.GetPtr())
  {
    m_pController->UpdateObject(this);
  }
}

void FreeMovingGameObject::RefreshHeightServer()
{
  Assert(m_pLevel);

  RecalcCollisionVol();

  // First refresh 'static' heightserver
  m_pHeightPoly = 0; 
  VisibleGameObject::RefreshHeightServer();
  // If the HS changes, reset pointer to last known floor.
  m_heightServerWithPlatforms = m_heightServer;

  // PLATFORMS and other game objects which COLLIDE with characters
  // Refresh the list of Platforms which we include in GetHeightServer().
  // We only include platforms whose bounding sphere intersects this character.
  // We only need to rebuild the sum of static scene HS + platform H.S. if the
  // list of intersecting platforms have changed, or if any of the intersecting
  // platforms have moved.
  // TODO
  // Add the static scene HS to the platforms HS. 
  // TODO Cache this so we only recalculate when necessary.
  // Simple implementation: no caching.
  // Add the HS for all platforms in the same room.
  // TODO speed up by caching the 'sum'/union of Platform HeightServers ?
  
  // Iterate over Visible GameObjects in this room. Find the ones which
  // are platforms. Add them to the height server we return.
  // Game Objects have a pointer to the Level they live in.

  int levelId = m_pLevel->GetId();
  int roomId = m_pLevel->GetRoomId();
  // Get the game objects which are in the same room as this character.
  GameObjectMap& objs = Engine::Instance()->GetGameObjects(levelId, roomId);

  // Iterate through map of Game Objects. 
  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    //GameObjectId gameObjId = it->first;
    PPoolGameObject pGo = it->second;

    if (pGo.GetPtr() == this)
    {
      continue; // don't collide with self
    }

    // All Game Objects which may collide with player should have a
    // CollisionVol. 
    VisibleGameObject* pVis = dynamic_cast<VisibleGameObject*>(pGo.GetPtr());
    if (pVis && pVis->GetState() != OUT_OF_PLAY)
    {
      const HeightServer* pCollisionVol = pVis->GetCollisionVol();
      if (pCollisionVol)
      {
        m_heightServerWithPlatforms.AddHeightServer(*pCollisionVol);
      }
    }
  }
}
}

