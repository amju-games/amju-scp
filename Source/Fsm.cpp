/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Fsm.cpp,v $
Revision 1.1  2004/09/08 15:42:49  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "Fsm.h"
#include "Message.h"

using namespace std;

namespace Amju
{
State Fsm::GetNewState(State currentState, const Message& m)
{
  if (m_transitions.find(Input(currentState, m.GetCode())) != m_transitions.end())
  {
    // There is a matching transition. Return the new state.
    return m_transitions[Input(currentState, m.GetCode())];
  }
  // No match. Return the current state.
  return currentState;
}
}
