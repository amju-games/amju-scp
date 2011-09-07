/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ThirdPersonCameraFollow.cpp,v $
Revision 1.1.10.1  2005/05/08 08:06:31  jay
Added Zoom functions, for key-controlled zooming.
Reset() iin subclasses should call the parent class version.

Revision 1.1  2004/09/08 15:42:59  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "ThirdPersonCameraFollow.h"
#include "Engine.h"
#include "Orientation.h"
#include "CharacterGameObject.h"
#include "SchAssert.h"

namespace Amju
{
ThirdPersonCameraFollow::ThirdPersonCameraFollow()
{
  m_sampletimer = 0;
  interptimer = 0;
  m_controlPointPeriod = 1.0f;
  m_points.reserve(4);
}

bool ThirdPersonCameraFollow::TimerExpired()
{
  m_sampletimer += Engine::Instance()->GetDeltaTime();
  if (m_sampletimer > m_controlPointPeriod)
  {
    m_sampletimer = 0;
    return true;
  }
  return false;
}

void ThirdPersonCameraFollow::GetControlPoint()
{
  Assert(m_points.size() <= 4);
  Orientation newPoint = *(m_pPlayer->GetOrientation());
  // Don't copy the same point
  if (m_points.size() > 0)
  {
    Orientation& lastPoint = m_points.rbegin()->second;
    if (lastPoint.GetVertex() == newPoint.GetVertex())
    {
      return;
    }
  }

  if (m_points.size() == 4)
  {
    // Erase the first element.
    m_points.erase(m_points.begin());
  }
  m_points.push_back(std::make_pair(0.0f, newPoint));
  // Set the times for the 1st and 2nd points (0-based).
  // The time for point[1] is zero. The time for point[2] is the sample
  // period. The camera pos is some point between these two.
  m_points[1].first = 0;
  m_points[2].first = m_controlPointPeriod;

  interptimer = 0;
}

void ThirdPersonCameraFollow::Update()
{
  // Periodically get the player coords, to use as control points for the cam.
  if (TimerExpired())
  {
    GetControlPoint();
  }
  if (m_points.size() == 4)
  {
    // We want to get the interpolated position between points[1] and [2].
    // It's valid to treat the vector as an array.
    Interpolate(interptimer, &m_points[0], &m_orientation);
    
    interptimer += Engine::Instance()->GetDeltaTime();
    if (interptimer > m_controlPointPeriod)
    {
      interptimer = m_controlPointPeriod;
    }
  }
}
}
