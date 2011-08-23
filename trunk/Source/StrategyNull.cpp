/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: StrategyNull.cpp,v $
Revision 1.1  2004/09/08 15:43:03  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "StrategyNull.h"
#include "Ai.h"

namespace Amju
{
StrategyNull::StrategyNull(Strategizer* pS) : Strategy(pS)
{
  m_priority = -100; // very low priority!
}

bool StrategyNull::IsValid() const
{
  return true;
}

void StrategyNull::OnActivate()
{
  m_pAi->SetState(UNKNOWN);
}
}
