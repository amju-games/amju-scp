/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStatePoolPlayerStats.cpp,v $
Revision 1.1.2.2  2006/08/30 21:12:15  Administrator
Get Pool to build in MSVC

Revision 1.1.2.1  2006/08/14 17:50:15  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:27  jay
Moved Pool code from Pool/ to Pool/

Revision 1.2  2004/09/17 14:04:11  jay
Fade to next state. Nice.

Revision 1.1  2004/09/15 09:25:01  Administrator
Changes to enable the Player Stats state. "Skill levels" of each player
are shown before each Hole starts.

*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "EngineStatePoolPlayerStats.h"
#include "EngineStatePoolFlyby.h"
#include "Engine.h"
#include "PoolMisc.h"

namespace Amju
{
const char* EngineStatePoolPlayerStats::Name = "golf_player_stats"; 
// Register this State with the Engine.
static const bool registered = Engine::Instance()->
  RegisterEngineState(EngineStatePoolPlayerStats::Name, 
  new EngineStatePoolPlayerStats);

void EngineStatePoolPlayerStats::Finished()
{
  // Finished showing stats - now go on to play.
  Engine::Instance()->ChangeState(EngineStatePoolFlyby::Name, Engine::FADE);
}

}



