/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: TextureSequence.h,v $
Revision 1.1  2004/09/08 15:42:59  jay
Added to repository
  
*/

#if !defined(TEXTURE_SEQUENCE_H_INCLUDED)
#define TEXTURE_SEQUENCE_H_INCLUDED

#include <string>
#include "SharedPtr.h"
#include "Texture.h"

namespace Amju
{
// Loads a texture which consists of a sequence of sub-textures.
// Examples are fonts and explosion sequences.
class TextureSequence : public Shareable
{
public:
  TextureSequence();
  ~TextureSequence();

  bool Load(
    const std::string& texturename, 
    const std::string& alphaname, 
    int numElementsX, int numElementsY,
    float sizeX, float sizeY);

  int GetNumElements() const;

  void Draw(int element);
 
  // Bind the texture. Doing this once up front instead of every call to
  // Draw() may be more efficient.
  void Bind();
  
  int GetTextureWidth();
  int GetTextureHeight();

protected:
  PoolTexture* m_pTexture; // the texure data, (owned by TextureServer).
  int m_numElements;
  int m_numElementsX;
  int m_numElementsY;
  float m_sizeX;
  float m_sizeY;
  float m_cellSizeX;
  float m_cellSizeY;
};
}

#endif

