/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: TexturedQuad.h,v $
Revision 1.1.10.1  2005/03/31 22:53:16  jay
Added Save() for SCENE_EDITOR

Revision 1.1  2004/09/08 15:42:59  jay
Added to repository
  
*/

#if !defined(TEXTURED_QUAD_H_INCLUDED)
#define TEXTURED_QUAD_H_INCLUDED

#include <string>

namespace Amju
{
class PoolTexture;
class File;

// Very useful textured 2D quad, for overlays.
class TexturedQuad
{
public:
  TexturedQuad();

  // Load the 2 bitmap names, then load the bitmaps.
  bool Load(File*);

  // Load if the 2 bitmaps names are known.
  bool Load(const std::string& tex, const std::string& alpha);

  // Draw the textured rectangle. The coord system is the same as used
  // by TextWriter. 
  void Draw(float top, float left, float bottom, float right);

  // Set the rectangular section of the texture which is drawn.
  // This defaults to the entire texture.
  // Which is equivalent to SetDrawArea(0, 0, 1.0, 1.0)
  void SetDrawArea(float top, float left, float bottom, float right);

#ifdef SCENE_EDITOR
  bool Save(File*);
#endif

protected:
  PoolTexture* m_pTex;
  // Set area of texture which is drawn.
  float m_areaTop, m_areaLeft, m_areaBottom, m_areaRight;
};
}
#endif

