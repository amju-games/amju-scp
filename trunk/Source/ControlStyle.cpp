/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ControlStyle.cpp,v $
Revision 1.1  2004/09/08 15:42:34  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "ControlStyle.h"
#include "Engine.h"
#include "EngineRunning.h"
#include "Player.h"

namespace Amju
{
EngineRunning* ControlStyle::ER()
{
  static EngineRunning* pEr = 0;
  if (!pEr)
  {
    pEr = dynamic_cast<EngineRunning*>(
      Engine::Instance()->GetEngineState(EngineRunning::Name).GetPtr());
  }
  return pEr;
}

void ControlStyle::DoAction(bool down)
{
}
}

