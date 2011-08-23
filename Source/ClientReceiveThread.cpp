/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ClientReceiveThread.cpp,v $
Revision 1.1  2004/09/08 15:42:46  jay
Added to repository
 
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "ClientReceiveThread.h"
#include "Engine.h"

namespace Amju
{
void ClientReceiveThread::ProcessMessage(PMessage pm)
{
  Engine::Instance()->NetworkReceiveMessage(pm);
}
}
