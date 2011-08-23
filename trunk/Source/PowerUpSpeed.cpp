/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: PowerUpSpeed.cpp,v $
Revision 1.1  2004/09/08 15:42:41  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "PowerUpSpeed.h"
#include "Engine.h"
#include "Message.h"
#include "EngineRunning.h"
#include <iostream>

using namespace std;
namespace Amju
{
void PowerUpSpeed::ReceiveMessage(const Message& m)
{
  EngineRunning* pEngineRunning = dynamic_cast<EngineRunning*>(
    Engine::Instance()->GetEngineState(EngineRunning::Name).GetPtr());
 
  // If code is zero, reset the player speed.
  if (m.GetCode() == 0)
  {
    pEngineRunning->RestorePlayerSpeed();
#if defined(_DEBUG)
/*
    cout << "Restoring player speed" << endl;
*/
#endif
  }
  else
  {
    // Code is percent speed increase.
    float speedMult = float(m.GetCode()) / 100.0f;
    pEngineRunning->MultPlayerSpeed(speedMult);
#if defined(_DEBUG)
/*
    cout << "Speed up!" << endl;
*/
#endif
  }
}
}

