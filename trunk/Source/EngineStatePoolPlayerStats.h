/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStatePoolPlayerStats.h,v $
Revision 1.1.2.1  2006/08/14 17:50:15  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:27  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1  2004/09/15 09:25:01  Administrator
Changes to enable the Player Stats state. "Skill levels" of each player
are shown before each Hole starts.

*/

#if !defined(ENGINE_STATE_GOLF_PLAYER_STATS_H_INCLUDED)
#define ENGINE_STATE_GOLF_PLAYER_STATS_H_INCLUDED

#include "EngineStatePlayerStats.h"

namespace Amju
{
class EngineStatePoolPlayerStats : public EngineStatePlayerStats
{
public:
  static const char * Name;

protected:
  virtual void Finished();
};
}

#endif

