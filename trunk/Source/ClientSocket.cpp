/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ClientSocket.cpp,v $
Revision 1.1.10.1  2005/10/21 17:50:03  jay
Connect() now has TCP/UDP switch - copied from Uptime Overlord :-)

Revision 1.2  2005/01/19 10:23:59  jay
Added parameter to Connect() to allow UDP in addition to TCP.

Revision 1.1  2005/01/03 11:08:50  jay
Added to Uptime Overlord project

Revision 1.1  2004/09/08 15:43:17  jay
Added to repository
  
*/

// This source code originally written by JASON COLMAN 2000-2003. 
// You may use it as you wish, at your own risk!  jason@amju.com.

// Sockets code based on sample code in BSD Sockets Primer by Jim Frost
// http://world.std.com/~jimf/papers/sockets/sockets.html

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <iostream>
#include "ClientSocket.h"
#if defined(WIN32)
#include <winsock2.h>
#include <windows.h>
#endif

namespace Amju
{
ClientSocket::ClientSocket()
{
}

bool ClientSocket::Connect(const std::string& serverName, int port, bool stream)
{
  struct sockaddr_in sa;
  struct hostent *hp;

  hp = gethostbyname(serverName.c_str());
  if (!hp) 
  { 
    return false;                                
  }

  memset(&sa,0,sizeof(sa));
  memcpy((char *)&sa.sin_addr,hp->h_addr,hp->h_length);     
  sa.sin_family= hp->h_addrtype;
  sa.sin_port= htons((u_short)port);

  int type = SOCK_STREAM;
  if (!stream)
  {
    type = SOCK_DGRAM;
  }

  if ((m_socket = socket(hp->h_addrtype, type, 0)) < 0) 
  {
std::cout << "CONNECT: call to socket() failed.\n";
    return false;
  }

  if (connect(m_socket,(struct sockaddr *)&sa,sizeof sa) < 0) 
  {
std::cout << "CONNECT: call to connect() failed.\n";
    return false;
  }
  return true;
}
}

