/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: PoolBonus.h,v $
Revision 1.1.2.2  2006/08/14 17:50:22  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:37  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.3  2005/09/20 13:17:20  jay
Fix collision test

*/

#ifndef POOL_BONUS_H_INCLUDED
#define POOL_BONUS_H_INCLUDED

#include "Bonus.h"

namespace Amju
{
// This kind of bonus releases more hearts at each Pocket
// when it is hit.
class PoolBonus : public Bonus
{
public:
  PoolBonus();
  virtual const char* GetTypeName() const;
  virtual void HandleObjectCollision(PoolGameObject*);
  virtual void OnRoomEntry();
};
}

#endif

