/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Npc.h,v $
Revision 1.1  2004/09/08 15:42:53  jay
Added to repository
  
*/

#if !defined(NPC_H_INCLUDED)
#define NPC_H_INCLUDED

#include "CharacterGameObject.h"

namespace Amju
{
class Behaviour;

// An NPC is a non-player character.
class Npc : public CharacterGameObject
{
public:
  Npc();
  virtual ~Npc();

  virtual bool Load(File*);

  virtual void Draw();

  virtual void Update();

  virtual void OnRoomEntry();

  // Collision with player. That's going to hurt!
  virtual void HandlePlayerCollision(CharacterGameObject* pPlayer);

  // Collision with another Game Object. 
  virtual void HandleObjectCollision(PoolGameObject* pObj);

  // Call to tell this NPC what it should be chasing.
  virtual void SetTarget(PoolGameObject*) {}

  // Checks all Game Objects for collision. Call whenever something moves, I suppose.
  void CheckCharacterCollisions();

  // Call when this NPC dies.
  virtual void Dead();

protected:

  // Call to set a new behaviour and activate it.
  void SetBehaviour(Behaviour* pB); 

  // Behaviour is private so it can't be set without calling SetBehavour().
  Behaviour* GetBehaviour();

protected:

  // player health is reduced by this much on contact.
  float m_playerDamage;

  // true if this NPC is currently colliding with the Player.
  // (Prevents causing damage every frame)
  bool m_collideWithPlayer;

  // Start orientation. Current orntn. should be reset to this when room is reset.
  Orientation m_initialOrientation;

  // NPC health: when this hits zero the NPC is dead. In this case we should fade to
  // transparent.
  float m_health;

  // Fade to transparent: 1 = opaque, 0 = transparent
  float m_fade;

  // Point to the current behaviour, which controls this Character.
  Behaviour* m_pCurrentBehaviour;

  // State we set the NPC to On Room Entry.
  int m_initialState;

#if defined(SCENE_EDITOR)
public:
  virtual bool Save(File*);

  virtual PropertyMap GetProperties() const;
  virtual void SetProperties(const PropertyMap&);

#endif
};
}
#endif

