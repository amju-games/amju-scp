/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: SimpleTargetCamera.cpp,v $
Revision 1.1  2004/09/08 15:42:56  jay
Added to repository
  
*/

#include "Common.h"
#include "SimpleTargetCamera.h"

namespace Amju
{
SimpleTargetCamera::SimpleTargetCamera()
{
  SetFollow(true);
}

void SimpleTargetCamera::SetFollow(bool follow)
{
  m_follow = follow;
}

void SimpleTargetCamera::Draw()
{
  float eyeX = m_orientation.GetX(); 
  float eyeY = m_orientation.GetY(); 
  float eyeZ = m_orientation.GetZ(); 

  AmjuGL::LookAt(eyeX, eyeY, eyeZ, 
            m_lookAtPos.x, m_lookAtPos.y, m_lookAtPos.z,
            0, 1.0, 0);
}

void SimpleTargetCamera::DrawRotation()
{
  AmjuGL::RotateY(-m_orientation.GetYRot()); //, 0.0f, 1.0f, 0.0f);
}

void SimpleTargetCamera::Update()
{
  if (!m_follow)
  {
    return;
  }

  Orientation t;
  if (!m_pTarget.GetPtr())
  { 
    t = m_orientation;
    t.SetZ(t.GetZ() - 1.0f); // ?
  }
  else
  {
    t = *(m_pTarget->GetOrientation());
  }
  m_lookAtPos = VertexBase(t.GetX(), t.GetY(), t.GetZ());
}

void SimpleTargetCamera::SetTarget(PPoolGameObject pTarget)
{
  m_pTarget = pTarget;
  SetFollow(true);
}

void SimpleTargetCamera::OnMouseDrag(int dx, int dy)
{
  // Only allow dragging if Ctrl is held down.
  if (!m_dragCtrl)
  {
    return;
  }

  // TODO: Zoom if shift held down ?

  float rot = (float)dx;
  VertexBase v(0, 0, 0); // TODO CONFIG
  if (m_pTarget.GetPtr())
  {
    v = m_pTarget->GetOrientation()->GetVertex();
  }
  RotateCameraHoriz(rot, v);

  // Adjust height
  float height = m_orientation.GetY();
  height += (float) dy / 4.0f; // TODO CONFIG 

/*
  // Set limits:
  // Never exceed the absolute max camera height.
  if (height > m_maxAbsHeight)
  {
    height = m_maxAbsHeight;
  }
*/

  m_orientation.SetY(height);
}

}
