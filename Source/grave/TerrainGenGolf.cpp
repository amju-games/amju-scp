/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: TerrainGenGolf.cpp,v $
Revision 1.1.2.2  2006/08/14 17:50:26  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:41  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1  2004/09/08 15:43:07  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <iostream>
#include "TerrainGenPool.h"
#include "File.h"
#include "Texture.h"
#include "TextureServer.h"
#include "SolidLeaf.h"
#include "SolidComposite.h"
#include "TextureServer.h"
#include "TextureMethod.h"
#include "Terrain/HeightmapFromTexture.h"
#include "Terrain/HeightmapFromHeightmapRegion.h"
#include "SchAssert.h"

namespace Amju
{
TerrainGenPool::TerrainGenPool() 
{
  m_leafgen.SetChopFloor(false);
}

#ifdef SCENE_EDITOR
bool TerrainGenPool::Save(File* pf)
{
  int numTex = m_textures.size();
  pf->WriteInteger(numTex);
  for (int i = 0; i < numTex; i++)
  {
    Texture* pTex = m_textures[i];
    if (!TextureServer::Instance()->Save(pf, pTex))
    {
      pf->ReportError("Failed to save terrain texture.");
      return false;
    }
  }
  pf->WriteInteger(GetQuadtreeDepth());
  // Terrain size (side length)
  pf->WriteFloat(GetSize());
  // Height
  pf->WriteFloat(GetVerticalSize());

  if (!m_pHeightmap->Save(pf))
  {
    return false;
  }

  return true;
}
#endif

bool TerrainGenPool::Load(File* pf)
{
  // Get textures for terrain leaves
  int numTextures = 0;
  if (!pf->GetInteger(&numTextures))
  {
    pf->ReportError("Expected number of textures for terrain.");
    return false;
  }
  for (int i = 0; i < numTextures; i++)
  {
    Texture* pTex = TextureServer::Instance()->Get(pf);
    if (!pTex)
    {
      pf->ReportError("Bad texture for terrain.");
      return false;
    }
    pTex->CreateBinding(Texture::REGULAR);

    AddTexture(pTex);
  }

  // Get quadtree size and grid size
  int quadtreeDepth = 0;
  if (!pf->GetInteger(&quadtreeDepth))
  {
    pf->ReportError("Expected quadtree depth.");
    return false;
  }
  SetQuadtreeDepth(quadtreeDepth);

  int terrainSize = 0;
  if (!pf->GetInteger(&terrainSize))
  {
    pf->ReportError("Expected terrain size (side length).");
    return false;
  }

  SetSize(terrainSize);

  // set vertical size
  float verticalSize = 0;
  if (!pf->GetFloat(&verticalSize))
  {
    pf->ReportError("Expected vertical terrain size.");
    return false;
  }

  SetVerticalSize(verticalSize);
  
  // TODO Heightmap factory, then load() as virtual Heightmap member.
  HeightmapFromTexture* pHm = new HeightmapFromTexture;

  if (!pHm->Load(pf))
  {
    return false;
  }

  SetHeightmap(pHm);

  return true;
}

void TerrainGenPool::AddTexture(Texture* pTexture)
{
  m_textures.push_back(pTexture);
}

Texture* TerrainGenPool::GetTextureForLeaf(int index)
{
  Assert(!m_textures.empty());
  int s = m_textures.size();
  int i = index % s;
  return m_textures[i];
}

Heightmap* TerrainGenPool::GetHeightmapForLeaf(
  int index, float x, float z, float size)
{
  // index is unique ID for leaf.
  // x and z are coords of centre of leaf in world units.
  // size is side length of single leaf in world units.

  // TODO

  // Get a region from the heightmap.
  HeightmapFromHeightmapRegion* pHeights = new HeightmapFromHeightmapRegion;
  pHeights->SetHeightmap(m_pHeightmap);

  // Region is a square, units are pixels (or rather heightmap units).
  // Side length of region is 
  //  total heightmap side length / number of leaves per side.
  // The number of leaves per side is 2 ^ quad tree recursion depth.
  // Also the no. of leaves is size / total world size.
  int w, h = 0;
  m_pHeightmap->GetSize(&w, &h);
  Assert(w == h);
  int regionSize = (int)(size / (float)m_size * (float)h);
  // Each leaf of the quadtree has a mini-heightmap of 'regionSize' pixels square.

  // regionX and regionZ are the coords of the region within the total
  // heightmap. 
  // Regions must overlap by one pixel for seamless terrain ???

  float size2 = size * 0.5f;
  // orgX, orgZ are world coords of corner of leaf, (rather than the centre),
  // adjusted so the extent is 0 - size, rather than from -size/2 to +size/2.
  float orgX = x - size2  + (float)m_size * 0.5f;
  float orgZ = z - size2  + (float)m_size * 0.5f;

  // Map (orgX, orgZ) to corresponding point in the terrain heightmap.
  // (regionX, regionZ) is a pixel coord.
  int regionX = (int)(orgX / m_size * (float)w);
  int regionZ = (int)(orgZ / m_size * (float)h);

  // Decrement regionX and regionZ by one pixel for each leaf position they are
  // away from the origin.
  // This is so regions overlap by one pixel, so terrain leaves always join up.
  int decX = regionX / regionSize;
  int decZ = regionZ / regionSize;
  regionX -= decX;
  regionZ -= decZ;

  std::cout << "region (" << decX << ", " << decZ << ") coords: (" << regionX << ", " << regionZ << ")" << std::endl; 

  pHeights->SetRegion(regionX, regionX + regionSize, regionZ, regionZ + regionSize); 

/*
  HeightmapFromTexture* pHeights = new HeightmapFromTexture;
  pHeights->SetTexture(TextureServer::Instance()->Get("wh8.bmp", ""));
*/
  return pHeights;
}

PSolidComponent TerrainGenPool::CreateLeaf(int leafIndex, float x, float z, float size)
{
  Heightmap* pHeights = GetHeightmapForLeaf(leafIndex, x, z, size);
  m_leafgen.SetHeightmap(pHeights);
  int w, h = 0;
  pHeights->GetSize(&w, &h);
  Assert(w == h);
  Assert(h > 1);
  float horUnitSize = size / (float)(h - 1);
  m_leafgen.SetHorUnitSize(horUnitSize, horUnitSize); // (x, z)
  m_leafgen.SetVertUnitSize(m_verticalSize);

  PSolidComponent pTerrain = m_leafgen.Create(1.0f, 1.0f); // (tx, tz)

  SolidLeaf* pLeaf = dynamic_cast<SolidLeaf*>(pTerrain.GetPtr());
  Assert(pLeaf);
  // Do some stuff not done by the terrain generator:
  // add a texture, add HeightServer; create OpenGL display list.
  pLeaf->SetTextureMethod(new TexMethodRegular);
  pLeaf->SetTexture(GetTextureForLeaf(leafIndex)); 
  pLeaf->CreateDisplayList();

  return pLeaf;
}

void TerrainGenPool::SetHeightmap(PHeightmap p)
{
  m_pHeightmap = p;
}

void AddPolys(
  SolidLeaf* pLeaf, 
  const SceneVertex& a,
  const SceneVertex& b,
  const SceneVertex& c,
  const SceneVertex& d)
{
  Polygon* pPoly1 = new Amju::Polygon;
  pPoly1->AddVertex(d); 
  pPoly1->AddVertex(b); 
  pPoly1->AddVertex(a); 

  // Add texture coords
  pPoly1->AddTexVertex(TexVertex(0, 0)); 
  pPoly1->AddTexVertex(TexVertex(1.0f, 1.0f));
  pPoly1->AddTexVertex(TexVertex(0, 1.0f)); 

  pPoly1->SetPerpendicularNormals();
  pLeaf->AddPolygon(pPoly1);

  Polygon* pPoly2 = new Amju::Polygon;
  pPoly2->AddVertex(b);
  pPoly2->AddVertex(d);
  pPoly2->AddVertex(c);

  pPoly2->AddTexVertex(TexVertex(1.0f, 1.0f));
  pPoly2->AddTexVertex(TexVertex(0, 0));  
  pPoly2->AddTexVertex(TexVertex(1.0f, 0));

  pPoly2->SetPerpendicularNormals();
  pLeaf->AddPolygon(pPoly2);
}

PSolidComponent TerrainGenPool::CreateSides(float tx, float tz)
{
  SolidLeaf* pLeaf = new SolidLeaf;
  int size; // no of data points on heightmap edge
  m_pHeightmap->GetSize(&size, &size);
  float step = (float)GetSize() / (float)(size - 1);

  float max = (float)GetSize();
  float med = max * 0.5f;
 
  int i = 0;
  for (i = 0; i < (size - 1); i++)
  {
    float x1 = (float)i * step - med;
    float x2 = (float)(i + 1) * step - med;
    float z = med;
    float y11 = (float)m_pHeightmap->GetHeight(i, 0) * (float)m_verticalSize;
    float y12 = (float)m_pHeightmap->GetHeight(i + 1, 0) * (float)m_verticalSize;
    SceneVertex a(x1, y11, -z);
    SceneVertex b(x2, y12, -z);
    SceneVertex c(x2, 0, -z);
    SceneVertex d(x1, 0, -z);
    AddPolys(pLeaf, d, c, b, a);

    float y21 = (float)m_pHeightmap->GetHeight(i, size - 1) * (float)m_verticalSize;
    float y22 = (float)m_pHeightmap->GetHeight(i + 1, size - 1) * (float)m_verticalSize;
    SceneVertex e(x1, y21, z);
    SceneVertex f(x2, y22, z);
    SceneVertex g(x2, 0, z);
    SceneVertex h(x1, 0, z);
    AddPolys(pLeaf, e, f, g, h);
/*
    float z1 = x1;
    float z2 = x2;
    float x = med;
    float y31 = (float)m_pHeightmap->GetHeight(0, i) * (float)m_verticalSize;
    float y32 = (float)m_pHeightmap->GetHeight(0, i + 1) * (float)m_verticalSize;
    SceneVertex j(x, y31, z1);
    SceneVertex k(x, y32, z2);
    SceneVertex l(x, 0, z2);
    SceneVertex m(x, 0, z1);
    AddPolys(pLeaf, m, l, k, j);

    float y41 = (float)m_pHeightmap->GetHeight(size - 1, i) * (float)m_verticalSize;
    float y42 = (float)m_pHeightmap->GetHeight(size - 1, i + 1) * (float)m_verticalSize;
    SceneVertex n(-x, y41, z1);
    SceneVertex o(-x, y42, z2);
    SceneVertex p(-x, 0, z2);
    SceneVertex q(-x, 0, z1);
    AddPolys(pLeaf, n, o, p, q);
*/
  }

  // Do some stuff not done by the terrain generator:
  // add a texture, add HeightServer; create OpenGL display list.
  pLeaf->SetTextureMethod(new TexMethodRegular);
  // TODO CONFIG
  pLeaf->SetTexture(TextureServer::Instance()->Get("bw.bmp", "")); 
  pLeaf->CreateDisplayList();

  return pLeaf;
}

PSolidComponent TerrainGenPool::CreateBottom(float tx, float tz)
{
  SolidComposite* pComp = new SolidComposite;

  return pComp;
}

}

