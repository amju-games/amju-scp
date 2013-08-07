/*
Amju Games source code (c) Copyright Jason Colman 2000-2006
*/

#ifndef GUI_SCROLLER_H_INCLUDED
#define GUI_SCROLLER_H_INCLUDED

#include "GuiComposite.h"

namespace Amju
{
class PoolGuiScroller : public PoolGuiComposite
{
public:
  PoolGuiScroller();
  virtual void DrawImpl();

  enum ScrollDir { LEFT, RIGHT, UP, DOWN };
  void Scroll(ScrollDir dir, float distance, float time);

protected:
  void Update();
protected:
  float m_xVel, m_yVel;
  float m_xAccel, m_yAccel;
};
}

#endif

