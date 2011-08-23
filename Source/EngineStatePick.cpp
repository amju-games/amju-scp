/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStatePick.cpp,v $
Revision 1.1  2004/09/08 15:42:48  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "EngineStatePick.h"
#include "Engine.h"

namespace Amju
{
const std::string EngineStatePick::Name = "pick";
// Register this State with the Engine.
static const bool registered = Engine::Instance()->
  RegisterEngineState(EngineStatePick::Name, new EngineStatePick);

void EngineStatePick::SetActive(bool active)
{
  GetEngine()->Freeze(active);

  if (active)
  {
  }
  else
  { 
  }
}

void EngineStatePick::Draw()
{
  // Only draw Game Objects which may possibly be selected.
  // Use distinct colour for each one.


  // Check colour under cursor
}

void EngineStatePick::Update()
{
  // Do nothing
}

}
