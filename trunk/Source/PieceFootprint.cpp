/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: PieceFootprint.cpp,v $
Revision 1.1  2004/09/08 15:42:40  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "PieceFootprint.h"
#include "TextureServer.h"
#include "HeightServer.h"
#include "File.h"
#include "Engine.h"
#include "Takeable.h"
#include "SchAssert.h"

namespace Amju
{
Texture* PieceFootprint::s_pTexture;

void PieceFootprint::SetTexture(Texture* pTexture)
{
  s_pTexture = pTexture;
  s_pTexture->CreateBinding(Texture::REGULAR);
}

PieceFootprint::PieceFootprint() : m_pTakeable(0), m_pHs(0)
{
  m_pAttrib = new TextureScrollAttrib(1.0f, 1.0f, 1.0f);
}

void PieceFootprint::SetOrientation(const Orientation& o)
{
  m_or = o;
}

void PieceFootprint::SetHeightServer(const HeightServer* pHs)
{
  m_pHs = pHs;
}

void PieceFootprint::Draw()
{
  Assert(s_pTexture);
  s_pTexture->Bind();

  m_pAttrib->Draw();

  // Disabling depth buffer writing: this is only a good idea if this footprint
  // is drawn after the scene and pieces.
  //AmjuGL::Disable(AmjuGL::AMJU_DEPTH_WRITE);

  for (Polys::iterator it = m_polys.begin(); it != m_polys.end(); ++it)
  {
    it->Draw();
  }

  m_pAttrib->Restore();

  //AmjuGL::Enable(AmjuGL::AMJU_DEPTH_WRITE);
}

void PieceFootprint::SetPiece(VisibleGameObject* pTakeable)
{
  m_pTakeable = pTakeable;
}

void PieceFootprint::BuildList()
{
  m_polys.clear();
  if (!m_pTakeable)
  {
    return;
  }
  if (!m_pHs)
  {
    return;
  }

  // for each unit square which has support under it, add the square.
  float xmin = m_pTakeable->GetCollisionVol()->GetBoundingBox().m_xmin;
  float xmax = m_pTakeable->GetCollisionVol()->GetBoundingBox().m_xmax;
  float zmin = m_pTakeable->GetCollisionVol()->GetBoundingBox().m_zmin;
  float zmax = m_pTakeable->GetCollisionVol()->GetBoundingBox().m_zmax;
  float h = m_pTakeable->GetCollisionVol()->GetBoundingBox().m_ymin;

  int count = 0;

  for (float i = xmin; i < (xmax - SMALLEST); i += 1.0f) // TODO size
  {
    for (float j = zmin; j < (zmax - SMALLEST); j += 1.0f)
    {
      BuildSquare(i, j, h + 0.01f);
      count++;
    }
  }
}

void PieceFootprint::BuildVertex(Polygon* pPoly, float s, float t, float x, float y, float z)
{
  TexVertex texv(s, t);
  SceneVertex sv(x, y, z);

  pPoly->AddVertex(sv);
  pPoly->AddTexVertex(texv);
}

void PieceFootprint::BuildSquare(float x, float z, float y)
{
  Polygon poly;
  BuildVertex(&poly, 0, 0, x, y, z);
  BuildVertex(&poly, 0, 1.0f, x, y, z + 1.0f);
  BuildVertex(&poly, 1.0f, 1.0f, x + 1.0f, y, z + 1.0f);
  BuildVertex(&poly, 1.0f, 0, x + 1.0f, y, z);

  poly.SetPerpendicularNormals(); 
  m_polys.push_back(poly);
}
}
