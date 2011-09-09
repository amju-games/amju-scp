/*
Amju Games source code (c) Copyright Jason Colman 2006
$Log: GuiText.h,v $
Revision 1.1.2.2  2007/03/09 21:48:56  jay
Copy latest GUI library from trunk

Revision 1.2  2006/01/16 13:27:55  jay
Add new GuiElements for future use

*/

#ifndef GUI_TEXT_H_INCLUDED
#define GUI_TEXT_H_INCLUDED

#include <string>
#include <vector>
#include "GuiElement.h"

namespace Amju
{
class PoolFont;

// A GuiElement displaying some text.
class GuiText : public GuiElement
{
public:
  GuiText();
  virtual void DrawImpl();
  virtual void MousePos(int, int);
  virtual void MouseButton(bool, bool, bool);

  void SetText(const std::string& s);
  void SetFont(PoolFont* pFont);

protected:
  PoolFont* m_pFont;
  typedef std::vector<std::string> Strings;
  Strings m_strings;
};
}

#endif

