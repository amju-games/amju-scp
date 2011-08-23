/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Behaviour.h,v $
Revision 1.1  2004/09/08 15:42:44  jay
Added to repository
  
*/

#if !defined(BEHAVIOUR_H_INCLUDED)
#define BEHAVIOUR_H_INCLUDED

#include <map>
#include "GameObject.h"
#include "SharedPtr.h"

namespace Amju
{
class CharacterGameObject;

// Subclasses control an NPC in a particular way. E.g. a Chasing behaviour will
// follow something (the player), a Scared behaviour will run away.
class Behaviour : public Shareable
{
public:
  Behaviour() : m_pCharacter(0), m_pTarget(0) {}
  virtual ~Behaviour() {}

  // Called every frame to update m_pCharacter.
  virtual void Update() = 0;

  // This is so path-finding behaviours can draw their paths.
  virtual void Draw() {}

  // Called when this Behaviour becomes the Active behaviour for m_pCharacter.
  virtual void OnActivated() {}
  
  // Set the Character whose behaviour we are performing.
  // Allows for the possibility of setting the player as the Character. 
  // This could be useful for e.g. a cut scene.
  void SetNpc(CharacterGameObject* pCharacter) { m_pCharacter = pCharacter; }

  // Set the object the Character is interested in. E.g. a Following behaviour would
  // follow the target's position. Normally this would be the Player.
  // NB a Behaviour can take into account all the other GameObjects - in this 
  // case you must query the Engine. E.g. for Flocking you might need to get the 
  // bounding sphere of all NPCs of a certain type.
  void SetTarget(PGameObject pTarget) { m_pTarget = pTarget; }
  PGameObject GetTarget() { return m_pTarget; }

protected:
  // Point to the Character whose behaviour this object controls.
  CharacterGameObject* m_pCharacter;
  // Point to some other game object that is the focus of the character's
  // behaviour. E.g. the target is the player, and we want the Character
  // we are controlling to chase the player.
  // SharedPtr so the target can't be destroyed while we are trying to reach it.
  PGameObject m_pTarget;
};

typedef std::map<std::string, Behaviour* > BehaviourMap;
}

#endif

