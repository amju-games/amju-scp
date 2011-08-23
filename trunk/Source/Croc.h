/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Croc.h,v $
Revision 1.1  2004/09/08 15:42:35  jay
Added to repository
  
*/

#if !defined(CROC_H_INCLUDED)
#define CROC_H_INCLUDED

#include "Npc.h"
#include "IdleBehaviour.h"
#include "ChaseBehaviour.h"
#include "RunHomeBehaviour.h"
#include "FrozenBehaviour.h"
#include "CrocTestBehaviour.h"
//#include "CrocDecideBehaviour.h"
//#include "ChickHolder.h"
#include <list>

namespace Amju
{
class Chick;

// A Croc is a bad kind of NPC. It chases Chicks and fights the player.
class Croc : public Npc 
//, public ChickHolder
{
public:
  Croc();

  virtual const char* GetTypeName() const;

  // Override NPC draw to change colour if we are frozen.
  virtual void Draw();

  // Collision with another Game Object. We check for Chicks: if we've got one, 
  // we want to run away with it.
  virtual void HandleObjectCollision(GameObject* pObj);

  // Collision with player: both Croc and Player drop their Chicks.
  virtual void HandlePlayerCollision(CharacterGameObject* pPlayer);

  virtual void SetState(State newState);
  virtual void SetTarget(GameObject*);

  virtual bool IsBad() { return true; }

  virtual void Dead();

protected:
  // Get the nearest "home" for the Croc, to which it takes captive Chicks.
  GameObject* GetHome();

  // Called when this Croc reaches home.
  void KillChicks();

  // Possible behaviours. We point to exactly one of these at one time.
  // (The pointer is in the base class)
  IdleBehaviour  m_idleBehaviour;
  ChaseBehaviour  m_chaseBehaviour;
  RunHomeBehaviour  m_runHomeBehaviour;
  FrozenBehaviour  m_frozenBehaviour;
  CrocTestBehaviour  m_testBehaviour;
/*
  CrocDecideBehaviour  m_decideBehaviour;
*/
};
}
#endif
