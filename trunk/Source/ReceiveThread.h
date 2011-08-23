/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ReceiveThread.h,v $
Revision 1.1  2004/09/08 15:42:55  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_RECEIVE_THREAD_H_INCLUDED)
#define SCHMICKEN_RECEIVE_THREAD_H_INCLUDED

#include "Thread.h"
#include "Socket.h"
#include "Message.h"

namespace Amju
{
// This is a Thread which receives Messages on a Socket. 
// When the other end of the socket goes away Work() finishes.
class ReceiveThread : public Thread
{
public:
  void SetSocket(SharedPtr<Socket> s);
  void SetClientId(int clientId);
  int GetClientId() const;

protected:
  virtual void Work();
  virtual void ProcessMessage(PMessage) = 0;

protected:
  SharedPtr<Socket> m_pSocket;
  int m_clientId;
};
}
#endif

