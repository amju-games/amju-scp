/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EvaluatorAddRamp.h,v $
Revision 1.1  2004/09/08 15:43:01  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_EVALUATOR_ADD_RAMP_H_INCLUDED)
#define SCHMICKEN_EVALUATOR_ADD_RAMP_H_INCLUDED

#include "Evaluator.h"
namespace Amju
{
// Returns 1 if the given piece is a ramp, and there are enough
// blocks at the height.
// If the ramp move is not suitable, returns 0.
class EvaluatorAddRamp : public Evaluator
{
public:
  EvaluatorAddRamp();

  virtual EvalResult Score(
    const RankedMove& rm, 
    Ai* pAi, 
    VisibilityGraphWithObjects* pVgo,
    float* pResult);
};
}

#endif