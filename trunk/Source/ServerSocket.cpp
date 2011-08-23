/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ServerSocket.cpp,v $
Revision 1.1  2004/09/08 15:43:19  jay
Added to repository
  
*/

// Sockets code based on sample code in BSD Sockets Primer by Jim Frost
// http://world.std.com/~jimf/papers/sockets/sockets.html

#include "ServerSocket.h"
#if defined(WIN32)
#include <winsock2.h>
#include <windows.h>
#endif

namespace Amju
{
ServerSocket::ServerSocket(int port)
{
  Bind(port);
  listen(m_socket, 3);
}

Socket ServerSocket::Accept()
{
  return accept(m_socket, 0, 0);
}
}

