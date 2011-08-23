/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: FirstPersonCamera.cpp,v $
Revision 1.1  2004/09/08 15:42:49  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "FirstPersonCamera.h"
#include "Common.h"
#include "Engine.h"
#include "CharacterGameObject.h"

namespace Amju
{
static const float MAX_X_OFFSET = 87.0f;
static const float MAX_Y_OFFSET = 170.0f;

FirstPersonCamera::FirstPersonCamera() :
  m_xRotOffset(0),
  m_yRotOffset(0)
{
  m_xVel = 0;
  m_yVel = 0;
}

void FirstPersonCamera::Reset()
{
  m_yRotOffset = 0;
  m_xRotOffset = 0;
  m_xVel = 0;
  m_yVel = 0;
}

float FirstPersonCamera::GetYRotation() const
{
  return m_orientation.GetYRot() - m_yRotOffset;
}

void FirstPersonCamera::Draw()
{
  float eyeX = m_orientation.GetX();
  float eyeY = m_orientation.GetY();
  float eyeZ = m_orientation.GetZ();

  // Get a point ahead of the player.
  // Fix 90 degree offset; convert to rads.
  float cameraYRotation = 
    (m_orientation.GetYRot() - m_yRotOffset - 90.0f) / 180.0f * pi;

  // Do some trig to get a point in front of the camera, which is in the 
  // direction we want to look.
  float behind = 1.0f;
  float dx = (float)(behind * cos(double(cameraYRotation)));
  float dz = -(float)(behind * sin(double(cameraYRotation)));

  float cameraXRotation = (m_orientation.GetXRot() - m_xRotOffset) / 180.0f * pi;
  float dy = (float)(behind * sin(double(cameraXRotation)));
  // Get x, y, z of some point in front of us.
  float x = m_orientation.GetX() + dx;
  float z = m_orientation.GetZ() + dz;
  float y = eyeY + dy;

  AmjuGL::LookAt(eyeX, eyeY, eyeZ, // origin - player coords
            x, y, z, // point in direction we want to look
            0, 1.0, 0 /* 'Up' vector */);

  // Store "look at" pos.
  m_lookAtPos = VertexBase(x, y, z);
}

void FirstPersonCamera::DrawRotation()
{
  // TODO This is wrong - the origin should be the centre of the Skybox,
  // i.e. (0, 0, 0).
  Draw();
}

void FirstPersonCamera::JoyX(float f)
{
}

void FirstPersonCamera::JoyY(float f)
{
}

void FirstPersonCamera::Update()
{
  float dt = Engine::Instance()->GetDeltaTime();

  m_yRotOffset += m_yVel * dt * 60.0f; // max rotate per sec.
  m_xRotOffset += m_xVel * dt * 60.0f; // max rotate per sec.

  if (m_xRotOffset > MAX_X_OFFSET)
  {
    m_xRotOffset = MAX_X_OFFSET;
  }

  if (m_xRotOffset < -MAX_X_OFFSET)
  {
    m_xRotOffset = -MAX_X_OFFSET;
  }

  if (m_yRotOffset > MAX_Y_OFFSET)
  {
    m_yRotOffset = MAX_Y_OFFSET;
  }

  if (m_yRotOffset < -MAX_Y_OFFSET)
  {
    m_yRotOffset = -MAX_Y_OFFSET;
  }

  // Always set the First-person camera orientation to the player's
  // position, so any change (falling, etc) is shown in 1st-person mode.
  if (GetPlayer() && GetPlayer()->GetOrientation())
  {
    // TODO
    // Need to work out how joysick can work in FP mode.
    // Moving joystick L-R should affect the Y-rotate of the player as well
    // as the FP camera. It's a real change, not just a change in offset.

    //GetPlayer()->GetOrientation()->SetYRot(m_orientation.GetYRot() + m_yRotOffset); // ???
    //m_yRotOffset = 0;

    Orientation o = *(GetPlayer()->GetOrientation());
    o.SetY(o.GetY() + 1.0f); // TODO CONFIG
    m_orientation = o;
  }
}

void FirstPersonCamera::OnMouseDrag(int xdiff, int ydiff)
{
  static const int SMALLEST_DIFF = 2; // TODO CONFIG
  static const float DIFF_SCALE = 2.0f; // TODO CONFIG
  static const float MAX_VEL = 2.0f; // TODO CONFIG

  // TODO m_dragCtrl test is frequired

  // Up/down => rotate around x-axis in first-person mode.
  if (fabs(ydiff) < SMALLEST_DIFF)
  {
    m_xVel = 0;
  } 
  else
  {
    m_xVel = ydiff / DIFF_SCALE; 
    if (m_xVel > MAX_VEL)
    {
      m_xVel = MAX_VEL;
    }
  }

  // Left/right => rotate about y-axis in first person mode
  if (fabs(xdiff) < SMALLEST_DIFF)
  {
    m_yVel = 0;
  } 
  else
  {
    m_yVel = xdiff / DIFF_SCALE;
    if (m_yVel > MAX_VEL)
    {
      m_yVel = MAX_VEL;
    }
  }
}

void FirstPersonCamera::MousePos(int x, int y)
{
  //static const float VEL_MULT = 2.0f;

  // This takes care of when the mouse moves.
  Camera::MousePos(x, y);

  // Reset the camera to face the forward direction when the mouse has not moved.
  if (m_newMousePos)
  {
    return;
  }

  if (m_drag)
  {
    return;
  }

  /*
  // We want to return m_yRotOffset and m_xRotOffset to Zero.
  if (fabs(m_yRotOffset) > 1.0f) // TODO CONFIG
  {
    m_yVel = - m_yRotOffset / MAX_Y_OFFSET * VEL_MULT;
  }
  else
  {
    m_yVel = 0;
    m_yRotOffset = 0;
  }

  if (fabs(m_xRotOffset) > 1.0f) // TODO CONFIG
  {
    m_xVel = - m_xRotOffset / MAX_X_OFFSET * VEL_MULT;
  }
  else
  {
    m_xVel = 0;
    m_xRotOffset = 0;
  }
  */
  m_yVel = 0;
  m_yRotOffset = 0;
  m_xVel = 0;
  m_xRotOffset = 0;

}

void FirstPersonCamera::Z(bool down)
{
  // TODO
  // Set camera style to 3rd peron.
}

}

