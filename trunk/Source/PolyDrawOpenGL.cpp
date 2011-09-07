/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: PolyDrawOpenGL.cpp,v $
Revision 1.1  2004/09/08 15:42:54  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "PolyDrawOpenGL.h"
#include "Polygon.h"
#include "Vertex.h"
#include "Geometry.h"
#include "Geometry.h"
#include "LeafData.h" // until we have a Polygon subclass

namespace Amju
{
void PolyDrawOpenGL::Draw(Polygon* poly)
{
  // TODO rewrite drqawing code to use big tri lists
  const int MAX_VERTS = 16;

  AmjuGL::Vert verts[MAX_VERTS];

  int sides = poly->m_theVertices.size();
  if (sides > MAX_VERTS) sides = MAX_VERTS;
  bool hasUvs = !poly->m_textureVerts.empty();
  for (int i = 0; i < sides; i++)
  {
    verts[i] = AmjuGL::Vert(
      poly->m_theVertices[i].x, 
      poly->m_theVertices[i].y, 
      poly->m_theVertices[i].z,  
      0, 0, 
      poly->m_theVertices[i].GetNormalX(), 
      poly->m_theVertices[i].GetNormalY(), 
      poly->m_theVertices[i].GetNormalZ());

    if (hasUvs)
    {
      verts[i].m_u = poly->m_textureVerts[i].s();
      verts[i].m_v = poly->m_textureVerts[i].t();
    }
  }

  if (sides == 3)
  {
/*    
    AmjuGL::Vert verts[3] =
    {
      AmjuGL::Vert(poly->m_theVertices[0].x, poly->m_theVertices[0].y, poly->m_theVertices[0].z,  
        0, 0, //poly->m_textureVerts[0].s(), poly->m_textureVerts[0].t(),  
        poly->m_theVertices[0].GetNormalX(), poly->m_theVertices[0].GetNormalY(), poly->m_theVertices[0].GetNormalZ()), 
      AmjuGL::Vert(poly->m_theVertices[1].x, poly->m_theVertices[1].y, poly->m_theVertices[1].z,  
        0, 0, //poly->m_textureVerts[1].s(), poly->m_textureVerts[1].t(),  
        poly->m_theVertices[1].GetNormalX(), poly->m_theVertices[1].GetNormalY(), poly->m_theVertices[1].GetNormalZ()), 
      AmjuGL::Vert(poly->m_theVertices[2].x, poly->m_theVertices[2].y, poly->m_theVertices[2].z, 
        0, 0, //poly->m_textureVerts[2].s(), poly->m_textureVerts[2].t(),  
        poly->m_theVertices[2].GetNormalX(), poly->m_theVertices[2].GetNormalY(), poly->m_theVertices[2].GetNormalZ()), 
    };
    int numUVs = poly->m_textureVerts.size();
    if (numUVs > 2)
    {
        verts[0].m_u = poly->m_textureVerts[0].s();
        verts[0].m_v = poly->m_textureVerts[0].t();
        verts[1].m_u = poly->m_textureVerts[1].s();
        verts[1].m_v = poly->m_textureVerts[1].t();
        verts[2].m_u = poly->m_textureVerts[2].s();
        verts[2].m_v = poly->m_textureVerts[2].t();
    }
*/

    AmjuGL::Tris tris;
    AmjuGL::Tri t;
    t.m_verts[0] = verts[0];
    t.m_verts[1] = verts[1];
    t.m_verts[2] = verts[2];
    tris.push_back(t);
    AmjuGL::DrawTriList(tris);
  }
  else if (sides == 4)
  {
    AmjuGL::Tris tris;
    AmjuGL::Tri t;
    t.m_verts[0] = verts[0];
    t.m_verts[1] = verts[1];
    t.m_verts[2] = verts[2];
    tris.push_back(t);
    t.m_verts[0] = verts[0];
    t.m_verts[1] = verts[2];
    t.m_verts[2] = verts[3];
    tris.push_back(t);
    AmjuGL::DrawTriList(tris);
  }
  else
  {
    AmjuGL::Tris tris;
    for (int i = 0; i < sides - 2; i++)
    {
      AmjuGL::Tri t;
      t.m_verts[0] = verts[0];
      t.m_verts[1] = verts[i + 1];
      t.m_verts[2] = verts[i + 2];
      tris.push_back(t);
    }
    AmjuGL::DrawTriList(tris);
  }
}
}
