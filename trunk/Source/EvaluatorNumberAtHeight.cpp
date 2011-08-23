/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EvaluatorNumberAtHeight.cpp,v $
Revision 1.1  2004/09/08 15:43:02  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "EvaluatorNumberAtHeight.h"
#include "Ai.h"
#include "Finder.h"

namespace Amju
{
EvaluatorNumberAtHeight::EvaluatorNumberAtHeight()
{
#if defined(_DEBUG)
  SetName("Punishment for too many pieces at given height");
#endif
}

Evaluator::EvalResult EvaluatorNumberAtHeight::Score(
  const RankedMove& rm, 
  Ai* pAi, 
  VisibilityGraphWithObjects* pVgo,
  float* pResult)
{
  float y = rm.m_or.GetY();

  Assert(pAi->GetCooperationGroup());

  float goalHeight = pAi->GetCurrentGoal()->GetBoundingSphere()->GetCentre().y;
  float diff = goalHeight - y;
  if (diff < 0)
  {
    *pResult = 0;
    return EvalOk;
  }

  int numPiecesAtHeight= pAi->GetCooperationGroup()->GetNumPiecesPlacedAtHeight(y);

  // TODO this is the same code as AddRamp
  int idealPieces = pAi->GetIdealNumPiecesAtHeight(diff); 

/*
  if (numPiecesAtHeight < idealPieces)
  {
    *pResult = 1.0f;
    return EvalOk;
  }
*/
  if (numPiecesAtHeight <= idealPieces)
  {
    *pResult = 0;
    return EvalOk;
  }

  // TODO fuzzier values
  *pResult = -1.0f;
  return EvalOk;
  
  // The score is proportional to the number of pieces in the layer.
  // But there's a limit, decided by the height diff.
  // More pieces is good, but too many for the height is bad.
  // This is to encourage bigger bases than tops.

}

}