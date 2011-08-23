/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: SimpleTargetCamera.h,v $
Revision 1.1  2004/09/08 15:42:56  jay
Added to repository
  
*/

#if !defined(SIMPLE_TARGET_CAMERA_H_INCLUDED)
#define SIMPLE_TARGET_CAMERA_H_INCLUDED

#include "Camera.h"
#include "GameObject.h"

namespace Amju
{
// Basic camera which points itself at a Game Object target.
// Does't follow, do collision detection, etc.
class SimpleTargetCamera : public Camera
{
public:
  SimpleTargetCamera();

  virtual void Draw();

  // Draw rotation only. Used for Skybox effect.
  virtual void DrawRotation();
  virtual void Update();

  virtual void OnMouseDrag(int x, int dy);

  void SetTarget(PGameObject pTarget);

  void SetFollow(bool follow);

protected:
  PGameObject m_pTarget;

  // If true, follow the current object (if there is one).
  // If false, the active object moves without being followed.
  bool m_follow;
};
}

#endif

