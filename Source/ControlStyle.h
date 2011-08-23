/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ControlStyle.h,v $
Revision 1.1  2004/09/08 15:42:34  jay
Added to repository
  
*/

#if !defined(CONTROL_STYLE_H_INCLUDED)
#define CONTROL_STYLE_H_INCLUDED

#include "SharedPtr.h"

namespace Amju
{
class EngineRunning;


class ControlStyle : public Shareable
{
public:
  virtual ~ControlStyle() {}

  virtual void Red(bool) {}
  virtual void Blue(bool) {}
  virtual void Green(bool) {}
  virtual void YellowUp(bool) {}
  virtual void YellowDown(bool) {}
  virtual void YellowLeft(bool) {}
  virtual void YellowRight(bool) {}

  // Trigger.
  virtual void Z(bool) {} 

  // '+' - shaped pad:
  virtual void PlusUp(bool) {}
  virtual void PlusDown(bool) {}
  virtual void PlusLeft(bool) {}
  virtual void PlusRight(bool) {}

  // Analogue joystick X position: 0 is centred; -1 is fully left;
  //  +1 is fully right.
  virtual void JoyX(float f) {}

  // Analogue Y position: 0 is centred; -1 is fully back; +1 is fully forward.
  virtual void JoyY(float f) {}

  virtual void MousePos(int x, int y) {}
  virtual void MouseButton(bool down, bool ctrl, bool shift) {}

  // Called every frame.
  virtual void Update() {}
  virtual void Reset() {}
  // Necessary for picking, where we draw to the back buffer.
  virtual void Draw() {}

protected:
  // Default blue (action) button code.
  void DoAction(bool);

protected:
  // Get the EngineRunning state. This is used a lot because these control
  // styles necessarily change the running of the game.
  static EngineRunning* ER();
};
}

#endif

