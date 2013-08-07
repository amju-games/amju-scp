/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: PoolGuiCommandUndo.h,v $
Revision 1.1  2004/09/08 15:43:07  jay
Added to repository
  
*/

#if !defined(AMJU_GUI_COMMAND_UNDO_H_INCLUDED)
#define AMJU_GUI_COMMAND_UNDO_H_INCLUDED
#include "GuiCommandHandler.h"
namespace Amju
{
class PoolGuiUndoCommand : public PoolGuiCommand
{
public:
  virtual bool Do()
  {
    if (PoolGuiCommandHandler::Instance()->CanUndo())
    {
      PoolGuiCommandHandler::Instance()->Undo();
    }
    return false;
  }
};

class PoolGuiRedoCommand : public PoolGuiCommand
{
public:
  virtual bool Do()
  {
    if (PoolGuiCommandHandler::Instance()->CanRedo())
    {
      PoolGuiCommandHandler::Instance()->Redo();
    }
    return false;
  }
};
}
#endif

