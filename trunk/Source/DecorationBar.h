/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: DecorationBar.h,v $
Revision 1.1  2004/09/08 15:42:47  jay
Added to repository
  
*/

#if !defined(DECORATION_BAR_H_INCLUDED)
#define DECORATION_BAR_H_INCLUDED

#include "TexturedQuad.h"
#include <string>
#include <vector>

namespace Amju
{
// Decoration made out of lots of textured quads. All quads are the same size.
class DecorationBar
{
public:
  bool Load(const std::string& filename);

  void Draw(float top, float left, float bottom, float right);

protected:
  std::vector<TexturedQuad> m_quads;
  // Quad width and height - same for all quads.
  float m_w, m_h; 
  float m_vInc, m_hInc; // vert/horix increment
};
}

#endif
