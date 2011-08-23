/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EvaluatorNumberAtHeight.h,v $
Revision 1.1  2004/09/08 15:43:02  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_EVALUATOR_NUM_AT_HEIGHT_H_INCLUDED)
#define SCHMICKEN_EVALUATOR_NUM_AT_HEIGHT_H_INCLUDED

#include "Evaluator.h"

namespace Amju
{
// This evaluator scores moves based on the number of touching pieces 
// at its height.
class EvaluatorNumberAtHeight : public Evaluator
{
public:
  EvaluatorNumberAtHeight();

  virtual EvalResult Score(
    const RankedMove& rm, 
    Ai* pAi, 
    VisibilityGraphWithObjects* pVgo,
    float* pResult);
};
}

#endif
