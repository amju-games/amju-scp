/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EvaluatorNearPrevious.cpp,v $
Revision 1.1  2004/09/08 15:43:02  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "EvaluatorNearPrevious.h"
#include "Ai.h"
#include "Takeable.h"
#include "TakeableCrate.h"

namespace Amju
{
EvaluatorNearPrevious::EvaluatorNearPrevious()
{
#if defined(_DEBUG)
  SetName("Encouragement to build near previous piece");
#endif
}


Evaluator::EvalResult EvaluatorNearPrevious::Score(
  const RankedMove& rm, 
  Ai* pAi, 
  VisibilityGraphWithObjects* pVgo,
  float* pResult)
{
  // Get distance from Move position to the position of the last piece 
  // placed by this AI.
  // OR should it be the last piece placed by any AI in the same
  // cooperation group ???
  // NB GetCurrentPiece returns the last piece placed by the AI.
  if (!pAi->GetLastPiecePlaced())
  {
    *pResult = 0;
    return EvalOk;
  }
  // If a ramp, don't try to get close
  if (((TakeableCrate*)pAi->GetLastPiecePlaced())->GetType() == "ramp2_2_4.comp")
  {
    *pResult = 1.0f;
    return EvalOk;
  }

  const VertexBase& v2 = pAi->GetLastPiecePlaced()->GetOrientation()->GetVertex();
  const VertexBase& v1 = rm.m_or.GetVertex();

  float sqdist = SquareDist(v1, v2);
  float dist = sqrt(sqdist);
  float r = Finder::GetGoalRadius();

  if (dist > r)
  {
    *pResult = 0;
  }
  else
  {
    float result = 1.0f - dist / r;
    *pResult = result;
  }
  return EvalOk;
}

}