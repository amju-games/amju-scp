/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: SendThread.cpp,v $
Revision 1.1  2004/09/08 15:42:56  jay
Added to repository
 
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "SendThread.h"

namespace Amju
{
void SendThread::SetSocket(SharedPtr<Socket> s)
{
  m_pSocket = s;
}

void SendThread::SendMessage(PMessage m)
{
  // Extremely bad way of doing this.
  m_serializer.WriteMessage(m);
  /*
  // This function is called from another thread. 
  // Put the message on the queue and return immediately.
  MutexLocker lock(m_mutex);
  m_queue.push_back(PMessage);
  // Open the Threadgate which the sending thread is waiting on.
  m_gate.Open();
  */
}

/*
void SendThread::Work()
{
  while (1)
  {
    // Wait on threadgate until queue is non-empty.
    // Then do atomic top and pop. Serialize the message we popped off.
  }
}
*/
}
