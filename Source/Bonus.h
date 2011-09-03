/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Bonus.h,v $
Revision 1.1  2004/09/08 15:42:34  jay
Added to repository
  
*/

#if !defined(BONUS_H_INCLUDED)
#define BONUS_H_INCLUDED

#include "SolidGameObject.h"
#include "ExplosionEffect.h"

namespace Amju
{
class File;
class SolidComponent;
class Matrix;

class Bonus : public SolidGameObject
{
public:
  Bonus();

  virtual bool Load(File* pFile);
  virtual const char* GetTypeName() const;

  virtual void HandlePlayerCollision(CharacterGameObject* pPlayer);
  virtual void HandleObjectCollision(PoolGameObject* );
  virtual void Update();
  virtual void Draw();
  virtual void CullDraw(Frustum*); // for explosion effect
  virtual void DrawShadow() const;
  virtual void RecalcCollisionVol();
  virtual void OnRoomEntry();

protected:
  SharedPtr<ExplosionEffect> m_pExplosion;
  int m_bonusPoints;

#if defined(SCENE_EDITOR)
public:
  virtual bool Save(File*);
  virtual PropertyMap GetProperties() const;
  virtual void SetProperties(const PropertyMap&);

#endif
};
}
#endif

