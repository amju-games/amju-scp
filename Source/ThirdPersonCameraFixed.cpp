/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ThirdPersonCameraFixed.cpp,v $
Revision 1.1  2004/09/08 15:42:59  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "ThirdPersonCameraFixed.h"

namespace Amju
{
void ThirdPersonCameraFixed::Update()
{
  if (m_pPlayer->IsIdle())
  {
    RecalcIdle();
  }
  else
  {
    RecalcNotIdle();
  }
  if (m_orientation.GetY() < m_minAbsHeight)
  {
    m_orientation.SetY(m_minAbsHeight);
  }
}

void ThirdPersonCameraFixed::RecalcNotIdle()
{
  Orientation player = *(GetPlayer()->GetOrientation());
  Orientation cam = *(GetOrientation());
  float goodDist = (m_behindMin + m_behindMax) / 2.0f;
  cam.SetZ(player.GetZ() - goodDist);
  cam.SetX(player.GetX());
  SetOrientation(cam);
}

void ThirdPersonCameraFixed::Reset()
{
  ThirdPersonCamera2::Reset();
  RecalcNotIdle();
}

void ThirdPersonCameraFixed::RotateCameraHoriz(float rot)
{
}

bool ThirdPersonCameraFixed::FollowTarget() const
{
  return true; 
}

}
