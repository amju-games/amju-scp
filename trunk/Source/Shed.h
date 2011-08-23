/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Shed.h,v $
Revision 1.1  2004/09/08 15:42:41  jay
Added to repository
  
*/

#if !defined(SHED_H_INCLUDED)
#define SHED_H_INCLUDED

#include <vector>
#include "SolidGameObject.h"
#include "Target.h"
#include "Npc.h"
#include "ExplosionEffect.h"

namespace Amju
{
class Shed : public Npc
{
public:
  Shed();
  virtual const char* GetTypeName() const;

  virtual void HandleObjectCollision(GameObject*);
  virtual void CullDraw(Frustum*);
  virtual bool Load(File*);
  virtual void OnRoomEntry();
  virtual void Draw();

protected:
  ExplosionEffect m_explosion;

  // IDs of hidden objects which appear when shed explodes.
  std::vector<int> m_hiddenIds;

  bool m_isColliding;

public:
#if defined(SCENE_EDITOR)
  virtual bool Save(File*);
  virtual PropertyMap GetProperties() const;
  virtual void SetProperties(const PropertyMap&);

#endif
};
}
#endif

