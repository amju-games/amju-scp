/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Takeable.h,v $
Revision 1.1  2004/09/08 15:42:57  jay
Added to repository
  
*/

#if !defined(TAKEABLE_H_INCLUDED)
#define TAKEABLE_H_INCLUDED

#include "Orientation.h"
#include "States.h"
#include "SolidComponent.h"

namespace Amju
{
class File;
class Matrix;
class VisibleGameObject;
class BoundingSphere;
class Level;

// A Takeable can be picked up by the Player, and later dropped or used.
class Takeable //: public SolidGameObject
{
public:
  Takeable(VisibleGameObject*);
  virtual ~Takeable() {}
  
  // Pass call on to Game Object member.
  const BoundingSphere* GetBoundingSphere();
  void SetState(State);
  State GetState();
  const HeightServer* GetCollisionVol();
  PSolidComponent GetSolid();
  int GetId();
  Orientation* GetOrientation();
  Level* GetLevel();

  // Decorated game object. Visible because you couldn't pick up something invisible.
  VisibleGameObject* GetGameObject();
  void SetGameObject(VisibleGameObject*);

  //virtual bool Load(File* pFile);

  //virtual const char* GetTypeName() const;

  // Override base implementation so we don't draw if state is TAKEN etc.
  //virtual void Draw();
  //virtual void DrawShadow() const;

  //virtual void HandlePlayerCollision(CharacterGameObject* pPlayer);

  virtual void Use() = 0;

  // Reset orientation back to start; the takeable may have been picked up and
  // moved somewhere else by a Character.
  void ResetPosition();

  //virtual const HeightServer* GetCollisionVol() const;

  // Called when this object is carried by the player.
  void DrawTaken();

  void SetStartOrientation(const Orientation&);

protected:
  // Set when player collides with this takeable.
  // Reset once player doesn't collide any more.
  bool m_collideWithPlayer;

  Orientation m_startOrientation;

  // When not taken, show the Takeable this far above the ground.
  float m_heightAboveGround;

  VisibleGameObject* m_pVis;
public:
  //virtual bool Save(File*);

};
}
#endif

