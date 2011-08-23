/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: TextureModifier.cpp,v $
Revision 1.1  2004/09/08 15:42:58  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "TextureModifier.h"

namespace Amju
{
int32* TextureModifier::s_pColourTable;

TextureModifier::TextureModifier(Texture* pTexture) :
  m_pTexture(pTexture)
{
  if (!s_pColourTable)
  {
    s_pColourTable = new int32[256];
    for (int i = 0; i < 256; i++)
    {
      // Not ENDIAN, as all values ==  their reverse value.
      s_pColourTable[i] = i + (i << 8) + (i << 16) + (i << 24);
    }
  }
}

}
