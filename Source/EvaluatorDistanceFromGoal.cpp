/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EvaluatorDistanceFromGoal.cpp,v $
Revision 1.1  2004/09/08 15:43:02  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "EvaluatorDistanceFromGoal.h"
#include "Ai.h"
#include "Finder.h"
#include "Vertex.h"
#include "TakeableCrate.h"
#include "SchAssert.h"

namespace Amju
{
EvaluatorDistanceFromGoal::EvaluatorDistanceFromGoal()
{
#if defined(_DEBUG)
  SetName("Punishment proportional to sqared dist. from goal");
#endif
}

Evaluator::EvalResult EvaluatorDistanceFromGoal::Score(
  const RankedMove& rm, 
  Ai* pAi, 
  VisibilityGraphWithObjects* pVgo,
  float* pResult)
{

  // Don't worry about distance if this type is a ramp.
  // As this evaluator returns values 0..-1, we return 0.
  // TODO piece types
  if (rm.m_pCrate->GetType() == "ramp2_2_4.comp")
  {
    *pResult = 0;
    return EvalOk;
  }
  
  const VertexBase v = GetGoalPos(pAi);
  float dx = rm.m_or.GetX() - v.x;
  float dy = rm.m_or.GetY() - v.y;
  float dz = rm.m_or.GetZ() - v.z;
  float sqdist = dx * dx + dy * dy + dz * dz;

  // TODO different radii per character ?
  static float maxSqDist = Finder::GetGoalRadius() * Finder::GetGoalRadius();
  // less distance is better.
  // Return result from 0..-1
  *pResult = -1.0f;
  if (sqdist < maxSqDist)
  {
    *pResult = -sqdist / maxSqDist;  
  }
  
  Assert(*pResult <= 0);
  Assert(*pResult >= -1.0f);

  return EvalOk; 
}
}

