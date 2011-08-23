/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: AiCooperationGroup.cpp,v $
Revision 1.1  2004/09/08 15:42:33  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <algorithm>
#include "AiCooperationGroup.h"
#include "Engine.h"
#include "Takeable.h"
#include "TakeableCrate.h"
#include "PieceMover.h"
#include "SchAssert.h"

namespace Amju
{
void CooperationGroup::Join(PieceMover* p)
{
  m_pms.push_back(p);
  p->SetCooperationGroup(this);
}

bool CooperationGroup::IsPieceUsed(int id) const
{
  bool b = (std::find(m_takeables.begin(), m_takeables.end(), id) != m_takeables.end());
  return b;
}

int CooperationGroup::GetPieceOrder(int id) const
{
  int i = 0;
  TakeableDeque::const_iterator it = m_takeables.begin();
  do
  {
    if (id == *it)
    {
      return i;
    }
    i++;
    ++it;
  }
  while (it != m_takeables.end());
  Assert(0);
  return -1;
}

void CooperationGroup::SetPieceUsed(int id, bool inUse)
{
  // Remove piece ID if it already exists in the deque, then add it to the front.
  if (IsPieceUsed(id))
  {
    m_takeables.erase(std::find(m_takeables.begin(), m_takeables.end(), id));
  }

  if (inUse)
  {
    m_takeables.push_front(id);
  }
}

void CooperationGroup::Clear()
{
  m_pms.clear();
  m_takeables.clear();
}

int CooperationGroup::GetNumPiecesPlacedAtHeight(float h)
{
  // Iterate over pieces placed by group members.
  // TODO maybe optimise this, i.e. index by height.
  int count = 0;
  for (TakeableDeque::iterator it = m_takeables.begin(); it != m_takeables.end(); ++it)
  {
    PGameObject pGo = Engine::Instance()->GetGameObject(*it);
    Takeable* pTakeable = dynamic_cast<Takeable*>(pGo.GetPtr());
    if (pTakeable && pTakeable->GetOrientation()->GetY() == h)
    {
      count++;
    }
  }
  return count;
}

int CooperationGroup::GetNumRampsPlacedAtHeight(float h)
{
  // TODO optimse this, by indexing by height..?
  int count = 0;
  for (TakeableDeque::iterator it = m_takeables.begin(); it != m_takeables.end(); ++it)
  {
    PGameObject pGo = Engine::Instance()->GetGameObject(*it);
    TakeableCrate* pTakeable = dynamic_cast<TakeableCrate*>(pGo.GetPtr());
    if (pTakeable && 
        pTakeable->GetType() == "ramp2_2_4.comp" && // TODO piece type codes
        pTakeable->Takeable::GetOrientation()->GetY() == h)
    {
      count++;
    }
  }
  return count;
}

}

