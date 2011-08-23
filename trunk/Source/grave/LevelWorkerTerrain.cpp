/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: LevelWorkerTerrain.cpp,v $
Revision 1.1.10.1  2006/08/14 17:38:04  jay
Rename "Pool"

Revision 1.1  2004/09/08 15:42:40  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <string>
#include "LevelWorkerTerrain.h"
#include "Level.h"
#include "SolidComposite.h"
#include "SolidLeaf.h"
#include "TextureServer.h"
#include "TextureMethod.h"
//#include "Pool/TerrainGenPool.h"
#include "Terrain/HeightmapFromTexture.h"
#include "Texture.h"
#include "TextureServer.h"
#include "Engine.h"

namespace Amju
{
extern void ReportError(const std::string&);

const char* LevelWorkerTerrain::TypeName = "terrain";
const char* LevelWorkerTerrain::GetTypeName() { return TypeName; }

PSolidComponent LevelWorkerTerrain::MakePyramidRect()
{
  static float textureRepsX = 
    Engine::Instance()->GetConfigFloat("terrain_box_tex_x");
  static float textureRepsZ = 
    Engine::Instance()->GetConfigFloat("terrain_box_tex_z");

  float h = 0; //m_terrainGen.GetVerticalSize(); // max terrain height
  float h0 = 0; // min terrain height
  float s = 300.0f; // rect z size  TODO CONFIG ?
  float r = 300.0f; // rect x size
  float t = 0; //m_terrainGen.GetSize(); // half terrain side length
  float ymin = h0;
  // Similar triangles to get max height of rectangle.
  float ymax = h + (s - t) * (h - h0) / t;
  

  SolidLeaf* pLeaf = new SolidLeaf;
  pLeaf->SetName("terrain cone leaf");

    const float x = -r; //m_horUnitSize * (float)i;
    const float z = -s; //m_horUnitSize * (float)j;
    const float x1 = r; //m_horUnitSize * (float)(i + 1);
    const float z1 = 0; //m_horUnitSize * (float)(j + 1);

    const float y0 = ymax; //m_vertUnitSize * m_pHeightmap->GetHeight(i, j);
    const float y1 = ymax; //m_vertUnitSize * m_pHeightmap->GetHeight(i + 1, j);
    const float y2 = ymin; //m_vertUnitSize * m_pHeightmap->GetHeight(i + 1, j + 1);
    const float y3 = ymin; //m_vertUnitSize * m_pHeightmap->GetHeight(i, j + 1);

    SceneVertex v0(x, y0, z);
    SceneVertex v1(x1, y1, z);
    SceneVertex v2(x1, y2, z1);
    SceneVertex v3(x, y3, z1);

    Polygon* pPoly1 = new Amju::Polygon;
    pPoly1->AddVertex(v3); // 2
    pPoly1->AddVertex(v1); // 1
    pPoly1->AddVertex(v0); // 0

    // Add texture coords
    pPoly1->AddTexVertex(TexVertex(0, 0)); // correspond to v3
    pPoly1->AddTexVertex(TexVertex(textureRepsX, textureRepsZ));  // v1
    pPoly1->AddTexVertex(TexVertex(0, textureRepsZ));  // v0

    pPoly1->SetPerpendicularNormals();
    pLeaf->AddPolygon(pPoly1);

    Polygon* pPoly2 = new Amju::Polygon;
    pPoly2->AddVertex(v1);  // 2
    pPoly2->AddVertex(v3);  // 1
    pPoly2->AddVertex(v2);  // 0

    pPoly2->AddTexVertex(TexVertex(textureRepsX, textureRepsZ));  // v1
    pPoly2->AddTexVertex(TexVertex(0, 0));  // v3
    pPoly2->AddTexVertex(TexVertex(textureRepsX, 0));  // v2

    pPoly2->SetPerpendicularNormals();
    pLeaf->AddPolygon(pPoly2);

  pLeaf->CreateNormals(true); // => smooth

  pLeaf->SetTextureMethod(new TexMethodRegular);
  // Texture name - TODO config
  std::string textureName = Engine::Instance()->GetConfigValue("terrain_box_tex");
  Texture* pTex = TextureServer::Instance()->Get(textureName, "");
  pTex->CreateBinding(Texture::REGULAR);
  pLeaf->SetTexture(pTex); 
  pLeaf->CreateDisplayList();

  return pLeaf;
}

PSolidComponent LevelWorkerTerrain::MakePyramid()
{
  // Make a cone of numSides sides.
  int numSides = 4;
  float degs = 360.0f / (float)numSides;
  SolidComposite* pComp = new SolidComposite;
  pComp->SetName("terrain cone");

  for (int i = 0; i < numSides; i++)
  {
    PSolidComponent pRect = MakePyramidRect();
    Orientation o;
    o.SetYRot((float)i * degs);
    pRect->SetOrientation(o);
    pComp->AddComponent(pRect);
  }
  return pComp;
}

#ifdef SCENE_EDITOR
bool LevelWorkerTerrain::Save(File* pf)
{
/*
  if (!m_terrainGen.Save(pf))
  {
    return false;
  }
*/
  return true;  
}
#endif

bool LevelWorkerTerrain::Load(File* pf)
{
/*
  if (!m_terrainGen.Load(pf))
  {
    return false;
  }
*/
  return true;
}

bool LevelWorkerTerrain::AddFeature(Level* pLevel, CubeMap* )
{
  PSolidComponent pTerrain;// = m_terrainGen.Create();

  Matrix m;
  //m.identity();

  // Get the existing Scene for the level. This should just be an empty
  // placeholder. 
  // Add the terrain to the scene.
/*
  PSolidComponent pScene = pLevel->GetScene();
  SolidComposite* pComp = dynamic_cast<SolidComposite*>(pScene.GetPtr());
  if (!pComp)
  {
    ReportError("No scene set for level ?");
    return false;
  }
*/
  SolidComposite* pComp = new SolidComposite;
  pComp->SetName("scene made by level-worker-terrain");

  pLevel->SetScene(pComp);
  pComp->AddComponent(pTerrain);

  // Add four rectangles to the scene. Each one slopes upwards from the 
  // (x, z) origin. The effect as far as the user can see is that the action
  // takes place inside an upturned pyramid. 
  // The lowest y-value for each rectangle is the lowest y-value of the
  // terrain. The highest y-value can be configurable.
  // The plane should touch the outside edge of the terrain area at the 
  // terrain's highest point. This is so there is never a gap between the
  // 'pyramid' and the terrain.
  PSolidComponent pPyramid = MakePyramid();
  pComp->AddComponent(pPyramid);
  

  // Redo bounding sphere tree of scene to take terrain into account.
  m.identity();
  pComp->RecalculateAbsoluteCoords(m);
  pComp->StoreHeights(m);
  pComp->CreateBoundingSphere(m);


  // TODO TEMP TEST
  pLevel->SetStartOrientation(Orientation(5.0f, 5.0f, 5.0f, 0, 0, 0));
  // Level play area size is doubled, as it is measured from -size to +size.
  // We want the level extent to be the same as the terrain extent.
  pLevel->SetPlayAreaSize(0); //m_terrainGen.GetSize()  * 0.5f );

  return true;
}

void LevelWorkerTerrain::SetTerrainPeakHeight(float h)
{
  //m_terrainGen.SetVerticalSize(h);
}

float LevelWorkerTerrain::GetTerrainPeakHeight() const
{
  return 0; //m_terrainGen.GetVerticalSize();
}
}

