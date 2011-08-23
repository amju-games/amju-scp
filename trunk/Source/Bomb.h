/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Bomb.h,v $
Revision 1.1  2004/09/08 15:42:34  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_BOMB_H_INCLUDED)
#define SCHMICKEN_BOMB_H_INCLUDED

#include "Takeable.h"
#include "SolidGameObject.h"
#include "Effect.h"
#include "BillBoard.h"

namespace Amju
{
class Bomb : public Takeable, public SolidGameObject
{
public:
  Bomb();

  virtual const char* GetTypeName() const;

  virtual bool Load(File*);
  virtual void Draw();
  virtual void CullDraw(Frustum*);
  virtual void SetState(State newState);
  virtual void DrawShadow() const;

  virtual void Use();

protected:
  void Detonate();

private:
  EffectVector m_fx;

  // Crocs within this radius get killed when the bomb detonates.
  float m_blastRadius;

  // TODO This should be another effect; for now it's a separate thing.
  // This is supposed to be a rapidly expanding flash of light.
  SharedPtr<BillBoard> m_pBillboard;

  float m_detonateTime;  // time since we detonated
  bool m_detonated; // true if we have been detonated.

#if defined(SCENE_EDITOR)
public:
  virtual bool Save(File*);
  virtual PropertyMap GetProperties() const;
  virtual void SetProperties(const PropertyMap&);

#endif
};
}

#endif

