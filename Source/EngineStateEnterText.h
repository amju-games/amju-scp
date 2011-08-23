/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStateEnterText.h,v $
Revision 1.1.10.2  2005/10/29 10:16:51  jay
Added SpecialEvent handler so user can paste in text.

Revision 1.1.10.1  2005/05/24 20:33:33  jay
Enforce max string length

Revision 1.1  2004/09/08 15:42:36  jay
Added to repository
  
*/

#if !defined(ENGINE_STATE_ENTER_TEXT_H_INCLUDED)
#define ENGINE_STATE_ENTER_TEXT_H_INCLUDED

#include "EngineStateText.h"
#include "Menu.h"
#include "TextLeaf.h"

namespace Amju
{
class EngineStateEnterText : public EngineStateText
{
public:
  virtual void SetActive(bool);
  virtual void Draw();
  virtual void DrawOverlays();
  virtual bool Load();
  virtual void OnKey(char key, bool down);
  // Handle paste
  virtual void OnSpecialEvent(SpecialEvent*);

  // Left and right cursor keys
  virtual void PlusLeft(bool);
  virtual void PlusRight(bool);

  // Mouse for menu
  virtual void MousePos(int x, int y);
  virtual void MouseButton(bool down, bool ctrl, bool shift);

  // Reset text to empty string.
  void ResetText();

protected:
  // Insert character at current position 
  void Insert(char c);
  // Make 3d text with cursor from m_text
  void MakeText();

  // Called when enter key hit or 'ok' menu item selected.
  virtual void Finished() = 0;

  // Called when 'back' menu item selected.
  virtual void Back() = 0;

  // Called when text is at max length and Insert is called.
  virtual void AlertMaxTextSize() {}

  void DrawCursor();

protected:
  std::string m_text;
  // Current cursor position.
  int m_pos;

  // Text on right hand side of cursor
  PSolidComponent m_pComp2;
  PSolidComponent m_pCursor;

  float m_leftSize; // extent of left-of-cursor text
  float m_totalSize; // full extent of text

  // Also we have a menu for 'back' and 'ok'
  PMenu m_pMenu;
  // Is mouse down flag - for mouse menu selection.
  bool m_mouseDown;

  // Test greeting/prompt
  TextLeaf m_greet;

  // Max length of m_text
  int m_maxTextSize;
};
}

#endif

