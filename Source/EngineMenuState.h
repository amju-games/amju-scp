/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineMenuState.h,v $
Revision 1.1  2004/09/08 15:42:48  jay
Added to repository
 
*/

#if !defined(ENGINE_MENU_STATE_H_INCLUDED)
#define ENGINE_MENU_STATE_H_INCLUDED

#include "EngineState.h"
#include "Menu.h"
#include "TexturedQuad.h"
#include "Attrib.h" // for Texture Scrolling
#include "Lighting.h"
#include "TextLeaf.h"

namespace Amju
{
// Abstract Base class for Engine states which show a menu. Handles
// joystick/keyboard input.
class EngineMenuState : public EngineState
{
public:
  EngineMenuState();

  virtual void JoyY(float f);
  virtual void PlusUp(bool);
  virtual void PlusDown(bool);
  virtual void PlusLeft(bool);
  virtual void PlusRight(bool);

  virtual bool Load(); // call from derived classes to load background

  virtual void Draw(); // common drawing; call from derived classes
  virtual void DrawOverlays();

  virtual void MousePos(int x, int y);
  virtual void MouseButton(bool down, bool ctrl, bool shift);

  void SetGreet(const TextLeaf& t);

protected:
  // Make sure camera-Z allows the widest menu item
  // to fit on the screen/window.
  void FitMenuToScreen();

protected:
  PMenu m_pMenu;

  // joystick must pass through zero before another item is selected.  
  bool m_zeroed; 

  // Background, behind everything else
  TexturedQuad m_bgQuad;
  // Texture Scroll attrib. 
  PAttrib m_pScroller;

  Lighting m_lighting;

  // Current orientation of text.
  Orientation m_or; 

  // Translation in y-axis, to centre the text.
  float m_verticalPos;

  // Initial position of topmost menu item.
  float m_initialMenuTop;

  // Camera
  RCPtr<Camera> m_pCam;

  // Greeting/prompt 
  TextLeaf m_greet;

  // Choice made when mouse button down. The choice must be the same when
  // the button goes up to be selected.
  int m_downChoice;
};
}
#endif
