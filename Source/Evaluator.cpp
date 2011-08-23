/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Evaluator.cpp,v $
Revision 1.1  2004/09/08 15:43:01  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "Evaluator.h"
#include "Ai.h"
#include "Vertex.h"

namespace Amju
{
Evaluator::Evaluator()
{
  m_weight = 1.0f;
}

Evaluator::~Evaluator()
{
}

bool Evaluator::Load(File*)
{
  return true;
}

void Evaluator::Reset(Finder*)
{
}

float Evaluator::GetWeight() const
{
  return m_weight;
}

void Evaluator::SetWeight(float w)
{
  m_weight = w;
}

// Useful functions for Evaluating moves

VertexBase Evaluator::GetGoalPos(Ai* pAi) 
{
  Assert(pAi);
  Assert(pAi->GetCurrentGoal().GetPtr());
  Assert(pAi->GetCurrentGoal()->GetBoundingSphere());

  return pAi->GetCurrentGoal()->GetBoundingSphere()->GetCentre();
}

#if defined(_DEBUG)
const std::string& Evaluator::GetName() const
{
  return m_name;
}

void Evaluator::SetName(const std::string& name)
{
  m_name = name;
}
#endif
}