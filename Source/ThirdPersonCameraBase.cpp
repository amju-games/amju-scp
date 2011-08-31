/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ThirdPersonCameraBase.cpp,v $
Revision 1.2.2.1.4.2  2005/05/24 20:36:45  jay
Enforce zoom in/out end points

Revision 1.2.2.1.4.1  2005/05/08 08:06:31  jay
Added Zoom functions, for key-controlled zooming.
Reset() iin subclasses should call the parent class version.

Revision 1.2.2.1  2004/10/03 17:45:35  Administrator
#95 - Distance change with camera height is now configureable

Revision 1.2  2004/09/17 13:44:50  jay
Added enable function for mouse control

Revision 1.1  2004/09/08 15:42:59  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "ThirdPersonCameraBase.h"
#include "Common.h"
#include "Engine.h"
#include "CharacterGameObject.h"
#include "Level.h"
#include "RotateHelper.h"
#include "CharacterGameObject.h"
#include "Player.h"
#include "AngleCompare.h"

namespace Amju
{
int Sign(float f)
{
  if (f > 0) return 1;
  if (f < 0) return -1;
  return 0;
}

ThirdPersonCameraBase::ThirdPersonCameraBase()
{
  m_dontFollow = false;

  // Defaults: read values from file.
  m_behindMin = 2.0f; 
  m_behindMax = 6.0f; 
  m_behind = 4.0f; 
  m_idleSwingTime = 3.0f;
  m_swingVel = 30.0f;
  m_idleRaiseTime = 2.0f;
  m_raiseVel = 0.25f;
  m_heightMax = 3.0f;
  m_heightMin = 1.0f;
  m_quickSwingVel = 120.0f;
  m_lowerVel = 1.5f;
  m_lookAbove = 2.0f;
  m_dropExtraHeight = 4.0f;
  m_minAbsHeight = 0;
  m_maxAbsHeight = 30.0f;

  m_oldPlayerX = 0;
  m_oldPlayerZ = 0;
  m_playerX = 0;
  m_playerZ = 0;

  m_quickSwing = false;

  m_heightDragged = false; 
  m_yRotVel = 0;
  m_upVel = 0;
  m_yRotAcc = 0;
  m_upAcc = 0;

  m_mouseDragEnabled = false;
  m_mouseZoomEnabled = false;

  m_zoomVel = 0;
}

void ThirdPersonCameraBase::Reset()
{
  Camera::Reset();
  m_zoomVel = 0;
}

void ThirdPersonCameraBase::GetConfigValues()
{
  Engine* e = Engine::Instance();
  m_behindMin       = atof(e->GetConfigValue("cam_behind_min").c_str());
  m_behindMax       = atof(e->GetConfigValue("cam_behind_max").c_str());
  m_idleSwingTime   = atof(e->GetConfigValue("cam_idle_swing_t").c_str());
  m_swingVel        = atof(e->GetConfigValue("cam_idle_swing_vel").c_str());
  m_idleRaiseTime   = atof(e->GetConfigValue("cam_idle_raise_t").c_str());
  m_raiseVel        = atof(e->GetConfigValue("cam_raise_v").c_str());
  m_heightMax       = atof(e->GetConfigValue("cam_height_max").c_str());
  m_heightMin       = atof(e->GetConfigValue("cam_height_min").c_str());
  m_quickSwingVel   = atof(e->GetConfigValue("cam_quick_swing_v").c_str());
  m_lowerVel        = atof(e->GetConfigValue("cam_lower_v").c_str());
  m_lookAbove       = atof(e->GetConfigValue("cam_look_above").c_str());
  m_dropExtraHeight = atof(e->GetConfigValue("cam_drop_extra").c_str());
  m_minAbsHeight    = e->GetConfigFloat("cam_min_abs_height");
  m_maxAbsHeight    = e->GetConfigFloat("cam_max_abs_height");
}

void ThirdPersonCameraBase::Update()
{
  Camera::Update();

  float dt = Engine::Instance()->GetDeltaTime();

  // New for POOL 
  // Zoom in/out, (not using mouse)
  if (m_zoomVel != 0)
  {
#ifdef ZOOM_DEBUG
std::cout << "ZOOM: old zoom vel: " << m_zoomVel << "\n";
#endif

    // Decelerate to zero
    static const float ZOOM_DECEL = Engine::Instance()->GetConfigFloat(
      "pool_cam_zoom_decel");
    Assert(ZOOM_DECEL > 0); 
    if (m_zoomVel > 0)
    {
      m_zoomVel -= ZOOM_DECEL * dt;
      if (m_zoomVel < 0)
      {
        m_zoomVel = 0;
      }
    }
    else if (m_zoomVel < 0)
    {
      m_zoomVel += ZOOM_DECEL * dt;
      if (m_zoomVel > 0)
      {
        m_zoomVel = 0;
      }
    }

#ifdef ZOOM_DEBUG
std::cout << "ZOOM: new zoom vel: " << m_zoomVel << "\n";
#endif

    // Move away or towards the target, at the zoom vel.
    VertexBase v = m_orientation.GetVertex() - m_lookAtPos;
    v.Normalize();
    v *= m_zoomVel * dt;
#ifdef ZOOM_DEBUG
std::cout << "ZOOM add vec: " << ToString(v).c_str() << "\n";
#endif

    VertexBase v0 = m_orientation.GetVertex(); 
    v0 += v;
    m_orientation.SetVertex(v0);
  }

  static float prevRot = m_yRotVel;
  if (m_yRotVel != 0)
  {
    static const float MAX_Y_ROT_VEL = 360.0f;
    //static const float MIN_Y_ROT_VEL = 10.0f;
    m_yRotVel += dt * m_yRotAcc;
    if (fabs(m_yRotVel) > MAX_Y_ROT_VEL)
    {
      m_yRotVel = MAX_Y_ROT_VEL;
      m_yRotAcc = 0;
    }
    if (Sign(m_yRotVel) != Sign(prevRot) && prevRot != 0)
    {
      m_yRotVel = 0;
      m_yRotAcc = 0;
    }
    float yRot = m_yRotVel * dt;
    RotateCameraHoriz(yRot);
  }
  prevRot = m_yRotVel;

  static float prevUp = m_upVel;
  if (m_upVel != 0)
  {
//std::cout << "Up vel: " << m_upVel << " acc: " << m_upAcc;

    float y = GetOrientation()->GetY();
    m_upVel += dt * m_upAcc;
    if (Sign(m_upVel) != Sign(prevUp) && prevUp != 0)
    {
      m_upVel = 0;
      m_upAcc = 0;
    }
    y += m_upVel * dt;

//std::cout << "  new Up vel: " << m_upVel << "\n";

    Orientation o = *(GetOrientation());
    // Never exceed the absolute max camera height.
    if (y > m_maxAbsHeight)
    {
      y = m_maxAbsHeight;
    }
    o.SetY(y);
    SetOrientation(o);
  }
  prevUp = m_upVel;
}

void ThirdPersonCameraBase::Draw()
{
  const Orientation* pPO = m_pPlayer->GetOrientation();
  
  float eyeX = m_orientation.GetX(); 
  float eyeY = m_orientation.GetY(); 
  float eyeZ = m_orientation.GetZ(); 

  float lookX = pPO->GetX();
  float lookY = pPO->GetY() + m_lookAbove;
  float lookZ = pPO->GetZ();

  // Adjust look at pos, so the higher the camera, the further we
  // look in the camera direction.
  float heightDiff = eyeY - (pPO->GetY() + m_lookAbove);
  if (heightDiff < 0)
  {
    heightDiff = 0;
  }
  // Tone down the distance change so you can still see the player
  static const float CAM_HEIGHT_DISTANCE =
    Engine::Instance()->GetConfigFloat("cam_height_distance");
  heightDiff *= CAM_HEIGHT_DISTANCE;

  float yrot = m_orientation.GetYRot();
  lookX += heightDiff * sin(DegToRad(yrot));
  lookZ += heightDiff * cos(DegToRad(yrot)); 

  AmjuGL::LookAt(eyeX, eyeY, eyeZ, 
            lookX, lookY, lookZ,
            0, 1.0, 0);

  m_lookAtPos = VertexBase(lookX, lookY, lookZ);
}

void ThirdPersonCameraBase::DrawRotation()
{
  AmjuGL::RotateY(-m_orientation.GetYRot()); //, 0.0f, 1.0f, 0.0f);
}

void ThirdPersonCameraBase::QuickSwingBehind()
{
  m_quickSwing = true;
}

void ThirdPersonCameraBase::RefreshHeightServer(Level* pLevel)
{
  m_heightServer.Clear();
  pLevel->GetHeightServer(&m_heightServer, *GetBoundingSphere());
}

bool ThirdPersonCameraBase::Collision(const Orientation& before, const Orientation& after)
{
  const BoundingSphere& bs = *GetBoundingSphere();

  BoundingSphere bsBefore(before.GetVertex(), bs.GetRadius());
  BoundingSphere bsAfter(after.GetVertex(), bs.GetRadius());

  const WallPoly* pWp = m_heightServer.Intersects(bsBefore, bsAfter);
  if (pWp)
  {
    return true; // We do collide with something
  }
  return false; // No collision
}

bool ThirdPersonCameraBase::OverlookDrop()
{
  // Special case: if this config value is zero, don't raise the camera 
  // when we overlook a drop.
  if (m_dropExtraHeight == 0)
  {
    return false;
  }

  // Get severity of drop ahead. 0 means no drop, 1 means vertical drop.
  float dropseverity = m_pPlayer->DropAhead();

  if (dropseverity < SMALLEST)
  {
    return false;
  }
  // We should raise the camera, with the max height proportional to dropseverity.
  const float deltaTime = Engine::Instance()->GetDeltaTime();
  // Max height in this case is not the normal max height above player.
  const float max = m_pPlayer->GetHeight() + m_heightMax + dropseverity * m_dropExtraHeight; 
  float newheight = m_orientation.GetY();
  newheight += m_raiseVel * deltaTime; 
  if (newheight > max)
  {
    newheight = max;
  }
  m_orientation.SetY(newheight);
  return true;
}

void ThirdPersonCameraBase::RotateCameraHoriz(float rot)
{
  // Rotate the camera 'rot' degrees, in x-z plane, about the player 
  // character position.
  const VertexBase& v = GetPlayer()->GetOrientation()->GetVertex();
  Camera::RotateCameraHoriz(rot, v);
}

void ThirdPersonCameraBase::MousePos(int x, int y)
{
  Camera::MousePos(x, y);
}

void ThirdPersonCameraBase::MouseButton(bool down, bool ctrl, bool shift)
{
  Camera::MouseButton(down, ctrl, shift);
  if (down)
  {
    m_heightDragged = false; 
  }
}

void ThirdPersonCameraBase::OnMouseDrag(int xdiff, int ydiff) 
{
  if (!m_mouseDragEnabled)
  {
    return;
  }

  // TODO check m_dragCtrl in future if required
  // Rotate about player
  float rot = (float)xdiff; // TODO CONFIG scale this
  RotateCameraHoriz(rot);

  // Adjust height
  if (ydiff == 0)
  {
    return;
  }

  // Set when height has been changed by player dragging mouse.
  m_heightDragged = true; 

  // Camera mouse different to Shot Power mouse ?
  ydiff = -ydiff;

  float height = m_orientation.GetY();
  height += (float) ydiff / 4.0f; // TODO CONFIG 
  // Set limits:
  // Never exceed the absolute max camera height.
  if (height > m_maxAbsHeight)
  {
    height = m_maxAbsHeight;
  }

  // Don't allow camera to go lower than player eye level.
  float playerHeight = GetPlayer()->GetHeight() + 1.0f;
  if (height < playerHeight)
  {
    height = playerHeight;
  }

  // Adjust the max height that the camera auto-raises itself to.
  if (m_heightMax < height)
  {
    m_heightMax = height;
  }
  // Bring down the max height if the camera height is dragged down.
  if (height < m_heightMax && height > playerHeight + 3.0f)
  {
    m_heightMax = height;
  }

  m_orientation.SetY(height);
}

void ThirdPersonCameraBase::Z(bool down)
{
  if (!down)
  {
    return;
  }

  if (Engine::Instance()->GetPlayer()->IsFalling())
  {
    return;
  }

  QuickSwingBehind();
}

void ThirdPersonCameraBase::PlusUp(bool d)
{
  static const float upVel = Engine::Instance()->GetConfigFloat("cam_plus_up_vel");
  if (d)
  {
    m_upVel = upVel; 
    m_upAcc = 1.0f;
  }
  else
  {
    //m_upVel = 0;
    m_upAcc = -upVel; // TODO TEMP TEST
  }
}

void ThirdPersonCameraBase::PlusDown(bool d)
{
  static const float upVel = Engine::Instance()->GetConfigFloat("cam_plus_up_vel");
  if (d)
  {
    float playerHeight = GetPlayer()->GetHeight() + 1.0f;
    if (m_orientation.GetY() > playerHeight)
    { 
      m_upVel = -upVel;
      m_upAcc = 0;
    }
  }
  else
  {
    //m_upVel = 0;
    m_upAcc = upVel; // TODO TEMP TEST 
  }
}

void ThirdPersonCameraBase::PlusLeft(bool d)
{
  static const float rotVel = Engine::Instance()->GetConfigFloat("cam_plus_rot_vel"); 
  if (d)
  {
    m_yRotVel = -rotVel;
    m_yRotAcc = 0.0f; // TODO TEMPT TEST
  }
  else
  {
    //m_yRotVel = 0;
    m_yRotAcc = 720.0f; // TODO TEMP TEST
  }
}

void ThirdPersonCameraBase::PlusRight(bool d)
{
  static const float rotVel = Engine::Instance()->GetConfigFloat("cam_plus_rot_vel"); 
  if (d)
  {
    m_yRotVel = rotVel; 
    m_yRotAcc = 0.0f; // TODO TEMP TEST
  }
  else
  {
    //m_yRotVel = 0;
    m_yRotAcc = -720.0f; // TODO TEMP TEST
  }
}

void ThirdPersonCameraBase::SetRotateVel(float vel, float accel)
{
  m_yRotVel = vel;
  m_yRotAcc = accel;
}

void ThirdPersonCameraBase::ZoomIn()
{
#ifdef _DEBUG
std::cout << "ZOOM IN!\n";
#endif
  static const float ZOOM_VEL = Engine::Instance()->GetConfigFloat("pool_zoom_vel");
  Assert(ZOOM_VEL > 0);

  // Only zoom in if the distance from the camera to the target is
  // greater than the min.
  static const float DIST_MIN = Engine::Instance()->GetConfigFloat("pool_zoom_min_dist");
  VertexBase v = m_orientation.GetVertex() - m_lookAtPos;
  if (v.Length() > DIST_MIN)
  { 
    m_zoomVel = -ZOOM_VEL; 
  }
  else
  {
    // Stop zooming immediately, as we can't wait until we decelerate to
    // a stop. By that time we will be much too close.
    m_zoomVel = 0;
#ifdef _DEBUG
std::cout  << "ZOOM IN: Can't zoom closer, dist to target is " << v.Length() << "\n";
#endif
  }
}

void ThirdPersonCameraBase::ZoomOut()
{
#ifdef _DEBUG
std::cout << "ZOOM OUT!\n";
#endif
  static const float ZOOM_VEL = Engine::Instance()->GetConfigFloat("pool_zoom_vel");
  Assert(ZOOM_VEL > 0);

  // Stop zooming if we are too far away.
  static const float DIST_MAX = Engine::Instance()->GetConfigFloat("pool_zoom_max_dist");
  VertexBase v = m_orientation.GetVertex() - m_lookAtPos;
  if (v.Length() < DIST_MAX)
  { 
    m_zoomVel = ZOOM_VEL; 
  }
  else
  {
#ifdef _DEBUG
std::cout  << "ZOOM OUT: Can't zoom out, dist to target is " << v.Length() << "\n";
#endif
  }
}
}

