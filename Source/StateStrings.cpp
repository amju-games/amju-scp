/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: StateStrings.cpp,v $
Revision 1.1.10.2  2007/06/17 20:36:16  jay
POOL ONLINE: add FOUL state

Revision 1.1.10.1  2007/06/10 08:59:00  jay
Add two more States for Online Pool

Revision 1.1  2004/09/08 15:42:57  jay
Added to repository
  
*/

#include "StateStrings.h"

namespace Amju
{
  // NB These strings are supposed to be looked up using a State.
  // So there is an unused entry for State 1, etc.
  const char* StateStrings[] =
  {
    "unknown/initial",
    "(not used)",
    "out of play",
    "invisible",
    "visible",
    "taken",
    "moving",
    "stationary",
    "egg show num",
    "dead",
    "fading",
    "chase",
    "near shed",
    "on ramp",
    "bonus explode",
    "fight",
    "hatch",
    "captured",
    "run home",
    "frozen",
    "set",
    "unset",
    "just hatched",
    "decide",
    "detonate",
    "detonating",
    "rise from dead",
    "ai collect",
    "ai pick up",
    "ai take",
    "ai rotate",
    "ai drop",
    "ai jump down",
    "ai dump",
    "ai rotate fail",
    "ai jump fail",
    "ai run to",
    "golf swing up",
    "golf swing down",
    "golf watch ball",
    "ball in pocket",
    "ball out of bounds",
    "foul"
  };

}
