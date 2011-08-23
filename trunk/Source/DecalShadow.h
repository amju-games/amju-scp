/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: DecalShadow.h,v $
Revision 1.1  2004/09/08 15:42:47  jay
Added to repository
  
*/

#if !defined(DECAL_SHADOW_H_INCLUDED)
#define DECAL_SHADOW_H_INCLUDED

#include "BasicShadow.h"
#include "Texture.h"

namespace Amju
{
class DecalShadow : public BasicShadow
{
public:
  DecalShadow() : m_pTexture(0) {}
  void SetTexture(Texture*);
  virtual void BindTexture();

protected:
  // Don't make decal translucent or smaller depending on object height
  // above ground.
  virtual void RecalcMult(float, float) { m_mult = 1.0f; }

private:
  Texture* m_pTexture;
};
}

#endif


