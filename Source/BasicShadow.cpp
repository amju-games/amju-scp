/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: BasicShadow.cpp,v $
Revision 1.1.10.2  2007/12/12 10:27:41  jay
Add per-shadow height offset

Revision 1.1.10.1  2006/08/14 17:37:29  jay
Rename "Pool"

Revision 1.1  2004/09/08 15:42:44  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <string>
#include "BasicShadow.h"
#include "TextureServer.h"
#include "Engine.h"
extern "C"
{
#include "Gpc/gpc.h" // Polygon clipping code - it's in C.
}

namespace Amju
{
PoolTexture* BasicShadow::s_pTexture;

bool BasicShadow::Init()
{
  std::string bitmapname = Engine::Instance()->GetConfigValue("shadow"); 
  std::string alphaname = Engine::Instance()->GetConfigValue("shadow_alpha");

  s_pTexture = TextureServer::Instance()->Get(bitmapname, alphaname); 
  if (s_pTexture)
  {
    s_pTexture->CreateBinding(PoolTexture::REGULAR);
  }
  else 
  {
    return false;
  }

  return Shadow::Init();
}

BasicShadow::BasicShadow()
{
  m_mult = 1.0f;

}

void BasicShadow::BindTexture()
{ 
  Assert(s_pTexture);
  s_pTexture->Bind();
}

void BasicShadow::RecalculateVerts()
{
  m_verts.clear();

  const float x1 = m_oldx + m_oldsize * m_mult;
  const float x2 = m_oldx - m_oldsize * m_mult;
  const float z1 = m_oldz + m_oldsize * m_mult;
  const float z2 = m_oldz - m_oldsize * m_mult;

  m_verts.push_back(std::make_pair(x1, z1));
  m_verts.push_back(std::make_pair(x1, z2));
  m_verts.push_back(std::make_pair(x2, z2));
  m_verts.push_back(std::make_pair(x2, z1));
}

int BasicShadow::GetNumShadowVerts()
{
  return m_verts.size();
}

float BasicShadow::GetShadowX(int index)
{
  Assert(index < GetNumShadowVerts());
  return m_verts[index].first;
}

float BasicShadow::GetShadowZ(int index)
{
  Assert(index < GetNumShadowVerts());
  return m_verts[index].second;
}

void BasicShadow::DrawList() 
{
  // 12/5/2004 commented out offset proportional to camera height.
  // It doesn't work in Pool, and player is allowed to go quite high.`
/*
  float h = 0;
  if (Engine::Instance()->GetCamera().GetPtr())
  {
    Assert(Engine::Instance()->GetCamera()->GetOrientation());
    h = Engine::Instance()->GetCamera()->GetOrientation()->GetY() * m_yOffset * 0.1f;
  }
*/
  // m_mult is between 1 (full size shadow) and 0 (no shadow). We use this to 
  // change the transparency of the shadow: smaller shadows are more
  // translucent.
  // Push the new alpha value, RGB is unchanged.
  Engine::Instance()->PushColour(1, 1, 1, m_mult);

  BindTexture();

  AmjuGL::PushAttrib(AmjuGL::AMJU_BLEND);
  AmjuGL::Enable(AmjuGL::AMJU_BLEND);
  ////////glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

  // Allow read-, but not write-access to the depth buffer. This overlaps
  // shadows properly.
  AmjuGL::Disable(AmjuGL::AMJU_DEPTH_WRITE);

  // Adjust height of shadow with height of camera. This is because, as the 
  // camera gets higher, the distance between shadow and ground needs to 
  // increase to avoid z-fighting.
  AmjuGL::PushMatrix();

/*  
  AmjuGL::Translate(0, h, 0);
*/

  for (ShadowList::iterator it = m_list.begin(); it != m_list.end(); ++it)
  {
    it->Draw();
  }

  AmjuGL::PopMatrix();

  AmjuGL::Enable(AmjuGL::AMJU_DEPTH_WRITE);
  AmjuGL::PopAttrib();

  Engine::Instance()->PopColour(); // pop off the alpha change.
}

void BasicShadow::RecalcMult(float y, float h)
{
  m_mult = (1.0f - (y - h)/s_maxHeight); 
}

void BasicShadow::RecalculateList(
  float x, float y, float z,
  float size,
  const HeightServer& hs)
{
  m_list.clear();

  // Get the height poly which is the heighest below y.
  const HeightPoly* pHp = hs.GetHeightPoly(x, y, z);
  if (!pHp)
  {
    return;
  }   
  float h = pHp->GetY(x, z);
  Assert(y >= h);
  if (y - s_maxHeight >= h) 
  {
    // We are too far above the ground to cast a shadow.
    return;
  }
  else 
  {
    RecalcMult(y, h);
    size *= m_mult;
  }
  Assert(size > 0);

  // Redo the list of shadow vertices. 
  RecalculateVerts();

  // Get a horizontal bounding square centred on x, z.
  // This is used to get all the heightservers we need to consider.
  // It doesn't imply that the shadow must be a quad.
  const float x1 = x + size;
  const float x2 = x - size;
  const float z1 = z + size;
  const float z2 = z - size;

  // The height at the edges of the shadow may be higher than at
  // the centre. To make sure we always get the correct Height Poly,
  // add an offset to the height.
  float y2 = y + 0.8f; // TODO get exact offset using trig. and size.

  std::list<const HeightPoly*> heightPolys;

  // Get all height polys which this shadow may cover.
  hs.GetAllHeightPolys(x2, x1, y2, y - 2.0f, z2, z1, &heightPolys);

  // Fix for shadows being cast through floors:
  // Only use the height polys with the highest max y value.
  // It's ok if there are several of these. But discard any polys with a
  // lower max y.
  // TODO
  // use std::partition ?

  for (std::list<const HeightPoly*>::const_iterator it = heightPolys.begin(); 
       it != heightPolys.end(); 
       ++it)
  {
    // Now for each height poly, clip the shadow poly to it.
    ClipShadowToHeightPoly(**it, x, y, z, size);
  }
}

// Map 2D point (x, z) to texture coord (s, t).
// Centre of texture map should fall on (centrex, centrez).
// x is between centrex-size and centrex+size.
// z is between centrez-size and centrez+size.
void BasicShadow::MapST(
  float centrex, 
  float centrez, 
  float x, 
  float z, 
  float size, 
  float* s, 
  float* t)
{
  Assert(size > SMALLEST);

  const float twosize = 2 * size;
  *s = -(x - centrex + size) / twosize;
  *t = (z - centrez + size) / twosize;
}

void BasicShadow::ClipShadowToHeightPoly(
  const HeightPoly& hp, 
  float centrex, 
  float y, 
  float centrez,
  float size) 
{
  // Clip the shadow poly to the given height poly (which should be a triangle).

  // One or more vertices of the shadow may fall outside the poly the shadow 
  // is cast on. We have to get the intersection of the two polygons.
  // Luckily we can use the GPC ("Generic Polygon Clipper").

  // Vector of shadow polygon vertices after clipping.
  std::vector<VertexBase> vertices;

  // Set up gpc data structures for the two polygons.
  // First poly: the shadow 
  gpc_vertex v1[36]; // TODO arbitrary max number of verts.
  int numShadowVerts = GetNumShadowVerts();

  int i;
  for (i = 0; i < numShadowVerts; i++)
  {
    v1[i].x = GetShadowX(i);
    v1[i].y = GetShadowZ(i);
  }

  gpc_vertex_list vl1;
  vl1.num_vertices = numShadowVerts;
  vl1.vertex = v1;

  gpc_polygon p1;
  p1.num_contours = 1;
  p1.contour = &vl1;
  int h1 = 0;
  p1.hole = &h1;

  // Second poly: convert hp to gpc data structures.
  const int numverts = 3; // vertices in ground poly
  const int MAX_VERTS = 36; // TODO get real max from scene ?
  gpc_vertex v2[MAX_VERTS]; // Don't allocate with new, for speed.
  int maxverts = numverts;
  if (maxverts > MAX_VERTS)
  {
    maxverts = MAX_VERTS;
  }

  for (i = 0; i < maxverts; i++)
  {
    v2[i].x = hp.GetVertex(i).x;
    v2[i].y = hp.GetVertex(i).z;
  }

  gpc_vertex_list vl2;
  vl2.num_vertices = numverts;
  vl2.vertex = v2;

  gpc_polygon p2;
  p2.num_contours = 1;
  p2.contour = &vl2;
  int h2 = 0;
  p2.hole = &h2;

  // Get intersection
  gpc_polygon result;
  gpc_polygon_clip(GPC_INT, &p1, &p2, &result);

  // Convert result into vector of VertexBase.
  if (result.num_contours != 1)
  {
    // Error, there should be one gpc 'contour' (== polygon here).
    return;
  }

  gpc_vertex_list& vlist = result.contour[0];
  for (int k = 0; k < vlist.num_vertices; k++)
  {
    gpc_vertex& v = vlist.vertex[k];
    VertexBase vb(v.x, 0, v.y);
    vertices.push_back(vb);
  }

  // Free the contour in the polygon result.
  gpc_free_polygon(&result);

  // TODO we could do this in the loop above.
  Polygon poly;
  for (unsigned int j = 0; j < vertices.size(); j++)
  {
    const VertexBase& v = vertices[j];
    float y = hp.GetY(v.x, v.z);

    float s, t;
    MapST(centrex, centrez, v.x, v.z, size, &s, &t);
    TexVertex texv(s, t);
    SceneVertex sv(v.x, y + m_yOffset, v.z);

    poly.AddVertex(sv);
    poly.AddTexVertex(texv);
  }
  poly.SetPerpendicularNormals(); // bug no 10, create correct normal.
  m_list.push_back(poly);
}

}

