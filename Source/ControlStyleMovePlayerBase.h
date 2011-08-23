/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ControlStyleMovePlayerBase.h,v $
Revision 1.1  2004/09/08 15:42:35  jay
Added to repository
  
*/

#if !defined(CONTROL_STYLE_MOVE_PLAYER_BASE_H_INCLUDED)
#define CONTROL_STYLE_MOVE_PLAYER_BASE_H_INCLUDED

// Base class for Control Styles which cause the player to move.
#include "ControlStyle.h"

namespace Amju
{
class ControlStyleMovePlayerBase : public ControlStyle
{
public:
  ControlStyleMovePlayerBase();
  virtual void Reset();

protected:
  void FlipPlayer(float degs);
  void RotatePlayer(float degsSec);

protected:
  static const float SLOW_TO_STOP;

  // Degrees/sec shuffle rotate velocity.
  float m_shuffle;
  bool m_flippedX, m_flippedY; // true if player 'flipped' about Y axis.


  // TODO
  // We should get these from Engine Running state, or not have to know about them
  // at all, is probably best.
  float m_currentMaxForwardVel;
  float m_playerRotateVel;
  float m_maxForwardVel;
  float m_walkForwardVel;
  float m_playerAccel;


};
}

#endif

