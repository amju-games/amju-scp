/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Fsm.h,v $
Revision 1.1  2004/09/08 15:42:49  jay
Added to repository
  
*/

#if !defined(FSM_H_INCLUDED)
#define FSM_H_INCLUDED

#include "States.h"
#include <map>
#include <utility>

namespace Amju
{
class Message;

// Finite State Machine, which Game Object may point to.
class Fsm
{
public:
  // Return new state given current state and a message.
  State GetNewState(State currentState, const Message& m);

  // Add a transition to the map.
  void AddTransition(State currentState, int input, State newState);

protected:
  // Map of <current state, input> to <output state>.
  typedef std::pair<State, int> Input;
  typedef std::map<Input, State> TransitionMap;
  TransitionMap m_transitions;
};
}
#endif

