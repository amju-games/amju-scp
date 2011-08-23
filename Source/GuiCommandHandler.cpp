/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: GuiCommandHandler.cpp,v $
Revision 1.1  2004/09/08 15:43:07  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "GuiCommandHandler.h"

namespace Amju
{
SingleGuiCommandHandler::SingleGuiCommandHandler()
{
  m_it = m_commands.end();
}

void SingleGuiCommandHandler::Undo()
{
  if (!CanUndo())
  {
    return;
  }
  --m_it;
  (*m_it)->Undo();
}

void SingleGuiCommandHandler::Redo()
{
  if (!CanRedo())
  {
    return; 
  }
  (*m_it)->Do();
  ++m_it;
}

bool SingleGuiCommandHandler::CanUndo() const
{
  bool b = (m_it != m_commands.begin() && m_commands.size() > 0);
  return b;
}

bool SingleGuiCommandHandler::CanRedo() const
{
  bool b = (m_it != m_commands.end() && m_commands.size() > 0);
  return b;
}

void SingleGuiCommandHandler::DoNewCommand(PCommand p)
{
  if (!p.GetPtr())
  {
    return;
  }

  if (p->Do())
  {
    // Is undoable, so add to undo list
    // Any undone commands are erased, so cannot be redone.
    m_commands.erase(m_it, m_commands.end());
    m_commands.push_back(p);
    m_it = m_commands.end();
  }
}
}


