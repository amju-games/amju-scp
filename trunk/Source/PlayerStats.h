/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: PlayerStats.h,v $
Revision 1.1.2.2  2005/09/29 19:02:36  jay
Add pure virtual Save()

Revision 1.1.2.1  2005/05/24 20:49:17  jay
New abstract base class for player info

*/

#ifndef PLAYER_STATS_H_INCLUDED
#define PLAYER_STATS_H_INCLUDED

#include "SharedPtr.h"

namespace Amju
{
class PlayerStats : public Shareable
{
public:
  virtual bool Save() = 0;
};
}

#endif

