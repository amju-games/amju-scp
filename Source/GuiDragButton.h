/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: PoolGuiDragButton.h,v $
Revision 1.1  2004/09/08 15:43:07  jay
Added to repository
  
*/

#if !defined(GUI_DRAG_BUTTON_H_INCLUDED)
#define GUI_DRAG_BUTTON_H_INCLUDED

#include "GuiButton.h"

// Windows defines "GetYValue" as a macro ??
#if defined(WIN32)
#ifdef GetYValue
#undef GetYValue
#endif
#endif

namespace Amju
{
class PoolGuiDragButton : public PoolGuiButton
{
public:
  PoolGuiDragButton();
  void SetDragArea(float minx, float maxx, float miny, float maxy);
  virtual void MousePos(int x, int y);

  // Get value of slider between 0 and 1.0.
  float GetXValue();
  float GetYValue();

  // Set position of button within drag area. Arg is between 0 and 1.0.
  void SetXValue(float);
  void SetYValue(float);

protected:
  float m_minx, m_maxx, m_miny, m_maxy;
};
}


#endif



