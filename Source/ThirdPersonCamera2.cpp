/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ThirdPersonCamera2.cpp,v $
Revision 1.1.8.5  2005/07/06 20:17:30  jay
Comments only - flag possible bugs

Revision 1.1.8.4  2005/06/04 22:26:02  jay
Just added an assert

Revision 1.1.8.3  2005/05/24 20:36:05  jay
Enforce max height; code tidy up

Revision 1.1.8.2  2005/05/08 08:06:29  jay
Added Zoom functions, for key-controlled zooming.
Reset() iin subclasses should call the parent class version.

Revision 1.1.8.1  2005/04/03 16:27:44  jay
Small change: if FollowTarget() returns false, we don't even turn (i.e. rotate
in y-axis) to face the target. Hmm.. maybe this is wrong.

Revision 1.1  2004/09/08 15:42:59  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "ThirdPersonCamera2.h"
#include "CharacterGameObject.h"
#include "Common.h"
#include "Engine.h"
#include "Level.h"
#include <iostream>

using namespace std;
namespace Amju
{
ThirdPersonCamera2::ThirdPersonCamera2() :
  m_isResetting(true),
  m_pullbackVel(0),
  m_hasReset(false),
  m_resetTimer(0),
  m_minResetTime(2.0f),
  m_oldDistance(0),
  m_resetToFront(false)
{
}

void ThirdPersonCamera2::GetConfigValues()
{
  Engine* e = Engine::Instance();
  m_pullbackVel = atof(e->GetConfigValue("cam_pullback_vel").c_str());
  // Min period between resets
  m_minResetTime = atof(e->GetConfigValue("cam_min_reset_time").c_str());

  ThirdPersonCameraBase::GetConfigValues();
}

void ThirdPersonCamera2::Update()
{
  ThirdPersonCameraBase::Update();

  float dt = Engine::Instance()->GetDeltaTime();
  m_resetTimer += dt;

  Orientation oldOr = m_orientation;

  // Implement "quick swing behind" using pull-back.
  if (m_quickSwing)
  {
    Reset();
    m_quickSwing = false;
  }

  if (m_isResetting)
  {
    // Move away from the player until we reach a good distance or we 
    // hit something.
    PullBack();
    // If we hit something, undo the last movement.
    if (Collision(oldOr, m_orientation))
    {
#if defined(_DEBUG)
/*
      cout << "Collision while resetting camera.\n";
*/
#endif

      // If m_behind < m_behindMin, we couldn't get far enough away 
      // from the player. 
      // We could try to Reset in a different direction, e.g. in front
      // of the player rather than behind.
      if (!m_resetToFront && (m_behind < m_behindMin))
      {
        m_resetToFront = true;
      }
      else
      {
        m_orientation = oldOr;
        m_isResetting = false;
      }
    }
  }
  else if (m_pPlayer->IsIdle())
  {
    RecalcIdle();
  }
  else
  {
    RecalcNotIdle();

    if (Collision(oldOr, m_orientation))
    {
      // Can't follow player. Reset (after a delay) in this case ?
      Reset(); 
    }
  }

  if (m_orientation.GetY() < m_minAbsHeight)
  {
    m_orientation.SetY(m_minAbsHeight);
  }
  if (m_orientation.GetY() > m_maxAbsHeight)
  {
    m_orientation.SetY(m_maxAbsHeight);
  }
}


bool ThirdPersonCamera2::FollowTarget() const
{
  // Decide whether or not tofollow player.
  if (m_behind > m_behindMax ||  // player is too far away - follow her!
     (m_behind < m_behindMin && m_behind < m_oldDistance)) // too close and getting closer
  {
    return true;
  }
  return false;
}

void ThirdPersonCamera2::Reset()
{
  ThirdPersonCameraBase::Reset();

/*
  if (m_resetTimer < m_minResetTime)
  {
#if defined(_DEBUG)
    cout << "Can't reset yet." << endl;
#endif
    return;
  }
*/
  m_resetTimer = 0;
  m_resetToFront = false;

  // Start at the player coords, then move back in subsequent frames.
  // Or if this fails, we try moving ahead of the player.
  Assert(m_pPlayer);
  m_orientation = *(m_pPlayer->GetOrientation());
  // Set height to eye-level, not floor-level.
  m_orientation.SetY(m_heightMin + m_orientation.GetY()); // TODO CONFIG ?
  m_behind = 0;
  m_isResetting = true;
  m_hasReset = true; // stops any further Resets while player is idle
}

void ThirdPersonCamera2::PullBack()
{
  const float deltaTime = Engine::Instance()->GetDeltaTime();
  const Orientation* pPO = m_pPlayer->GetOrientation();

  m_oldPlayerX = m_playerX;
  m_oldPlayerZ = m_playerZ;
  m_playerX = pPO->GetX();
  m_playerZ = pPO->GetZ();
 
  float dx = m_orientation.GetX() - pPO->GetX();
  float dz = m_orientation.GetZ() - pPO->GetZ();
  float theta = atan2(dz, dx);  // TODO errr... shurely should be dx, dz ??
  // Convert rads to degs, and fix 90 degree offset
  theta = -theta * 180.0f / pi - 90.0f;
  m_orientation.SetYRot(theta);

  m_behind = sqrt(dx * dx + dz * dz);
  // Stop if we have reached a reasonable distance from the player.
  if (m_behind > ((m_behindMax + m_behindMin) / 2.0f))
  {
    m_isResetting = false; // stop pulling back
  }
  else
  {
    // Move the camera backwards.
    // We go in the opposite direction to the direction the player is facing,
    // which seems to work ok.
    float vecx = 0;
    float vecz = 0;
    m_pPlayer->GetForwardVector(&vecx, &vecz);

    if (m_resetToFront)
    {
      vecx = -vecx;
      vecz = -vecz;
    }

    float unitsFwd = m_pullbackVel * deltaTime;
    float addX = unitsFwd * vecx; 
    float addZ = unitsFwd * vecz;
    m_orientation.SetX(m_orientation.GetX() - addX);
    m_orientation.SetZ(m_orientation.GetZ() - addZ);
    // Get the height at the new (x, z). Increase the height if less than the min.
    const HeightPoly* pHp = m_heightServer.GetHeightPoly(
      m_orientation.GetX(), m_orientation.GetY(), m_orientation.GetZ());
    if (pHp)
    {
      float height = pHp->GetY(m_orientation.GetX(), m_orientation.GetZ());
      if ((m_orientation.GetY() - height) < m_heightMin)
      {
        m_orientation.SetY(height + m_heightMin);
      }
    }
  }
}

void ThirdPersonCamera2::RecalcIdle()
{
#if defined CAMERA_AUTOMATICALLY_RISES
  if (!OverlookDrop())
  {
    if (m_pPlayer->GetIdleTime() > m_idleRaiseTime) 
    {
      // Raise camera height.
      float newheight = m_orientation.GetY();
      newheight += m_raiseVel * deltaTime; 
      if (newheight > (pPO->GetY() + m_heightMax))
      {
        newheight = pPO->GetY() + m_heightMax;
      }
      m_orientation.SetY(newheight);
    }
  }
#endif // CAMERA_AUTOMATICALLY_RISES
}

void ThirdPersonCamera2::RecalcNotIdle()
{
  const float deltaTime = Engine::Instance()->GetDeltaTime();
  const Orientation* pPO = m_pPlayer->GetOrientation();

  // Bring camera height down to player eye level. But not if the player is 
  // overlooking a drop. In this case we want to raise the camera so we can see
  // the drop.
  if (!OverlookDrop())
  {
    // No drop, so lower camera.
    float newheight = m_orientation.GetY();
    newheight -= m_lowerVel * deltaTime; 
    if (newheight < (pPO->GetY() + m_heightMin))
    {
      newheight = pPO->GetY() + m_heightMin;
    }
    m_orientation.SetY(newheight);

  }

  // Fix problem with sloping floors: the camera might go throught the floor!
  // Get the height at the new (x, z). Increase the height if less than the min.
  const HeightPoly* pHp = m_heightServer.GetHeightPoly(
    m_orientation.GetX(), m_orientation.GetY(), m_orientation.GetZ());
  if (pHp)
  {
    float height = pHp->GetY(m_orientation.GetX(), m_orientation.GetZ());
    if ((m_orientation.GetY() - height) < m_heightMin)
    {
      m_orientation.SetY(height + m_heightMin);
    }
  }

  // Track player movement.

  // Set the camera orientation. 
  m_oldPlayerX = m_playerX;
  m_oldPlayerZ = m_playerZ;
  m_playerX = pPO->GetX();
  m_playerZ = pPO->GetZ();
 
  float dx = m_orientation.GetX() - pPO->GetX();
  float dz = m_orientation.GetZ() - pPO->GetZ();
  float theta = atan2(dz, dx); // TODO ????? s/b dx, dz ????
  // Convert rads to degs, and fix 90 degree offset
  theta = -theta * 180.0f / pi - 90;

  m_behind = sqrt(dx * dx + dz * dz);

  if (FollowTarget())
  {
    float xdiff = m_playerX - m_oldPlayerX;
    float zdiff = m_playerZ - m_oldPlayerZ;

    m_orientation.SetX(m_orientation.GetX() + xdiff);
    m_orientation.SetZ(m_orientation.GetZ() + zdiff);
    m_orientation.SetYRot(theta);
  }

  m_oldDistance = m_behind;

  if (m_behind < 1.0f   // - too close TODO CONFIG
      && !m_hasReset)
  {
    // Too close - pull back.
    Reset();
  }
  
  if (m_behind > 1.0f + 0.2f) // hysteresis to avoid jitter TODO CONFIG
  {
    // Only reset this flag when the camera has reached the min. distance
    // from the player. Otherwise we try to Reset all the while we are too
    // close.
    m_hasReset = false; 
  }
}
}
