/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: MenuCommand.cpp,v $
Revision 1.1  2004/09/08 15:42:52  jay
Added to repository
  
*/

#include "MenuCommand.h"
#include "Menu.h"

namespace Amju
{
MenuCommand::MenuCommand(Menu* p, float inc) : m_pMenu(p), m_inc(inc)
{
}

bool MenuCommand::Do()
{
  float t = m_pMenu->GetTop();
  m_pMenu->SetTop(t + m_inc); 
  return false; // can't undo
}
}
