/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Polygon.cpp,v $
Revision 1.1.10.3  2007/07/26 18:09:30  jay
Fix stupid debug crash

Revision 1.1.10.2  2007/07/25 07:48:43  jay
Fix SCENE_EDITOR/DEBUG defines

Revision 1.1.10.1  2007/07/15 21:56:58  Administrator
DEBUG mode: Each Plane stores the name of the leaf which created it.
This is for debugging HeightServer issues, where you want to know where
a poly has come from.

Revision 1.1  2004/09/08 15:42:54  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "Polygon.h"
#include "Vertex.h"
#include "File.h"
#include "Geometry.h"
#include "TextureServer.h"
#include "HeightServer.h"
#include "PolyLoader.h"
#include "PolyDrawer.h"
#include "SolidLeaf.h" // Leafdata
#include "SchAssert.h"

namespace Amju
{
PolyDrawer* Polygon::s_polyDrawer = 0;
PolyLoader* Polygon::s_polyLoader = 0;

Polygon::Polygon() 
#if defined(SCENE_EDITOR) || defined(_DEBUG)
  : m_pLeaf(0)
#endif
{
}

#if defined(SCENE_EDITOR) || defined(_DEBUG)
Polygon::Polygon(LeafData* pParent) : m_pLeaf(pParent)
{
  Init();
}

Polygon::Polygon(SolidLeaf* pLeaf)
{
  Assert(pLeaf);
  m_pLeaf = pLeaf->m_pLeafData;
  Init();
}
#endif

#ifdef SCENE_EDITOR
const Matrix& Polygon::GetMatrix() const
{
  Assert(m_pLeaf);
  return m_pLeaf->GetMatrix(); 
}

#endif

void Polygon::Init()
{
}

Polygon::~Polygon()
{
}

const SceneVertex& Polygon::GetVertex(int index) const
{
  return m_theVertices[index];
}

SceneVertex* Polygon::GetVertex(int index)
{
  return &m_theVertices[index];
}

void Polygon::InsertVertex(const SceneVertex& vertex, int index)
{
  VertexVector::iterator it = m_theVertices.begin();
  for (int i = 0; i < index; i++)
  {
    ++it;
  }
  m_theVertices.insert(it, vertex);
}

void Polygon::Draw()
{
  GetDrawer()->Draw(this);
}

void Polygon::SetPerpendicularNormals()
{
  float n[3]; // Normal to this polygon
  // Calculate the normal.
  // We must have at least 3 vertices or we don't have a polygon.
  // Polygon must be plane for this to work.
  float a[3];
  float b[3];
  float c[3];

  a[0] = m_theVertices[0].x;
  a[1] = m_theVertices[0].y;
  a[2] = m_theVertices[0].z;

  b[0] = m_theVertices[1].x;
  b[1] = m_theVertices[1].y;
  b[2] = m_theVertices[1].z;

  c[0] = m_theVertices[2].x;
  c[1] = m_theVertices[2].y;
  c[2] = m_theVertices[2].z;

  Geometry::MakeNormal(a, b, c, n);

  for (unsigned int i = 0; i < m_theVertices.size(); i++)
  {
    m_theVertices[i].SetNormal(n[0], n[1], n[2]);
  }
}

bool Polygon::Load(File* pf, bool hasTexCoords)
{
  // The real loading work is delegated to a PolyLoader object.
  return GetLoader()->Load(pf, this, hasTexCoords);
}

void Polygon::AddVertex(const SceneVertex& vertex)
{
  m_theVertices.push_back(vertex);
}

void Polygon::AddTexVertex(const TexVertex& tv)
{
  m_textureVerts.push_back(tv);
}

#if defined(SCENE_EDITOR) 
//|| defined(_DEBUG)
bool Polygon::Save(File* pf)
{
  // Delegate the saving job to the loader too.
  return GetLoader()->Save(pf, this);
}
#endif

void Polygon::StoreAbsoluteCoords(Matrix m, HeightServer* pHs)
{
#ifdef _DEBUG
  // Convert each vertex to absolute coords; store in Height Server.
  Plane poly(m_pLeaf ? m_pLeaf->GetName().c_str() : "<unknown>");
#else
  Plane poly;
#endif

  for (unsigned int i = 0; i < m_theVertices.size(); i++)
  {
    SceneVertex& sv = m_theVertices[i];
    sv.CalcAbsoluteCoord(m);

    Vec3f v(sv.x, sv.y, sv.z);
    Vec3f absolute = v * m;
    poly.AddVertex(absolute, i);
  }

  pHs->InsertPoly(poly);
}

void Polygon::RecalculateAbsoluteCoords(const Matrix& cm)
{
  for (unsigned int i = 0; i < m_theVertices.size(); i++)
  {
    SceneVertex& sv = m_theVertices[i];
    sv.CalcAbsoluteCoord(cm);
  }
}
}

