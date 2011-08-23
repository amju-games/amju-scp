/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EvaluatorDistanceFromAi.cpp,v $
Revision 1.1  2004/09/08 15:43:01  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "EvaluatorDistanceFromAi.h"
#include "Ai.h"
#include "Finder.h"
#include "Vertex.h"
#include "TakeableCrate.h"

namespace Amju
{
EvaluatorDistanceFromAi::EvaluatorDistanceFromAi()
{
#if defined(_DEBUG)
  SetName("Round trip distance for AI");
#endif
}

Evaluator::EvalResult EvaluatorDistanceFromAi::Score(
  const RankedMove& rm, 
  Ai* pAi, 
  VisibilityGraphWithObjects* pVgo,
  float* pResult)
{
  // Get the distance from the AI to the piece wherever it is now.
  // Then add the distance to the place to where the piece would
  // be moved.
  const VertexBase v1 = pAi->GetPosition();
  const VertexBase v2 = rm.m_pCrate->Takeable::GetBoundingSphere()->GetCentre();
  const VertexBase v3(rm.m_or.GetX(), rm.m_or.GetY(), rm.m_or.GetZ());

  float dx1 = v1.x - v2.x;
  float dy1 = v1.y - v2.y;
  float dz1 = v1.z - v2.z;
  float dx2 = v2.x - v3.x;
  float dy2 = v2.y - v3.y;
  float dz2 = v2.z - v3.z;
  float dsq1 = dx1 * dx1 + dy1 * dy1 + dz1 * dz1;
  float dsq2 = dx2 * dx2 + dy2 * dy2 + dz2 * dz2;
  float sqdist = dsq1 + dsq2;

  // TODO different radii per character ?
  static float maxSqDist = Finder::GetGoalRadius() * Finder::GetGoalRadius();
  // less distance is better.
  // Return result from 0..1
  *pResult = 0;
  if (sqdist < maxSqDist)
  {
    *pResult = (maxSqDist - sqdist) / maxSqDist;  
  }
  
  return EvalOk; 
}

}
