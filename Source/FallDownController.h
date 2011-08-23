/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: FallDownController.h,v $
Revision 1.1.2.2  2005/09/11 19:00:48  jay
Get fall-down controller to actually work

Revision 1.1.2.1  2005/09/08 19:59:46  jay
Add simple controller for objects which just fall down.

*/

#ifndef FALL_DOWN_CONTROLLER_H_INCLUDED
#define FALL_DOWN_CONTROLLER_H_INCLUDED

#include "Controller.h"

namespace Amju
{
// Very simple controller for things which just fall down to the ground.
class FallDownController : public Controller
{
public:
  virtual void UpdateObject(FreeMovingGameObject* );

protected:
  virtual void UpdateCharacterYPos(FreeMovingGameObject* pCharacter, float stepHeight); 
};
}

#endif

