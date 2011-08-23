/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: PowerUpFlight.cpp,v $
Revision 1.1  2004/09/08 15:42:40  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "PowerUpFlight.h"
#include "Engine.h"
#include "Message.h"
#include "GameState.h"

namespace Amju
{
void PowerUpFlight::ReceiveMessage(const Message& m)
{
  // Allow player character to FLY by flapping, for a time period.
  // Show on-screen guage of power-up time remaining.
  // TODO TimedPowerUp base class ?
}
}

