/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Strategy.h,v $
Revision 1.1  2004/09/08 15:43:03  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_STRATEGY_H_INCLUDED)
#define SCHMICKEN_STRATEGY_H_INCLUDED

#include "SharedPtr.h"

namespace Amju
{
class Ai;
class File;
class Strategizer;

// Base class for AI strategies.
class Strategy : public Shareable
{
public:
  // Create strategy with a pointer to owning Strategizer, (which decides
  // shich strategy to follow).
  Strategy(Strategizer* pS);
  virtual ~Strategy();

  // Returns true if this strategy is valid.
  virtual bool IsValid() const = 0;

  // Get the Priority: the valid Strategy with the highest priority 
  // should be chosen.
  int GetPriority() const;

  // Update the AI's state.
  // NB This is not called every frame, but is called when the strategy
  // needs to be updated.
  // TODO this is confused
  virtual bool UpdateTask();
  // What to call when e.g. visibility graph changes ?

  // Called when this Strategy becomes Active.
  virtual void OnActivate() = 0;

  // Reset any state, e.g. new level
  virtual void Reset();

  // Called when the AI completes its current task
  virtual void OnTaskFinished();

  // Called when AI is unable to carry out its current task.
  virtual void OnTaskFailed();

  // Load any data required by the Strategy.
  virtual bool Load(File*); 

  // Called when objects in the scene have been moved.
  virtual void RecalculatePath();

  void SetAi(Ai*);

protected:
  int m_priority;

  // The AI which this strategy is controlling.
  Ai* m_pAi;
  // Or just need its ID ?


  Strategizer* m_pStrategizer;
};

typedef SharedPtr<Strategy> PStrategy;
}

#endif
