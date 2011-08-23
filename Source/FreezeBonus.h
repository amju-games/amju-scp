/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: FreezeBonus.h,v $
Revision 1.1.2.2  2006/08/14 17:50:19  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:33  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1  2004/09/08 15:43:06  jay
Added to repository
  
*/

#if !defined(FREEZE_BONUS_H_INCLUDED)
#define FREEZE_BONUS_H_INCLUDED 

#include "Bonus.h"

namespace Amju
{
class FreezeBonus : public Bonus
{
public:
  FreezeBonus();
  virtual const char* GetTypeName() const;
  virtual void HandleObjectCollision(GameObject*);

};
}

#endif

