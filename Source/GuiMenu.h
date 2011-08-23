/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: GuiMenu.h,v $
Revision 1.1.10.1  2007/03/09 21:48:56  jay
Copy latest GUI library from trunk

Revision 1.4  2006/12/11 22:07:05  jay
Creator (for each menu item) sets text and font of the item

Revision 1.3  2006/12/11 09:43:24  jay
Factor out ButtonCreator

Revision 1.2  2006/12/03 21:24:51  jay
Changes to make GuiMenu really useable

Revision 1.1  2004/09/08 15:43:07  jay
Added to repository
  
*/

#if !defined(AMJU_GUI_MENU_H_INCLUDED)
#define AMJU_GUI_MENU_H_INCLUDED

#include "GuiComposite.h"
#include "Font.h"
#include "ButtonCreator.h"

namespace Amju
{
class GuiMenu : public GuiComposite
{
public:
  GuiMenu();
  virtual bool Load(File*);
  void SetButtonCreator(ButtonCreator*);
  void SetFont(Font* pFont);

protected:
  GuiElement* CreateGuiTextButton(Font* pFont, const std::string& text);

protected:
  // Title bar above menu items
  SharedPtr<GuiElement> m_pTitlebar;

  SharedPtr<ButtonCreator> m_pButtonCreator;

  Font* m_pFont;
};
}

#endif

