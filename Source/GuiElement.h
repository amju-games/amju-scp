/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: GuiElement.h,v $
Revision 1.1.10.4  2007/03/09 21:48:55  jay
Copy latest GUI library from trunk

Revision 1.5  2007/01/13 13:26:30  jay
Handle ESC in OnKey; Cancel flag

Revision 1.4  2007/01/05 22:50:48  jay
New virtual functions for keyboard input

Revision 1.3  2006/06/17 12:42:14  jay
Added PlayerSelectable flag - button cannot be clicked but does not look
disabled. (Limited use)

Revision 1.2  2006/01/16 13:24:56  jay
Merge improvements from pool; check mouse coords before expensive mouse
over test.

Revision 1.1.10.3  2005/10/28 12:50:34  jay
Add GetWidth, GetHeight

Revision 1.1.10.2  2005/08/26 22:06:35  jay
Add MouseOver(), add click WAV, no ToolTip Text

Revision 1.1.10.1  2005/07/05 09:44:05  jay
Added tooltip-style help message over GUI elements.

Revision 1.1  2004/09/08 15:43:07  jay
Added to repository
  
*/

#if !defined(AMJU_GUI_ELEMENT_H_INCLUDED)
#define AMJU_GUI_ELEMENT_H_INCLUDED

#include "RCPtr.h"
#include "GuiCommandHandler.h"
#include "TexturedQuad.h"

namespace Amju
{
class File;

// Base class for 2D GUI components.
class GuiElement : public RefCounted
{
public:
  // Call on startup to initialise the GUI elements.
  static bool Init();

  GuiElement();
  virtual ~GuiElement() {}
  virtual void DrawImpl();
  virtual bool Load(File*) { return true; }
  virtual void MousePos(int x, int y);
  virtual void MouseButton(bool down, bool ctrl, bool shift);

  // Default implementation handles ESC if this element is a Cancel Element.
  // Overrides should probably call this base impl.
  virtual void OnKey(char c);

  // Cursor keys
  virtual void OnPlusUp(bool down) {}
  virtual void OnPlusDown(bool down) {}
  virtual void OnPlusLeft(bool down) {}
  virtual void OnPlusRight(bool down) {}

  // Returns true if mouse cursor is above element and mouse button is down.
  virtual bool IsSelected() const;

  // TODO Not sure why this is public.
  // Called when this Element is Clicked on by the user; 
  //  executes any Command set.
  void OnClicked();

  // Set position relative to parent Element.
  // (If there's no parent this is the absolute coord.)
  // Virtual so composites can set new absolute positions for children.
  virtual void SetRelPos(float top, float left);
  void GetRelPos(float* pTop, float* pLeft);

  // Set absolute position. Virtual so composites can set Child positions.
  virtual void SetAbsPos(float top, float left);
  void GetAbsPos(float* pTop, float* pLeft);

  void SetSize(float w, float h);
  void GetSize(float* pWidth, float* pHeight) const;
  float GetWidth() const;
  float GetHeight() const;

  // NB This is not virtual, but calls virtual DrawImpl() to draw
  // the subclass details.
  void Draw();

  // Set a Command which will be executed when the element is clicked.
  void SetCommand(GuiCommand*);
  // Or, set a non-undoable function call to be executed when the
  //  element is clicked.
  typedef void (*CommandFunc)();
  void SetCommand(CommandFunc f);

  bool IsEnabled() const;
  void SetEnabled(bool);

  void SetParent(GuiElement*);
  GuiElement* GetParent();

  void SetVisible(bool visible);
  bool IsVisible() const;

  // Open file and call virtual Load()
  bool Load(const std::string& filename);

  // Returns true if mouse cursor is above this element.
  bool IsMouseOver() const;
  // Set flag for mouse over to be tested every time we draw.
  // This slows down drawing as we test the back buffer.
  void SetCheckMouseOver(bool );
  bool GetCheckMouseOver() const;

  bool IsPlayerSelectable() const;
  void SetIsPlayerSelectable(bool isSelectable);

  // Set if this element has a 'Cancel' or 'back' command set. 
  // Then if ESC is pressed, the command will be executed.
  bool IsCancel() const;
  void SetIsCancel(bool isCancel);

protected:
  // If true, moving the mouse drags this Element.
  virtual bool CanDrag();

  virtual void DrawToolTipText();

  virtual void PlayClickWav();

protected:
  void DrawBg() const;

  // If file version has tool tip text call this to load it.
  bool LoadToolTipText(File*);

  // Draw() calls this function if no mouse over checking.
  void DrawNoCheckMouseOver();

protected:
  // True if this item is enabled.
  bool m_isEnabled;
  // True if mouse cursor overlaps this element.
  bool m_isSelected;

  // Time for which this element has been selected. Used to decide if it's
  // time to draw a tool tip.
  float m_selectedTime;

  // True if selected when mouse button pressed down.
  bool m_downSelected;
  // size and position.
  float m_absTop, m_absLeft, m_relTop, m_relLeft;
  float m_width, m_height;

  // Command executed if this GUI element is clicked.
  RCPtr<GuiCommand> m_pCommand;
  CommandFunc m_pCommandFunc;

  // Gui Element which is the parent of this one.
  GuiElement* m_pParent;

  // Visible flag, especially useful for submenu children of menus.
  bool m_isVisible;

  // If true, this element is draggable.
  bool m_canDrag;

  // Background for all GUI elements.
  static TexturedQuad s_bg;

  // "Tooltip"-style text - appears when mouse hovers over GUI Element.
  typedef std::vector<std::string> ToolTipText;
  ToolTipText m_toolTipText;

  // Box in which tooltip text is drawn.
  TexturedQuad m_toolTipBg;

  // If flag is set, we check for mouse over every time we draw.
  // If flag is not set, we only check for mouse over if a mouse button
  // is down.
  bool m_checkMouseOver;

  // If true, element responds to mouse events
  bool m_isPlayerSelectable;

  bool m_isCancel;
};
typedef RCPtr<GuiElement> PGuiElement;
}

#endif


