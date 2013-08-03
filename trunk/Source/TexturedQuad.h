#if !defined(TEXTURED_QUAD_H_INCLUDED)
#define TEXTURED_QUAD_H_INCLUDED

#include <string>
#include <TriList.h>

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
  RCPtr<TriListDynamic> m_triList;
};
}
#endif

