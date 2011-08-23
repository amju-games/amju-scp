/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: SendThread.h,v $
Revision 1.1  2004/09/08 15:42:56  jay
Added to repository
 
*/

#if !defined(SCHMICKEN_SEND_THREAD_H_INCLUDED)
#define SCHMICKEN_SEND_THREAD_H_INCLUDED

#include "Thread.h"
#include "Socket.h"
#include "Message.h"
#include "Mutex.h"
#include "MessageSerializer.h"
#include <queue>

namespace Amju
{
// Sends Messsages along a Socket. The messages are queued, the idea being
// that the thread which wants to send the message doesn't have to wait.
class SendThread 
{
public:
  void SetSocket(SharedPtr<Socket> s);
  
  // Send a message on the socket. This returns immediately, so there is no
  // status returned. 
  void SendMessage(PMessage);

protected:
  // When the socket is closed Work() returns.
  //virtual void Work();

protected:
  SharedPtr<Socket> m_pSocket;
  MessageSerializer m_serializer;
  /*
  typedef std::deque<PMessage> MessageQueue;
  MessageQueue m_queue;
  Mutex m_mutex;
  */
};
}

#endif

