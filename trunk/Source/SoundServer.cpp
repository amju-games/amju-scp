/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: SoundServer.cpp,v $
Revision 1.1  2004/09/08 15:42:57  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "SoundServer.h"
#include "Engine.h"
#include <iostream>

using namespace std;
namespace Amju
{
void SoundServer::ReceiveMessage(const Message& m)
{
#if defined(_DEBUG)
/*
  cout << m << endl;
*/
#endif
  // Get WAV filename from message name.
  Engine::Instance()->PlayWav(m.GetName());
}
}
