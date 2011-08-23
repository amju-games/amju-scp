/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EvaluatorPathToGoal.h,v $
Revision 1.1  2004/09/08 15:43:02  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_EVALUATOR_PATH_TO_GOAL_H_INCLUDED)
#define SCHMICKEN_EVALUATOR_PATH_TO_GOAL_H_INCLUDED

#include "Evaluator.h"
namespace Amju
{
// Returns 1.0f if placing the piece creates/maintains a path to
// the goal position.
class EvaluatorPathToGoal : public Evaluator
{
public:
  EvaluatorPathToGoal();

  virtual EvalResult Score(
    const RankedMove& rm, 
    Ai* pAi, 
    VisibilityGraphWithObjects* pVgo,
    float* pResult);
};
}

#endif
