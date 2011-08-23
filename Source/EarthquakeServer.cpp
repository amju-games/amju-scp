/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EarthquakeServer.cpp,v $
Revision 1.1  2004/09/08 15:42:47  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "EarthquakeServer.h"
#include "Engine.h"

namespace Amju
{
void EarthquakeServer::ReceiveMessage(const Message& m)
{
  // message code is time period for earthquake.
  float activeSecs = (float)m.GetCode();
  Engine::Instance()->GetEarthquake().SetActive(activeSecs);
}
}
