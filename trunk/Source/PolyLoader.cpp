/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: PolyLoader.cpp,v $
Revision 1.2  2004/09/18 14:35:13  Administrator
Really sucky hack to get Scene Editor to link

Revision 1.1  2004/09/08 15:42:54  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "PolyLoader.h"
#include "Polygon.h"
#include "Vertex.h"
#include "File.h"
#include "Geometry.h"
#include "TextureServer.h"
#include "HeightServer.h"
#include "PolyLoader.h"
#include "PolyDrawer.h"
#include "LeafData.h"
#include "SchAssert.h"

namespace Amju
{
#if defined(SCENE_EDITOR)
bool PolyLoader::Save(File* pf, Polygon* poly)
{ 
  // Write number of vertices
  int verts = poly->m_theVertices.size();
  pf->WriteComment("// Number of vertices");
  pf->WriteInteger(verts);

  // Write each vertex.
  for (int i = 0; i < verts; i++)
  {
    char buf[100];
    sprintf(buf, "// Vertex %d follows.", i);
    pf->WriteComment(File::SEPARATOR);    
    pf->WriteComment(buf);
    SceneVertex sv = poly->m_theVertices[i];

    // *** HACK ***
    // MSVC Won't link SceneEditor with this call to SceneVertex::Save():
    // sv.Save(pf);
    // So we are just going to have to save the coords until I
    // can work out what the problem is :-(
    pf->WriteFloat(sv.x);
    pf->WriteFloat(sv.y);
    pf->WriteFloat(sv.z);
    // *** HACK ***

    // Texture coords
    if (poly->m_pLeaf->HasTextureCoords())
    {
      float s = 0;
      float t = 0;
      if (poly->m_textureVerts.size() > i)
      {
        s = poly->m_textureVerts[i].s();
        t = poly->m_textureVerts[i].t();
      }
      else
      {
        pf->ReportError("Bad number of texture coords in leaf.");
      }
      pf->WriteFloat(s);
      pf->WriteFloat(t);
    }
  }
  return true;
}

#endif

bool PolyLoader::LoadTexVert(File* pf, Polygon* poly)
{
  // If the texture mode for the owning Leaf is the "Specify (s, t)" kind
  // (REGULAR) then we need some texture coords now.
#if defined(SCENE_EDITOR)
  Assert(poly->m_pLeaf->HasTextureCoords());
#endif
  
  float s = 0;
  float t = 0;
  if (!pf->GetFloat(&s))
  {
    pf->ReportError("Expected texture s-coord");
    return false;
  }

  if (!pf->GetFloat(&t))
  {
    pf->ReportError("Expected texture t-coord");
    return false;
  }
  TexVertex tv;
  tv.SetS(s);
  tv.SetT(t);
  poly->AddTexVertex(tv);

  return true;
}

bool PolyLoader::LoadVersion6(File* pf, Polygon* poly)
{
  // Specify texture (1) or colour (0)
  int c = 0;
  if (!pf->GetInteger(&c))
  {
    pf->ReportError("Expected texture/colour flag.");
    return false;
  }

  if (c == 0)
  {
  }
  else
  {
    LoadTexture(pf, poly);
  }

  // v.4 and above: next integer is treated as a flag. 1 means include the
  // poly in getting the height; 0 means ignore the poly when getting the
  // height.
  if (pf->GetVer() >= 4)
  {
    int h = 0;
    if (!pf->GetInteger(&h))
    {
      pf->ReportError("Expected ignore height flag.");
      return false;
    }
  }

  // Specify normal (1) or calculate it (0) ?
  int normal = 0;
  if (!pf->GetInteger(&normal))
  {
    pf->ReportError("Expected normal flag.");
    return false;
  }

  // Get number of vertices.
  int vertices = 0;
  if (!pf->GetInteger(&vertices))
  {
    pf->ReportError("Expected number of vertices.");
    return false;
  }
  
  if (vertices < 3)
  {
    pf->ReportError("Bad number of vertices.");
    return false;
  }
  // Get each one.
  for (int i = 0; i < vertices; i++)
  {
    SceneVertex v;

    if (!v.Load(pf))
    {
      return false;
    }

    if (!LoadTexVert(pf, poly))
    {
      return false;
    }

    poly->m_theVertices.push_back(v);
  }

  return true;
}

bool PolyLoader::Load(File* pf, Polygon* poly, bool hasTexCoords)
{
  if (pf->GetVer() < 7)
  {
    return LoadVersion6(pf, poly);
  }

  // Get number of vertices.
  int vertices = 0;
  if (!pf->GetInteger(&vertices))
  {
    pf->ReportError("Expected number of vertices.");
    return false;
  }
  
  if (vertices < 3)
  {
    pf->ReportError("Bad number of vertices.");
    return false;
  }

  if (vertices > 3)
  {
    pf->ReportError("Bad number of vertices, only 3 is ok");
    return false;
  }
  
  // Get each one.
  for (int i = 0; i < vertices; i++)
  {
    SceneVertex v;

    if (!v.Load(pf))
    {
      return false;
    }

    if (hasTexCoords && !LoadTexVert(pf, poly))
    {
      return false;
    }

    poly->m_theVertices.push_back(v);
  }

  return true;

}

bool PolyLoader::LoadTexture(File* pf, Polygon* poly)
{
  // This code is here to keep the file in sync.
  TextureServer::Instance()->Get(pf);
  int i = 0;  
  pf->GetInteger(&i);

  return true;
}
}

