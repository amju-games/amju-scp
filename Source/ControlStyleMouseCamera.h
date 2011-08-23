/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ControlStyleMouseCamera.h,v $
Revision 1.1  2004/09/08 15:42:35  jay
Added to repository
  
*/

#if !defined(CONTROL_STYLE_MOUSE_CAMERA_H_INCLUDED)
#define CONTROL_STYLE_MOUSE_CAMERA_H_INCLUDED

#include "ControlStyleMouseBase.h"

namespace Amju
{
// This kind of control style rotates the camera about the player
// character when the mouse is dragged.
class ControlStyleMouseCamera : public ControlStyleMouseBase
{
public:
  ControlStyleMouseCamera();

  virtual void Red(bool);
  virtual void Blue(bool);
  virtual void Green(bool);
  virtual void YellowUp(bool);
  virtual void YellowDown(bool);
  virtual void YellowLeft(bool);
  virtual void YellowRight(bool);
  virtual void Z(bool);
  virtual void PlusUp(bool);
  virtual void PlusDown(bool);
  virtual void PlusLeft(bool);
  virtual void PlusRight(bool);
  virtual void JoyX(float f);
  virtual void JoyY(float f);
  virtual void MousePos(int x, int y);
  virtual void MouseButton(bool down, bool ctrl, bool shift);

  virtual void Update();
  virtual void Reset();

protected:
  // If true, mouse button is pressed, and we want to find what is being
  // selected.
  bool m_pickObjectMode;
  // User is dragging mouse to rotate camera about player character.
  bool m_panMode;

};
}

#endif

