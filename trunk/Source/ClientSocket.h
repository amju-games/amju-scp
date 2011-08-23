/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ClientSocket.h,v $
Revision 1.1.10.1  2005/10/21 17:50:03  jay
Connect() now has TCP/UDP switch - copied from Uptime Overlord :-)

Revision 1.2  2005/01/19 10:23:59  jay
Added parameter to Connect() to allow UDP in addition to TCP.

Revision 1.1  2005/01/03 11:08:51  jay
Added to Uptime Overlord project

Revision 1.1  2004/09/08 15:43:17  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_CLIENT_SOCKET_H_INCLUDED)
#define SCHMICKEN_CLIENT_SOCKET_H_INCLUDED

#include "Socket.h"

namespace Amju
{
class ClientSocket : public Socket
{
public:
  ClientSocket();

  // Connect to the specified server on the port given.
  // Returns true if connection succeeds.
  // If Stream is true, we use SOCK_STREAM to connect, else SOCK_DGRAM.
  bool Connect(const std::string& serverName, int port, bool stream = true);
};
}
#endif

