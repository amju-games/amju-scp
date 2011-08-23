/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ControlSyleMouseBase.h,v $
Revision 1.1  2004/09/08 15:42:35  jay
Added to repository
  
*/

#if !defined(CONTROL_STYLE_MOUSE_BASE_H_INCLUDED)
#define CONTROL_STYLE_MOUSE_BASE_H_INCLUDED

namespace Amju
{
// Base class for all Control Styles which use the Mouse.
class ControlStyleMouseBase : public ControlStyle
{
public:
  ControlStyleMouseBase();

protected:
  // Window coords of mouse cursor
  int m_mouseX, m_mouseY;

  // true if mouse cursor pos has changed since last time
  bool m_newMousePos;

  // Set to true when mouse button is down.
  bool m_drag;

};
}

#endif

