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
#include "LeafRenderer.h"
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
//  if (m_displayList != -1)
//  {
//    glDeleteLists(m_displayList, 1);
//  }

  Clear();
}

void LeafData::Clear()
{
  // Delete polygons allocated in Load().
  for (PolyVector::iterator it = m_polygons.begin();
       it != m_polygons.end(); ++it)
  {
    Polygon* pPoly = *it;
    delete pPoly;
  }
  m_polygons.clear();
}

void LeafData::SetTexture(PoolTexture* pTexture)
{
  Assert(0);
  Assert(m_pTextureMethod.GetPtr());
  if (m_pTextureMethod.GetPtr()) // TODO unnecessary check
  {
    m_pTextureMethod->SetTexture(pTexture);
  }
}

PoolTexture* LeafData::GetTexture()
{
  Assert(0);
  Assert(m_pTextureMethod.GetPtr());
  if (m_pTextureMethod.GetPtr())
  {
    return m_pTextureMethod->GetTexture();
  }
  return 0;
}

void LeafData::CreatePolygonDisplayList()
{
/*
  // Draw each polygon making up the solid.
  for (PolyVector::iterator it = m_polygons.begin();
       it != m_polygons.end(); 
       ++it)
  {
    Polygon* pPoly = *it;
    pPoly->Draw();
  }
*/
}

void LeafData::CreateDisplayList()
{
/*
  if (!m_isVisible)
  {
    return;
  }

  if (!m_pTextureMethod.GetPtr())
  {
    string error = m_name;
    error += ": No Texture Method exists.";
    ReportError(error.c_str());
    return;
  }

//  if (m_displayList != -1)
//  {
//    glDeleteLists(m_displayList, 1);
//  }

//  m_displayList = glGenLists(1); // create one display list name
//  glNewList(m_displayList, GL_COMPILE); // start compiling display list

  m_pTextureMethod->Bind(); // use this texture
  m_pTextureMethod->BeginDisplayList(this); // set up display list for this method..

  CreatePolygonDisplayList(); // generate display list for solid shape

  m_pTextureMethod->EndDisplayList(this); // cleanup display list for this method
//  glEndList(); // finish compiling display list

  // Having created the display list, it's ok to delete the data.
  // Except that this creates probs for anything that needs to know the
  // size, abs coords etc of the leaf :-(
  //Clear();
*/
}

void LeafData::ExecuteDisplayList()
{
  Assert(0);
/*
  m_obj.Draw();

  CreateDisplayList();
  if (m_displayList != -1)
  {
    glCallList(m_displayList);
  }
*/
}

bool LeafData::HasTextureCoords() const
{
  Assert(0);
  if (m_pTextureMethod.GetPtr())
  {
    return m_pTextureMethod->HasTextureCoords();
  }
  return false;
}

void LeafData::SetTexGenMode() const
{
  Assert(0);
  Assert(m_pTextureMethod.GetPtr());
  m_pTextureMethod->SetTexGenMode();
}

void LeafData::SetTextureMethod(TextureMethod* pTm)
{
  m_pTextureMethod = pTm;
}

bool LeafData::LoadTexture(File* pf)
{
  // Get the texture type. Create the appropriate TextureMethod.
  if (pf->GetVer() >= 5)
  {
    // Version 5 and above supports different texture methods.
    TextureMethod* pTm = TextureMethodFactory::Create(pf);
    if (!pTm)
    {
      return false;
    }
    SetTextureMethod(pTm);
  }
  else
  {
    SetTextureMethod(new TexMethodRegular);
  }

  // Load and create binding.
  if (!m_pTextureMethod->Load(pf))
  {
    pf->ReportError("Couldn't load texture.");
    return false;
  }

  return true;
}

bool LeafData::LoadNormalsFlag(File* pf)
{
  int smoothflag = 0;
  if (!pf->GetInteger(&smoothflag))
  {
    pf->ReportError("Expected smooth/perpendicular normal flag.");
    return false;
  }
  if (smoothflag == 1)
  {
    m_smoothNormals = true;
  }
  else if (smoothflag == 2)
  {
    m_smoothNormals = false;
  }
  else
  {
    pf->ReportError("Bad normal flag.");
    return false;
  }
  return true;
}

bool LeafData::LoadPolygons(File* pf)
{  
  // Get number of polygons.
  int polys = 0;
  if (!pf->GetInteger(&polys))
  {
    pf->ReportError("Expected number of polygons.");
    return false;
  }

  for (int i = 0; i < polys; i++)
  {
    // TODO Have a Polygon subclass for Texture-Coord polys ?
#if defined(SCENE_EDITOR)
    Polygon* pPoly = new Polygon(this);
#else
    Polygon* pPoly = new Polygon;
#endif
    bool hasTexCoords = HasTextureCoords();
    if (!pPoly->Load(pf, hasTexCoords))
    {
      delete pPoly;
      return false;
    }

    m_polygons.push_back(pPoly);
  }
  return true;
}

bool LeafData::LoadVersion6(File* pf)
{
  if (!LoadPolygons(pf))
  {
    return false;
  }

  CreateNormals();

  return true;
}

bool LeafData::LoadSolidityFlag(File* pf)
{
  m_isSolid = true;
  if (pf->GetVer() < 8)
  {
    return true;
  }
  int solid;
  if (!pf->GetInteger(&solid))
  {
    pf->ReportError("Expected solidity flag.");
    return false;
  }
  m_isSolid = (solid == 1);
  return true;
}

bool LeafData::LoadVisibleFlag(File* pf)
{
  m_isVisible = true;
  if (pf->GetVer() < 11)
  {
    return true;
  }
  int vis;
  if (!pf->GetInteger(&vis))
  {
    pf->ReportError("Expected visibility flag.");
    return false;
  }
  m_isVisible = (vis == 1);
  return true;
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
    Polygon* poly = new Polygon; // Dear god
    for (int i = 0; i < 3; i++)
    {
      Vec3f v = tri.m_verts[i];
      poly->AddVertex(SceneVertex(v));
    }
    m_polygons.push_back(poly);
  }

std::cout << " loaded " << objname << " OK!\n";
  return true;
}

bool LeafData::Load(File* pf)
{
  Assert(0);
  return false;

/*
  Clear();

  if (pf->GetVer() < 7)
  {
    return LoadVersion6(pf);
  }
  
  if (!LoadTexture(pf))
  {
    pf->ReportError("Failed to load texture.");
    return false;
  }

  // load smooth normals flag.
  if (!LoadNormalsFlag(pf))
  {
    pf->ReportError("Expected normals flag.");
    return false;
  } 

  if (!LoadPolygons(pf))
  {
    return false;
  }

  // Latest addition to file format: a flag specifying whether the Leaf contributes
  // to HeightServers or not. If it doesn't, the Leaf is visible but does not
  // provide a solid floor or walls. E.g. leaves, water etc.
  if (!LoadSolidityFlag(pf))
  {
    return false;
  }

  // Visible flag, allowing invisible, blocking solids.
  if (!LoadVisibleFlag(pf))
  {
    return false;
  }

  CreateNormals();

  return true;
*/
}

void LeafData::AddPolygon(Polygon* pPoly)
{
  Assert(0);
  m_polygons.push_back(pPoly);
}

void LeafData::CreateSmoothNormals()
{
  Assert(0);


  // Smoothing
  // ---------
  // For each vertex in each poly, find any other polys with a vertex that's the same
  // (or close enough). Hmm, indices into a list of vertices would be better.
  // Anyway for each vertex, average all the normals.
  
  // Map vertices to normals. The plan is to iterate through every vertex in the leaf.
  // We add the normals up, so they point in the average direction.
  // Then we normalise each one, and iterate again to copy each normal to its vertex.
  typedef map<Vec3f, Vec3f> VertexToNormal;
  VertexToNormal vmap;

  // Iterate through every vertex. Add its corresponding perpendicular to the map.
  // Where a vertex already exists in the map, the normals will be added, so we 
  // point in the average direction.
  int i;
  for (i = 0; i < (int)m_polygons.size(); i++)
  {
    const Polygon* pPoly = m_polygons[i];
    for (int j = 0; j < pPoly->Vertices(); j++)
    {
      Vec3f v(pPoly->GetVertex(j).x,
                   pPoly->GetVertex(j).y,
                   pPoly->GetVertex(j).z );
 
      const Vec3f n(pPoly->GetVertex(j).GetNormalX(),
                         pPoly->GetVertex(j).GetNormalY(),
                         pPoly->GetVertex(j).GetNormalZ() );
      
      vmap[v] += n;
    }
  }
  // Now the map contains one copy of each vertex, with an average normal.
  // Iterate through all the vertices again, replacing the exisiting
  // perpendicular normal with one from the map.
  // Vec3f objects make sure each normal has unit length.
  for (i = 0; i < (int)m_polygons.size(); i++)
  {
    const Polygon* pPoly = m_polygons[i];
    for (int j = 0; j < m_polygons[i]->Vertices(); j++)
    {
      Vec3f v(pPoly->GetVertex(j).x,
                   pPoly->GetVertex(j).y,
                   pPoly->GetVertex(j).z );

      const Vec3f n = vmap[v];

      m_polygons[i]->GetVertex(j)->SetNormal(n.x, n.y, n.z);
    }
  }
}

void LeafData::CreateNormals()
{
  Assert(0);

  for (unsigned int i = 0; i < m_polygons.size(); i++)
  {
    m_polygons[i]->SetPerpendicularNormals();
  }
  if (m_smoothNormals)
  {
    CreateSmoothNormals();
  }
}

#if defined(SCENE_EDITOR)

bool LeafData::SaveTexture(File* jf)
{
  if (!m_pTextureMethod.GetPtr())
  {
    string error = m_name;
    error += ": No Texture Method for leaf data.";
    ReportError(error.c_str());
    return false;
  }

  return m_pTextureMethod->Save(jf);
}

bool LeafData::SaveNormalsFlag(File* jf)
{
  // Write smooth/perpendicular normals flag
  jf->WriteComment("// Normals flag 1 = smooth, 2 = perpendicular.");
  jf->WriteInteger(m_smoothNormals ? 1 : 2);
  return true;
}

bool LeafData::SaveSolidityFlag(File* pf)
{
  pf->WriteComment("// Solidity flag: 1 == solid");
  pf->WriteInteger(m_isSolid ? 1 : 0);
  return true;
}

bool LeafData::SaveVisibilityFlag(File* pf)
{
  pf->WriteComment("// Visible ?");
  return (pf->WriteInteger(m_isVisible ? 1 : 0));
}

bool LeafData::Save(File* jf)
{
  if (!SaveTexture(jf))
  {
    return false;
  }

  // Save normals flag
  SaveNormalsFlag(jf);

  jf->WriteComment("// Number of polygons follows on next line.");
  int polys = m_polygons.size();
  jf->WriteInteger(polys);
  for (int i = 0; i < polys; i++)
  {
    jf->WriteComment(File::SEPARATOR);
    char buf[100];
    sprintf(buf, "// Polygon %d follows.", i);
    jf->WriteComment(buf);
    m_polygons[i]->Save(jf);
  }

  SaveSolidityFlag(jf);
  SaveVisibilityFlag(jf);

  return true;
}
#endif

void LeafData::Draw()
{
  m_obj.Draw();

/*
  Assert(s_pLeafRenderer);
  if (s_pLeafRenderer)
  {
    s_pLeafRenderer->Draw(this);
  }
*/
}

bool LeafData::DeletePolygon(Polygon* p)
{
  Assert(0);

  for (PolyVector::iterator it = m_polygons.begin();
       it != m_polygons.end(); 
       ++it)
  {
    Polygon* pPoly = *it;
    if (pPoly == p)
    {
      // Found it - delete.
      m_polygons.erase(it);
      delete p; // Leaf data owns all polygons
      return true;
    }
  }
  return false; // couldn't find it.
}

void LeafData::StoreAbsoluteCoords(Matrix m, HeightServer* pHs)
{
  // Given the matrix, we can generate absolute coords for each vertex in
  // each poly in this leaf. This is used by the Height Server.
  for (PolyVector::iterator it = m_polygons.begin();
       it != m_polygons.end(); 
       ++it)
  {
    Polygon* pPoly = *it;
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
    Polygon* pPoly = *it;
    pPoly->RecalculateAbsoluteCoords(cm);
  }

#if defined(SCENE_EDITOR)
  // matrix not be required except by Editor
  m_matrix = cm;
#endif
}

void LeafData::MakeDisplayList()
{
  Assert(0);
/*
  Assert(s_pLeafRenderer);
  if (s_pLeafRenderer)
  {
    s_pLeafRenderer->Init(this);
  }
*/
}
}

