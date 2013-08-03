/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: BasicShadow.cpp,v $
Revision 1.17  2008/05/08 10:57:24  jay
New memory management code

Revision 1.16  2008/04/27 06:16:36  jay
AmjuGL API changes

Revision 1.15  2007/12/19 11:44:35  jay
We must explicitly set the texture mode whenever we draw

Revision 1.14  2007/12/14 16:22:04  staff
Fixes for MSVC 8

Revision 1.13  2007/11/24 22:30:59  jay
Use a private Polygon class to draw Shadows; we should remove Polygon
from AmjuGL

Revision 1.12  2007/11/13 21:33:09  jay
Remove all OpenGL calls from game code

Revision 1.11  2007/11/10 13:19:22  jay
LeafData: different version for game/Scene Ed

Revision 1.10  2007/11/09 09:33:16  jay
Game Objects can now be saved if SAVE_GO is defined. SCENE_EDITOR should
not be defined for game code.

Revision 1.9  2007/11/08 00:26:05  jay
First pass at wrapping all OpenGL calls

Revision 1.8  2006/11/16 09:22:13  jay
Set height range for floor polys, so we don't cast shadows on floors which
are too high

Revision 1.7  2006/11/13 22:08:48  jay
Refine height range for getting floor polys

Revision 1.6  2006/11/10 19:30:25  jay
Debug: draw floor poly for shadow

Revision 1.5  2006/10/27 21:49:10  jay
Tweaks

Revision 1.4  2006/10/15 12:05:29  jay
Turn off debug output

Revision 1.3  2006/10/14 20:25:19  Administrator
Debug output, increase height range

Revision 1.2  2006/10/12 19:57:01  jay
Debug output, minor tweaking which could fix some issues with
shadows flickering sometimes.

Revision 1.1  2004/09/08 15:42:44  jay
Added to repository
  
*/

#include "AmjuFirst.h"
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
#include "AmjuFinal.h"
}

//#define SHADOW_DEBUG

namespace Amju
{
void BasicShadow::Polygon::Tesselate()
{
  AMJU_CALL_STACK;

  int numVerts = m_verts.size();
  const AmjuGL::Vert firstVert = m_verts[0];
  AmjuGL::Tris tris;
  for (int i = 1; i < numVerts - 1; i++)
  {
    AmjuGL::Tri tri;
    tri.m_verts[0] = firstVert;
    tri.m_verts[1] = m_verts[i];
    tri.m_verts[2] = m_verts[i + 1];
    tris.push_back(tri);
  }
  m_triList = MakeTriList(tris);
}

void BasicShadow::Polygon::Draw()
{
  AMJU_CALL_STACK;

  //AmjuGL::SetTextureMode(AmjuGL::AMJU_REGULAR);
////  AmjuGL::DrawTriList(m_tris);
  AmjuGL::Draw(m_triList);
}

void BasicShadow::Polygon::AddVertex(const AmjuGL::Vert& v)
{
  AMJU_CALL_STACK;

  m_verts.push_back(v);
}

PoolTexture* BasicShadow::s_pTexture;

bool BasicShadow::Init()
{
  AMJU_CALL_STACK;

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
  AMJU_CALL_STACK;

  m_mult = 1.0f;

  static const float HEIGHT_RANGE_DOWN = 3.0f;
  static const float HEIGHT_RANGE_UP = 2.0f;

  m_heightRangeUp = HEIGHT_RANGE_UP; 
  m_heightRangeDown = HEIGHT_RANGE_DOWN;
}

void BasicShadow::SetHeightRange(float up, float down)
{
  AMJU_CALL_STACK;

  m_heightRangeUp = up; 
  m_heightRangeDown = down;
}

void BasicShadow::BindTexture()
{ 
  AMJU_CALL_STACK;

  Assert(s_pTexture);
  s_pTexture->Bind();
}

void BasicShadow::RecalculateVerts()
{
  AMJU_CALL_STACK;

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
  AMJU_CALL_STACK;

  return m_verts.size();
}

float BasicShadow::GetShadowX(int index)
{
  AMJU_CALL_STACK;

  Assert(index < GetNumShadowVerts());
  return m_verts[index].first;
}

float BasicShadow::GetShadowZ(int index)
{
  AMJU_CALL_STACK;

  Assert(index < GetNumShadowVerts());
  return m_verts[index].second;
}

void BasicShadow::DrawList() 
{
  AMJU_CALL_STACK;

#ifdef SHADOW_DEBUG
  if (m_list.empty())
  {
std::cout << "Shadow list is empty for obj: " << m_pCaster->GetId() 
  << ": " << m_pCaster->GetTypeName() << "\n";
  }
#endif

  // m_mult is between 1 (full size shadow) and 0 (no shadow). We use this to 
  // change the transparency of the shadow: smaller shadows are more
  // translucent.
  // Push the new alpha value, RGB is unchanged.
  Engine::Instance()->PushColour(1, 1, 1, m_mult);

  BindTexture();

  AmjuGL::PushAttrib(AmjuGL::AMJU_BLEND);
  AmjuGL::Enable(AmjuGL::AMJU_BLEND);

  // Allow read-, but not write-access to the depth buffer. This overlaps
  // shadows properly.
  AmjuGL::Disable(AmjuGL::AMJU_DEPTH_WRITE); //ZWrite(false);

  // Adjust height of shadow with height of camera. This is because, as the 
  // camera gets higher, the distance between shadow and ground needs to 
  // increase to avoid z-fighting.
  AmjuGL::PushMatrix();

  for (ShadowList::iterator it = m_list.begin(); it != m_list.end(); ++it)
  {
    it->Draw();
  }

  AmjuGL::PopMatrix();

  //AmjuGL::EnableZWrite(true);
  AmjuGL::Enable(AmjuGL::AMJU_DEPTH_WRITE); //ZWrite(false);
  AmjuGL::PopAttrib();

  Engine::Instance()->PopColour(); // pop off the alpha change.
}

void BasicShadow::RecalcMult(float y, float h)
{
  AMJU_CALL_STACK;

  m_mult = (1.0f - (y - h)/s_maxHeight); 
}

void BasicShadow::RecalculateList(
  float x, float y, float z,
  float size,
  const HeightServer& hs)
{
  AMJU_CALL_STACK;

  m_list.clear();

  // Get the height poly which is the heighest below y.
  // TODO TEMP TEST Add offset..?
  const HeightPoly* pHp = hs.GetHeightPoly(x, y + 1.0f, z);
  if (!pHp)
  {
#ifdef SHADOW_DEBUG
std::cout << "BASIC SHADOW for " << m_pCaster->GetId() << " " 
  << m_pCaster->GetTypeName() << ": No height poly.\n";
#endif

    return;
  }   
  float h = pHp->GetY(x, z);
  if (y < h)
  {
    y = h;

#ifdef SHADOW_DEBUG
std::cout << "BASIC SHADOW for " << m_pCaster->GetId() << " " 
  << m_pCaster->GetTypeName() << ": obj height below ground height ?\n";
#endif

  }
  Assert(y >= h);
  if (y - s_maxHeight >= h) 
  {
    // We are too far above the ground to cast a shadow.
#ifdef SHADOW_DEBUG
std::cout << "BASIC SHADOW for " << m_pCaster->GetId() << " " 
  << m_pCaster->GetTypeName() << ": TOO HIGH TO CAST SHADOW??\n";
#endif

    return;
  }
  else 
  {
    RecalcMult(y, h);
// TODO TEMP TEST//    size *= m_mult;
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
  // TODO get exact offset using trig. and size.
  float ymax = y + m_heightRangeUp; 
  float ymin = y - m_heightRangeDown;
  std::list<const HeightPoly*> heightPolys;

  // Get all height polys which this shadow may cover.
  hs.GetAllHeightPolys(x2, x1, ymax, ymin, z2, z1, &heightPolys);

#ifdef SHADOW_DEBUG
  if (heightPolys.empty())
  {
    std::cout << "BASIC SHADOW for obj: " << m_pCaster->GetId()
      << " " << m_pCaster->GetTypeName() << ": NO HEIGHT POLYS!!!\n";
  }
#endif

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
#ifdef SHADOW_DEBUG
    // Draw the floor poly
    //glLineWidth(4);
    AmjuGL::Disable(GL_TEXTURE_2D);
    Engine::Instance()->PushColour(0, 0, 0, 1.0f);
    (*it)->Draw();
    Engine::Instance()->PopColour();
    AmjuGL::Enable(GL_TEXTURE_2D);
#endif

    // Now for each height poly, clip the shadow poly to it.
    ClipShadowToHeightPoly(**it, x, y, z, size);
  }
#ifdef SHADOW_DEBUG
  if (m_list.empty())
  {
    std::cout << "HEY! Empty shadow list!\n";
  }
#endif
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
  AMJU_CALL_STACK;

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
  AMJU_CALL_STACK;

  // Clip the shadow poly to the given height poly (which should be a triangle).

  // One or more vertices of the shadow may fall outside the poly the shadow 
  // is cast on. We have to get the intersection of the two polygons.
  // Luckily we can use the GPC ("Generic Polygon Clipper").

  // Vector of shadow polygon vertices after clipping.
  std::vector<Vec3f> vertices;

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

  // Convert result into vector of Vec3f.
  if (result.num_contours != 1)
  {
    // Error, there should be one gpc 'contour' (== polygon here).
#ifdef SHADOW_DEBUG
std::cout << "SHADOW: BAILING before cliiping shadow poly to height poly!\n";
#endif
    return;
  }

  gpc_vertex_list& vlist = result.contour[0];
  for (int k = 0; k < vlist.num_vertices; k++)
  {
    gpc_vertex& v = vlist.vertex[k];
    Vec3f vb((float)v.x, 0, (float)v.y);
    vertices.push_back(vb);
  }

  // Free the contour in the polygon result.
  gpc_free_polygon(&result);

  // TODO we could do this in the loop above.
  Polygon poly;
  for (unsigned int j = 0; j < vertices.size(); j++)
  {
    const Vec3f& v = vertices[j];
    float y = hp.GetY(v.x, v.z);
    Assert(y != HeightServer::BadHeight);

    float s, t;
    MapST(centrex, centrez, v.x, v.z, size, &s, &t);
    //TexVertex texv(s, t);
    //SceneVertex sv(v.x, y + OFFSET, v.z);

    poly.AddVertex(AmjuGL::Vert(v.x, y + OFFSET, v.z, s, t, 0, 1.0f, 0));
    //poly.AddTexVertex(texv);
  }

  // Finished creating the polygon - now tesselate it
  poly.Tesselate();

  //poly.SetPerpendicularNormals(); // bug no 10, create correct normal.
  m_list.push_back(poly);
}

}

