/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: GuiDragButton.cpp,v $
Revision 1.1.10.1  2007/03/09 21:48:55  jay
Copy latest GUI library from trunk

Revision 1.2  2006/09/15 12:48:01  Administrator
Fixes to get PetZoo+SceneEditor to build on Windows/MSVC

Revision 1.1  2004/09/08 15:43:07  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "GuiDragButton.h"

namespace Amju
{
GuiDragButton::GuiDragButton() : m_minx(0), m_maxx(10.0f), m_miny(0), m_maxy(10.0f)
{
  m_canDrag = true;
}

void GuiDragButton::SetDragArea(float minx, float maxx, float miny, float maxy)
{
  m_minx = minx;
  m_maxx = maxx;
  m_miny = miny;
  m_maxy = maxy;
}

float GuiDragButton::GetXValue()
{
  float f =  (m_relLeft - m_minx) / (m_maxx - m_minx); 
  return f;
}

float GuiDragButton::GetYValue()
{
  float f = (m_relTop - m_miny) / (m_maxy - m_miny);
  return f;
}

void GuiDragButton::SetXValue(float f)
{
  Assert(f >= 0);
  Assert(f <= 1.0f);
  m_relLeft = m_minx + f * (m_maxx - m_minx);
}

void GuiDragButton::SetYValue(float f)
{
  Assert(f >= 0);
  Assert(f <= 1.0f);
  m_relTop = m_miny + f * (m_maxy - m_miny);
}
 
void GuiDragButton::MousePos(int x, int y)
{
  static int oldx = x;
  static int oldy = y;
  if (!m_downSelected)
  {
    oldx = x;
    oldy = y;
    return;
  }
  int xdiff = x - oldx;
  int ydiff = y - oldy;
  oldx = x;
  oldy = y;

  float posTop = 0; 
  float posLeft = 0;

  GetRelPos(&posTop, &posLeft);
  float newPosTop = posTop;
  float newPosLeft = posLeft;
  // TODO Convert mouse coords (pixels) to our Gui coord system.
  float dx = (float)xdiff * 0.1f; // TODO TEMP TEST
  float dy = (float)ydiff * 0.1f; // TODO TEMP TEST
  newPosTop -= dy;
  newPosLeft += dx; 

  // check limits 
  if (newPosTop >= m_miny && newPosTop <= m_maxy)
  {
    posTop = newPosTop;
  }
  if (newPosLeft >= m_minx && newPosLeft <= m_maxx)
  {
    posLeft = newPosLeft;
  }

  SetRelPos(posTop, posLeft);
}

}

