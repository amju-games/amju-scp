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
  m_displayList = (unsigned int)-1;
  m_pTexture = 0;
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
  m_numElements = numElementsX * numElementsY;

  m_pTexture = TextureServer::Instance()->Get(texturename, alphaname);
  if (!m_pTexture)
  {
    return false;
  }
  m_pTexture->CreateBinding(Texture::REGULAR, false /* clamp, don't wrap */);
  return true;
}

/*
  // OLD - creating disaply list for individual quads !?!
  float dx = 1.0f / (float)numElementsX;
  float dy = 1.0f / (float)numElementsY;
    
  m_displayList = glGenLists(m_numElements);

  for (int i = 0; i < m_numElements; ++i)
  {
    float x = float(i % numElementsX) * dx;
    float y = float(i / numElementsX) * dy;
        
    glNewList(m_displayList + i, GL_COMPILE);
    //m_pTexture->Bind();

    glBegin(GL_QUADS);

    glTexCoord2f(x, 1.0f - y - dy);
    glVertex2f(0, 0);
    
    glTexCoord2f(x + dx, 1.0f - y - dy);
    glVertex2f(sizeX, 0);
    
    glTexCoord2f(x + dx, 1.0f - y);
    glVertex2f(sizeX, sizeY);
    
    glTexCoord2f(x, 1.0f - y);
    glVertex2f(0, sizeY);
        
    glEnd();
    glEndList();
  }
  return true;
}
*/

int TextureSequence::GetNumElements() const
{
  return m_numElements;
}

void TextureSequence::Draw(int element)
{
  Assert(m_pTexture);
  Assert(element < m_numElements);
////  glCallList(m_displayList + element);

  // TODO
}

void TextureSequence::Bind()
{
  m_pTexture->Bind();
}
}
