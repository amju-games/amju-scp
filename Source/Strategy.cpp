/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Strategy.cpp,v $
Revision 1.1  2004/09/08 15:43:03  jay
Added to repository
  
*/

#include "Strategy.h"
#include "Strategizer.h"

namespace Amju
{
Strategy::Strategy(Strategizer* pS) : m_pStrategizer(pS)
{
  m_priority = 0;
}

Strategy::~Strategy()
{
}

void Strategy::SetAi(Ai* pAi)
{
  m_pAi = pAi;
}

int Strategy::GetPriority() const
{
  return m_priority;
}

bool Strategy::UpdateTask()
{
  return false;
}

void Strategy::Reset()
{
}

void Strategy::OnTaskFinished()
{
}

void Strategy::OnTaskFailed()
{
}

void Strategy::RecalculatePath()
{
}

bool Strategy::Load(File*)
{
  return true;
}

}
