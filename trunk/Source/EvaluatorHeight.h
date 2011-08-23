/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EvaluatorHeight.h,v $
Revision 1.1  2004/09/08 15:43:02  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_EVALUATOR_HEIGHT_H_INCLUDED)
#define SCHMICKEN_EVALUATOR_HEIGHT_H_INCLUDED

#include "Evaluator.h"
namespace Amju
{
class EvaluatorHeight : public Evaluator
{
public:
  EvaluatorHeight();

  virtual EvalResult Score(
    const RankedMove& rm, 
    Ai* pAi, 
    VisibilityGraphWithObjects* pVgo,
    float* pResult);
};
}

#endif
