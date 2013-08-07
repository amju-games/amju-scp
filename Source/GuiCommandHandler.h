/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: PoolGuiCommandHandler.h,v $
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
class PoolGuiElement;

class PoolGuiCommand : public RefCounted
{
public:
  PoolGuiCommand() : m_pPoolGui(0) {}

  virtual ~PoolGuiCommand() {}

  // Execute the command. Return true if the command executed successfully
  // and should be added to the undo stack.
  virtual bool Do() = 0;
  virtual void Undo() {}

  // Set/get the GUI element which the user clicks on to execute this
  // command.
  void SetPoolGuiElement(PoolGuiElement* p) { m_pPoolGui = p; }
  PoolGuiElement* GetPoolGuiElement() { return m_pPoolGui; }

protected:
  // The PoolGui element which owns this Command and fires it when required.
  PoolGuiElement* m_pPoolGui;
};
typedef RCPtr<PoolGuiCommand> PCommand;

// See Command pattern.
class SinglePoolGuiCommandHandler
{
public:
  SinglePoolGuiCommandHandler();
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

typedef Singleton<SinglePoolGuiCommandHandler> PoolGuiCommandHandler;
}

#endif

