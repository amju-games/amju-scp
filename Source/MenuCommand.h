/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: MenuCommand.h,v $
Revision 1.1  2004/09/08 15:42:52  jay
Added to repository
  
*/

#if !defined(MENU_COMMAND_H_INCLUDED)
#define MENU_COMMAND_H_INCLUDED

#include "GuiCommandhandler.h"

namespace Amju
{
class Menu;

// Adjust topmost visible item in a menu.
class MenuCommand : public GuiCommand
{
public:
  MenuCommand(Menu*, float inc);
  virtual bool Do();

protected:
  Menu* m_pMenu;
  // m_inc is added to menu top position.
  float m_inc;
};
}

#endif

