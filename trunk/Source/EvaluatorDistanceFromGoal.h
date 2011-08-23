/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EvaluatorDistanceFromGoal.h,v $
Revision 1.1  2004/09/08 15:43:02  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_EVALUATOR_DISTANCE_FROM_GOAL_H_INCLUDED)
#define SCHMICKEN_EVALUATOR_DISTANCE_FROM_GOAL_H_INCLUDED

#include "Evaluator.h"

namespace Amju
{
class EvaluatorDistanceFromGoal : public Evaluator
{
public:
  EvaluatorDistanceFromGoal();

  virtual EvalResult Score(
    const RankedMove& rm, 
    Ai* pAi, 
    VisibilityGraphWithObjects* pVgo,
    float* pResult);
};
}

#endif
