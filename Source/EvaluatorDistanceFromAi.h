/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EvaluatorDistanceFromAi.h,v $
Revision 1.1  2004/09/08 15:43:02  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_EVALUATOR_DISTANCE_FROM_AI_H_INCLUDED)
#define SCHMICKEN_EVALUATOR_DISTANCE_FROM_AI_H_INCLUDED

#include "Evaluator.h"

namespace Amju
{
// Rank each move based on the distance from the AI to the piece 
// to the goal, i.e. the total distance the AI must travel to make
// the move.
class EvaluatorDistanceFromAi : public Evaluator
{
public:
  EvaluatorDistanceFromAi();

  virtual EvalResult Score(
    const RankedMove& rm, 
    Ai* pAi, 
    VisibilityGraphWithObjects* pVgo,
    float* pResult);
};
}

#endif
