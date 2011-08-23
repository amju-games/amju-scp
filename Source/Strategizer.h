/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Strategizer.h,v $
Revision 1.1  2004/09/08 15:43:03  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_STRATEGIZER_H_INCLUDED)
#define SCHMICKEN_STRATEGIZER_H_INCLUDED

#include <vector>
#include "Strategy.h"
#include "GameObject.h"

namespace Amju
{
class Ai;
class File;

// Strategizer chooses among its collection of Strategies. It picks the 
// best Strategy for the current game state.
// If the game state changes, the Strategizer must make sure the current
// strategy is still valid. If not, it must pick a new strategy.
// When the current task set by the current strategy is complete, the
// Strategizer picks a new strategy, which may be the same as before.
class Strategizer
{
public:
  Strategizer();

  bool Load(File*);


  // When this function is called the current strategy may be dumped, as 
  // the best curently valid Strategy becomes active. 
  // This is called when
  //  - the current task is completed
  //  - the current task fails
  //  - the game state (e,g, visibility graph) changes
  void ChooseStrategy();

  // Set the AI which owns this Strategizer, and so performs the tasks 
  // set by its current strategy.
  void SetAi(Ai*);

  void OnTaskFinished();

  void OnTaskFailed();

  void SetGoal(PGameObject pGoal);

  // Called when objects in the scene have been moved.
  // We must recalculate th path, and maybe 're-strategize'.
  void RecalculatePath();

  // Refresh validity flag for all strategies
  void RefreshStrategies();

protected:
  typedef std::vector<PStrategy> Strategies;
  Strategies m_strategies;

  PStrategy m_pCurrent;

  // This is the goal we are trying to get to, (and may need to build something
  // in order to reach it). 
  PGameObject m_pGoal;

};
}
#endif
