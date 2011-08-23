/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ThirdPersonCameraFixed.h,v $
Revision 1.1  2004/09/08 15:42:59  jay
Added to repository
  
*/

#if !defined(THIRD_PERSON_CAMERA_FIXED_H_INCLUDED)
#define THIRD_PERSON_CAMERA_FIXED_H_INCLUDED

#include "ThirdPersonCamera2.h"

namespace Amju
{
// This kind of 3rd person camera follows the player but cannot be
// rotated about the player.
// This is useful for directional controls.
class ThirdPersonCameraFixed : public ThirdPersonCamera2
{
public:
  virtual void Update();
  virtual void Reset();
  virtual void RotateCameraHoriz(float rot);

protected:
  virtual void RecalcNotIdle();
  virtual bool FollowTarget() const;

};
}

#endif

