/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: TextureSequence.cpp,v $
Revision 1.1  2004/09/08 15:42:58  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "Common.h"
#include "TextureSequence.h"
#include "TextureServer.h"
#include "SchAssert.h"

namespace Amju
{
TextureSequence::TextureSequence()
{
  //m_displayList = (unsigned int)-1;
  m_pTexture = 0;

  m_numElementsX = 0;
  m_numElementsY = 0;
  m_sizeX = 0;
  m_sizeY = 0;
  m_cellSizeX = 0;
  m_cellSizeY = 0;
}

TextureSequence::~TextureSequence()
{
  //glDeleteLists(m_displayList, m_numElements);
}

int TextureSequence::GetTextureWidth()
{
  return m_pTexture->GetWidth();
}

int TextureSequence::GetTextureHeight()
{
  return m_pTexture->GetHeight();
}

bool TextureSequence::Load(
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
  m_pTexture->CreateBinding(Texture::REGULAR, false /* clamp, don't wrap */);
  return true;
}

void TextureSequence::Draw(int element)
{
  Assert(m_pTexture);
  Assert(element < m_numElements);

  // OLD - creating disaply list for individual quads !?!
//  float dx = 1.0f / (float)m_numElementsX;
//  float dy = 1.0f / (float)m_numElementsY;
    
  //m_displayList = glGenLists(m_numElements);

  //for (int i = 0; i < m_numElements; ++i)
  //{
    float x = float(element % m_numElementsX) * m_sizeX;
    float y = float(element / m_numElementsX) * m_sizeY;
        
    //glNewList(m_displayList + i, GL_COMPILE);
    m_pTexture->Bind();
    //glBegin(GL_QUADS);
/*
    glTexCoord2f(x, 1.0f - y - dy);
    glVertex2f(0, 0);
    
    glTexCoord2f(x + dx, 1.0f - y - dy);
    glVertex2f(sizeX, 0);
    
    glTexCoord2f(x + dx, 1.0f - y);
    glVertex2f(sizeX, sizeY);
    
    glTexCoord2f(x, 1.0f - y);
    glVertex2f(0, sizeY);
        
    //glEnd();
    //glEndList();
*/

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

  AmjuGL::Disable(AmjuGL::AMJU_DEPTH_READ); // TODO have a member flag
  AmjuGL::Disable(AmjuGL::AMJU_LIGHTING);
  AmjuGL::PushMatrix();
//  AmjuGL::MultMatrix(m_combined); // NB combined
//  m_texture->UseThisTexture();
  AmjuGL::DrawTriList(tris);
  AmjuGL::PopMatrix();
  AmjuGL::Enable(AmjuGL::AMJU_DEPTH_READ);

  return;
}

int TextureSequence::GetNumElements() const
{
  return m_numElements;
}



void TextureSequence::Bind()
{
  m_pTexture->Bind();
}
}
