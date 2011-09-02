/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: PoolBall.cpp,v $
Revision 1.1.2.24  2007/07/21 10:26:01  Administrator
MSVC warning

Revision 1.1.2.23  2007/07/18 22:13:33  jay
Don't get bonus for potting cue ball

Revision 1.1.2.22  2007/07/15 22:01:05  Administrator
Draw heights - turned off

Revision 1.1.2.21  2007/06/26 09:09:24  jay
Turn off extract debug info

Revision 1.1.2.20  2007/06/24 11:42:46  jay
Fix logic bug for Ball out of bounds

Revision 1.1.2.19  2007/06/21 18:17:44  jay
Don't set ball out of bounds state if we are replaying a shot sent from the
 server. In this case we should wait for a message in the received data.

Revision 1.1.2.18  2007/06/19 08:25:51  jay
Turn off debug output

Revision 1.1.2.17  2007/06/17 20:39:17  jay
Move 'send state' flag from here to GameObject

Revision 1.1.2.16  2007/06/10 21:33:20  Administrator
Check for extra states BALL_IN_POCKET and BALL_OUT_OF_BOUNDS

Revision 1.1.2.15  2007/06/10 08:56:18  jay
Override SetState() - this is where Rules are notified of balls in pocket
 or out of bounds. This should fix consistency bugs where collisions
 are different on the two clients

Revision 1.1.2.14  2007/04/28 11:30:59  Administrator
Turn off draw heights

Revision 1.1.2.13  2007/03/27 09:19:43  jay
Always update, even if forward vel is zero: we must update height. This
 reverts the previous change which broke trick shots

Revision 1.1.2.12  2007/03/22 22:48:43  Administrator
Remove assert: cue ball may be of type PoolBall.
Only draw ball HS in debug mode

Revision 1.1.2.11  2007/03/19 23:23:31  jay
Fix POOL_ONLINE bug - bad #define

Revision 1.1.2.10  2007/03/16 08:50:40  jay
Online changes: each ball state change/collision is sent to the server, and
used in the replay to keep consistency between clients.

Revision 1.1.2.9  2007/03/12 22:53:30  Administrator
Don't update orientation if forward speed is zero

Revision 1.1.2.8  2007/03/12 22:08:44  jay
More ShotLog logging

Revision 1.1.2.7  2007/03/11 20:55:09  Administrator
Pool online debug

Revision 1.1.2.6  2007/03/11 20:32:06  Administrator
Pool online debug

Revision 1.1.2.5  2007/03/10 23:39:04  Administrator
Override CueBall::SetOrientation() for ShotLOg

Revision 1.1.2.4  2007/03/10 22:25:55  jay
Add calls to ShotLog

Revision 1.1.2.3  2006/08/30 21:12:17  Administrator
Get Pool to build in MSVC

Revision 1.1.2.2  2006/08/14 17:50:21  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:36  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.37  2006/04/13 18:20:59  jay
Tidy up

Revision 1.1.2.36  2006/04/13 18:19:19  jay
Fix collision volume

Revision 1.1.2.35  2006/04/11 17:06:44  jay
Don't add Stupid collision volume in RefreshHeightServer, as this causes the
ball to try to Extract itself from the volume - i.e. Stupids push balls around.
Add a collision volume to PoolBall so we can prevent Stupids from walking
through balls.

Revision 1.1.2.34  2005/09/29 19:08:48  jay
Slightly rearrange collision handling code so subclasses can do accuarate
physics.

Revision 1.1.2.33  2005/09/26 20:57:39  jay
Extract: looks like it may have been disabled - it is now enabled, and so
balls which drop into play get extracted.

Revision 1.1.2.32  2005/09/25 21:50:27  Administrator
Intersects(): Use capsule-sphere test, not sphere-sphere test

Revision 1.1.2.31  2005/09/25 20:15:59  Administrator
Fix TQ name for Scene Ed

Revision 1.1.2.30  2005/09/23 19:33:24  jay
Don't play cash reg wav if cue ball potted

Revision 1.1.2.29  2005/09/20 13:16:54  jay
Remove IsFalling() test - apprently does nothing though.

Revision 1.1.2.28  2005/09/16 19:48:51  jay
Replace body of Recalculate() with special version rather than calling
base class version.
This fixes bug where a ball which drops down onto the table goes flying into
the air when hit.

Revision 1.1.2.27  2005/09/13 23:07:23  jay
Make balls more bouncy

Revision 1.1.2.26  2005/09/12 20:43:44  jay
Replaced changes to m_prevOrientation with calls to SetPrevOrientation().
Added some debug output.
This is to fix capsule collision so all collisions are detected, but changes
in FreeMovingGameObject have had the effect of making the balls ricochet
around like crazy.

Revision 1.1.2.25  2005/09/11 19:02:14  jay
Turn off debug output

Revision 1.1.2.24  2005/09/08 20:03:49  jay
Custom RefreshHeightServer to ignore NonHeadArea.. maybe not the best way
of dealing with this :-(
Also simplified HandleObjectCollision for new objects - call is bounced
back to the other object.

Revision 1.1.2.23  2005/09/07 20:56:53  jay
Handle collisions with animals (Stupids)

Revision 1.1.2.22  2005/09/06 20:00:42  jay
Improve ball-ball collision by always using one capsule if there is one.

Revision 1.1.2.21  2005/09/02 18:18:30  jay
Make sure GetExactCollisionCoords returns the first point of contact,
not the 'exit' point.

Revision 1.1.2.20  2005/08/26 21:53:27  jay
Added Draw/Roll

Revision 1.1.2.19  2005/07/30 12:22:23  jay
Use capsule-sphere test instead of sphere-sphere test in Intersects()

Revision 1.1.2.18  2005/07/06 20:18:58  jay
Changes to wavs

Revision 1.1.2.17  2005/06/29 20:22:53  jay
Experiement with new collision response algo. Not much good :-(

Revision 1.1.2.16  2005/06/22 21:42:19  jay
Ball-ball extract: make both balls move away from each other, and don't
wait until ball stops completely.

Revision 1.1.2.15  2005/06/19 21:34:08  jay
GetExact collision coords without affecting ball positions;
Store all balls we intersect so we move away from all of them

Revision 1.1.2.14  2005/06/04 22:36:06  jay
Changes to make GetExactCollision positions more accurate. Now uses
doubles; is accurate I think; errors now may be due to posision of ball
before impact rather than bad exact position.

Revision 1.1.2.13  2005/05/15 17:19:34  jay
Add GetTq(); fix OnBounceOffWall so vel is really reduced.

Revision 1.1.2.12  2005/05/08 09:57:04  jay
Added capsule collision test - but disabled until capsules are accurate.
Added code to get exact positions of colliding balls.

Revision 1.1.2.11  2005/04/21 17:34:34  jay
Override RefreshHeightServer to ignore NonHeadArea.

Revision 1.1.2.10  2005/04/17 21:59:24  jay
Added english/spin

Revision 1.1.2.9  2005/04/17 17:25:38  Administrator
Changes for Scene Editor

Revision 1.1.2.8  2005/04/11 21:23:20  jay
Move away from another ball if we intersect it.

Revision 1.1.2.7  2005/04/03 16:32:55  jay
Add potted balls to player's list

Revision 1.1.2.6  2005/04/01 05:57:50  jay
Now uses a quaternion to show natural roll. This fixes problems with
gimbal lock.

Revision 1.1.2.5  2005/03/20 20:13:27  jay
Ongoing work for Pool game

Revision 1.1.2.4  2005/03/19 22:05:46  Administrator
Scene Ed fixes

Revision 1.1.2.3  2005/03/19 17:22:57  jay
Add Scene Ed code - saving, properties.

Revision 1.1.2.2  2005/03/17 23:12:25  Administrator
Scene Ed needs a default solid

Revision 1.1.2.1  2005/03/17 20:53:39  jay
New classes for Pool game

*/

#if defined(MSVC)
#pragma warning(disable: 4786)
#endif

#include <iostream>
#include "PoolBall.h"
#include "File.h"
#include "Level.h"
#include "LevelServer.h"
#include "BallController.h"
#include "Engine.h"
#include "PoolHole.h"
#include "PoolMisc.h" // GetRules
#include "AngleCompare.h"
#include "Frustum.h"
#include "EngineStatePoolBase.h"
#include "PoolGameState.h"
#include "Geom2d.h"
#include "StringUtils.h"
#include "Capsule.h"
#include "Stupid.h"
#include "PoolBlock.h"
#include "NonHeadArea.h"
#include "ShotLog.h"
#include "StateStrings.h"

namespace Amju
{
using namespace Geom2d;

GameObject* CreatePoolBall()
{
  return new PoolBall;
}

GameObject* CreateCueBall()
{
  return new CueBall;
}

PoolBall::PoolBall()
{
#ifdef SCENE_EDITOR
  m_pSolid = SolidComponent::LoadSolid("ball1.comp");
  m_tqBmp = "ball1.bmp";
  m_tqAlpha = "moon_a.bmp";
  m_tq.Load(m_tqBmp, m_tqAlpha);
#endif

  SetController(new BallController);
  SetDeadStopAfterFall(false);

  m_isSliding = false;
  m_rollVel = 90;
  m_rollAngle = 0;
  m_englishVel = 0;
  m_englishAngle = 0;
  m_englishAccel = 0;
  m_number = 0;
  m_intersectVel = 0;

  m_rollPause = false;
  m_rollTime = 0;
  m_rollDx = 0;
  m_rollDz = 0;
  m_rollApplyVel = 0;

  m_bounceDampen = 0.5f;

}

void PoolBall::SetState(State newState)
{
#ifdef SET_STATE_DEBUG
std::cout << "PoolBall::SetState: current state: "
  << StateStrings[GetState()] << " new state is "
  << StateStrings[newState] << "\n";
#endif

  // Notify Rules if ball is in pocket
  // Notify the Rules that this ball has gone in a hole.
  if (GetState() == UNKNOWN && newState == BALL_IN_POCKET)
  {
#ifdef SET_STATE_DEBUG
std::cout << "PoolBall::SetState: new state is BALL_IN_POCKET\n";
#endif
    FreeMovingSolidGameObject::SetState(newState);

    SetSendState(true);
    GetRules(m_pLevel)->BallInPocket(this);

    // POOL HUD: add the bitmap for this ball to the current player's list.
    if (!IsCueBall(this))
    {
      // Play ball in pocket wav
      static const std::string sfx = Engine::Instance()->GetConfigValue(
        "golf_wav_12");
      SoundFxManager::Instance()->PlayWav(sfx.c_str());

      Engine::Instance()->GetGameState()->GetCurrentPlayerInfo()->
        m_tqs.push_back(m_tq);
    }
  }

  // Notify Rules if ball is off table
  else if (GetState() == UNKNOWN && newState == BALL_OUT_OF_BOUNDS)
  {
#ifdef SET_STATE_DEBUG
std::cout << "PoolBall::SetState: new state is BALL_OUT_OF_BOUNDS\n";
#endif
    FreeMovingSolidGameObject::SetState(newState);

    SetSendState(true);
    GetRules(m_pLevel)->BallOutOfBounds(this);
  }
  else
  {
#ifdef SET_STATE_DEBUG
std::cout << "PoolBall::SetState: new state is " 
  << StateStrings[newState] << "\n";
#endif
    FreeMovingSolidGameObject::SetState(newState);
  }
}

void PoolBall::SetOrientation(const Orientation& o) 
{
#ifdef OR_DEBUG
std::cout << "**Setting ball OR " << GetNumber() << "\n";
#endif

  Assert(!dynamic_cast<CueBall*>(this));
//  Assert(!IsCueBall(this));

  m_x = o.GetX();
  m_z = o.GetZ();
  FreeMovingSolidGameObject::SetOrientation(o);
}

const Orientation* PoolBall::GetOrientation()
{
  return FreeMovingSolidGameObject::GetOrientation();
}

const char* PoolBall::GetTypeName() const
{
  return "pool_ball";
}

TexturedQuad* PoolBall::GetTq()
{
  return &m_tq;
}

int PoolBall::GetNumber() const
{
  return m_number;
}

void PoolBall::SetEnglishVel(float degsPerSec)
{
  m_englishVel = degsPerSec;
}

void PoolBall::SetDrawRollVel(float degsPerSec)
{
  m_rollVel = degsPerSec;
  m_rollPause = false;
}

float PoolBall::GetDrawRollVel() const
{
  return m_rollVel;
}

void PoolBall::OnBounceOnGround()
{
#ifdef GROUND_DEBUG
std::cout << "Pool ball bounce on ground\n";
#endif

  static const std::string sfx = Engine::Instance()->GetConfigValue("golf_wav_11");
  SoundFxManager::Instance()->PlayWav(sfx.c_str());
}

void PoolBall::OnBounceOffWall()
{
#ifdef BALL_DEBUG
std::cout << "POOL BALL: ON BOUNCE OFF WALL\n";
#endif

  // WAV
  static const std::string sfx = Engine::Instance()->GetConfigValue("golf_wav_11");
  SoundFxManager::Instance()->PlayWav(sfx.c_str());

  static const float wallBounceDamp = 
    Engine::Instance()->GetConfigFloat("pool_wallbouncedamp");  

  //float fv = GetForwardVel();
  //float newfv = fv * wallBounceDamp;
  
#ifdef BALL_DEBUG
std::cout << " Damp: " << wallBounceDamp << " old: " << fv << " new: " << newfv << "\n";
#endif
  //SetForwardVel(GetForwardVel() * wallBounceDamp);
  m_forwardVel *= wallBounceDamp;
  // TODO What about previous version ?

#ifdef POOL_ONLINE
  SetSendState(true);
#endif
}

// Handle ball-ball collision 
// Either ball may be moving.
// Let's assume the balls have been reset to the exact 
// positions at the time of the collision (i.e. no intersection,
// but touching).
void PoolBall::HandleBallCollision(PoolBall* a, PoolBall* b)
{
#ifdef POOL_ONLINE
  std::string s = "Pool balls collide: ID 1: ";
  s += ToString(a->GetId());
  s += " ID 2: ";
  s += ToString(b->GetId());
  ShotLog(s);

  // Flag both balls - they should send their states this frame
  a->SetSendState(true);
  b->SetSendState(true);
#endif

  float aspeed = a->GetForwardVel();
  float bspeed = b->GetForwardVel();

  Assert(aspeed >= 0);
  Assert(bspeed >= 0);
  float totalSpeedBefore = aspeed + bspeed;

  if (aspeed == 0 && bspeed == 0)
  {
    return;
  }

  // Get ball centres
  float xa = a->GetOrientation()->GetX();
  float za = a->GetOrientation()->GetZ();
  float xb = b->GetOrientation()->GetX();
  float zb = b->GetOrientation()->GetZ();
  // This gives us the "centre line" vector along which both balls 
  // experience an impulsive force.
  float dx = xa - xb;
  float dz = za - zb;

  // Get the forward vector of each ball.
  float fxa, fza;
  a->GetForwardVector(&fxa, &fza);
  float fxb, fzb;
  b->GetForwardVector(&fxb, &fzb);
  // These vectors are normalised. Let's get their lengths proportional
  // to the balls' speeds.

  // Normalise dx and dz
  float centrelensq = dx * dx + dz * dz;
  float centrelen = sqrt(centrelensq);
  dx /= centrelen;
  dz /= centrelen;

#ifdef POOL_DEBUG
std::cout << "  Ball A vector: " << fxa << ", " << fza << " Len: " << aspeed << "\n";
std::cout << "  Ball B vector: " << fxb << ", " << fzb << " Len: " << bspeed << "\n";
#endif

  // For each of these two forward vectors, 
  // get the component of the vector which lies parallel to the centre line
  // we found above. The sum of these two components (one for each ball) act 
  // on the balls, along this centre line. The force on each ball is the same
  // (equal and opposite reaction).

  // Get the component of a if we project a onto the centre line.
  float compa = aspeed * DotProduct(Vec2d(fxa, fza), Vec2d(dx, dz));

  // Same for ball b
  float compb = bspeed * DotProduct(Vec2d(fxb, fzb), Vec2d(dx, dz));

#ifdef POOL_DEBUG
std::cout << "  Ball A centre-line-component len: " << compa << "\n";
std::cout << "  Ball B centre-line-component len: " << compb << "\n";
#endif

  // The total acting on both balls is the sum of these lengths.
  float total = fabs(compa) + fabs(compb);

#ifdef POOL_DEBUG
std::cout << "  Total vector along centre line: " << dx << ", " << dz << "\n";
#endif

  // The result vector for each ball is the vector sum of its original vector,
  // and the impulse vector along the centre line.
  // (This ignores all spin and friction.)
  float resxa = fxa * aspeed + dx * total;
  float resza = fza * aspeed + dz * total;

#ifdef POOL_DEBUG
std::cout << "  Result vector for ball A: " << resxa << ", " << resza << "\n";
#endif

  float resxb = fxb * bspeed - dx * total;
  float reszb = fzb * bspeed - dz * total;

#ifdef POOL_DEBUG
std::cout << "  Result vector for ball B: " << resxb << ", " << reszb << "\n";
#endif

  // Normalise these to get forward vector and new forward speed.
  float resalensq = resxa * resxa + resza * resza;
  float resalen = sqrt(resalensq); // new forward speed for ball a
  resxa /= resalen; // new forward vector x for ball a
  resza /= resalen; // new forward vector z for ball a
  
  float resblensq = resxb * resxb + reszb * reszb;
  float resblen = sqrt(resblensq); // new forward speed for ball b
  resxb /= resblen; // new forward vector x for ball b
  reszb /= resblen; // new forward vector z for ball b

  // TODO HACK
  // Make sure total speed after == total speed before.
  // I would think that if the physics was correct this hack would not
  // be necessary :-(
  float totalSpeedAfter = resalen + resblen;
  resalen *= (totalSpeedBefore / totalSpeedAfter);
  resblen *= (totalSpeedBefore / totalSpeedAfter);
  totalSpeedAfter = resalen + resblen; // recalculate it
  float spdiff = totalSpeedAfter - totalSpeedBefore;

  a->SetForwardVel(resalen);
  a->SetForwardVector(resxa, resza);
  b->SetForwardVel(resblen);
  b->SetForwardVector(resxb, reszb);

#ifdef SPEED_DEBUG
std::cout << "Total speed before: " << totalSpeedBefore << "\n";
std::cout << "Total speed after:  " << totalSpeedAfter << "\n";
std::cout << "Diff: " << spdiff << "\n";
#endif

  Assert(fabs(spdiff) < 0.0001f); // conservation + rounding error
 
  // TODO Maybe update a second set of member vars for new position and speed,
  // so collisions of >2 balls works out ok ?
  // For now we directly update the ball pos and speed.
  
  // Is ball b sliding or rolling ? If sliding with Draw, it will come
  // back after the collision.
  a->ApplyDrawRoll(fxa, fza, resalen);
  b->ApplyDrawRoll(fxb, fzb, resblen);
}

//#define EXACT_DEBUG
struct VertexD
{
  double x, y, z;
  VertexD(double x1 = 0, double y1 = 0, double z1 = 0) : x(x1), y(y1), z(z1) {}
  VertexD operator-(const VertexD& rhs)
  {
    VertexD r;
    r.x = x - rhs.x;
    r.y = y - rhs.y;
    r.z = z - rhs.z;
    return r;
  }
  double Length() const
  {
    double ds = x * x + y * y + z * z;
    double d = sqrt(ds);
    return d;
  }
  bool operator==(const VertexD& rhs) const
  {
    return (x == rhs.x && y == rhs.y && z == rhs.z);
  }
};

VertexD GetNewPos(const VertexD& v0, double dx, double dz, 
  double accel, double vel, double t)
{
  // Get new position of ball at time t.
  // Use standard equn s = u . t + 1/2 . a . t^2

  VertexD v(v0);
  double s = vel * t + 0.5 * accel * t * t; 
  v.x += dx * s;
  v.z += dz * s;
  return v;
}

void PoolBall::SetPositionAtTime(float t)
{
  VertexBase v10f = m_prevOrientation.GetVertex();
  VertexD v10(v10f.x, v10f.y, v10f.z);

  float dx1f = 0, dz1f = 0; // forward vec of ball 1

  // TODO Surely should be previous fwd vec ??
  GetForwardVector(&dx1f, &dz1f);
  double dx1 = dx1f, dz1 = dz1f; // forward vec of ball 1

  const double accel1 = GetForwardAccel();

  // TODO Surely should be previous vel ?
  const double vel1 = GetForwardVel();

  VertexD v1 = GetNewPos(v10, dx1, dz1, accel1, vel1, t); 

  Orientation o = *(GetOrientation());
  o.SetVertex(VertexBase(v1.x, v1.y, v1.z));

  // Update previous position data
  //m_prevOrientation = m_orientation;
  // TODO POOL HACK
// WILD 1: no good - try to fix wildly richocheting balls
  SetPrevOrientation(m_orientation);

  // TODO TEMP TEST this looked like it would blat over m_prevOrientation,
  // so moved it to after the above line.
  SetOrientation(o); 
#ifdef POOL_ONLINE
        ShotLog("place cue ball 11");
#endif
}

// Set positions of p1 and p2 so they are exactly touching. 
// Initially we know that the capsules of p1 and p2 intersected.
// This means the balls may have collided, although it's possible
// they did not.
void PoolBall::GetExactCollisionCoords(PoolBall* p1, PoolBall* p2)
{
  float t = 0;
  VertexBase v1, v2;
  GetExactCollisionCoords(p1, p2, &t, &v1, &v2);
  
  Orientation o1 = *(p1->GetOrientation());
  o1.SetVertex(v1);
  p1->SetOrientation(o1);
#ifdef POOL_ONLINE
        ShotLog("place cue ball 12");
#endif

  Orientation o2 = *(p2->GetOrientation());
  o2.SetVertex(v2);
  p2->SetOrientation(o2);
#ifdef POOL_ONLINE
        ShotLog("place cue ball 13");
#endif

}

// Get the max time (0..1) we should consider. This is the time it takes
// for the moving ball to hit the stationary one. If we consider a longer time
// we may find the point where the moving ball 'exits' the stationary one.
float PoolBall::GetTmaxLimit(const PoolBall* pMoving, const PoolBall* pStationary) 
{
  // Ignore delceleration - vel is constant over one frame.
  // Get the distance from the moving ball initial position to the end
  // position, and from the initial position to the stationary ball.

  Assert(pStationary->GetForwardVel() == 0);

  const VertexBase& v0 = pMoving->m_prevOrientation.GetVertex();
  const VertexBase& v1 = pMoving->m_orientation.GetVertex();
  const VertexBase& v2 = pStationary->m_orientation.GetVertex();

  float d1 = (v1 - v0).Length();
  if (d1 < 0.00001f)
  {
    return 1.0f;
  }
  float d2 = (v2 - v0).Length();
  float r = d2 / d1;

  Assert(r >= 0);

  // r can routinely be > 1.0 if the moving ball travels a short distance.
  // This is ok; we are interested in values < 1.0, where there may be
  // two possible exact collision points.
  if (r > 1.0f)
  {
    r = 1.0f;
  }

  return r;  
}

void PoolBall::GetExactCollisionCoords(
    const PoolBall* p1, 
    const PoolBall* p2,
    float* pCollisionTime,
    VertexBase* pResultV1, 
    VertexBase* pResultV2)
{
  // TODO Reset forward vector too ? Maybe it changed if ball
  // hit a wall ?

  double tmax = Engine::Instance()->GetDeltaTime();
  // Now that we use capsule-collision testing, there may be two positions 
  // where the balls touch - on 'entry' and 'exit'.
  // Limit tmax so the 'exit' position can't be considered - it happens later.

  float dx1f = 0, dz1f = 0; // forward vec of ball 1
  p1->GetForwardVector(&dx1f, &dz1f);
  double dx1 = dx1f, dz1 = dz1f; // forward vec of ball 1

  float dx2f = 0, dz2f = 0; // forward vec of ball 2
  p2->GetForwardVector(&dx2f, &dz2f);
  double dx2 = dx2f, dz2 = dz2f; // forward vec of ball 2

  const double accel1 = p1->GetForwardAccel();
  const double accel2 = p2->GetForwardAccel();
  const double vel1 = p1->GetForwardVel();
  const double vel2 = p2->GetForwardVel();

  if (vel1 == 0)
  {
    // Ball 1 not moving. Limit tmax to the time taken for ball2 to get within
    // distance 2R.
    tmax *= GetTmaxLimit(p2, p1);
  }
  else if (vel2 == 0)
  {
    // Ball 2 not moving, so limit tmax to the time taken for ball 1 to get
    // within dist 2R of ball 2.
    tmax *= GetTmaxLimit(p1, p2);
  }

#ifdef EXACT_DEBUG_1
if (p1->m_orientation == p1->m_prevOrientation)
{
  std::cout << "EXACT: Ball " << p1->GetId() << " old and new Ors are the same :-(\n";
}
if (p2->m_orientation == p2->m_prevOrientation)
{
  std::cout << "EXACT: Ball " << p2->GetId() << " old and new Ors are the same :-(\n";
}
#endif

  VertexBase v10f = p1->m_prevOrientation.GetVertex();
  VertexBase v20f = p2->m_prevOrientation.GetVertex(); 
  VertexD v10(v10f.x, v10f.y, v10f.z);
  VertexD v20(v20f.x, v20f.y, v20f.z);

  static double TWO_R = 0;
  if (TWO_R == 0)
  {
    // Get the radius of a ball - they are all the same, so use the 
    // cue ball.
    double r = GetBall()->GetBoundingSphere()->GetRadius();
    TWO_R = 2.0 * r;
  }
#ifdef EXACT_DEBUG
std::cout << "TWO_R: " << TWO_R << "\n";
#endif

  // If the balls already intersected before moving, we can't 
  // work out the collision point - we have lost the info about 
  // what happened before.
  // This can happen if balls intersect in their starting positions.
  double t = 0;
#ifdef EXACT_DEBUG
  {
    VertexD v1 = GetNewPos(v10, dx1, dz1, accel1, vel1, t); 
    Assert(v1 == v10);
    VertexD v2 = GetNewPos(v20, dx2, dz2, accel2, vel2, t); 
    Assert(v2 == v20);
  }
#endif

  double dist = (v10 - v20).Length();
  if (dist < TWO_R)
  {
#ifdef EXACT_DEBUG_1
std::cout << "EXACT: already intersecting: " << p1->GetId() << " and " << p2->GetId() << "\n";
#endif
    *pCollisionTime = 0;
    *pResultV1 = v10f;
    *pResultV2 = v20f;
    return;
  }

#ifdef EXACT_DEBUG_1
std::cout << "EXACT: **** Finding exact collision point for " << p1->GetId() << " and " << p2->GetId() << "\n";
#endif

  // Work out the positions of both the balls at different times.
  // The idea is to zero in on the time when the balls were exactly
  // 2R away from each other - i.e. touching.

  // Find time t where dist(t) > 2R and dist(t+dt) < 2R.
  // Then halve dt and repeat.
  // Initially dt is half of tmax.
  double dt = tmax * 0.5;

  bool broke = false;
  int count = 0;
  while (count++ < 20) // TODO TEMP TEST  
  {
    // Get new positions at time t
    VertexD v1 = GetNewPos(v10, dx1, dz1, accel1, vel1, t); 
    VertexD v2 = GetNewPos(v20, dx2, dz2, accel2, vel2, t); 
    double dist = (v1 - v2).Length();

#ifdef EXACT_DEBUG
std::cout << "EXACT: dist: " << dist << "     ";
#endif

    // Is dist < or > 2R ?
    // If > 2R, increase dt. If <2R, decrease ?
    if (dist > TWO_R)
    {
      t += dt;
    }
    else if (dist < TWO_R)
    {
      Assert(t >= dt && "Balls were intersecting already ?!");
      // At this t, the balls are intersecting. We want to find an earlier t.
      t -= dt;
    }
    else
    {
      broke = true;
#ifdef EXACT_DEBUG
std::cout << "EXACT: breaking after " << count << " loops, dist=" << dist << "\n";
#endif
      break;
    } 
    dt *= 0.5f;
  }

#ifdef EXACT_DEBUG
  if (!broke)
  {
    std::cout << "EXACT: Didn't break, count=" << count << "\n";
  }
#endif

  // So at time t the balls collide ..?
  // Set the ball positions ready for the collision response code.
#ifdef EXACT_DEBUG   //EXACT_DEBUG_1
double badDist = (p1->m_orientation.GetVertex() - p2->m_orientation.GetVertex()).Length();
std::cout << "EXACT: Bad Distance (when collision detected): " << badDist << "\n";
std::cout << "EXACT: t = " << t << " tmax = " << tmax << "\n";
Assert(t <= tmax);
{
  VertexD v1 = GetNewPos(v10, dx1, dz1, accel1, vel1, t); 
  VertexD v2 = GetNewPos(v20, dx2, dz2, accel2, vel2, t); 
  double dist = (v1 - v2).Length();
  std::cout << "EXACT: good dist: " << dist << "\n";

}
#endif

  *pCollisionTime = t;
  
  VertexD v1 = GetNewPos(v10, dx1, dz1, accel1, vel1, t); 
  *pResultV1 = VertexBase(v1.x, v1.y, v1.z); 
  VertexD v2 = GetNewPos(v20, dx2, dz2, accel2, vel2, t); 
  *pResultV2 = VertexBase(v2.x, v2.y, v2.z);
}

void PoolBall::HandleBallCollision(PoolBall* b)
{
    if (IsCueBall(this))
    {
      GetRules(m_pLevel)->CueBallHitsBall(b);
    }

    // Remember the last ball we intersect. If we have stopped but are still
    // intersecting it later we will move away from it.
    m_intersectBalls.push_back(b);
    // Also we want the other ball to move away from this one. 
    b->m_intersectBalls.push_back(this);

    // If one or both balls are moving, do our usual collision response.
    // If both balls have stopped, we have bad intersecting balls :-(
    if (GetForwardVel() > 0 || b->GetForwardVel() > 0)
    {
      // Get the exact coords of the balls at the collision point.
      GetExactCollisionCoords(this, b);

#ifdef EXACT_DEBUG
static const int decplaces = 5;
// Compare exact positions with ghost ball position calculated by AI
std::cout << "COLLISION: Exact pos 1: " << ToString(this->GetBoundingSphere()->GetCentre(), decplaces).c_str() << " exact pos 2: " << ToString(b->GetBoundingSphere()->GetCentre(), decplaces).c_str() << "\n";
#endif

      HandleBallCollision(this, b);

      // Play ball collision WAV
      // TODO Need a decent wav
      static const std::string sfx = Engine::Instance()->GetConfigValue("golf_wav_10");
      SoundFxManager::Instance()->PlayWav(sfx.c_str());

      // Both balls will roll after collision - no sliding.
      m_isSliding = false;
      b->m_isSliding = false;

      // All balls should share deceleration value
      // TODO should set this just once
      static const double acc = Engine::Instance()->GetConfigFloat("golf_ball_accel"); 
      Assert(acc != 0);
      SetForwardAccel(acc);
      b->SetForwardAccel(acc);
 
      // Set intersect vel positive, so we can extract ourselves from intersecting
      // balls even if our vel decelerates to zero.
      static const double INTERSECT_VEL = 
        Engine::Instance()->GetConfigFloat("pool_intersect_vel");
      m_intersectVel = INTERSECT_VEL;
    }
}

void PoolBall::HandleObjectCollision(GameObject* pObj)
{
  // Don't do anything if ball is out of play, right ?
  if (!InPlay(this))
  {
    return;
  }

  if (IsPoolBall(pObj))
  {
    PoolBall* b = dynamic_cast<PoolBall*>(pObj);

#ifdef COLL_DEBUG
    std::cout << "** COLLISION between 2 balls: " << this->GetId() 
       << " (" << this->m_number << ") and " << b->GetId() << " (" << b->m_number << ")\n";
#endif
    HandleBallCollision(b);

    return;
  }
 
  PoolHole* h = dynamic_cast<PoolHole*>(pObj);
  if (h)
  {
    // TODO Ideally this would be in SetState(), but we don't know the
    // pocket then. We would need a different message for each pocket..?     
    // Award any bonuses for this pocket.
    if (!IsCueBall(this)) // Don't get a bonus for pocketing the cue ball!
    {
      h->PlayerGetsBonuses();
    }

#ifdef POOL_ONLINE
    // Don't check if we are getting all state changes from the other client
    //  that took the shot.
    if (IsOnlineGame() && !IsUserControlled())
    {
#ifdef _DEBUG
std::cout << "Local client: ball intersects pocket, but do nothing until we get state change from other client.\n";
#endif
      return;
    }
#endif

    Assert(GetState() == UNKNOWN);
    SetState(BALL_IN_POCKET);

    return;
  }

  // Collisions with other types are handled by those types.
  pObj->HandleObjectCollision(this);
}

bool PoolBall::Intersects(const GameObject& g)
{
  static const float radius = GetBoundingSphere()->GetRadius();

  const GameObject* pGo = &g;
  const PoolBall* pB = dynamic_cast<const PoolBall*>(pGo);
  if (!pB)
  {
    // Other object is not a pool ball - use 
	// Capsule-sphere test so we don't miss anything
    Capsule c1(m_prevOrientation.GetVertex(), m_orientation.GetVertex(), radius);
    bool b = c1.Intersects(*(pGo->GetBoundingSphere()));
    return b;
  }
 
  // Get the capsules made by the previous and current orientations.
  // If the capsules intersect, then the balls intersected - even
  // though their spheres may not intersect now.
  Capsule c1(m_prevOrientation.GetVertex(), m_orientation.GetVertex(), radius);
  Capsule c2(pB->m_prevOrientation.GetVertex(), pB->m_orientation.GetVertex(), radius);

#ifdef SPHERE_CHECK_DEBUG
if (c1.IsSphere() && c2.IsSphere())
{
  std::cout << "CAPSULES for ball " << GetNumber() 
    << " and " << pB->GetNumber() << " are both SPHERES!!\n";
}
#endif

  // To try to simplify things, test the capsule of this ball with the 
  // static sphere of pB.
  // The idea is that capsule-capsule testing brings too much complexity,
  // but we want one capsule so we don't miss collisions.
  bool b = false;

/*
  // Too complex if both are capsules - did the balls actually collide ?!
  bool b = c1.Intersects(c2);
*/
  if (!c1.IsSphere())
  {
    // Capsule-sphere test, not capsule-capsule
    b = c1.Intersects(*(pB->GetBoundingSphere()));
  }
  else
  {
    // c1 is a sphere - hopefully c2 is not.
    b = c2.Intersects(*(GetBoundingSphere()));
  }

  return b; 
}

void PoolBall::CullDraw(Frustum* pFrustum)
{
  if (pFrustum->Contains(*(GetBoundingSphere())))
  {
    Draw();
  }
  else if (GetForwardVel() > 0)
  {
    // TODO specify which ball is not in the frustum, so camera can be a bit
    // more selective :-)
    EngineStatePoolBase::SetMovingBallNotInFrustum();
  }
}

//#define POOL_TABLE_SHOW_HEIGHTS_DEBUG

void PoolBall::Draw()
{
  AmjuGL::PushMatrix();
  Orientation o = *(GetOrientation());
  o.SetXRot(0);
  o.SetYRot(0);
  o.SetZRot(0);
  o.Draw(); // glTranslate
 
  float quatMatrix[16];
  m_quat.CreateMatrix(quatMatrix);
  AmjuGL::MultMatrix(quatMatrix);

  m_pSolid->Draw();
  AmjuGL::PopMatrix();

#ifdef POOL_TABLE_SHOW_HEIGHTS_DEBUG
  Engine::Instance()->PushColour(0, 0, 0, 1.0f);
  GetHeightServer()->DrawHeights();
  GetHeightServer()->DrawWalls();
  Engine::Instance()->PopColour();
#endif
}

void PoolBall::Recalculate()
{
  float deltaTime = Engine::Instance()->GetDeltaTime();

  // If ball drops down into play we want to react even if Falling.
  //if (!IsFalling())
  //{
    m_prevForwardVel = m_forwardVel;
    m_forwardVel += m_forwardAccel * deltaTime;
  //}

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

  double units = (double)m_forwardVel * (double)deltaTime;

// Start of Forward()
  //Forward(units);
  const float y = m_orientation.GetY();

  // Always update, even if units == 0, so we set height correctly.
    m_x += units * m_vectorX;
    m_z += units * m_vectorZ;

    //m_prevOrientation = m_orientation;
    SetPrevOrientation(m_orientation);
    m_orientation.SetX(m_x);
    m_orientation.SetZ(m_z);

    // Get the new Y-value if it exists on the current HeightPoly.
    if (m_pHeightPoly)
    {
      float newY = m_pHeightPoly->GetY(m_x, m_z);
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
      m_pHeightPoly = GetHeightServer()->
        GetHeightPoly(m_x, y + 0.9f, y - 0.9f, m_z);
    }
    // End of Forward()

  float degs = (double)m_yRotateVel * (double)deltaTime;
  RotateY(degs);

  // Calculate new height if we're falling.
  RecalcHeight(deltaTime);
}

void PoolBall::ApplyDrawRoll(float dx, float dz, float vel)
{
  if (m_rollVel == 0)
  {
    return;
  }

  // ???
  if (m_rollPause)
  {
    // Another collision while waiting - don't do draw/roll
    m_rollApplyVel = 0;
  }

  // Wait for a shot delay (< 1s) then apply the given speed+direction
  // to this ball's forward vector.
  m_rollPause = true;
  m_rollTime = 0;
  Assert(vel >= 0);
  // If roll vel is negative, this is backspin, so the effect on the
  // ball vector should be to move backwards.
  if (m_rollVel < 0)
  {
    dx = -dx;
    dz = -dz; 
  }

  // Store the vel change - we will add them to the ball vector when 
  // m_rollTime reaches some limit.
  m_rollDx = dx;
  m_rollDz = dz;
  // The backspin or topspin vel is proportional to the original vel ?
  // Or is it just proportional to the angular vel of the ball ? 
  static const float VEL_EFFECT = Engine::Instance()->GetConfigFloat(
    "pool_spin_vel_effect");
  static const float SPIN_EFFECT = Engine::Instance()->GetConfigFloat(
    "pool_spin_spin_effect");
  m_rollApplyVel = vel * VEL_EFFECT + fabs(m_rollVel) * SPIN_EFFECT;
  
  // Draw/roll has only one effect per shot; after the first collision
  // there is no top/back spin on the ball.
  m_rollVel = 0; 
}

void PoolBall::Update()
{
  // TODO : we could handle ball-ball-extract differently, by adding
  // the extract vector to the current ball dir. Then we let the
  // HandleWallCollision function take care of wall collisions, and
  // balls should never get stuck in walls.

  // CRAZY POOL: If ball is 'dead' don't update it.
  // This means it's being carried around by an animal.
  if (GetState() == DEAD)
  {
    return;
  }

  FreeMovingSolidGameObject::Update();

  float dt = Engine::Instance()->GetDeltaTime();

  // Move away from intersect ball using intersect vel. This is to try to 
  // extract from intersecting balls if regular vel has decelerated to 
  // close to zero.
  //static const float EXTRACT_VEL = Engine::Instance()->GetConfigFloat(
  //  "pool_extract_vel");

  if (!m_intersectBalls.empty())
  {
#ifdef EXTRACT_DEBUG
std::cout << "Ball " << GetId() 
  << " extracting from " << m_intersectBalls.size() << " balls.\n";
#endif

    float units = m_intersectVel * dt;
    // Move away from all intersecting balls 
    float dx = 0;
    float dz = 0;
    for (unsigned int i = 0; i < m_intersectBalls.size(); i++)
    {
      float x1 = m_intersectBalls[i]->GetBoundingSphere()->GetCentre().x; 
      float x2 = this->GetBoundingSphere()->GetCentre().x;
      dx += (x1 - x2);
      float z1 =  m_intersectBalls[i]->GetBoundingSphere()->GetCentre().z; 
      float z2 = this->GetBoundingSphere()->GetCentre().z; 
      dz += (z1 - z2);
    }

#ifdef ADD_VECTORS
    // Mult dx and dz to get faster extraction
    // TODO
    dx *= EXTRACT_VEL;
    dz *= EXTRACT_VEL;

    VertexBase newFwdVec(m_vectorX * m_forwardVel, 0, m_vectorZ * m_forwardVel);
    newFwdVec += VertexBase(dx, 0, dz);
    // Reverse of RecalcForwardVector
    //SetForwardVel(newFwdVec.Length());
    m_forwardVel = newFwdVec.Length();
    newFwdVec.Normalize();
    SetForwardVector(newFwdVec.x, newFwdVec.z); 
#else
    // Bad old way - directly move balls away from each other.
    float len = sqrt(dx * dx + dz * dz);
#ifdef EXTRACT_DEBUG
std::cout << "  Extract: distance: " << len 
    << " units: " << units 
    << " m_intVel: " << m_intersectVel 
    << " dt: " << dt << "\n";
#endif

    if (len != 0)
    {
      dx /= len;
      dz /= len;

      Orientation o = *(GetOrientation());
      float x = o.GetX();
      float z = o.GetZ();
      x -= units * dx;
      z -= units * dz;

      // Store old orientation
      //m_prevOrientation = m_orientation;
      //SetPrevOrientation(m_orientation);
#ifdef EXTRACT_DEBUG
std::cout << "  Extract: old (x, z): " << o.GetX() << ", " << o.GetZ() << "\n";
std::cout << "  Extract: new (x, z): " << x << ", " << z << "\n";
#endif

      o.SetX(x); 
      o.SetZ(z);
      SetOrientation(o);
#ifdef POOL_ONLINE
        ShotLog("place cue ball 14");
#endif

    }
#endif

    // Reset intersect balls, this will be set again in 
    // HandleObjectCollision if we still intersect it.
    m_intersectBalls.clear();
  }

  // Extract from any wall we are stuck in
  m_pController->ExtractCharacterFromWall(this);

  // Check if off table - if so notify Rules.
  if (GetOrientation()->GetY() < -2.0f && 
      GetState() == UNKNOWN) 
  {
    // Don't set this state if we are replaying shot from other client --
    // wait to get the message from server.
    if (!IsOnlineGame())
    {
std::cout << "Ball out of bounds... Not online game so setting state.\n";
      SetState(BALL_OUT_OF_BOUNDS);
    }
    else if (IsOnlineGame() && IsUserControlled())
    {
std::cout << "Ball out of bounds... online and user played shot so setting state.\n";
      SetState(BALL_OUT_OF_BOUNDS);
    }
    else
    {
      Assert(IsOnlineGame());
      Assert(!IsUserControlled());
std::cout << "Ball out of bounds... online replay so NOT setting state.\n";
    }
    return;
  }

  // Update Roll
  // Roll vel: natural roll
  static float r = GetBoundingSphere()->GetRadius();
  // angular vel is forward vel / radius.
  float naturalRollVel = RadToDeg(GetForwardVel() / r);
  float yr = atan2(m_vectorX, m_vectorZ); 

  static const float NINETY_DEGREES = DegToRad(90.0f);
  yr += NINETY_DEGREES; 

  float x = sin(yr);
  float z = cos(yr);

  Quaternion q;
  float a = (m_rollVel + naturalRollVel) * dt;
  q.CreateFromAxisAngle(x, 0, z, a);
  m_quat = q * m_quat; 

  // Update the forward/draw roll of the ball.
  // Decelerate draw/roll vel
  static const float drawRollAccel = Engine::Instance()->GetConfigFloat("pool_dr_accel");
  Assert(drawRollAccel < 0);
  if (m_rollVel > 0)
  {
    m_rollVel += drawRollAccel * dt;
    if (m_rollVel < 0)
    {
      m_rollVel = 0;
    }
  }
  else if (m_rollVel < 0)
  {
    m_rollVel -= drawRollAccel * dt;
    if (m_rollVel > 0)
    {
      m_rollVel = 0;   
    }
  }

  // Are we Paused waiting to apply draw/roll ?
  if (m_rollPause)
  {
    m_rollTime += dt;
    static const float MAX_ROLL_PAUSE = Engine::Instance()->GetConfigFloat(
     "pool_max_roll_pause");
    if (m_rollTime > MAX_ROLL_PAUSE)
    {
      // It's time to apply the change in vel due to top/back spin.
      m_rollPause = false;

      // Need to do vector addition of current vector + draw/roll vector
      float dx = m_vectorX * m_forwardVel;
      float dz = m_vectorZ * m_forwardVel;
      dx += m_rollDx * m_rollApplyVel;
      dz += m_rollDz * m_rollApplyVel;

      float len = sqrt(dx * dx + dz * dz);
      if (len < 0.00001f)
      {
        m_forwardVel = 0;
      } 
      else
      {
        m_forwardVel = len;
        m_vectorX = dx / len;
        m_vectorZ = dz / len;
      }
      // All balls should share deceleration value
      // TODO should set this just once
      static const double acc = Engine::Instance()->GetConfigFloat(
        "golf_ball_accel"); 
      Assert(acc != 0);
      SetForwardAccel(acc);
    }
  }

  // English
  static const float englishAccel = Engine::Instance()->GetConfigFloat("pool_eng_accel");
  // Decelerate english
  Assert(englishAccel < 0);
  if (m_englishVel > 0)
  {
    m_englishVel += englishAccel * dt;
    if (m_englishVel < 0)
    {
      m_englishVel = 0;
    }
  }
  else if (m_englishVel < 0)
  {
    m_englishVel -= englishAccel * dt;
    if (m_englishVel > 0)
    {
      m_englishVel = 0;
    }
  }
  // Adjust turn vel of ball depending on english.
  static const float ENGLISH_TURN = Engine::Instance()->GetConfigFloat("pool_eng_turn");
  float yrv = m_englishVel * ENGLISH_TURN;
  SetYRotateVel(yrv);
  Quaternion qEnglish;
  qEnglish.CreateFromAxisAngle(0, 1, 0, m_englishVel * dt);
  m_quat = qEnglish * m_quat;
}

bool PoolBall::Load(File* pf)
{
  std::string solidName;
  if (!pf->GetDataLine(&solidName))
  {
    pf->ReportError("Expected pool ball solid name.");
    return false;
  }
  PSolidComponent pComp = SolidComponent::LoadSolid(solidName);
  if (!pComp.GetPtr())
  {
    pf->ReportError("Bad solid for pool ball.");
    return false;
  }
  m_pSolid = pComp;
  // Get start position - store it so we can reset when balls are
  // re-racked.
  if (!m_startOr.Load(pf))
  {
    pf->ReportError("Failed to load pool ball orientation.");
    return false;
  }
  // This doesn't set balls to their correct positions in scene ed.
//  m_pSolid->SetOrientation(m_startOr);
  // This is what we need to do for scene ed.
  SetOrientation(m_startOr);

#ifdef POOL_ONLINE
        ShotLog("place cue ball 15");
#endif

  // Get ball number
  if (!pf->GetInteger(&m_number))
  {
    pf->ReportError("Expected pool ball number.");
    return false;
  }

  // Get bitmap for 2D image of this ball. This is used to show the HUD,
  // scores, etc.
  std::string tq, tqAlpha;
  if (!pf->GetDataLine(&tq))
  {
    pf->ReportError("Expected ball TQ bmp name");
    return false;
  }
  int tqFlag = 0;
  if (!pf->GetInteger(&tqFlag))
  {
    pf->ReportError("Expected ball TQ flag");
    return false;
  }
  if (!pf->GetDataLine(&tqAlpha))
  {
    pf->ReportError("Expected ball TQ alpha name");
    return false;
  } 
 
  if (!m_tq.Load(tq, tqAlpha))
  {
    pf->ReportError("Failed to load Quad for pool ball.");
    return false;
  }
#ifdef SCENE_EDITOR
  m_tqBmp = tq;
#endif

  // Load a collision volume. This is not used for ball-ball collisions,
  // it is only needed for crazy animal pool.
  std::string collisionVolName = Engine::Instance()->GetConfigValue(
    "pool-ball-collision-vol");
  m_pCollisionVol = SolidComponent::LoadSolid(collisionVolName);
  if (!m_pCollisionVol.GetPtr())
  {
    pf->ReportError("PoolBall: failed to load collision volume.");
    return false;
  }
  Matrix m;
  m.identity();
  m_pCollisionVol->CreateBoundingSphere(m);

  return true;
}

void PoolBall::OnRoomEntry()
{
  SetState(UNKNOWN);
  SetFalling(false);
  SetVerticalVel(0);
  SetForwardVel(0);
  SetForwardAccel(0);
  // TODO lose all spin etc.
  SetOrientation(m_startOr);
#ifdef POOL_ONLINE
  if (IsCueBall(this))
  {
    ShotLog("place cue ball 10");
  }
#endif

  // Required for collisions with CRAZY game objects ?
  RefreshHeightServer();

  // TODO Can't go in Load(), m_pLevel not set ?!
  SetShadowSize(1.5f); // TODO CONFIG
  RefreshShadow();
  // Set to true for cue ball when struck.
  m_isSliding = false;

  static const double INTERSECT_VEL = 
    Engine::Instance()->GetConfigFloat("pool_intersect_vel");
  m_intersectVel = INTERSECT_VEL;
//  m_intersectVel = 0;
  m_intersectBalls.clear();

  m_englishVel = 0;
  m_rollPause = false;
}

void PoolBall::RefreshHeightServer()
{
  // We want some collision vols, e.g. PoolBlockers, for CRAZY levels.
  // But we DON'T want the Non-Headstring area collision vol, so check for
  // this specifically.

  Assert(m_pLevel);

  RecalcCollisionVol();

  // First refresh 'static' heightserver
  m_pHeightPoly = 0;
  VisibleGameObject::RefreshHeightServer();
  // If the HS changes, reset pointer to last known floor.
  m_heightServerWithPlatforms = m_heightServer;

  int levelId = m_pLevel->GetId();
  int roomId = m_pLevel->GetRoomId();
  // Get the game objects which are in the same room as this character.
  GameObjectMap& objs = Engine::Instance()->GetGameObjects(levelId, roomId);

  // Iterate through map of Game Objects.
  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    //GameObjectId gameObjId = it->first;
    PGameObject pGo = it->second;

    if (pGo.GetPtr() == this)
    {
      continue; // don't collide with self
    }

    VisibleGameObject* pVis = dynamic_cast<VisibleGameObject*>(pGo.GetPtr());
    NonHeadArea* pNha = dynamic_cast<NonHeadArea*>(pGo.GetPtr());
    Stupid* pStupid = dynamic_cast<Stupid*>(pGo.GetPtr());
    PoolBall* pBall = dynamic_cast<PoolBall*>(pGo.GetPtr());

    // DON'T collide with Non Headstring Area
    // DON'T collide with Stupids! -- TODO TEMP TEST
    // DON'T collide with PoolBalls - we have special code to do that!

    if (pVis && !pNha && !pStupid && !pBall && InPlay(pVis))
    {
      const HeightServer* pCollisionVol = pVis->GetCollisionVol();
      if (pCollisionVol)
      {
        m_heightServerWithPlatforms.AddHeightServer(*pCollisionVol);
      }
    }
  }
}

#if defined(SCENE_EDITOR)
bool PoolBall::Save(File* pf) 
{
  if (!pf->WriteComment("// Solid name"))
  {
    return false;
  }
  pf->Write(m_pSolid->GetName());
  pf->WriteComment("// Orientation");
  m_pSolid->GetOrientation()->Save(pf);
  pf->WriteComment("// Ball no.");
  pf->WriteInteger(m_number);
  pf->WriteComment("// Textured Quad");
  m_tq.Save(pf);

  return true;
}

static const char* BALL_NUM = "Ball num";
static const char* COMP_FILE = "Solid comp file";
static const char* TQ_BMP = "Textured quad bmp";

GameObject::PropertyMap PoolBall::GetProperties() const
{
  PropertyMap pm;
  pm[BALL_NUM] = m_number;
  pm[COMP_FILE] = m_pSolid->GetName();
  pm[TQ_BMP] = m_tqBmp;

  return pm;
}

void PoolBall::SetProperties(const PropertyMap& pm)
{
  m_number = const_cast<GameObject::PropertyMap&>(pm)[BALL_NUM].GetInt();
  std::string compFile = 
    const_cast<GameObject::PropertyMap&>(pm)[COMP_FILE].GetString();
  PSolidComponent pComp = SolidComponent::LoadSolid(compFile);
  if (pComp.GetPtr())
  {
    m_pSolid = pComp;
  }

  std::string tqBmp = 
    const_cast<GameObject::PropertyMap&>(pm)[TQ_BMP].GetString();
  if (tqBmp.empty())
  {
    tqBmp = "ball" + ToString(m_number) + ".bmp";
  }
  if (tqBmp != m_tqBmp)
  {
    m_tqBmp = tqBmp;
    m_tq.Load(m_tqBmp, m_tqAlpha);
  }
}
#endif

CueBall::CueBall()
{
}

bool CueBall::Load(File* pf)
{
  return PoolBall::Load(pf);  
}

void PoolBall::RecalcCollisionVol()
{
  // Set orientation of collision solid so it encloses this character.
  m_pCollisionVol->SetOrientation(m_orientation);

  Matrix m;
  m.identity();
  Assert(m_pCollisionVol.GetPtr());
  
  GetOrientation()->TransformMatrix(&m);
  m_pCollisionVol->StoreHeights(m);

  m_collisionVolume.Clear();
  BoundingSphere bs(VertexBase(0, 0, 0), 1000); // big B.S.
  m_pCollisionVol->AddHeights(&m_collisionVolume, bs);
}

void CueBall::SetOrientation(const Orientation& o)
{
  //PoolBall::SetOrientation(o);

  // Don't call base class for now, so we can check that this fn always gets called for Cue ball.
  m_x = o.GetX();
  m_z = o.GetZ();
  FreeMovingSolidGameObject::SetOrientation(o);

#ifdef POOL_ONLINE
  std::string s = "Setting cue ball pos: x: ";
  s += ToString(o.GetX());
  s += " y: ";
  s += ToString(o.GetY());
  s += " z: ";
  s += ToString(o.GetZ());

  ShotLog(s);
#endif
}
}

