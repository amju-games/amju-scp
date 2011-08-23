/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ControlStyleRotateKeys.h,v $
Revision 1.1  2004/09/08 15:42:35  jay
Added to repository
  
*/

#if !defined(CONTROL_STYLE_ROTATE_KEYS_H_INCLUDED)
#define CONTROL_STYLE_ROTATE_KEYS_H_INCLUDED

#include "ControlStyleMovePlayerBase.h"

namespace Amju
{
class ControlStyleRotateKeys: public ControlStyleMovePlayerBase
{
public:
  ControlStyleRotateKeys();

  virtual void Red(bool);
  virtual void Blue(bool);
  virtual void Green(bool);
  virtual void YellowUp(bool);
  virtual void YellowDown(bool);
  virtual void YellowLeft(bool);
  virtual void YellowRight(bool);
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
    // If true, player is walking (used for keyboard/d pad)
  bool m_isWalking;
  // Time player has been walking (kb/d pad)
  float m_walkTime;
  // If player walks for this time, she breaks into a run (kb/d pad)
  float m_maxWalkTime;
  // Hold time left/right button is down for - a short click means "flip"...
  float m_plusTime;
  // ..length of the "short" click.
  float m_plusFlipTime;
  // Z-button timer.
  float m_zTime; // current time z-button has been held down
  float m_zTriggerTime; // time z-button must be held down for
  bool m_zDown; // true if z-button is currently down.

  // Base value for m_shuffle, which we always return to.
  float m_shuffleBase;
};
}

#endif

