/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EvaluatorAddRamp.cpp,v $
Revision 1.1  2004/09/08 15:43:01  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "EvaluatorAddRamp.h"
#include "Finder.h"
#include "Ai.h"

namespace Amju
{
EvaluatorAddRamp::EvaluatorAddRamp()
{
#if defined(_DEBUG)
  SetName("Discouragement to add ramp too early");
#endif
}

Evaluator::EvalResult EvaluatorAddRamp::Score(
  const RankedMove& rm, 
  Ai* pAi, 
  VisibilityGraphWithObjects* pVgo,
  float* pResult)
{
  *pResult = 0;
  // If move is not for a ramp, return 0.
  if (rm.m_pieceType != "ramp2_2_4.comp") // TODO better way of classifying pieces
  {
    return EvalOk;
  }
  // Get the number of pieces already placed at the given height.

  // At a lower height, more pieces of other types need to be placed.
  // Higher up, fewer other pieces need to be placed before a ramp.
  float y = rm.m_or.GetY();

  Assert(pAi->GetCooperationGroup());

  // Check if there's already a ramp at height y. If so, there's not
  // much point in placing another. But don't give too much discouragement -
  // the other ramp(s) may be blocked.
  int numRampsAtHeight = pAi->GetCooperationGroup()->GetNumRampsPlacedAtHeight(y);
  if (numRampsAtHeight == 1)
  {
    return EvalOk;
  }
  // TODO CONFIG configurable score to no. of ramps ratio
  if (numRampsAtHeight == 2)
  {
    *pResult = -0.5f; // NB this evaluator returns -ve and +ve values
    return EvalOk;
  }

  if (numRampsAtHeight > 2)
  {
    *pResult = -1.0f;
    return EvalOk;
  }

  int numPiecesAtHeight= pAi->GetCooperationGroup()->GetNumPiecesPlacedAtHeight(y);

  // Get difference between goal height and piece height
  // (Piece height should be lower).
  float goalHeight = pAi->GetCurrentGoal()->GetBoundingSphere()->GetCentre().y;
  float diff = goalHeight - y;
  if (diff < 0)
  {
    return EvalOk;
  }
  // Get the ideal number of pieces for the height difference.
  // TODO CONFIG for this rule.
  // This will depend on the distance to the goal too.
  int idealPieces = pAi->GetIdealNumPiecesAtHeight(diff); 
  int RAMP_ENCOURAGE = 2; 

  int p = numPiecesAtHeight + RAMP_ENCOURAGE;

  if (p > idealPieces)
  {
    *pResult = 0; 
    return EvalOk;
  }
  else 
  {
    /*
    // Discourage early addition of ramp.
    // Score is -1 when there are no pieces placed, 0 when there are
    // idealPieces pieces placed.
    float fp = p / idealPieces;
    if (fp > 1.0f)
    {
      fp = 1.0f;
    }
    if (fp < 0)
    {
      fp = 0;
    }

    *pResult = fp - 1.0f;
    */
    *pResult = -1.0f;
    return EvalOk;
  }

  return EvalOk; 
}
}