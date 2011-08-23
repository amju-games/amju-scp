/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ThirdPersonCamera.h,v $
Revision 1.1  2004/09/08 15:42:59  jay
Added to repository
  
*/

#if !defined(THIRD_PERSON_CAMERA_H_INCLUDED)
#define THIRD_PERSON_CAMERA_H_INCLUDED

#include "ThirdPersonCameraBase.h"

namespace Amju
{
class ThirdPersonCamera : public ThirdPersonCameraBase
{
public:
  virtual void Update();
  void Reset();

protected:
  void RecalcIdle();
  void RecalcNotIdle();
  // Recalc position, swinging behind player.
  void RecalcSwing(float swingVel);

  // Set the distance from the camera to the target (player).
  // Make sure it is between the min and max distances allowed.
  void RecalcDistance();
};
}
#endif

