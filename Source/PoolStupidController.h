/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: PoolStupidController.h,v $
Revision 1.1.2.2  2006/08/14 17:50:25  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:40  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.1  2005/10/03 08:14:49  jay
Add new Controller so Stupids don't fall down holes.

*/

#ifndef POOL_STUPID_CONTROLLER_H_INCLUDED
#define POOL_STUPID_CONTROLLER_H_INCLUDED

#include "CharacterControl.h"

namespace Amju
{
// This controller doesn't change the height of the character.
// For POOL, this prevents Stupids from falling down the pockets.
class PoolStupidController : public CharacterControl
{
protected:
  virtual void UpdateCharacterYPos(FreeMovingGameObject* pCharacter, float stepHeight);
};
}

#endif

