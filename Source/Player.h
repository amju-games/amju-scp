/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Player.h,v $
Revision 1.1  2004/09/08 15:42:54  jay
Added to repository
  
*/

#if !defined(PLAYER_H_INCLUDED)
#define PLAYER_H_INCLUDED

#include "CharacterGameObject.h"
#include <string>

namespace Amju
{
class Player : public CharacterGameObject
{
public:
  Player();

  virtual void Update();

  virtual void SetState(State newState);

  virtual void FellFromHeight(float heightFallenFrom);

  // Collision with another Game Object. 
  virtual void HandleObjectCollision(GameObject* pObj);

  virtual bool Load(File*); // override GameObject load

  bool Load(const std::string& filename); // calls virtual function
};
}

#endif
