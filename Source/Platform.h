/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Platform.h,v $
Revision 1.1.10.1  2005/05/08 08:03:42  jay
Fix GetOrientation - now returns const

Revision 1.1  2004/09/08 15:42:54  jay
Added to repository
  
*/

#if !defined(PLATFORM_H_INCLUDED)
#define PLATFORM_H_INCLUDED

#include "VisibleGameObject.h"
#include "MovingSolid.h"
#include <string>

namespace Amju
{
class File;
class FreeMovingGameObject;

class Platform : public VisibleGameObject
{
public:
  Platform();
  virtual ~Platform();

  virtual void Draw();
  virtual void CullDraw(Frustum* pFrustum);

  virtual bool Load(File*);
  virtual const char* GetTypeName() const;

  virtual void CreateBoundingSphere(const Matrix& cm);
  virtual const BoundingSphere* GetBoundingSphere() const;
  virtual void RecalcCollisionVol();
  virtual void HandlePlayerCollision(CharacterGameObject* pPlayer);
  virtual void HandleObjectCollision(GameObject* pObj);
  virtual void Update(); 
  virtual void OnRoomEntry();

  virtual void SetState(State newState);

  void GetOrientationDelta(Orientation* pOrientation);

  // This is for dealing with rotating platforms. Get the centre of rotation.
  VertexBase GetRotationCentre() const;

  virtual void SetOrientation(const Orientation&);
  virtual const Orientation* GetOrientation();

protected:
  // Solid - should be a MovingSolid for a Platform.
  SharedPtr<MovingSolid> m_pMovingSolid;

  // If true, then when the State of this Platform is set to UNKNOWN, it becomes
  // set to STATIONARY. If false, the state is set to MOVING.
  bool m_unknownIsStationary;

  // Points to last object which collided with this platform,
  // or zero when all colliding objects no longer collide.
  FreeMovingGameObject* m_pActiveObject;

#ifdef SCENE_EDITOR
public:
  virtual bool Save(File*);

  virtual PropertyMap GetProperties() const;
  virtual void SetProperties(const PropertyMap&);

  void SetMovingSolid(SharedPtr<MovingSolid> pMs) { m_pMovingSolid = pMs; }
  SharedPtr<MovingSolid> GetMovingSolid() { return m_pMovingSolid; }

  void SetUnknownStationary(bool b) { m_unknownIsStationary = b; }
  bool GetUnknownStationary() const { return m_unknownIsStationary; }

#endif
};
}
#endif

