/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: BlockShadow.cpp,v $
Revision 1.1  2004/09/08 15:42:45  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <string>
#include "BlockShadow.h"
#include "TextureServer.h"
#include "SolidGameObject.h"
#include "Engine.h"
#include "HeightServer.h"

namespace Amju
{
Texture* BlockShadow::BlockShadowImpl::s_pBlockTexture;

bool BlockShadow::Init()
{
  return BlockShadowImpl::Init();
}

BlockShadow::BlockShadow()
{
}

void BlockShadow::SetCaster(VisibleGameObject* pVgo)
{
  m_pCaster = dynamic_cast<SolidGameObject*>(pVgo);
  m_shadows.clear();

  // For each distinct poly in the caster, create a new BlockShadowImpl 
  const HeightServer* pHs = m_pCaster->GetCollisionVol();
  int numWalls = pHs->GetNumWallPolys();
  int i;
  for (i = 0; i < numWalls; i++)
  {
    BlockShadowImpl* p = new BlockShadowImpl;
    p->SetCaster(pVgo);
    p->SetIndex(i, true /* is a wall */);
    m_shadows.push_back(p);
  }

  int numFloors = pHs->GetNumHeightPolys();
  for (i = 0; i < numFloors; i++)
  {
    BlockShadowImpl* p = new BlockShadowImpl;
    p->SetCaster(pVgo);
    p->SetIndex(i, false /* is NOT a wall */);
    m_shadows.push_back(p);
  }
}

void BlockShadow::Draw(float x, float y, float z, float size, const HeightServer& hs)
{
  static float alpha = 0.25f;// Engine::Instance()->GetConfigFloat("block_shadow_alpha");

  Engine::Instance()->PushColour(1.0f, 1.0f, 1.0f, alpha); 
  Shadow::Draw(x, y, z, size, hs);
  Engine::Instance()->PopColour();
}

void BlockShadow::DrawList()
{
  // draw list for each BlockShadowImpl
  for (Shadows::iterator it = m_shadows.begin(); it != m_shadows.end(); ++it)
  {
    (*it)->DrawList();
  }
}

void BlockShadow::RecalculateList(
  float x, 
  float y, 
  float z,
  float size,
  const HeightServer& hs)
{
  // recalc each BlockShadowImpl
  for (Shadows::iterator it = m_shadows.begin(); it != m_shadows.end(); ++it)
  {
    (*it)->RecalculateList(x, y, z, size, hs);
  }
}

void BlockShadow::BindTexture()
{
  Assert(BlockShadowImpl::s_pBlockTexture);
  BlockShadowImpl::s_pBlockTexture->Bind();
}

// ----------------------------------------------------------------------------

void BlockShadow::BlockShadowImpl::SetPlane(const Plane& plane)
{
  m_plane = plane;
}

void BlockShadow::BlockShadowImpl::SetIndex(int i, bool wall)
{
  m_polyIndex = i;
  m_isWall = wall;
}

bool BlockShadow::BlockShadowImpl::Init()
{
  std::string bitmapname = Engine::Instance()->GetConfigValue("block_shadow"); 
  std::string alphaname = Engine::Instance()->GetConfigValue("block_shadow_alpha");

  s_pBlockTexture = TextureServer::Instance()->Get(bitmapname, alphaname); 
  if (s_pBlockTexture)
  {
    s_pBlockTexture->CreateBinding(Texture::REGULAR);
  }
  else
  {
    return false;
  }
  return true;
}

void BlockShadow::BlockShadowImpl::BindTexture()
{ 
  Assert(s_pBlockTexture);
  s_pBlockTexture->Bind();
}

void BlockShadow::BlockShadowImpl::MapST(
  float centrex, 
  float centrez, 
  float x, 
  float z, 
  float size, 
  float* s, 
  float* t)
{
  *s = 0;
  *t = 0;
}

void BlockShadow::BlockShadowImpl::RecalculateVerts()
{
  const HeightServer* pHs = m_pCaster->GetCollisionVol();
  // Re-fetch the plane because the vertices may have changed.
  if (m_isWall)
  {
    SetPlane(*(pHs->GetWallPoly(m_polyIndex)));
  }
  else
  {
    SetPlane(*(pHs->GetHeightPoly(m_polyIndex)));
  }

  // The shape will depend on the position of the light source.
  // dx and dz - use similar triangles to get distance in x and z
  // from a vertex at height 1 to its projection on the floor (height 0).
  // This is the same for all vertices and for all objects, assuming 
  // a very distant light source.
  // TODO update from light source in current room.
  float dx = 1.0f;
  float dz = 1.0f; 

  // For each vertex on the object, get its projected (x ,z) position on the 
  // ground. This is x + dx * (y - groundheight), z + dz * (y - groundheight)

  m_verts.clear();
  if (!m_pCaster)
  {
    return;
  }

  // TODO
  // Conceptually we want to cast a ray from the light source (sun or moon)
  // through each vertex of the object, to the ground.
  // We want to end up with a single poly, with the vertices in the right order,
  // tracing around the edge of the shadow.

  // Ground height
  float hGround = pHs->GetBoundingBox().m_ymin;

  // For each vertex in the plane poly for this shadow, map it to a 2D x-z
  // coord. 
  for (int i = 0; i < 3; i++) // Always 3 vertices.
  {
    const VertexBase& v = m_plane.GetVertex(i);
    // h is height difference between vertex y-coord and ground.
    float h = v.y - hGround;
    float x = v.x + dx * h;
    float z = v.z + dz * h;
    m_verts.push_back(std::make_pair(x, z));
  }
}

void BlockShadow::BlockShadowImpl::SetCaster(VisibleGameObject* pVgo)
{
  m_pCaster = dynamic_cast<SolidGameObject*>(pVgo);
}

}

