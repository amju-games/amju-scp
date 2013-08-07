/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: PoolGuiCommandHandler.cpp,v $
Revision 1.1  2004/09/08 15:43:07  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "GuiCommandHandler.h"

namespace Amju
{
SinglePoolGuiCommandHandler::SinglePoolGuiCommandHandler()
{
  m_it = m_commands.end();
}

void SinglePoolGuiCommandHandler::Undo()
{
  if (!CanUndo())
  {
    return;
  }
  --m_it;
  (*m_it)->Undo();
}

void SinglePoolGuiCommandHandler::Redo()
{
  if (!CanRedo())
  {
    return; 
  }
  (*m_it)->Do();
  ++m_it;
}

bool SinglePoolGuiCommandHandler::CanUndo() const
{
  bool b = (m_it != m_commands.begin() && m_commands.size() > 0);
  return b;
}

bool SinglePoolGuiCommandHandler::CanRedo() const
{
  bool b = (m_it != m_commands.end() && m_commands.size() > 0);
  return b;
}

void SinglePoolGuiCommandHandler::DoNewCommand(PCommand p)
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


