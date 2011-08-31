/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: GuiContactPos.cpp,v $
Revision 1.1.2.3  2006/08/30 21:12:16  Administrator
Get Pool to build in MSVC

Revision 1.1.2.2  2006/08/14 17:50:20  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:36  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.10  2005/08/03 21:05:52  Administrator
Fixed return value

Revision 1.1.2.9  2005/07/17 23:03:27  jay
Configurable size/pos

Revision 1.1.2.8  2005/06/13 22:15:00  jay
Moved GUI item to lower left

Revision 1.1.2.7  2005/06/04 22:33:22  jay
Changes to debug output

Revision 1.1.2.6  2005/05/24 20:43:32  jay
Add SetContactPos so we can avoid cue intersections

Revision 1.1.2.5  2005/05/15 17:18:21  jay
Allow user to move contact pos without clicking on the GUI.

Revision 1.1.2.4  2005/05/08 10:03:09  jay
Bug fix, now dies not activate until you click on it.

Revision 1.1.2.3  2005/04/17 22:01:40  jay
Minor debug tweak

Revision 1.1.2.2  2005/04/11 21:22:18  jay
Minor debug changes

Revision 1.1.2.1  2005/04/09 20:41:35  jay
Added new GUI element for specifying the cue contact position on the cue ball.

*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "Common.h"
#include "GuiContactPos.h"
#include "Engine.h"
#include "Mouse.h"
#include "TextWriter.h"

//#define  CONTACT_GUI_DEBUG

namespace Amju
{
float GuiContactPos::SIZE = 2.5f;
float GuiContactPos::LEFT = 2.0f;
float GuiContactPos::TOP = 12.5f;
float GuiContactPos::SPOT_SIZE = 0.6f;

GuiContactPos::GuiContactPos()
{
  m_top = 0;
  m_left = 0;
  m_buttonDown = false;
  m_isEnabled = true;
}

bool GuiContactPos::Load()
{
  SIZE = Engine::Instance()->GetConfigFloat("contact_pos_size");
  LEFT = Engine::Instance()->GetConfigFloat("contact_pos_left");
  TOP = Engine::Instance()->GetConfigFloat("contact_pos_top");
  SPOT_SIZE = Engine::Instance()->GetConfigFloat("contact_pos_spot_size"); 

  std::string ball = Engine::Instance()->GetConfigValue("golf_ball_tq");
  std::string ballAlpha = Engine::Instance()->GetConfigValue("golf_ball_tq_a");
  if (!m_bg.Load(ball, ballAlpha))
  {
    return false;
  }

  if (!m_spot.Load("black32.bmp", "cross32.bmp")) // TODO CONFIG
  {
    return false;
  }
  return true;
}

void GuiContactPos::Draw()
{
  float a = 1.0f;
  if (!m_isEnabled)
  {
    a = 0.4f;
  }

  Engine::Instance()->PushColour(1.0f, 1.0f, 1.0f, a);

  m_bg.Draw(TOP, LEFT, TOP + SIZE, LEFT + SIZE); // t, l, b, r
  m_spot.Draw(TOP + m_top - SPOT_SIZE * 0.5f, LEFT + m_left - SPOT_SIZE * 0.5f, TOP + m_top + SPOT_SIZE * 0.5f, LEFT + m_left + SPOT_SIZE * 0.5f);

  Engine::Instance()->PopColour();
}

void GuiContactPos::SetContactPos(float x, float y)
{
  Assert(fabs(x) <= 1.0f);
  Assert(fabs(y) <= 1.0f);

  m_left = (x + 1.0f) * SIZE * 0.5f;
  m_top = (y + 1.0f) * SIZE * 0.5f;

#ifdef CONTACT_GUI_DEBUG
std::cout << "CONTACT GUI: Set pos: x: " << x << " y: " << y << " m_left->" << m_left << " m_top->" << m_top << "\n";
#endif
}

void GuiContactPos::GetContactPos(float* x, float* y)
{
  // Convert m_top/m_left into the required range.
  // If m_top is midway between TOP and TOP + SIZE, it's zero. At either of these
  // extremes, it's +/-1. 
  *x = m_left / SIZE * 2.0f - 1.0f;
  *y = m_top / SIZE * 2.0f - 1.0f;

#ifdef CONTACT_GUI_DEBUG
std::cout << "CONTACT GUI: Getting contact pos: x: " << *x << " y: " << *y << "\n";
std::cout << " Details: m_left: " << m_left << " m_top: " << m_top << " SIZE: " << SIZE << "\n";
#endif
}

void GuiContactPos::ResetContactPos()
{
  m_left = SIZE * 0.5f;
  m_top = SIZE * 0.5f;
  m_buttonDown = false;
  m_mouseInGuiArea = false;
  m_isEnabled = false;
}

void GuiContactPos::MousePos(int x, int y)
{
  static int oldx = x;
  static int oldy = y;
  if (!m_isEnabled)
  {
    oldx = x;
    oldy = y;
    return;
  }

  int dx = oldx - x;
  int dy = oldy - y;
  oldx = x;
  oldy = y;

  static const float sens = 0.1f; // TODO CONFIG
  float oldtop = m_top;
  float oldleft = m_left;
  m_top -= (float)dy * sens;
  m_left -= (float)dx * sens;
  float x1 = m_left / SIZE * 2.0f - 1.0f;
  float y1 = m_top / SIZE * 2.0f - 1.0f;
  float dsq = x1 * x1 + y1 * y1;
  // Max value of dsq is 1.0f.
  if (dsq > 0.75f) // TODO CONFIG
  {
#ifdef CONTACT_GUI_DEBUG
std::cout << "CONTACT GUI: Position is OFF CUE BALL!\n";
#endif

    // Position is off edge of cue ball, so revert to earlier values.
    m_top = oldtop;
    m_left = oldleft;
  }

  if (m_top < 0) m_top = 0;
  if (m_top > SIZE) m_top = SIZE;
  if (m_left < 0) m_left = 0;
  if (m_left > SIZE) m_left = SIZE;
  
#ifdef CONTACT_GUI_DEBUG
std::cout << "CONTACT GUI: Mouse coord is IN GUI RADIUS\n";
std::cout << "CONTACT GUI: new m_left: " << m_left << " new m_top: " << m_top << "\n";
#endif

/*
  // Get the current mouse pos.
  // Get the coords of this GUI element in mouse coords, and the radius.
  static const float CENTRE_MOUSE_X = 
    Engine::Instance()->GetConfigFloat("pool_contact_mousex");
  static const float CENTRE_MOUSE_Y = 
    Engine::Instance()->GetConfigFloat("pool_contact_mousey");
  static const float MOUSE_RADIUS = 
    Engine::Instance()->GetConfigFloat("pool_contact_mouseradius");

  // Centre mouse X is fraction of window width. So get current window size.
  // Centre mouse Y is fraction of window height. 
  // Radius is fraction of width.
  int windowW = Engine::Instance()->GetWindowX();
  int windowH = Engine::Instance()->GetWindowY();
  int centrex = (int)(CENTRE_MOUSE_X * (float)windowW);
  int centrey = (int)(CENTRE_MOUSE_Y * (float)windowH);
  int radius = (int)(MOUSE_RADIUS * (float)windowW);

  // Work out if the mouse pos is in the GUI area - we know the centre and radius
  // in mouse coords.
  // TODO
  int dx = x - centrex;
  int dy = y - centrey;
  int d = (int)sqrt(dx * dx + dy * dy);
  if (d < radius)
  {
#ifdef CONTACT_GUI_DEBUG
std::cout << "CONTACT GUI: x: " << x << " y: " << y << " centrex: " << centrex << " centrey: " << centrey << " radius: " << radius << "\n";
#endif

    m_mouseInGuiArea = true;
    m_isEnabled = true;

    if (m_buttonDown)
    {
      m_left = (float)((float)(x - (centrex - radius))/(float)(2 * radius))  * SIZE;
      m_top = (float)((float)(y - (centrey - radius))/(float)(2 * radius)) * SIZE;
  
      if (m_top < 0) m_top = 0;
      if (m_top > SIZE) m_top = SIZE;
      if (m_left < 0) m_left = 0;
      if (m_left > SIZE) m_left = SIZE;

#ifdef CONTACT_GUI_DEBUG
std::cout << "CONTACT GUI: Mouse coord is IN GUI RADIUS\n";
std::cout << "CONTACT GUI: new m_left: " << m_left << " new m_top: " << m_top << "\n";
#endif
    }
  }
  else
  {
    m_mouseInGuiArea = false;
    m_isEnabled = false;
  }
*/ 
}

void GuiContactPos::MouseButton(bool down, bool ctrl, bool shift)
{
  if (down)
  {
    m_buttonDown = m_mouseInGuiArea;
  }
  else
  {
    m_buttonDown = false;
  }
}

void GuiContactPos::SetEnabled(bool b)
{
  m_isEnabled = b;
}

bool GuiContactPos::IsEnabled() const
{
  return m_isEnabled;
}
}

