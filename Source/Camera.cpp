/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Camera.cpp,v $
Revision 1.1  2004/09/08 15:42:45  jay
Added to repository
  
*/

// This source code originally written by JASON COLMAN 2000-2003. 
// You may use it as you wish, at your own risk!  jason@amju.com.

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "Camera.h"
#include "Common.h"
#include "CharacterGameObject.h"
#include "RotateHelper.h"

namespace Amju
{
Camera::Camera()
{
  m_pPlayer = 0;
  Reset();
  m_mouseX = 0;
  m_mouseY = 0;

  // Notional radius of camera
  m_bsphere.SetRadius(0.05f); // TODO CONFIG 
}

const char* Camera::GetTypeName() const
{
  return "cam";
}

#if defined(SCENE_EDITOR)
bool Camera::Save(File*)
{
  return true;
}

Camera::PropertyMap Camera::GetProperties() const
{
  return PropertyMap();
}

void Camera::SetProperties(const Camera::PropertyMap&)
{
}
#endif

const BoundingSphere* Camera::GetBoundingSphere() const
{
  m_bsphere.SetCentre(const_cast<Camera*>(this)->GetOrientation()->GetVertex()); 
  return &m_bsphere;
}

void Camera::Reset()
{
  m_dragCtrl = false;
  m_drag = false;
}

void Camera::SetPlayer(FreeMovingGameObject* p)
{
  m_pPlayer = p;
}

FreeMovingGameObject* Camera::GetPlayer()
{
  return m_pPlayer;
}

void Camera::RefreshHeightServer(Level* )
{
}

void Camera::MouseButton(bool down, bool ctrl, bool shift)
{
  // If control is pressed, dragging the mouse Zooms the camera.
  // If control is not pressed, dragging the mouse rotates the camera about
  // the player.
  if (down)
  {
    m_drag = true;
    m_dragCtrl = ctrl;
  }
  else
  {
    m_drag = false;
  }
}

void Camera::MousePos(int x, int y) 
{
  static const int MOUSEMOVE = 1;

  static bool first = true;
  if (first)
  {
    first = false;
    m_mouseX = x;
    m_mouseY = y;
  }


  if (m_drag && (fabs((float)(x - m_mouseX)) > MOUSEMOVE || 
      fabs((float)(y - m_mouseY)) > MOUSEMOVE))
  {
    m_newMousePos = true;

    int xdiff = x - m_mouseX;
    int ydiff = -(y - m_mouseY);

    OnMouseDrag(xdiff, ydiff);
  }
  else
  {
    m_newMousePos = false;
  }
  m_mouseX = x;
  m_mouseY = y;

}

void Camera::RotateCameraHoriz(float rot, const VertexBase& centre)
{
  Orientation cam = *(GetOrientation());
  float xadd = 0;
  float zadd = 0;
  RotateHelper(centre.x, centre.z, cam.GetX(), cam.GetZ(), rot, &xadd, &zadd);
  cam.SetX(cam.GetX() + xadd);
  cam.SetZ(cam.GetZ() + zadd);
  cam.SetYRot(cam.GetYRot() + rot);
  SetOrientation(cam);
}


}


