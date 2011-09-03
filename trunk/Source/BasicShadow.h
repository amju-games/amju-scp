/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: BasicShadow.h,v $
Revision 1.5  2007/11/24 22:31:00  jay
Use a private Polygon class to draw Shadows; we should remove Polygon
from AmjuGL

Revision 1.4  2007/11/13 21:33:10  jay
Remove all OpenGL calls from game code

Revision 1.3  2007/11/10 13:19:22  jay
LeafData: different version for game/Scene Ed

Revision 1.2  2006/11/16 09:22:13  jay
Set height range for floor polys, so we don't cast shadows on floors which
are too high

Revision 1.1  2004/09/08 15:42:44  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_BASIC_SHADOW_H_INCLUDED)
#define SCHMICKEN_BASIC_SHADOW_H_INCLUDED

#include "Shadow.h"
#include "AmjuGL.h"

namespace Amju
{
// Implement a simple kind of shadow: a dark circle on the ground.
class BasicShadow : public Shadow
{
public:
  static bool Init();

  BasicShadow();

  void SetHeightRange(float up, float down);

protected:
  virtual void BindTexture();

  virtual void RecalculateVerts();

  // Recalculate the list of polys. 
  virtual void RecalculateList(
    float x, 
    float y, 
    float z,
    float size,
    const HeightServer& hs);

  void ClipShadowToHeightPoly(
    const HeightPoly& hp, 
    float x, 
    float y, 
    float z,
    float size);

  // Called by Draw() - draw the polys making up the shadow.
  virtual void DrawList();

  // Map 2D point (x, z) to texture coord (s, t).
  // Centre of texture map should fall on (centrex, centrez).
  // x is between centrex-size and centrex+size.
  // z is between centrez-size and centrez+size.
  virtual void MapST(
    float centrex, 
    float centrez, 
    float x, 
    float z, 
    float size, 
    float* s, 
    float* t);

  // Set m_mult depending on the height of the object above the ground.
  // Should set 0 < m_mult <= 1.0.
  virtual void RecalcMult(float objY, float groundY);

  int GetNumShadowVerts();
  // Get the shadow vertices
  float GetShadowX(int index);
  float GetShadowZ(int index);

protected:
  static PoolTexture* s_pTexture;

/* Use AmjuGL version
  struct Vert
  {
    Vert() {}
    Vert(float x, float y, float z, float u, float v) : m_x(x), m_y(y), m_z(z), m_u(u), m_v(v) {}

    float m_x, m_y, m_z, m_u, m_v;
  };
*/
  // Unknown number of verts, as we clip the shadow to the ground poly.
  struct Polygon
  {
    void Draw();
    void AddVertex(const AmjuGL::Vert& v);
    // Call once all vertices have been added
    void Tesselate();
 
    //typedef std::vector<Vert> Verts;
    //AmuGL::Verts m_verts;
    // Create tris as verts added
    AmjuGL::Tris m_tris;

    std::vector<AmjuGL::Vert> m_verts;
 };

  typedef std::vector<Polygon> ShadowList;
  ShadowList m_list;

  // multiplier of size and alpha - depends on height above ground.
  float m_mult; 

  // List of (x, z) shadow verts. 
  // By default this is a simple square.
  // These verts are recalculated in RecalculateVerts().
  std::vector<std::pair<float, float> > m_verts;

  float m_heightRangeUp;
  float m_heightRangeDown;
};
}

#endif

