/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: PoolGuiElement.cpp,v $
Revision 1.1.10.9  2007/03/09 21:48:55  jay
Copy latest GUI library from trunk

Revision 1.10  2007/02/27 10:22:55  jay
Minor fixes

Revision 1.9  2007/01/13 13:26:30  jay
Handle ESC in OnKey; Cancel flag

Revision 1.8  2006/12/15 12:42:54  jay
Fix IsEnabled() for Composite GUI Elements: don't use m_isEnabled directly!

Revision 1.7  2006/12/11 22:05:51  jay
OnClicked: pass call on to Parent if no command

Revision 1.6  2006/11/14 00:02:50  Administrator
More accurate mouse region

Revision 1.5  2006/06/17 12:42:14  jay
Added PlayerSelectable flag - button cannot be clicked but does not look
disabled. (Limited use)

Revision 1.4  2006/05/30 22:20:25  jay
Don't test colour of pixel under mouse - just use approx rectangle test.

Revision 1.3  2006/01/20 13:03:44  jay
No tool tips for now

Revision 1.2  2006/01/16 13:24:56  jay
Merge improvements from pool; check mouse coords before expensive mouse
over test.

Revision 1.1.10.8  2005/10/28 12:50:34  jay
Add GetWidth, GetHeight

Revision 1.1.10.7  2005/08/26 22:06:35  jay
Add MouseOver(), add click WAV, no ToolTip Text

Revision 1.1.10.6  2005/08/02 18:36:15  jay
Remove tool tip text for v.1.1

Revision 1.1.10.5  2005/07/11 07:32:52  jay
Attempt to fix tool tip text off right of window

Revision 1.1.10.4  2005/07/05 12:55:56  jay
Hide debug output

Revision 1.1.10.3  2005/07/05 09:44:05  jay
Added tooltip-style help message over GUI elements.

Revision 1.1.10.2  2005/04/11 21:16:50  jay
Transparent if not selected.

Revision 1.1.10.1  2005/04/05 21:51:25  jay
Elements are now drawn translucent if disabled

Revision 1.1  2004/09/08 15:43:07  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "GuiElement.h"
#include "GuiCommandHandler.h"
#include "Engine.h"
#include "Mouse.h"
#include "TextWriter.h"
#include "Font.h"

#define NO_TOOL_TIPS

namespace Amju
{
TexturedQuad PoolGuiElement::s_bg;

bool PoolGuiElement::Init()
{
  static bool first = true;
  if (!first)
  {
    return true;
  }
  first = false;
/*
  std::string bg = Engine::Instance()->GetConfigValue("gui_bg");
  std::string bg_a = Engine::Instance()->GetConfigValue("gui_bg_a");
  if (!s_bg.Load(bg, bg_a))
  {
    return false;
  }
*/
  return true;
}

PoolGuiElement::PoolGuiElement()
{
  m_relTop = 0;
  m_relLeft = 0;
  m_absTop= 0;
  m_absLeft = 0;
  m_width = 0;
  m_height = 0;
  m_isSelected = false;
  m_selectedTime = 0;
  m_downSelected = false;
  m_isEnabled = true;
  m_pParent = 0;
  SetVisible(true);
  m_canDrag = false; // not normally draggable
  m_pCommandFunc = 0;
  // By default, check for mouse over. Turn off for more speed.
  SetCheckMouseOver(true);
  m_isPlayerSelectable = true;
  m_isCancel = false;
}

void PoolGuiElement::OnKey(char c)
{
  // Handle ESC
  if (c == 27 /* ESC */ && IsCancel())
  {
    OnClicked(); 
  }
}

bool PoolGuiElement::IsCancel() const
{
  return m_isCancel;
}

void PoolGuiElement::SetIsCancel(bool isCancel)
{
  m_isCancel = isCancel;
}

bool PoolGuiElement::IsPlayerSelectable() const
{
  return m_isPlayerSelectable;
}

void PoolGuiElement::SetIsPlayerSelectable(bool isSelectable)
{
  m_isPlayerSelectable = isSelectable;
}

void PoolGuiElement::SetCheckMouseOver(bool b)
{
  m_checkMouseOver = b;
}

bool PoolGuiElement::GetCheckMouseOver() const
{
  return m_checkMouseOver;
}

void PoolGuiElement::SetRelPos(float top, float left)
{
  m_relTop = top;
  m_relLeft = left;

  if (m_pParent)
  {
    float parentTop, parentLeft;
    m_pParent->GetAbsPos(&parentTop, &parentLeft);
    m_absTop = parentTop + m_relTop;
    m_absLeft = parentLeft + m_relLeft;
  }
  else
  {
    m_absTop = m_relTop;
    m_absLeft = m_relLeft;
  }
}

void PoolGuiElement::GetRelPos(float* pTop, float* pLeft)
{
  *pTop = m_relTop;
  *pLeft = m_relLeft;
}

void PoolGuiElement::SetAbsPos(float top, float left)
{
  m_absTop = top;
  m_absLeft = left;
}

void PoolGuiElement::GetAbsPos(float* pTop, float* pLeft)
{
  *pTop = m_absTop;
  *pLeft = m_absLeft;
}

void PoolGuiElement::SetSize(float w, float h)
{
  m_width = w;
  m_height = h;
}

void PoolGuiElement::GetSize(float* pWidth, float* pHeight) const
{
  *pWidth = m_width;
  *pHeight = m_height;
}

float PoolGuiElement::GetWidth() const
{
  return m_width;
}

float PoolGuiElement::GetHeight() const
{
  return m_height;
}

void PoolGuiElement::MousePos(int x, int y)
{
}

void PoolGuiElement::MouseButton(bool down, bool ctrl, bool shift)
{
  if (!IsVisible())
  {
    return;
  }

  if (down)
  {
    m_downSelected = (m_isSelected && IsEnabled());
  }

  // If mouse button up, this element was selected when the mouse button was
  // pressed down, and is selected now, then do the clicked action.
  if (!down && m_downSelected && m_isSelected)
  {
    OnClicked();
  }

  if (!down)
  {
    m_downSelected = false;
  }

  Mouse::s_mouseDown = down;
}

bool PoolGuiElement::IsEnabled() const
{
  // Traverse parent, grandparent etc to find any disabled ancestor
  if (!m_isEnabled)
  {
    return false;
  }
  if (m_pParent)
  {
    return m_pParent->IsEnabled();
  }
  return true;
}

void PoolGuiElement::SetEnabled(bool b)
{
  m_isEnabled = b;
}

bool PoolGuiElement::IsSelected() const
{
  return m_downSelected;
}

bool PoolGuiElement::IsMouseOver() const
{
  return m_isSelected;
}

void PoolGuiElement::DrawNoCheckMouseOver()
{
  static const float GUI_SELECT_OFFSET = Engine::Instance()->GetConfigFloat("gui_select_offset");

  float oldRelTop = m_relTop;
  float oldRelLeft = m_relLeft;
  if (m_downSelected)
  {
    SetRelPos(m_relTop + GUI_SELECT_OFFSET, m_relLeft + GUI_SELECT_OFFSET); 
  }

  DrawImpl();

  if (m_downSelected)
  {
    SetRelPos(oldRelTop, oldRelLeft);
  }
}

void PoolGuiElement::Draw()
{
  static const float GUI_SELECT_OFFSET = Engine::Instance()->GetConfigFloat("gui_select_offset");

  if (!IsVisible())
  {
    return;
  }

  float windowX = (float)Engine::Instance()->GetWindowX();
  float windowY = (float)Engine::Instance()->GetWindowY();

#ifdef MOUSE_ON_SCREEN_DEBUG
  // Mouse cursor within window ?
  if (Mouse::s_mousex < 0)
  {
    std::cout << "Mouse off LEFT<<< side of window!\n";
  }
  if (Mouse::s_mousex > windowX) 
  {
    std::cout << "Mouse off RIGHT>>> side of window!\n";
  }
  if (Mouse::s_mousey < 0)
  {
    std::cout << "Mouse off TOP^^^ of window!\n";
  }
  if (Mouse::s_mousey > windowY) 
  {
    std::cout << "Mouse off BOTTOMvvv of window!\n";
  }
#endif
  bool mouseOut = (Mouse::s_mousex < 0 || 
    Mouse::s_mousey < 0 || 
    Mouse::s_mousex > windowX || 
    Mouse::s_mousey > windowY);
  if (mouseOut)
  {
    m_isSelected = false;
    m_selectedTime = 0;
    m_downSelected = false;
  }

  // Translucent if disabled.
  // TODO Fade from fully opaque to fully translucent and back
  float tr = 1.0f;
  if (!IsEnabled())
  {
    tr = 0.4f;
    Engine::Instance()->PushColour(1.0f, 1.0f, 1.0f, tr);
    DrawImpl();
    Engine::Instance()->PopColour();
    m_isSelected = false;
    m_selectedTime = 0;
    return;
  }

  if (mouseOut)
  {
    DrawImpl();
    return;
  }

  // Work out if the mouse cursor is over this Element's rectangle.
  // If it isn't we don't have to check for possible mouse over.
  // But if the mouse was over this button, we must keep checking.

  static const float GRID_X_OFF = 0.5f;
  static const float GRID_Y_OFF = 0.1f;
  static const float MAX_GRID_X = 24.0f;
  static const float MAX_GRID_Y = 16.0f;
  // Convert rectangle position (TextWriter coords) to mouse coords (pixels).
  int pixTop = (int)((m_absTop - GRID_Y_OFF) / MAX_GRID_Y * windowY);
  int pixBottom = (int)((m_absTop + m_height - GRID_Y_OFF) / MAX_GRID_Y * windowY);
  int pixLeft = (int)((m_absLeft - GRID_X_OFF) / MAX_GRID_X * windowX);
  int pixRight = (int)((m_absLeft + m_width - GRID_X_OFF) / MAX_GRID_X * windowX);

  bool checkMouse = 
    Mouse::s_mousey > pixTop && Mouse::s_mousey < pixBottom &&
    Mouse::s_mousex > pixLeft && Mouse::s_mousex < pixRight;

#ifdef CHECKMOUSE_DEBUG
std::cout << "GuiElement top: " << m_absTop << " left: " << m_absLeft
  << " bottom: " << m_absTop + m_height << " right: " << m_absLeft + m_width
  << "\n";
std::cout << "pixTop: " << pixTop << " pixLeft: " << pixLeft 
  << " pixBottom: " << pixBottom << " pixRight: " << pixRight << "\n";
std::cout << "Mousex: " << Mouse::s_mousex << " mousey: " << Mouse::s_mousey 
  << "\n";
std::cout << "Checkmouse: " << checkMouse << "\n";
#endif

  if (!IsMouseOver() && !checkMouse)  //////!GetCheckMouseOver())
  {
    DrawNoCheckMouseOver();
    return;
  }

  bool selected = m_downSelected && IsPlayerSelectable();

  float oldRelTop = m_relTop;
  float oldRelLeft = m_relLeft;
  if (selected)
  {
    SetRelPos(m_relTop + GUI_SELECT_OFFSET, m_relLeft + GUI_SELECT_OFFSET); 
  }

  // If MOUSE_PICK_READ_PIXEL is defined, we test the pixel under the
  // mouse cursor to see if we change its colour by drawing the button.
  // The idea is to give pixel-perfect tests for odd-shaped buttons
  // but it is buggy. It doesn't detect black pixels in the button, and
  // can cause flicker at the edges as the button may be moved out from
  // under the mouse cursor!
  // Better to just use the mouse position and use roughly rectangular 
  // buttons.
#ifdef MOUSE_PICK_READ_PIXEL
  Engine::Instance()->PushColour(0.5f, 0.5f, 0.5f, 1.0f);
  DrawImpl();
  Colour c1 = PickHelper::ReadCursorColour();
  Engine::Instance()->PopColour();
#endif // MOUSE_PICK_READ_PIXEL

  DrawImpl();

  if (selected)
  {
    SetRelPos(oldRelTop, oldRelLeft);
  }

#ifdef MOUSE_PICK_READ_PIXEL
  Colour c2 = PickHelper::ReadCursorColour();
  m_isSelected = (c1 != c2);
#else
  m_isSelected = checkMouse;
#endif // MOUSE_PICK_READ_PIXEL

  if (m_isSelected)
  {
    m_selectedTime += Engine::Instance()->GetDeltaTime();
  }
  else
  {
    m_selectedTime = 0;
  }
  if (m_selectedTime > 1.0f && m_selectedTime < 6.0f) // TODO CONFIG
  {
    // TODO Need to fix so that tooltip text is drawn last. 
    // Otherwise buttons etc drawn afterwards can overwrite the tooltip
    // text.
    //DrawToolTipText();
  }
}

void PoolGuiElement::DrawToolTipText()
{
  static PoolFont* pFont = TheFontManager::Instance()->GetFont("tooltip");

  if (m_toolTipText.empty())
  {
    return;
  }
  // Work out if we should draw the text to the left or right of this
  // element.
  //bool left = false;
   
  float x = m_absLeft;
  if (x > 15.0f) // TODO CONFIG
  {
    //left = true;
    float w = pFont->GetTextWidth(m_toolTipText[0]);

#ifdef _DEBUG
std::cout << "TOOLTIP: width: " << w << " for text: " << m_toolTipText[0]
  << "\n";
#endif
    x -= w * 10.0f; // TODO CONFIG
  }
  float y = m_absTop; 

/*
  // TODO fit box to text extent
  float w = 5.0f;
  float h = 2.5f;
  m_toolTipBg.Draw(y, x, y + h, x + w); // t, l, b, r
*/

  for (int i = 0; i < (int)m_toolTipText.size(); i++)
  {
    pFont->PrintNoBlend(x, y, m_toolTipText[i].c_str());
    y += 0.5; // TODO CONFIG
  }
}

bool PoolGuiElement::CanDrag()
{
  return m_canDrag;
}

void PoolGuiElement::DrawBg() const
{
  s_bg.Draw(m_absTop, m_absLeft, m_height + m_absTop, m_width + m_absLeft);
}

void PoolGuiElement::PlayClickWav()
{
  static const std::string wavFile = Engine::Instance()->GetConfigValue(
    "gui_element_click_wav");
  Engine::Instance()->PlayWav(wavFile);
}

void PoolGuiElement::OnClicked()
{
  if (m_pCommandFunc)
  {
    PlayClickWav();
    (m_pCommandFunc)();
  }
  else if (m_pCommand.GetPtr())
  {
    PlayClickWav();
    PoolGuiCommandHandler::Instance()->DoNewCommand(m_pCommand);
  }
  else if (m_pParent)
  {
    // No handler, so try parent: this works if a button is
    // decorated
    m_pParent->OnClicked();
  }
}

void PoolGuiElement::DrawImpl()
{
}

void PoolGuiElement::SetCommand(PoolGuiCommand* pCommand)
{
  m_pCommand = pCommand;
  m_pCommand->SetPoolGuiElement(this);

  m_pCommandFunc = 0;
}

void PoolGuiElement::SetCommand(CommandFunc f)
{
  m_pCommandFunc = f;
  m_pCommand = 0;
}

void PoolGuiElement::SetParent(PoolGuiElement* p)
{
  m_pParent = p;
}

PoolGuiElement* PoolGuiElement::GetParent()
{
  return m_pParent;
}

void PoolGuiElement::SetVisible(bool visible)
{
  if (!visible)
  {
    m_isSelected = false;
    m_downSelected = false;
  }
  m_isVisible = visible;
}

bool PoolGuiElement::IsVisible() const
{
  return m_isVisible;
}

bool PoolGuiElement::Load(const std::string& filename)
{
  File f;
  if (!f.OpenRead(filename))
  {
    f.ReportError("Failed to open file");
    return false;
  }
  if (!Load(&f))
  {
    f.ReportError("Couldn't load GUI element.");
    return false;
  }
  return true;
}

bool PoolGuiElement::LoadToolTipText(File* pf)
{
#ifdef NO_TOOL_TIPS
  return true;
#else
  int numLines = 0;
  if (!pf->GetInteger(&numLines))
  {
    // Return true because it's ok if there is no text specified.
    // Not much good - this generates a warning.
    return true;
  }
  if (numLines == 0)
  {
    // No help text - that's ok.
    return true;
  }
  for (int i = 0; i < numLines; i++)
  {
    std::string s;
    if (!pf->GetDataLine(&s))
    {
      pf->ReportError("Bad tool tip text.");
      return false;
    }
    m_toolTipText.push_back(s);
  }
  // Load tq box for help text to go in.
  if (!m_toolTipBg.Load(pf))
  {
    pf->ReportError("Failed to load tool tip bg.");
    return false;
  }
  return true;
#endif
}
}

