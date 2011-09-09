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
  const std::string& alphaname, 
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

  m_pTexture = TextureServer::Instance()->Get(texturename, alphaname);
  if (!m_pTexture)
  {
    return false;
  }
  m_pTexture->CreateBinding(PoolTexture::REGULAR, false /* clamp, don't wrap */);
  return true;
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

//  AmjuGL::PushAttrib(AmjuGL::AMJU_DEPTH_READ | AmjuGL::AMJU_LIGHTING);
//  AmjuGL::Disable(AmjuGL::AMJU_DEPTH_READ); // TODO have a member flag ?
//  AmjuGL::Disable(AmjuGL::AMJU_LIGHTING);
  AmjuGL::DrawTriList(tris);
//  AmjuGL::PopAttrib();

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
