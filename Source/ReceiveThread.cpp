/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ReceiveThread.cpp,v $
Revision 1.1  2004/09/08 15:42:55  jay
Added to repository
 
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "ReceiveThread.h"
#include "MessageSerializer.h"
#include <iostream>

using namespace std;

namespace Amju
{
void ReceiveThread::SetSocket(SharedPtr<Socket> s)
{
  m_pSocket = s;
}

void ReceiveThread::SetClientId(int clientId)
{
  m_clientId = clientId;
}

int ReceiveThread::GetClientId() const
{
  return m_clientId;
}

void ReceiveThread::Work()
{
  // Wait for a Packet to come in on the socket.
  // If the Packet contains a Message, store it in the server's collection of
  // Messages. 

  MessageSerializer serializer(m_pSocket.GetPtr());

  int fails = 0;
  while (1)
  {
    PMessage m = serializer.ReadMessage();
    if (m.GetPtr())
    {
#if defined(_DEBUG)
      cout << "Received message from client " << m_clientId << ": " << *(m.GetPtr()) << endl;
#endif
      ProcessMessage(m);
    }
    else
    {
      fails++;
#if defined(_DEBUG)
      cout << "Client " << m_clientId << " bad message." << endl;
#endif
      if (fails > 5)
      {
        // Client is dead. Finish this thread.
#if defined(_DEBUG)
        cout << "Client " << m_clientId << " appears to be dead: exiting." << endl;
#endif
        break;
      }
    }
  }
  
  // Finish work, which should then exit the thread.
}
}
