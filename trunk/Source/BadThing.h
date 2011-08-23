/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: BadThing.h,v $
Revision 1.1  2004/09/08 15:42:44  jay
Added to repository
  
*/

#if !defined(BAD_THING_H_INCLUDED)
#define BAD_THING_H_INCLUDED

#include "GameObject.h"

namespace Amju
{
// A BadThing is a sphere. If the player intersects the sphere, he suffers some
// damage. The sphere isn't visible, like a Locator. The sphere is static.
class BadThing : public GameObject
{
public:
  BadThing();

  virtual void Draw(); // Draws bounding sphere only.
  virtual bool Load(File* pf);
  virtual const char* GetTypeName() const;

  virtual const BoundingSphere* GetBoundingSphere() const
  {
    return &m_bsphere; 
  }
  virtual void CreateBoundingSphere(const Matrix& cm) {}

  virtual void HandlePlayerCollision(CharacterGameObject* pPlayer);

protected:
  BoundingSphere m_bsphere;
  float m_playerDamage; // damage inflicted on player on contact.
  bool m_collideWithPlayer; // if true we are touching player.


#if defined(SCENE_EDITOR)
public:
  virtual bool Save(File*);
  virtual PropertyMap GetProperties() const;
  virtual void SetProperties(const PropertyMap&);

#endif
};
}

#endif
