/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EvaluatorPieceSupport.h,v $
Revision 1.1  2004/09/08 15:43:03  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_EVALUATOR_PIECE_SUPPORT_H_INCLUDED)
#define SCHMICKEN_EVALUATOR_PIECE_SUPPORT_H_INCLUDED

#include "Evaluator.h"
namespace Amju
{
// This evaluator checks that the piece is supported at all corners,
// and is not on top of a ramp.
class EvaluatorPieceSupport : public Evaluator
{
public:
  EvaluatorPieceSupport();

  virtual EvalResult Score(
    const RankedMove& rm, 
    Ai* pAi, 
    VisibilityGraphWithObjects* pVgo,
    float* pResult);
};
}

#endif
