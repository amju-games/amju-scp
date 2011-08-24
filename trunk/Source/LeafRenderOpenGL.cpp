/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: LeafRenderOpenGL.cpp,v $
Revision 1.1  2004/09/08 15:42:52  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "LeafRenderOpenGL.h"
#include "LeafData.h"
#include "Common.h"
#include "SchAssert.h"

namespace Amju
{
void LeafRenderOpenGL::Init(LeafData* pLd)
{
//  pLd->CreateDisplayList();
}

void LeafRenderOpenGL::Draw(LeafData* pLd)
{
  Assert(pLd);

  // Get the Leaf to set the Texture coords generation function. This doesn't
  // seem to be compiled into the display list.
  pLd->SetTexGenMode();
  //glCallList(pLd->m_displayList);
  pLd->CreateDisplayList();
}

void LeafRenderOpenGL::Clear(LeafData* pLd)
{
  //glDeleteLists(pLd->m_displayList, 1);
}
}
