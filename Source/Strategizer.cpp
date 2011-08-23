/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Strategizer.cpp,v $
Revision 1.1  2004/09/08 15:43:03  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "Strategizer.h"
//#include "StrategyBuild.h"
#include "StrategyFindBuild.h"
#include "StrategyFindTest.h"
#include "StrategyRunTo.h"
#include "StrategyNull.h"
#include "File.h"
#include "SchAssert.h"

namespace Amju
{
Strategizer::Strategizer()
{
}

bool Strategizer::Load(File*)
{
  Strategy* pS;

  // Test Finder algorithm without AI moving
/*
  pS = new StrategyFindTest(this);
  File f3;
  f3.OpenRead("findbuild.txt");
  if (!pS->Load(&f3))
  {
    return false;
  }
  m_strategies.push_back(pS);
*/


  pS = new StrategyFindBuild(this);
  // Load building data for this strategy
  // TODO We can't do anything with any error here!
  if (!pS->Load(0))
  {
    return false;
  }
  // TODO comment this out to disable Ais 
  m_strategies.push_back(pS);



  pS = new StrategyRunTo(this);
  m_strategies.push_back(pS);

  pS = new StrategyNull(this);
  m_strategies.push_back(pS);

  // TODO more strategies

  return true;
}

void Strategizer::OnTaskFinished()
{
  if (m_pCurrent.GetPtr())
  {
    m_pCurrent->OnTaskFinished();
  }
}

void Strategizer::OnTaskFailed()
{
  if (m_pCurrent.GetPtr())
  {
    m_pCurrent->OnTaskFailed();
  }
}

void Strategizer::SetAi(Ai* pAi)
{
  for (Strategies::iterator it = m_strategies.begin(); it != m_strategies.end(); ++it)
  {
    PStrategy& ps = *it;
    ps->SetAi(pAi);
  }
}

void Strategizer::ChooseStrategy()
{
  // Find the Strategy with the best priority and which is valid under the
  // current game situation.
  PStrategy pNewStrategy;
  int bestSoFar = -1000; // any higher priority is better
  for (Strategies::iterator it = m_strategies.begin(); it != m_strategies.end(); ++it)
  {
    PStrategy& ps = *it;
    if (ps->GetPriority() > bestSoFar && ps->IsValid())
    {
        bestSoFar = ps->GetPriority();
        pNewStrategy = ps;
    }
  }

  Assert(pNewStrategy.GetPtr());

  m_pCurrent = pNewStrategy;
  m_pCurrent->OnActivate(); 
}

void Strategizer::SetGoal(PGameObject pGoal)
{
  m_pGoal = pGoal;
}

void Strategizer::RecalculatePath()
{
  // TODO only need to recalculate the active strategy, right ?
  m_pCurrent->RecalculatePath(); 
/*
  for (Strategies::iterator it = m_strategies.begin(); it != m_strategies.end(); ++it)
  {
    PStrategy& ps = *it;
    ps->RecalculatePath(); 
  }
*/
}

void Strategizer::RefreshStrategies()
{
  // Refresh the valid state for all strategies.
  for (Strategies::iterator it = m_strategies.begin(); it != m_strategies.end(); ++it)
  {
    PStrategy& ps = *it;
    ps->Reset(); 
  }
}

}
