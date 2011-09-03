/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: BasicShadow.h,v $
Revision 1.1  2004/09/08 15:42:44  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_BASIC_SHADOW_H_INCLUDED)
#define SCHMICKEN_BASIC_SHADOW_H_INCLUDED

#include "Shadow.h"

namespace Amju
{
// Implement a simple kind of shadow: a dark circle on the ground.
class BasicShadow : public Shadow
{
public:
  static bool Init();

  BasicShadow();

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

  typedef std::vector<Polygon> ShadowList;
  ShadowList m_list;

  // multiplier of size and alpha - depends on height above ground.
  float m_mult; 

  // List of (x, z) shadow verts. 
  // By default this is a simple square.
  // These verts are recalculated in RecalculateVerts().
  std::vector<std::pair<float, float> > m_verts;

};
}

#endif

