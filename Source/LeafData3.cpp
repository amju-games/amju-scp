/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: LeafData.cpp,v $
Revision 1.1  2004/09/08 15:42:51  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "LeafData.h"
#include "Common.h"
#include "TextureServer.h"
#include "TextureMethodFactory.h"
#include <CollisionMesh.h>
#include <StringUtils.h>
#include "SchAssert.h"

using namespace std;

namespace Amju
{
extern void ReportError(const string& msg);

LeafRenderer* LeafData::s_pLeafRenderer;

LeafData::LeafData(const string& name) : m_name(name)
{
  m_displayList = (unsigned int)-1;
  m_smoothNormals = false;
  m_pTextureMethod = 0;
  m_isSolid = true;
  m_isVisible = true;
}

LeafData::~LeafData()
{
  Clear();
}

void LeafData::Clear()
{
  m_polygons.clear();
}

void LeafData::SetTextureMethod(TextureMethod* pTm)
{
  Assert(0);
  m_pTextureMethod = pTm;
}

bool LeafData::Load(const std::string& filename)
{
  std::string objname = GetFileNoExt(filename) + ".obj";
std::cout << "Loading " << objname << "... ";
  if (!m_obj.Load(objname))
  {
std::cout << "Oh no, failed\n";
    return false;
  }
  // TODO populate polygon vec from obj mesh, for collisions
  CollisionMesh cm;
  m_obj.CalcCollisionMesh(&cm);
  const CollisionMesh::Tris& tris = cm.GetAllTris();
  unsigned int numtris = tris.size();
  m_polygons.reserve(numtris);

  for (CollisionMesh::Tris::const_iterator it = tris.begin(); it != tris.end(); ++it)
  {
    const Tri& tri = *it;
    Polygon poly; ///// = new Polygon; // Dear god
    for (int i = 0; i < 3; i++)
    {
      const Vec3f& v = tri.m_verts[i];
      poly.m_theVertices[i] = v; //AddVertex(SceneVertex(v));
    }
    m_polygons.push_back(poly);
  }

std::cout << " loaded " << objname << " OK!\n";
  return true;
}

void LeafData::Draw()
{
  m_obj.Draw();
}

void LeafData::StoreAbsoluteCoords(Matrix m, HeightServer* pHs)
{
  // Given the matrix, we can generate absolute coords for each vertex in
  // each poly in this leaf. This is used by the Height Server.
  for (PolyVector::iterator it = m_polygons.begin();
       it != m_polygons.end(); 
       ++it)
  {
    Polygon* pPoly = &(*it);
    pPoly->StoreAbsoluteCoords(m, pHs);
  }

#if defined(SCENE_EDITOR)
  // matrix not be required except by Editor
  m_matrix = m;
#endif
}

void LeafData::RecalculateAbsoluteCoords(const Matrix& cm)
{
  for (PolyVector::iterator it = m_polygons.begin();
       it != m_polygons.end(); 
       ++it)
  {
    Polygon* pPoly = &(*it);
    pPoly->RecalculateAbsoluteCoords(cm);
  }

#if defined(SCENE_EDITOR)
  // matrix not be required except by Editor
  m_matrix = cm;
#endif
}

}

