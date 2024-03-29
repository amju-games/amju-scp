/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: PoolTextureSequence.cpp,v $
Revision 1.1  2004/09/08 15:42:58  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "Common.h"
#include "TextureSequence.h"
#include "TextureServer.h"
#include "AmjuAssert.h"
#include <iostream>

namespace Amju
{
PoolTextureSequence::PoolTextureSequence()
{
  m_pTexture = 0;

  m_numElementsX = 0;
  m_numElementsY = 0;
  m_sizeX = 0;
  m_sizeY = 0;
  m_cellSizeX = 0;
  m_cellSizeY = 0;
}

PoolTextureSequence::~PoolTextureSequence()
{
}

int PoolTextureSequence::GetTextureWidth()
{
  return m_pTexture->GetWidth();
}

int PoolTextureSequence::GetTextureHeight()
{
  return m_pTexture->GetHeight();
}

bool PoolTextureSequence::Load(
  const std::string& texturename, 
  int numElementsX, int numElementsY,
  float sizeX, float sizeY)
{
  m_numElementsX = numElementsX;
  m_numElementsY = numElementsY;
  m_sizeX = sizeX;
  m_sizeY = sizeY;
  m_cellSizeX = 1.0f / (float) m_numElementsX;
  m_cellSizeY = 1.0f / (float) m_numElementsY;

  m_numElements = numElementsX * numElementsY;

  m_pTexture = TextureServer::Instance()->Get(texturename, "");
  if (!m_pTexture)
  {
    return false;
  }
  m_pTexture->CreateBinding(PoolTexture::REGULAR, false /* clamp, don't wrap */);
  return true;
}


void PoolTextureSequence::MakeTris(int element, float size, AmjuGL::Tri tris[2], float xOff, float yOff)
{
  AMJU_CALL_STACK;

  Assert(m_pTexture);

  float dx = 1.0f / (float)m_numElementsX;
  float dy = 1.0f / (float)m_numElementsY;

  float x = float(element % m_numElementsX) * dx;
  float y = float(element / m_numElementsX) * dy; // yes this is correct

  float sizeX = m_sizeX * size;
  float sizeY = m_sizeY * size;

  const float Z = 0.5f;
  AmjuGL::Vert v[4] =
  {
    AmjuGL::Vert(xOff,         yOff,         Z,  x,      1.0f - y - dy,     0, 1.0f, 0),
    AmjuGL::Vert(xOff + sizeX, yOff,         Z,  x + dx, 1.0f - y - dy,     0, 1.0f, 0),
    AmjuGL::Vert(xOff + sizeX, yOff + sizeY, Z,  x + dx, 1.0f - y,          0, 1.0f, 0),
    AmjuGL::Vert(xOff,         yOff + sizeY, Z,  x,      1.0f - y,          0, 1.0f, 0)
  };

  tris[0].m_verts[0] = v[0];
  tris[0].m_verts[1] = v[1];
  tris[0].m_verts[2] = v[2];

  tris[1].m_verts[0] = v[0];
  tris[1].m_verts[1] = v[2];
  tris[1].m_verts[2] = v[3];
}

void PoolTextureSequence::Draw(int element)
{
  Assert(m_pTexture);
  Assert(element < m_numElements);

  float x = float(element % m_numElementsX);
  float y = float(element / m_numElementsX);

  x *= m_cellSizeX;
  y *= m_cellSizeY;
        
  //m_pTexture->Bind();

  AmjuGL::Vert verts[] = 
  {
    AmjuGL::Vert(0,       0,       0,   x,               1.0f - y - m_cellSizeY,   0, 0, 1),
    AmjuGL::Vert(m_sizeX, 0,       0,   x + m_cellSizeX, 1.0f - y - m_cellSizeY,   0, 0, 1),
    AmjuGL::Vert(m_sizeX, m_sizeY, 0,   x + m_cellSizeX, 1.0f - y,                 0, 0, 1),
    AmjuGL::Vert(0,       m_sizeY, 0,   x,               1.0f - y,                 0, 0, 1)
  };
 
  AmjuGL::Tris tris;
  tris.resize(2);
  AmjuGL::Tri* tri =  &tris[0];
  tri->m_verts[0] = verts[0];
  tri->m_verts[1] = verts[1];
  tri->m_verts[2] = verts[2];

  tri = &tris[1];
  tri->m_verts[0] = verts[0];
  tri->m_verts[1] = verts[2];
  tri->m_verts[2] = verts[3];

  AmjuGL::DrawTriList(tris);

  return;
}

int PoolTextureSequence::GetNumElements() const
{
  return m_numElements;
}

void PoolTextureSequence::Bind()
{
  m_pTexture->Bind();
}
}
