/*
Amju Games source code (c) Copyright Jason Colman 2000-2006
*/

#ifndef GUI_SENDABLE_H_INCLUDED
#define GUI_SENDABLE_H_INCLUDED

#include "GuiComposite.h"

namespace Amju
{
class PoolGuiSendable : public PoolGuiComposite
{
public:
  PoolGuiSendable();
  virtual void DrawImpl();

  void SendToNewPos(float newX, float newY, float time);

protected:
  void Update();

protected:
  // Time left until animation is complete
  float m_animateTimeRemaining;
  float m_newX, m_newY; // position to animate to
  float m_xVel, m_yVel; // animation vel

  // State of bounce animation at end of SendTo movement.
  // 0: not started
  // 1: going up
  // 2: going down
  // 3: finished
  int m_bounceState; 
  
  // Per-tile bounce time so all tiles do not behave identically.
  float m_bounceTime;

};
}

#endif

