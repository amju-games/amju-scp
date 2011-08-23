/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: StrategyRunTo.cpp,v $
Revision 1.1  2004/09/08 15:43:03  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "StrategyRunTo.h"
#include "Ai.h"

namespace Amju
{
StrategyRunTo::StrategyRunTo(Strategizer* pS) : Strategy(pS)
{
  m_priority = 1;
  m_isPath = true;
}

bool StrategyRunTo::IsValid() const
{
  // Is there a path to the goal ? If path is pending return true
  // - we can fail if we eventually find out there is no path.
  return m_isPath;
}

void StrategyRunTo::OnActivate()
{
  // Set state to run to the goal.
  // There may not be a path, but that will be found out by the
  // AiRunToBehaviour.
  m_pAi->SetState(AI_RUN_TO);
}

void StrategyRunTo::Reset()
{
  m_isPath = true;
}

void StrategyRunTo::OnTaskFinished()
{
  // The AI has successfully reached the target.
  m_pAi->CurrentGoalReached();
}

void StrategyRunTo::OnTaskFailed()
{
  // There's no path. Reset this flag so this Strategy won't be chosen again
  // until something changes, making a path a possibility again.
  m_isPath = false;
}

void StrategyRunTo::RecalculatePath()
{
//  m_isPath = true; // make choosing this strategy a possibility

  if (m_pAi->GetState() == AI_RUN_TO)
  {
    m_pAi->SetState(UNKNOWN);
    m_pAi->SetState(AI_RUN_TO);
  }
}

}