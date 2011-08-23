/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: GuiContactPos.h,v $
Revision 1.1.2.2  2006/08/14 17:50:20  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:36  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.5  2005/07/17 23:03:27  jay
Configurable size/pos

Revision 1.1.2.4  2005/05/24 20:43:32  jay
Add SetContactPos so we can avoid cue intersections

Revision 1.1.2.3  2005/05/15 17:18:21  jay
Allow user to move contact pos without clicking on the GUI.

Revision 1.1.2.2  2005/05/08 10:03:09  jay
Bug fix, now dies not activate until you click on it.

Revision 1.1.2.1  2005/04/09 20:41:35  jay
Added new GUI element for specifying the cue contact position on the cue ball.

*/

#ifndef GUI_CONTACT_POS_H_INCLUDED
#define GUI_CONTACT_POS_H_INCLUDED

#include "SharedPtr.h"
#include "TexturedQuad.h"

namespace Amju
{
// GUI Element with single purpose: to control the cue contact position
// on the cue ball.
// When the user clicks on this button, the contact pos is centred on the
// mouse cursor.
// NB This works differently from other GUI Elements - currently it isn't
// in the class tree.
class GuiContactPos : public Shareable
{
public:
  GuiContactPos();

  bool Load();
  void Draw();
  void MousePos(int x, int y);
  void MouseButton(bool down, bool ctrl, bool shift);

  // Get contact position as offset from (0, 0). The max distance from the
  // origin is 1. I.e. length of hypot. <= 1.0.
  // The idea is that non-zero x => english, or spin;
  // non-zero y => roll or draw, and some cue ball jump.
  void GetContactPos(float* x, float* y);

  // Set pos - used if new pos turns out to be invalid - i.e. cue
  // intersects ball or table.
  void SetContactPos(float x, float y);

  // Reset spot to centre of ball.
  void ResetContactPos();

  // Enable the control with out the mouse being over it.
  void SetEnabled(bool);

  bool IsEnabled() const;

protected:
  TexturedQuad m_bg;
  TexturedQuad m_spot;

  // Position of spot on bg. 
  // These coords are relative to the top-left coord of the background. 
  float m_top, m_left;

  // true if mouse button down, but NB the cursor should be over the
  // GUI area to activate it.
  bool m_buttonDown;

  // If disabled, draw translucent or hidden. User enables this control
  // by mousing over it.
  bool m_isEnabled;

  // Set if mouse cursor is on the GUI.
  bool m_mouseInGuiArea;

  static float SIZE;
  static float LEFT;
  static float TOP;
  static float SPOT_SIZE;

};
}

#endif


