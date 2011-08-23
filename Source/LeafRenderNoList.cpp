/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: LeafRenderNoList.cpp,v $
Revision 1.1  2004/09/08 15:42:52  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "LeafRenderNoList.h"
#include "LeafData.h"
#include "Common.h"
#include "SchAssert.h"

namespace Amju
{
void LeafRenderNoList::Init(LeafData* pLd)
{
}

void LeafRenderNoList::ResetColourCounter()
{
  m_col = 0;
}

void LeafRenderNoList::Draw(LeafData* pLd)
{
  // Just use colours, to help identify the building blocks.
  AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);
/*
  m_col++;
  switch (m_col%6)
  {
  case 0:
    glColor4f(1, 0, 0, 1);
    break;
  case 1:
    glColor4f(0, 1, 0, 1);
    break;
  case 2:
    glColor4f(1, 1, 0, 1);
    break;
  case 3:
    glColor4f(0, 0, 1, 1);
    break;
  case 4:
    glColor4f(1, 0, 1, 1);
    break;
  case 5:
    glColor4f(0, 1, 1, 1);
    break;
  }
*/
  // TODO use the right method
  //pLd->GetTexture()->DrawNoBinding(Texture::AUTO_GENERATE); 
  //pLd->m_pTextureMethod->BeginDisplayList(pLd);
  pLd->CreateNormals(); 
  pLd->CreatePolygonDisplayList(); 
  //pLd->m_pTextureMethod->EndDisplayList(pLd); 
}

void LeafRenderNoList::Clear(LeafData* pLd)
{
  // Nothing to do
}
}

