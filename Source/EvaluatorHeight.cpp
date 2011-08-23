/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EvaluatorHeight.cpp,v $
Revision 1.1  2004/09/08 15:43:02  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "EvaluatorHeight.h"
#include "Ai.h"
#include "Finder.h"

namespace Amju
{
EvaluatorHeight::EvaluatorHeight()
{
#if defined(_DEBUG)
  SetName("Encouragement proportional to height, (with punishment for too high)");
#endif
}

Evaluator::EvalResult EvaluatorHeight::Score(
  const RankedMove& rm, 
  Ai* pAi, 
  VisibilityGraphWithObjects* pVgo,
  float* pResult)
{
  // This Evaluator ranks moves by the height of the piece.
  // If the piece is too high, move is very bad.
  // Otherwise we reward higher pieces.

  float pieceHeight = rm.m_or.GetY();
  // Need AI or goal against which to evaluate.
  float goalHeight = GetGoalPos(pAi).y;
  float diff = goalHeight - pieceHeight;
  // If piece is higher than goal. move is pointless, so very low score.
  *pResult = 0;
  if (diff < 0)
  {
    *pResult = -1.0f;
  }
  else
  {
    // Score is proportional to height, to encourage building upwards.
    // However, we punish a piece at height (h) if there are not enough pieces
    // at height (h - 1).
    int idealPieces = pAi->GetIdealNumPiecesAtHeight(diff + 2.0f); 
    int numPiecesAtHeight = pAi->GetCooperationGroup()->
      GetNumPiecesPlacedAtHeight(pieceHeight - 2.0f);
    if (numPiecesAtHeight > 0 && numPiecesAtHeight < idealPieces)
    {
      *pResult = -1.0f;
      return EvalOk;
    }
  
    //const float max = 20.0f; // TODO CONFIG
    //*pResult = (max - diff) / max;
    *pResult = 0;
    if (*pResult < 0)
    {
      *pResult = 0;
    }
  }

  return EvalOk;
}
}