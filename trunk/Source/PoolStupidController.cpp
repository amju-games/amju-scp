/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: PoolStupidController.cpp,v $
Revision 1.1.2.2  2006/08/14 17:50:24  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:39  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.1  2005/10/03 08:14:49  jay
Add new Controller so Stupids don't fall down holes.

*/

#include "PoolStupidController.h"

namespace Amju
{
void PoolStupidController::UpdateCharacterYPos(
  FreeMovingGameObject* pCharacter, float stepHeight)
{
  // Do nothing so Stupids don't fall through pockets
}
}


