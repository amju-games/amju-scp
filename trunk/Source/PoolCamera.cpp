/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: PoolCamera.cpp,v $
Revision 1.1.2.4  2007/06/11 20:38:34  jay
Check max distance

Revision 1.1.2.3  2006/08/30 21:12:19  Administrator
Get Pool to build in MSVC

Revision 1.1.2.2  2006/08/14 17:50:22  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:38  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.9  2005/09/05 20:07:32  jay
Turn off debug output

Revision 1.1.2.8  2005/08/26 21:57:41  jay
Pull back further for AI players

Revision 1.1.2.7  2005/07/30 12:25:48  jay
Adjust camera to cue height.

Revision 1.1.2.6  2005/07/17 23:04:44  jay
Keep camera directly behind cue

Revision 1.1.2.5  2005/05/24 20:43:56  jay
Enforce max height

Revision 1.1.2.4  2005/05/15 17:21:11  jay
Comment change

Revision 1.1.2.3  2005/05/08 09:59:59  jay
Camera pull back does not go up so far

Revision 1.1.2.2  2005/04/03 16:32:31  jay
Override FollowTarget() so camera does not chase after cue ball. The original
purpose was to make placing the cue ball work, but seems like a generally
good idea.

Revision 1.1.2.1  2005/03/31 22:30:49  jay
Added pool-specific camera: it pulls back to ensure that all moving balls
are in the view frustum.

*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "PoolCamera.h"
#include "Engine.h"
#include "GameState.h"
#include "AngleCompare.h"
#include "PoolMisc.h"

namespace Amju
{
PoolCamera::PoolCamera()
{
  m_poolPullBackRequired = false;
  m_poolPullBackVel = 0; // default val
  m_poolMaxPullBackVel = 0;
  m_poolPullBackAccel = 0;
  m_adjustToCueHeight = false;

  // Allow mouse drag camera when not setting up shot
  SetMouseDragEnabled(true);
}

void PoolCamera::SetAdjustToCueHeight(bool b)
{
  m_adjustToCueHeight = b;
}

void PoolCamera::GetConfigValues()
{
  ThirdPersonCamera2::GetConfigValues();
  m_poolMaxPullBackVel = Engine::Instance()->GetConfigFloat("pool_pullbackvel");
  Assert(m_poolMaxPullBackVel != 0);
  m_poolPullBackAccel = Engine::Instance()->GetConfigFloat("pool_pullbackaccel"); 
  Assert(m_poolPullBackAccel != 0);
}

void PoolCamera::SetIsAiPlayer(bool b)
{
  if (b)
  {
    static const float vel = Engine::Instance()->GetConfigFloat(
      "pool_aipullbackvel"); 
    m_poolPullBackVel = vel;
  }
}

bool PoolCamera::FollowTarget() const
{
  return false;
}

void PoolCamera::GoDirectlyBehindCueNow(float degs)
{
  // Keep the same distance from target but change position immediately
  // so we are directly behind the cue.
  // Keep the current height.
   
  // Current vec 
  VertexBase vec = m_orientation.GetVertex() - m_lookAtPos;
  vec.y = 0; // just get distance in (x, z)
  float len = vec.Length();
  // Set new orientation
  m_orientation.SetYRot(degs);
  float rads = DegToRad(degs);
  m_orientation.SetX(m_lookAtPos.x - len * sin(rads));  
  m_orientation.SetZ(m_lookAtPos.z - len * cos(rads));  
}

void PoolCamera::Update()
{
  ThirdPersonCamera2::Update();

  static const float CAM_UP_DOWN_ACCEL = Engine::Instance()->GetConfigFloat(
    "pool_cam_up_down_accel");

  if (m_poolPullBackVel != 0)
  {
    // Don't pull back further if max distance reached
    if (m_orientation.GetVertex().Length() < m_behindMax)
    {
      const float deltaTime = Engine::Instance()->GetDeltaTime();
      // Move the camera backwards.
      // Move in a line from origin through the position, 
      // away from the origin.
      VertexBase vec = m_orientation.GetVertex();
      //  not:  - m_pPlayer->GetOrientation()->GetVertex();
      vec.Normalize();

      float unitsFwd = m_poolPullBackVel * deltaTime;
      float addX = unitsFwd * vec.x;
      float addY = unitsFwd * vec.y;
      float addZ = unitsFwd * vec.z;
      m_orientation.SetX(m_orientation.GetX() - addX);
      // Try to move away from the table rather than up.
      m_orientation.SetY(m_orientation.GetY() - addY * 0.25f);
      m_orientation.SetZ(m_orientation.GetZ() - addZ);
    }
  }

  if (m_poolPullBackVel < 0)
  {
    // We are pulling back. Decelerate the pull back vel.
    const float deltaTime = Engine::Instance()->GetDeltaTime();
    m_poolPullBackVel += m_poolPullBackAccel * deltaTime;
  }
  else
  {
    // If > 0, set to 0.
    m_poolPullBackVel = 0;
  }

  Orientation o = *(GetOrientation());
  float y = o.GetY();

  if (m_adjustToCueHeight)
  {
    // Reset the flag so we decelerate to a stop if the user stops
    // moving the cue.
    m_adjustToCueHeight = false;

    // POOL: Keep the camera above the cue.
    // Get the cue elevation. Get the height of the camera, and the
    // height of a line drawn from the cue ball through the cue, to the 
    // same distance in x-z from the ball as the camera.

    // Standard distance from cue ball, if we are not zoomed.
    float CAM_BALL_DIST = (m_behindMin + m_behindMax) * 0.5f;
    // Camera height within this zone above the cue is ok.
    static const float HEIGHT_ZONE = 20.0f; // TODO CONFIG
    static const float LOW_ZONE = 5.0f; // TODO CONFIG

    // Get the height of the cue at this distance.
    float elev = Engine::Instance()->GetGameState()->GetCurrentPlayerInfo()->
      m_golfStroke.m_cueElevationDegs;
    static const float CUE_BALL_Y = 
      GetBall()->GetBoundingSphere()->GetCentre().y;
    float cueHeight = CUE_BALL_Y + CAM_BALL_DIST * sin(DegToRad(elev));
  
    // Compare camera height with cue height at the default distance.
    // If we are loo low, set an upward vel, with decel.
    // If we are too high, set a downward vel with decel.

    // TODO switch this on and off, so it only applises when the user is aiming
    // the cue.
    if (y < (cueHeight + LOW_ZONE))
    {
#ifdef CAM_DEBUG
std::cout << "***** CAM TOO LOW: cue: " << cueHeight << " y: " << y << "\n";
#endif
      static const float upVel = Engine::Instance()->GetConfigFloat(
        "pool_cam_up_vel");
      m_upVel = upVel;
      m_upAcc = CAM_UP_DOWN_ACCEL;
    }
    else if (y > cueHeight + HEIGHT_ZONE)
    {
#ifdef CAM_DEBUG
std::cout << "***** CAM TOO HIGH: cue: " << cueHeight << " y: " << y << "\n";
#endif
      static const float upVel = Engine::Instance()->GetConfigFloat(
        "pool_cam_up_vel");
      m_upVel = -upVel;
      m_upAcc = -CAM_UP_DOWN_ACCEL;
    }
    else
    {
#ifdef CAM_DEBUG
std::cout << "***** CAM JUST RIGHT: cue: " << cueHeight << " y: " << y << "\n";
#endif
      //m_upVel = 0;
      //m_upAcc = 0;
    }
  } // adjust camera height to cue

  // Always decelerate up/down vel
  if (m_upVel > 0)
  {
    m_upAcc = -CAM_UP_DOWN_ACCEL;
  }
  else if (m_upVel < 0)
  {
    m_upAcc = CAM_UP_DOWN_ACCEL;
  }

  // Never exceed the absolute max camera height.
  if (y > m_maxAbsHeight) 
  {
#ifdef CAM_DEBUG
std::cout << "CAMERA: reached max height " << m_maxAbsHeight << "\n";
#endif

    y = m_maxAbsHeight;
    o.SetY(y);
    SetOrientation(o); 
  } 
}

void PoolCamera::Reset()
{
  ThirdPersonCamera2::Reset();
  m_poolPullBackRequired = false;
  m_poolPullBackVel = 0;
}

void PoolCamera::SetPoolPullBackRequired(bool b)
{
  m_poolPullBackRequired = b;
  if (b)
  {
    m_poolPullBackVel = m_poolMaxPullBackVel;
  }
}

}

