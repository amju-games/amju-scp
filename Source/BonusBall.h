/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: BonusBall.h,v $
Revision 1.1.2.2  2006/08/14 17:50:07  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:25  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1  2004/09/08 15:43:04  jay
Added to repository
  
*/

#if !defined(BONUS_BALL_H_INCLUDED)
#define BONUS_BALL_H_INCLUDED

#include "Bonus.h"

namespace Amju
{
class BonusBall : public Bonus
{
public:
  BonusBall();
  virtual const char* GetTypeName() const;
  virtual void HandleObjectCollision(GameObject*);
};
}

#endif


