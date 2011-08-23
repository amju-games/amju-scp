/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EvaluatorNearPrevious.h,v $
Revision 1.1  2004/09/08 15:43:02  jay
Added to repository
  
*/

#if !defined(EVALUATOR_NEAR_PREVIOUS_H_INCLUDED)
#define EVALUATOR_NEAR_PREVIOUS_H_INCLUDED

#include "Evaluator.h"
namespace Amju
{
// This evaluator encourages pieces to be placed near the last piece
// placed.
class EvaluatorNearPrevious : public Evaluator
{
public:
  EvaluatorNearPrevious();

  virtual EvalResult Score(
    const RankedMove& rm, 
    Ai* pAi, 
    VisibilityGraphWithObjects* pVgo,
    float* pResult);
};
}

#endif
