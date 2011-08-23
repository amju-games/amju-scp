/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ButtonBase.cpp,v $
Revision 1.1.10.1  2006/08/30 21:08:45  Administrator
Get Pool to build in MSVC

Revision 1.1  2004/09/08 15:42:34  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "ButtonBase.h"
#include "LevelServer.h"
#include "File.h"

namespace Amju
{
ButtonBase::ButtonBase() : m_buttonState(0)
{
}

bool ButtonBase::LoadMessageLists(File* pf)
{
  int n = 0;
  if (!pf->GetInteger(&n))
  {
    pf->ReportError("Expected number of message lists.");
    return false;
  }
  for (int i = 0; i < n; i++)
  {
    MessageList m;
    if (!m.Load(pf, GetId()))
    {
      pf->ReportError("Failed to load message list.");
      return false;
    }
    m_lists.push_back(m);
  }
  return true;
}

#ifdef SCENE_EDITOR
bool ButtonBase::SaveMessageLists(File* pf)
{
  pf->WriteInteger(m_lists.size());
  for (int i = 0; i < m_lists.size(); i++)
  {
    m_lists[i].Save(pf);
  }
  return true;
}
#endif

void ButtonBase::OnRoomEntry()
{
  // Make sure bonus is resting on the ground underneath it.
  m_pLevel = LevelServer::Instance()->GetCurrentLevel().GetPtr();
  BoundingSphere bigBs(VertexBase(0, 0, 0), 1000.0f);
  HeightServer hs;
  m_pLevel->GetHeightServer(&hs, bigBs);
  Orientation o = *(m_pSolid->GetOrientation());
  const HeightPoly* pHp = hs.GetHeightPoly(o.GetX(), o.GetY() + 2.0f, o.GetY() - 20.0f, o.GetZ());
  //  (x, ymax, ymin, z)

  if (pHp)
  {
    float newy = pHp->GetY(o.GetX(), o.GetZ());
    o.SetY(newy);
    SetOrientation(o);
  }

  SetShadowSize(1.0f); // TOCO TEMP TEST, why no shadows ?
  RefreshShadow();
}
}

