/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ThirdPersonCamera.cpp,v $
Revision 1.1  2004/09/08 15:42:59  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "ThirdPersonCamera.h"
#include "Common.h"
#include "Engine.h"
#include "CharacterGameObject.h"
#include "Level.h"

namespace Amju
{
void ThirdPersonCamera::Reset()
{
  RecalcIdle();
}

void ThirdPersonCamera::Update()
{
  // Store the current position. After it has changed, find out if we have collided with
  // anything.
  Orientation oldOr = m_orientation;

  if (m_pPlayer->IsIdle())
  {
    RecalcIdle();
  }
  else
  {
    RecalcNotIdle();
  }

  if (m_quickSwing)
  {
    RecalcSwing(m_quickSwingVel);
  }

  // TODO maybe don't do the collsion test if m_quickSwing.
  if (Collision(oldOr, m_orientation))
  {
    m_orientation = oldOr;
    // Give up trying to swing behind player.
    m_quickSwing = false;
  }

  //Orientation newOr;
/*
  // If we have collided with something, move the position back.
  // Split this into x-z and y movement. Keep a copy of the new coords.
  newOr = m_orientation;
  // Reset the x-z coords and test y.
  m_orientation.SetX(oldOr.GetX());
  m_orientation.SetZ(oldOr.GetZ());
  if (Collision())
  {
    // Changing the height caused a collision, so reset it.
    m_orientation.SetY(oldOr.GetY());
  }
  // Change the x-z coords back to the new position
  m_orientation.SetX(newOr.GetX());
  m_orientation.SetZ(newOr.GetZ());
*/
/*
  // Do the same test but now reset y. If there's a collision it's due to the 
  // new x-z coords.
  newOr = m_orientation;
  m_orientation.SetY(oldOr.GetY());
  if (Collision())
  {
    // Reset the x-z coords.
    m_orientation.SetX(oldOr.GetX());
    m_orientation.SetZ(oldOr.GetZ());
  }
  // Return the y coord to its new value
  m_orientation.SetY(newOr.GetY());
*/
  // Camera cannot drop below abs min height. So if the player falls below this
  // we don't follow.
  if (m_orientation.GetY() < m_minAbsHeight)
  {
    m_orientation.SetY(m_minAbsHeight);
  }
}

void ThirdPersonCamera::RecalcNotIdle()
{
  m_dontFollow = false; // Reset idle flag

  const float deltaTime = Engine::Instance()->GetDeltaTime();
  const Orientation* pPO = m_pPlayer->GetOrientation();

  //RecalcDistance();

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

  // Track player movement.

  // Set the camera orientation. 
  m_oldPlayerX = m_playerX;
  m_oldPlayerZ = m_playerZ;
  m_playerX = pPO->GetX();
  m_playerZ = pPO->GetZ();
 
  float dx = m_orientation.GetX() - pPO->GetX();
  float dz = m_orientation.GetZ() - pPO->GetZ();
/*
  if (fabs(dz) < 0.01f)
  {
    dx = 0.01f;
  } 
*/
  float theta = atan2(dz, dx);
  // Convert
  theta = -theta * 180.0f / pi - 90;
  m_orientation.SetYRot(theta);

/*
  // Move towards player. Use the player's 2D vector and forward vel.
  float vecx = 0;
  float vecz = 0;
  m_pPlayer->GetForwardVector(&vecx, &vecz);
  float unitsFwd = m_pPlayer->GetForwardVel() * deltaTime;
  float addX = unitsFwd * vecx; 
  float addZ = unitsFwd * vecz;
  m_orientation.SetX(m_orientation.GetX() + addX);
  m_orientation.SetZ(m_orientation.GetZ() + addZ);
*/
  m_behind = sqrt(dx * dx + dz * dz);
  if (m_behind > m_behindMax)
  {
    float xdiff = m_playerX - m_oldPlayerX;
    float zdiff = m_playerZ - m_oldPlayerZ;

    m_orientation.SetX(m_orientation.GetX() + xdiff);
    m_orientation.SetZ(m_orientation.GetZ() + zdiff);
  }
  else if (m_behind < m_behindMin)
  {
    float xdiff = m_playerX - m_oldPlayerX;
    float zdiff = m_playerZ - m_oldPlayerZ;

    m_orientation.SetX(m_orientation.GetX() + xdiff);
    m_orientation.SetZ(m_orientation.GetZ() + zdiff);
  }

  RecalcSwing(m_swingVel);
}

void ThirdPersonCamera::RecalcDistance()
{
  // Set the distance from the camera to the target (player).
  // Make sure it is between the min and max distances allowed.
  // TODO check there is a clear path from the current distance to the new 
  // distance!
  if (m_behind > m_behindMax)
  {
    m_behind = m_behindMax;
  }
  else if (m_behind < m_behindMin)
  {
    m_behind = m_behindMin;
  }
}

void ThirdPersonCamera::RecalcIdle()
{
  const float deltaTime = Engine::Instance()->GetDeltaTime();
  const Orientation* pPO = m_pPlayer->GetOrientation();

  m_oldPlayerX = m_playerX;
  m_oldPlayerZ = m_playerZ;
  m_playerX = pPO->GetX();
  m_playerZ = pPO->GetZ();

  RecalcDistance();

  // Camera swings around behind player after a delay.
  // We get the difference between the camera Y rotation and the
  // player Y rotation. We reduce the difference by some factor,
  // and could incorporate the player velocity too.
  RecalcSwing(m_swingVel);
/*
  float diff = pPO->GetYRot() - GetOrientation()->GetYRot();

  bool neg = false;
  if (diff < 0) 
  {
    neg = true;
  }
  diff = fabs(diff);
  // If player has been idle for more than some timeout value, 
  // swing around behind the player.
  if (m_pPlayer->GetIdleTime() > m_idleSwingTime 
      && !m_dontFollow) 
  {
    diff -= deltaTime * m_swingVel; // TODO slow down as diff gets smaller

    if (diff < SMALLEST)
    {
      // no need to follow player rotation any more. Turn this off until
      // player is non-idle.
      m_dontFollow = true;
    }
  }
*/
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

/*
  if (diff < 0) 
  {
    diff = 0;
  }
  if (neg) 
  {
    diff = -diff;
  }

  float newCamera = m_pPlayer->GetOrientation()->GetYRot() - diff;
  m_orientation.SetYRot(newCamera);

  // Fix 90 degree offset; convert to rads.
  float cameraYRotation = 
    (m_orientation.GetYRot() - 90.0f) / 180.0f * pi;

  // Do some trig to get the camera position.
  float dx = (float)(m_behind * cos(double(cameraYRotation)));
  float dz = -(float)(m_behind * sin(double(cameraYRotation)));

  m_orientation.SetX(pPO->GetX() - dx);
  m_orientation.SetZ(pPO->GetZ() - dz);
*/
}

void ThirdPersonCamera::RecalcSwing(float swingVel)
{
  // TODO Pasted from RecalcIdle - factor out the common code.
  const float deltaTime = Engine::Instance()->GetDeltaTime();
  const Orientation* pPO = m_pPlayer->GetOrientation();

  // Get the diff between the player rotation and cam rotation:
  // Make sure both are between 0 and 360 so we get the smallest diff.
  float r1 = pPO->GetYRot();
  float r2 = GetOrientation()->GetYRot();
  r1 += 3600.0f; // in case < 0
  r2 += 3600.0f;
  r1 = r1 - 360.0f * (float)(int)(r1/360);
  r2 = r2 - 360.0f * (float)(int)(r2/360);

  float diff = r1 - r2;
  bool neg = false;
  if (diff < 0) 
  {
    neg = true;
  }
  diff = fabs(diff);

  // Bug no 8 - swing behind doesn't stop if player moving.
  if (diff < 1.0f) // TODO config
  {
    m_quickSwing = false;
    return;
  }

  diff -= deltaTime * swingVel; 

  if (diff < 0) 
  {
    diff = 0;
  }
  if (neg) 
  {
    diff = -diff;
  }
  float newCamera = m_pPlayer->GetOrientation()->GetYRot() - diff;
  m_orientation.SetYRot(newCamera);

  // Fix 90 degree offset; convert to rads.
  float cameraYRotation = 
    (m_orientation.GetYRot() - 90.0f) / 180.0f * pi;

  // Do some trig to get the camera position.
  float dx = (float)(m_behind * cos(double(cameraYRotation)));
  float dz = -(float)(m_behind * sin(double(cameraYRotation)));

  m_orientation.SetX(pPO->GetX() - dx);
  m_orientation.SetZ(pPO->GetZ() - dz);

}
}
