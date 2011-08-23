/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: PowerUpServer.cpp,v $
Revision 1.1  2004/09/08 15:42:55  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "PowerUpServer.h"
#include "Engine.h"
#include "File.h"

namespace Amju
{
void PowerUpServer::ReceiveMessage(const Message& m)
{
  // Message name tells us which power up to grant the player.
  // (This could be extended to grant power ups to other Game Objects
  // in fact)
  // Look up the PowerUp in a map, and execute it.

  // This now just shows a Timer.
  float time = (float)m.GetCode();
  m_guage.SetPowerTime(time);
}

void PowerUpServer::DrawOverlays()
{
  m_guage.Draw();
}

bool PowerUpServer::Load()
{
  //return true;
  File f;
  f.OpenRead(Engine::Instance()->GetConfigValue("powertimer"));
  return m_guage.Load(&f);
}
}

