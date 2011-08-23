/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Portal.h,v $
Revision 1.1.10.1  2005/05/08 08:02:28  jay
Fix GetOrientation - now returns const

Revision 1.1  2004/09/08 15:42:40  jay
Added to repository
  
*/

#if !defined(PORTAL_H_INCLUDED)
#define PORTAL_H_INCLUDED

#include "GameObject.h"
#include "Orientation.h"

namespace Amju
{
// A Portal is a doorway from one room in a level to another room. 
// It is like a Locator, in that is it invisible, with a bounding sphere, 
// and reacts to one object - but for Portals, the object is
// always the Player. If the Player intersects the Portal b.sphere,
// the current room is changed.
// Portals are also used for Culling. If a Portal is in the view
// frustum, the room it connects to should be drawn; if not, the
// room can be culled. This works best for indoor levels.
class Portal : public GameObject
{
public:
  Portal();

  // Draw the bounding sphere, for debugging.
  virtual void Draw();

  // Load portal data from file.
  virtual bool Load(File* pf);

  virtual const char* GetTypeName() const;

  // Called when the Player intersects this Portal.
  virtual void HandlePlayerCollision(CharacterGameObject* pPlayer);

  // Called when another object intersects this portal.
  virtual void HandleObjectCollision(GameObject* pObj);

  virtual const BoundingSphere* GetBoundingSphere() const { return &m_bsphere; }

  virtual void CreateBoundingSphere(const Matrix& cm) {};

  virtual void SetOrientation(const Orientation&);
  virtual const Orientation* GetOrientation();

protected:
  BoundingSphere m_bsphere;

  // ID of PortalDestination to set the Player to if the Player goes through
  // (i.e. intersects) this Portal.
  // This makes it easy to edit the portal destination in the Editor, 
  // because it's a separate Game Object.
  // NB Run-time error if m_destId is not a valid ID for a PortalDestination.
  int m_destId;

#if defined(SCENE_EDITOR)
  virtual bool Save(File*);
  virtual PropertyMap GetProperties() const;
  virtual void SetProperties(const PropertyMap&);

#endif
};
}
#endif

