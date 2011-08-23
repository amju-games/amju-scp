/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: TextureModifier.h,v $
Revision 1.1  2004/09/08 15:42:58  jay
Added to repository
  
*/

#if !defined(TEXTURE_MODIFIER_H_INCLUDED)
#define TEXTURE_MODIFIER_H_INCLUDED

#include "Common.h"
#include "Texture.h"

namespace Amju
{
// TextureModifiers alter textures. E.g. creating a fire texture,
// rotating a texture, etc. 
// This is like an AttribDecorator, because it changes an attribute
// (the texture appearance) of a child. However, a procedural texture (e.g.
// fire) needs access to the texture of the child, which isn't possible 
// with the interfaces we have, without a horrible hack. So not all
// TextureModfiers are Attribs.
class TextureModifier 
{
public:
  TextureModifier(Texture* pTexture);

  virtual void Update() = 0;

  void SetTexture(Texture* pTexture) { m_pTexture = pTexture; }

protected:
  // The texture which we modify.
  Texture* m_pTexture;
  // For procedural textures, this table can be useful. It maps uint8
  // values to uint32 (i.e. RGBA) equivalents.
  // uint8(0x00) => uint32(0x00000000)
  // uint8(0x01) => uint32(0x01010101)
  // ...
  // uint8(0xf5) => uint32(0xf5f5f5f5)
  // ..etc.
  static int32* s_pColourTable;
};
}

#endif

