/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EvaluatorRampFacing.cpp,v $
Revision 1.1  2004/09/08 15:43:03  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <iostream>
#include "Common.h"
#include "EvaluatorRampFacing.h"
#include "Finder.h"
#include "Geometry.h"
#include "Ai.h"
#include "TakeableCrate.h"
#include "AngleCompare.h"

namespace Amju
{
EvaluatorRampFacing::EvaluatorRampFacing()
{
#if defined(_DEBUG)
  SetName("Encourage ramps to face the goal");
#endif
}

Evaluator::EvalResult EvaluatorRampFacing::Score(
  const RankedMove& rm, 
  Ai* pAi, 
  VisibilityGraphWithObjects* pVgo,
  float* pResult)
{
  *pResult = 0;
  if (rm.m_pieceType != "ramp2_2_4.comp")
  {
    // Move piece is not a ramp - return neutral score.
    return EvalOk;
  }

  // Get angle of goal in relation to piece.
  VertexBase vgoal = pAi->GetCurrentGoal()->GetBoundingSphere()->GetCentre();
  VertexBase vpiece = rm.m_or.GetVertex(); //m_pCrate->GetBoundingSphere()->GetCentre();
  float a = Geometry::GetXZAngle(vgoal, vpiece);

  //std::cout << "angle between " << vgoal.x << ", " << vgoal.z << " -> " 
  //  << vpiece.x << ", " << vpiece.z << " = " << a << "\n";

  // Score depends on difference between goal angle and piece orientation.
  float apiece = rm.m_or.GetYRot();
  //apiece -= 90.0f; // TODO TEMP TEST

  float f = fabs(AngleCompare::DegDiff(apiece, a));
  
  if (fabs(f - 180.0f) < SMALLEST)
  {
    // Ramp faces goal
    *pResult = 1.0f; 
    return EvalOk;
  }
  else if (fabs(f - 90.0f) < SMALLEST)
  {
    // Ramp is at 90 degrees to facing goal.
    // NB This is given the same score as directly facing the goal,
    // so only facing completely the wrong way is punished.
    *pResult = 0.3f; // TODO CONFIG 
    return EvalOk;
  }
  else 
  {
    *pResult = -1.0f;
    return EvalOk;
  }
  
  return EvalOk;
}
}
