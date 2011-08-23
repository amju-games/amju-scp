/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: BadPlatform.h,v $
Revision 1.1  2004/09/08 15:42:44  jay
Added to repository
  
*/

#if !defined(BAD_PLATFORM_H_INCLUDED)
#define BAD_PLATFORM_H_INCLUDED

#include "Platform.h"

namespace Amju
{
// Like a platform, but if the player collides with it, he gets damaged.
class BadPlatform : public Platform
{
public:
  BadPlatform();

  // Like platform version, but also get player damage.
  virtual bool Load(File*);

  virtual const char* GetTypeName() const;

  // Like platform version, but also inflict damage.
  virtual void HandlePlayerCollision(CharacterGameObject* pPlayer);

  virtual bool IsBad() { return true; }

protected:
  float m_playerDamage;
  bool m_isColliding;


#if defined(SCENE_EDITOR)
public:
  virtual bool Save(File*);
  virtual PropertyMap GetProperties() const;
  virtual void SetProperties(const PropertyMap&);

  void SetPlayerDamage(float f) { m_playerDamage = f; }
  float GetPlayerDamage() const { return m_playerDamage; }

#endif
};
}
#endif
