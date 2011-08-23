/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: TextureModRotate.h,v $
Revision 1.1  2004/09/08 15:42:58  jay
Added to repository
  
*/

#if !defined(TEXTURE_MOD_ROTATE_H_INCLUDED)
#define TEXTURE_MOD_ROTATE_H_INCLUDED

#include "TextureModifier.h"

namespace Amju
{
// This Texture Modifier rotates the texture, using the texture matrix.
class TextureModRotate : public TextureModifier
{
public:
  TextureModRotate(Texture* pTexture);
  virtual void Update();

protected:
  float m_time;
};
}
#endif
