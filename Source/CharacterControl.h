/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: CharacterControl.h,v $
Revision 1.1  2004/09/08 15:42:45  jay
Added to repository
  
*/

#if !defined(CHARACTER_CONTROL_H_INCLUDED)
#define CHARACTER_CONTROL_H_INCLUDED

#include "Controller.h"

namespace Amju
{
class FreeMovingGameObject;
class WallPoly;
class HeightServer;
class BoundingSphere;

// Updates a character's position, performing collision detection.
class CharacterControl : public Controller
{
public:
  virtual void UpdateObject(FreeMovingGameObject* pCharacter); 

protected:
  void UpdateCharacter(
    FreeMovingGameObject* pCharacter,
    float stepHeight);
 
};
}

#endif

