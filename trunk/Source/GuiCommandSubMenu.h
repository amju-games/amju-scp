/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: GuiCommandSubMenu.h,v $
Revision 1.1  2004/09/08 15:43:07  jay
Added to repository
  
*/

#if !defined(AMJU_GUI_COMMAND_SUB_MENU_H_INCLUDED)
#define AMJU_GUI_COMMAND_SUB_MENU_H_INCLUDED

#include "GuiCommandHandler.h"

namespace Amju
{
class GuiMenu;
  
// This command displays a submenu when executed.
class GuiCommandSubMenu : public GuiCommand
{
public:
  GuiCommandSubMenu(GuiElement* pSubMenu, GuiMenu* pParentMenu);
  virtual bool Do();

protected:
  // The submenu which is activated when this command is executed.
  GuiElement* m_pSubMenu;
  // The Menu for which we are activating a submenu.
  GuiMenu* m_pParentMenu;
  // NB m_pGui points to the GUI element which owns this command,
  // which will be a menu item.
};
}

#endif

