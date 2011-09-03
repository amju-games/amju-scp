/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: PoolPuck.h,v $
Revision 1.1.2.2  2006/08/14 17:50:24  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:39  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.1  2005/09/23 20:00:14  jay
Add 'puck' - non-pottable block which slides around.

*/

#ifndef POOL_PUCK_H_INCLUDED
#define POOL_PUCK_H_INCLUDED

#include "PoolBall.h"

namespace Amju
{
// Like balls, but they don't rotate and you don't pot them. 
// Instead they slide along like pucks. So they can block pockets, 
// and be used strategically by the players.
class PoolPuck : public PoolBall 
{
public:
  virtual const char* GetTypeName() const;
  virtual void HandleObjectCollision(PoolGameObject*);
  virtual void Draw();
};
}

#endif

