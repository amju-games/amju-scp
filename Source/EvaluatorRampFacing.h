/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EvaluatorRampFacing.h,v $
Revision 1.1  2004/09/08 15:43:03  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_EVALUATOR_RAMP_FACING_H_INCLUDED)
#define SCHMICKEN_EVALUATOR_RAMP_FACING_H_INCLUDED

#include "Evaluator.h"
namespace Amju
{
// This evaluator encourages ramps to face the right way.
class EvaluatorRampFacing : public Evaluator
{
public:
  EvaluatorRampFacing();

  virtual EvalResult Score(
    const RankedMove& rm, 
    Ai* pAi, 
    VisibilityGraphWithObjects* pVgo,
    float* pResult);
};
}

#endif


