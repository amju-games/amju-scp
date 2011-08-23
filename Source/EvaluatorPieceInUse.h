/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EvaluatorPieceInUse.h,v $
Revision 1.1  2004/09/08 15:43:02  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_EVALUATOR_PIECE_IN_USE_H_INCLUDED)
#define SCHMICKEN_EVALUATOR_PIECE_IN_USE_H_INCLUDED

#include "Evaluator.h"

namespace Amju
{
// Score is lower if a piece has recently been placed by a player
// in the same cooperation group as the evaluating player.
class EvaluatorPieceInUse : public Evaluator
{
public:
  EvaluatorPieceInUse();

  virtual EvalResult Score(
    const RankedMove& rm, 
    Ai* pAi, 
    VisibilityGraphWithObjects* pVgo,
    float* pResult);
};
}

#endif
