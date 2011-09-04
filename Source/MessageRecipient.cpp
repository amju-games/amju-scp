/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: MessageRecipient.cpp,v $
Revision 1.1  2004/09/08 15:42:53  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "MessageRecipient.h"
#include "Message.h"
#include "Fsm.h"
#include "Engine.h"
#include "PoolGameState.h"
#include <iostream>

using namespace std;

namespace Amju
{
MessageRecipient::MessageRecipient() 
{
  m_id = 0;
  m_state = UNKNOWN;
}

MessageRecipient::~MessageRecipient()
{
}

void MessageRecipient::SetId(int id)
{
  m_id = id;
}

void MessageRecipient::ReceiveMessage(const Message& m)
{
#if defined(_DEBUG)
/*
  cout << m << endl;
*/
#endif
  // No FSM, so just treat the Message code as the new state.
  SetState( (State)m.GetCode() );
}

void MessageRecipient::SetState(State s)
{
  m_state = s;

  // Debug: print the state change
#if defined(_DEBUG)
/*
  cout << "Object " << m_id << " state set to " << s << endl;
*/
#endif
}
}

