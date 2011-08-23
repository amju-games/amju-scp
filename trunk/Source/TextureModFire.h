/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: TextureModFire.h,v $
Revision 1.1  2004/09/08 15:42:58  jay
Added to repository
  
*/

#if !defined(TEXTURE_MOD_FIRE_H_INCLUDED)
#define TEXTURE_MOD_FIRE_H_INCLUDED

#include "TextureModifier.h"
#include "Common.h"

namespace Amju
{
const int FIRE_SIZE = 32;
const int FIRE_ARRAY_SIZE = FIRE_SIZE * FIRE_SIZE;
class TextureModFire : public TextureModifier
{
public:
  TextureModFire(Texture* pTexture);
  virtual void Update();

protected:
  uint8 src[FIRE_ARRAY_SIZE];
  uint8 dst[FIRE_ARRAY_SIZE];
};
}

#endif

