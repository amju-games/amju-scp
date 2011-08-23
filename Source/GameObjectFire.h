/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: GameObjectFire.h,v $
Revision 1.1  2004/09/08 15:42:50  jay
Added to repository
  
*/

#if !defined(GAME_OBJECT_FIRE_H_INCLUDED)
#define GAME_OBJECT_FIRE_H_INCLUDED 

#include "VisibleGameObject.h"
#include "SolidComponent.h"

namespace Amju
{
class GameObjectFire : public VisibleGameObject
{
public:
  GameObjectFire();

  virtual const char* GetTypeName() const;
  virtual bool Load(File*);
  virtual void Draw();
  virtual void DrawShadow() const;
  virtual void Update();
  virtual void OnRoomEntry();
  virtual void SetState(State newState);
  virtual void HandlePlayerCollision(CharacterGameObject* pPlayer);
  virtual void HandleObjectCollision(GameObject*);
  virtual const BoundingSphere* GetBoundingSphere() const;
  virtual void CreateBoundingSphere(const Matrix& cm); 

protected:
  PSolidComponent m_pFire;

#if defined(SCENE_EDITOR)
public:
  virtual bool Save(File*);

  virtual PropertyMap GetProperties() const;
  virtual void SetProperties(const PropertyMap&);
#endif

};
}

#endif


