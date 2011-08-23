/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: FirstPersonCamera.h,v $
Revision 1.1  2004/09/08 15:42:49  jay
Added to repository
  
*/

#if !defined(FIRST_PERSON_CAMERA_H_INCLUDED)
#define FIRST_PERSON_CAMERA_H_INCLUDED

#include "Camera.h"

namespace Amju
{
class FirstPersonCamera : public Camera
{
public:
  FirstPersonCamera();

  virtual void Draw();
  virtual void DrawRotation();
  // Call whenever first person camera is turned on.
  virtual void Reset();
  virtual void Update();

  // Set joystick pos.
  virtual void JoyX(float f);
  virtual void JoyY(float f);

  // Override Z button to reset the camera to 3rd person.
  virtual void Z(bool down);

  // Override base version so we can reset the camera to face the 
  // forward direction when the mouse is not moved.
  virtual void MousePos(int x, int y);

  // Get the direction the camera is facing.
  float GetYRotation() const;

protected:
  // Implement mouse control
  virtual void OnMouseDrag(int x, int dy);

private:
  float m_xRotOffset; // Offset to x-axis rotation
  float m_yRotOffset; // Offset to y-axis rotation
  float m_xVel;
  float m_yVel;
};
}
#endif

