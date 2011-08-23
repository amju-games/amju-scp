/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: SimpleCamera.cpp,v $
Revision 1.1.8.2  2005/04/09 23:06:32  jay
Fixed looking straight down for Pool.

Revision 1.1.8.1  2005/04/03 16:26:30  jay
Added DrawRotation - used for Pool birds eye camera.

Revision 1.1  2004/09/08 15:42:56  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "Common.h"
#include "SimpleCamera.h"
#include "Mouse.h"

namespace Amju
{
SimpleCamera::SimpleCamera()
{
  m_direction = DOWN;
  m_drag = false;
  m_zoom = false;
}

SimpleCamera::Direction SimpleCamera::GetDirection() const
{
  return m_direction;
}

void SimpleCamera::SetDirection(Direction d)
{
  m_lookAtPos = m_orientation.GetVertex();

  switch (d)
  {
  case DOWN:
    m_lookAtPos.y -= 1.0f;
    m_lookAtPos.z -= 0.01f;
    break;
/*
  case UP:
    m_lookAtPos.y += 1.0f;
    m_lookAtPos.z += 1.0f;
    break;
*/
  case NORTH:
    m_lookAtPos.z += 1.0f;
    break;
  case SOUTH:
    m_lookAtPos.z -= 1.0f;
    break;
  case EAST:
    m_lookAtPos.x += 1.0f;
    break;
  case WEST:
    m_lookAtPos.x -= 1.0f;
    break;

  }
  m_direction = d;
}

void SimpleCamera::DrawRotation()
{
  // POOL: This seems to do nothing when we look straight down.
  AmjuGL::RotateY(-m_orientation.GetYRot()); //, 0.0f, 1.0f, 0.0f);
}

void SimpleCamera::Draw()
{
  float eyeX = m_orientation.GetX();
  float eyeY = m_orientation.GetY();
  float eyeZ = m_orientation.GetZ();

  // POOL: Changed "up" vector from 0, 1, 0 to 1, 0, 0.
  // This rotates the view so the pool table fits nicely in the window.
  AmjuGL::LookAt(eyeX, eyeY, eyeZ, 
            m_lookAtPos.x, m_lookAtPos.y, m_lookAtPos.z, // point in direction we want to look
            1.0, 0, 0 /* 'Up' vector */);

}

void SimpleCamera::SetOrientation(const Orientation& o)
{
  Camera::SetOrientation(o);
  // Hacky way of resetting the look-at pos.
  
  m_lookAtPos = m_orientation.GetVertex();

  switch (m_direction)
  {
  case DOWN:
    m_lookAtPos.y -= 1.0f;
    m_lookAtPos.z -= 0.01f;
    break;

  case UP:
    m_lookAtPos.y += 1.0f;
    break;
  case NORTH:
    m_lookAtPos.z += 1.0f;
    break;
  case SOUTH:
    m_lookAtPos.z -= 1.0f;
    break;
  case EAST:
    m_lookAtPos.x += 1.0f;
    break;
  case WEST:
    m_lookAtPos.x -= 1.0f;
    break;

  }
}

void SimpleCamera::MousePos(int x, int y)
{
  if (!m_zoom && !m_drag)
  {
    return;
  }

  int dx = x - m_oldx;
  int dy = y - m_oldy;
  m_oldx = x;
  m_oldy = y;

  if (m_zoom)
  {
    // Get mouse coord diff; zoom in or out.
    if (dy != 0)
    {
      float s = (float)dy * 0.1f;
      switch (m_direction)
      {
      case UP:
      case DOWN:
        {
          Orientation o = m_orientation;
          o.SetY(o.GetY() - s);
          SetOrientation(o);
        }
        break;

      case NORTH:
      case SOUTH:
        {
          Orientation o = m_orientation;
          o.SetZ(o.GetZ() + s);
          SetOrientation(o);
        }
        break; 
      case EAST:
      case WEST:
        {
          Orientation o = m_orientation;
          o.SetX(o.GetX() + s);
          SetOrientation(o);
        }
        break;

      }
    }
  }
  else if (m_drag)
  {
    float sx = (float)dx * 0.1f;
    float sy = (float)dy * 0.1f;

    switch (m_direction)
    {
    case UP:
    case DOWN:
      m_orientation.SetX(m_orientation.GetX() + sx);
      m_orientation.SetZ(m_orientation.GetZ() + sy);
      m_lookAtPos.x += sx;
      m_lookAtPos.z += sy;
      break;
    case NORTH:
    case SOUTH:
      m_orientation.SetX(m_orientation.GetX() + sx);
      m_orientation.SetY(m_orientation.GetY() + sy);
      m_lookAtPos.x += sx;
      m_lookAtPos.z += sy;
      break;
    case EAST:
    case WEST:
      m_orientation.SetX(m_orientation.GetX() + sx);
      m_orientation.SetZ(m_orientation.GetZ() + sy);
      m_lookAtPos.x += sx;
      m_lookAtPos.z += sy;
      break;
    }
  }
}

void SimpleCamera::MouseButton(bool down, bool ctrl, bool shift)
{

  if (down && shift)
  {
    m_zoom = true;
    m_oldx = Mouse::s_mousex;
    m_oldy = Mouse::s_mousey;
  }
  else if (down && ctrl)
  {
    m_drag = true;
    m_oldx = Mouse::s_mousex;
    m_oldy = Mouse::s_mousey;
  }
  else
  {
    m_drag = false;
    m_zoom = false;
  }
}

}

