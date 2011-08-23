/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: GuiCommandUndo.h,v $
Revision 1.1  2004/09/08 15:43:07  jay
Added to repository
  
*/

#if !defined(AMJU_GUI_COMMAND_UNDO_H_INCLUDED)
#define AMJU_GUI_COMMAND_UNDO_H_INCLUDED
#include "GuiCommandHandler.h"
namespace Amju
{
class GuiUndoCommand : public GuiCommand
{
public:
  virtual bool Do()
  {
    if (GuiCommandHandler::Instance()->CanUndo())
    {
      GuiCommandHandler::Instance()->Undo();
    }
    return false;
  }
};

class GuiRedoCommand : public GuiCommand
{
public:
  virtual bool Do()
  {
    if (GuiCommandHandler::Instance()->CanRedo())
    {
      GuiCommandHandler::Instance()->Redo();
    }
    return false;
  }
};
}
#endif

