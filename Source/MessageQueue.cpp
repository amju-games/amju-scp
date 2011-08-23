/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: MessageQueue.cpp,v $
Revision 1.1  2004/09/08 15:42:53  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "MessageQueue.h"
#include "MessageList.h"

// TODO Thread safety for all functions:
// Locking has been commented out for performance.

using namespace std;

namespace Amju
{
bool MessageComp::operator()(const PMessage& lhs, const PMessage& rhs)
{
  // This is used to prioritise messages. We want the message with the earliest
  // (smallest) real time to be on top of the queue.
  return lhs->GetRealTime() > rhs->GetRealTime();
}

void MessageQueue::Push(PMessage pM)
{
/*
  MutexLocker locker(m_mutex); // THREADING
*/
  m_queue.push(pM);
}

void MessageQueue::Clear()
{
/*
  MutexLocker locker(m_mutex); // THREADING
*/
  while(!m_queue.empty())
  {
    m_queue.pop();
  }
}

void MessageQueue::Clear(int recipientId)
{
/*
  MutexLocker locker(m_mutex); // THREADING
*/
  // Conceptutally we want to iterate through the messages in the queue, deleting
  // those which match the recipient ID.
  // Sadly, priority_queue doesn't support iteration, so we have to do it another way:
  // Pop messages off the queue into a new container, until the queue is empty.
  // The new container obviously _should support iteration. Then we delete the
  // unwanted messages, and push the remaining ones back on the queue.

  // TODO replace priority_queue with something else.
  MessageList messagelist;
  while (!m_queue.empty())
  {
    messagelist.push_back(m_queue.top());
    m_queue.pop();
  }
  for (MessageList::iterator it = messagelist.begin(); it != messagelist.end(); ++it)
  {
    PMessage& m = *it;
    if (m->GetRecipientId() != recipientId)
    {
      m_queue.push(m);
    }
  }

}

PMessage MessageQueue::Top() const
{
/*
  MutexLocker locker(m_mutex); // THREADING
*/
  return m_queue.top();
}

void MessageQueue::Pop()
{
/*
  MutexLocker locker(m_mutex); // THREADING
*/
  m_queue.pop();
}

PMessage MessageQueue::TopAndPop()
{
/*
  MutexLocker locker(m_mutex); // THREADING
*/
  PMessage m = m_queue.top();
  m_queue.pop();
  return m;
}

bool MessageQueue::Empty() const
{
/*
  MutexLocker locker(m_mutex); // THREADING
*/
  return m_queue.empty();
}
}
