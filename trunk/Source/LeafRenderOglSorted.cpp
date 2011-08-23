/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: LeafRenderOglSorted.cpp,v $
Revision 1.1  2004/09/08 15:42:52  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "LeafRenderOglSorted.h"
#include "Common.h"
#include "LeafData.h"
#include "Texture.h"
#include <string>

using namespace std;

namespace Amju
{
void LeafRenderOglSorted::Draw(LeafData* pLd)
{
  Matrix m;
  m.ModelView();
  DisplayListItem item(pLd, m);
  m_sortedList.push_back(item);
}

void LeafRenderOglSorted::Init(LeafData* pLd)
{
  pLd->CreateDisplayList();
}

void LeafRenderOglSorted::Clear(LeafData* pLd)
{
}

void LeafRenderOglSorted::ClearList()
{
  m_sortedList.clear();
}

void LeafRenderOglSorted::DrawList()
{
  // Sort the list
  m_sortedList.sort();

  AmjuGL::PushMatrix();
  for (SortedDisplayList::iterator it = m_sortedList.begin(); 
       it != m_sortedList.end(); 
       ++it)
  {
    DisplayListItem& item = *it;
    item.Draw();
  }
  AmjuGL::PopMatrix();
  ClearList();
}

int LeafRenderOglSorted::DisplayListItem::s_id;

bool LeafRenderOglSorted::DisplayListItem::operator<(
  const LeafRenderOglSorted::DisplayListItem& rhs) const
{
  // Sort so that leaves with the same texture name are grouped together.
  // Also textures with an alpha layer should go at the end of the list.

  const bool lhsalpha = m_pLd->GetTexture()->HasAlpha();
  const bool rhsalpha = rhs.m_pLd->GetTexture()->HasAlpha();

  if (lhsalpha && !rhsalpha)
  {
    return false;
  }
  if (!lhsalpha && rhsalpha)
  {
    return true;
  }
  // Alpha flags same - sort on texture name.
  const string lhsname = m_pLd->GetTexture()->GetFileName() + 
    m_pLd->GetTexture()->GetAlphaFilename();
  const string rhsname = rhs.m_pLd->GetTexture()->GetFileName() + 
    rhs.m_pLd->GetTexture()->GetAlphaFilename();

  if (lhsname != rhsname)
  {
    return lhsname < rhsname;
  }
  // Same texture, so just sort on any value which differentiates objects.
  return m_id < rhs.m_id;
}

LeafRenderOglSorted::DisplayListItem::DisplayListItem(
  LeafData* pLd, const Matrix& m) :
  m_pLd(pLd),
  m_matrix(m),
  m_id(s_id++) // unique ID for each object
{
}

void LeafRenderOglSorted::DisplayListItem::Draw()
{
  AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
  m_matrix.LoadMatrix();
  m_pLd->SetTexGenMode();
  m_pLd->ExecuteDisplayList();
}


}

