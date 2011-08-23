/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: MessageQueue.h,v $
Revision 1.1  2004/09/08 15:42:53  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_MESSAGE_QUEUE_H_INCLUDED)
#define SCHMICKEN_MESSAGE_QUEUE_H_INCLUDED

#include "Message.h"
#include "Mutex.h"
#include <queue>

namespace Amju
{
// Compare two PMessages, for prioritizing.
struct MessageComp
{
  bool operator()(const PMessage& lhs, const PMessage& rhs);
};

// Thread-safe priority-queue of messages. The highest-priority
// message (i.e. at the front of the queue) has the earliest
// time. 
class MessageQueue
{
public:
  void Push(PMessage pM);
  
  // Clear all messages
  void Clear();

  // Clear all messages for specified recipient
  void Clear(int recipient);

  PMessage Top() const;

  void Pop();

  // Returns the top message, and pops it off the queue.
  // The item popped is guaranteed to be the one returned.
  // This is necessary when more than one thread accesses the queue.
  PMessage TopAndPop();

  bool Empty() const;

private:
  typedef std::priority_queue<PMessage, std::vector<PMessage>, MessageComp> InternalQueue;
  InternalQueue m_queue;

  // We need operations on this queue to be thread-safe.
  mutable Mutex m_mutex;
};
}

#endif

