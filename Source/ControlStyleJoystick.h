/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ControlStyleJoystick.h,v $
Revision 1.1  2004/09/08 15:42:35  jay
Added to repository
  
*/

#if !defined(CONTROL_STYLE_JOYSTICK_H_INCLUDED)
#define CONTROL_STYLE_JOYSTICK_H_INCLUDED

#include "ControlStyleMovePlayerBase.h"

namespace Amju
{
class ControlStyleJoystick : public ControlStyleMovePlayerBase
{
public:
  ControlStyleJoystick();

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
  // Joysick raw x/y. -1..1. 0 is centre.
  float m_joyX, m_joyY;
  // Joystick pos as polar coord r, theta
  float m_joyRadius; 
  float m_joyTheta;
  float m_oldJoyTheta; // previous value
  // Threshold: if joystick movement is above this threshold, player 
  // orientation 'flips'.
  float m_flip;

};
}

#endif

