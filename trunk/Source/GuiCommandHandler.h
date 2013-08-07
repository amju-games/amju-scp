/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: GuiCommandHandler.h,v $
Revision 1.1  2004/09/08 15:43:07  jay
Added to repository
  
*/

#if !defined(AMJU_GUI_COMMAND_HANDLER_H_INCLUDED)
#define AMJU_GUI_COMMAND_HANDLER_H_INCLUDED

#include <vector>
#include "RCPtr.h"
#include "Singleton.h"

namespace Amju
{
class GuiElement;

class GuiCommand : public RefCounted
{
public:
  GuiCommand() : m_pGui(0) {}

  virtual ~GuiCommand() {}

  // Execute the command. Return true if the command executed successfully
  // and should be added to the undo stack.
  virtual bool Do() = 0;
  virtual void Undo() {}

  // Set/get the GUI element which the user clicks on to execute this
  // command.
  void SetGuiElement(GuiElement* p) { m_pGui = p; }
  GuiElement* GetGuiElement() { return m_pGui; }

protected:
  // The Gui element which owns this Command and fires it when required.
  GuiElement* m_pGui;
};
typedef RCPtr<GuiCommand> PCommand;

// See Command pattern.
class SingleGuiCommandHandler
{
public:
  SingleGuiCommandHandler();
  void Undo();
  void Redo();
  bool CanUndo() const;
  bool CanRedo() const;
  void DoNewCommand(PCommand);

protected:
  // TODO deque ?
  typedef std::vector<PCommand> Commands;
  Commands m_commands;
  Commands::iterator m_it;
};

typedef Singleton<SingleGuiCommandHandler> GuiCommandHandler;
}

#endif

