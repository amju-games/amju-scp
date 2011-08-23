/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: SocketService.cpp,v $
Revision 1.1  2004/09/08 15:43:20  jay
Added to repository
  
*/

#include "SocketService.h"

#if defined(WIN32)
#include <winsock2.h>
#endif

namespace Amju
{
SocketService::SocketService() 
{ 
#if defined(WIN32)
  WSADATA wsaData;
  WSAStartup(MAKEWORD(2, 0), &wsaData);
#endif
}

SocketService::~SocketService()
{
#if defined(WIN32)
  WSACleanup();
#endif
}
}
