/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Locator.h,v $
Revision 1.1.10.1  2005/05/08 08:03:41  jay
Fix GetOrientation - now returns const

Revision 1.1  2004/09/08 15:42:52  jay
Added to repository
  
*/

#if !defined(LOCATOR_H_INCLUDED)
#define LOCATOR_H_INCLUDED

#include "GameObject.h"
#include "Common.h"
#include "Message.h"
#include "MessageList.h"
#include <list>

namespace Amju
{
// A locator is a sphere which reacts to another game object.
// It isn't visible; but will probably centre on a Solid, either part of the
// static scene, or a (non-moving) platform, etc.
// When another object or the player intersects the Locator, we
// trigger events. E.g. change state of some game object(s), start/stop
// a platform, start a cut scene.
class Locator : public GameObject
{
public:
  Locator();

  virtual void Draw(); // Draws bounding sphere only.
  virtual bool Load(File* pf);
  virtual const char* GetTypeName() const;

  virtual const BoundingSphere* GetBoundingSphere() const
  {
    return &m_bsphere; 
  }
  virtual void CreateBoundingSphere(const Matrix& cm) {}

  // Called when pObj collides with this object.
  virtual void HandleObjectCollision(GameObject* pObj);

  virtual void HandlePlayerCollision(CharacterGameObject* pPlayer);

  // Only required by Editor currently
  virtual void SetOrientation(const Orientation&);
  virtual const Orientation* GetOrientation();


protected:
  void SendMessages();

  // IDof object which causes this Locator to fire.
  // Currently only reacts to one object.
  // 0 means the player is the thing which causes the Locator to fire.
  GameObjectId m_reactId;

  BoundingSphere m_bsphere;

  MessageList m_messages;

#if defined(SCENE_EDITOR)
public:
  virtual bool Save(File*);
  virtual PropertyMap GetProperties() const;
  virtual void SetProperties(const PropertyMap&);

  MessageList* GetMessages() { return &m_messages; }
  GameObjectId GetReactId() const { return m_reactId; }
  void SetReactId(GameObjectId id) { m_reactId = id; }

#endif

};
}
#endif

