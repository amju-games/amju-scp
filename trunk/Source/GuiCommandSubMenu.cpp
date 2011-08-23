/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: GuiCommandSubMenu.cpp,v $
Revision 1.1  2004/09/08 15:43:07  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "GuiCommandSubMenu.h"
#include "GuiElement.h"
#include "GuiMenu.h"

namespace Amju
{
GuiCommandSubMenu::GuiCommandSubMenu(GuiElement* pSubMenu, GuiMenu* pParentMenu) 
  : m_pSubMenu(pSubMenu), m_pParentMenu(pParentMenu)
{
  m_pParentMenu->AddChild(m_pSubMenu);
  m_pSubMenu->SetVisible(false);
}

bool GuiCommandSubMenu::Do()
{
  // Set the position of the submenu: relative to the parent item, it's
  // at the same height, at the end of the width of the parent.
  float w, h;
  // Get size of GUI Element owner of this Command.
  m_pGui->GetSize(&w, &h);
  // Set the position of the Submenu to the end of the gui parent
  m_pSubMenu->SetRelPos(0, w); // top, left
  m_pSubMenu->SetParent(m_pGui);
  m_pSubMenu->SetVisible(true);
  return false; // can't undo
}

}

