/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ThirdPersonCameraFollow.h,v $
Revision 1.1  2004/09/08 15:42:59  jay
Added to repository
  
*/

#if !defined(THIRD_PERSON_CAMERA_FOLLOW_H_INCLUDED)
#define THIRD_PERSON_CAMERA_FOLLOW_H_INCLUDED

#include "ThirdPersonCameraBase.h"
#include "Interpolate.h"
#include <vector>

namespace Amju
{
// Unsuccessful attempt at a Camera which follows the player by 
// using the player's prior positions as control points on a 
// spline curve.
class ThirdPersonCameraFollow : public ThirdPersonCameraBase
{
public:
  ThirdPersonCameraFollow();
  virtual void Update();

protected:
  bool TimerExpired();
  void GetControlPoint();

  std::vector<TimePoint> m_points;
  float m_sampletimer; // counts up to the period between samples.
  float m_controlPointPeriod; // period between samples of player position.
  float interptimer;
};
}

#endif

