/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EvaluatorPieceInUse.cpp,v $
Revision 1.1  2004/09/08 15:43:02  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "EvaluatorPieceInUse.h"
#include "Ai.h"
#include "Finder.h"
#include "TakeableCrate.h"

namespace Amju
{
EvaluatorPieceInUse::EvaluatorPieceInUse()
{
#if defined(_DEBUG)
  SetName("Punishment for moving a recently-placed piece");
#endif
}

Evaluator::EvalResult EvaluatorPieceInUse::Score(
  const RankedMove& rm, 
  Ai* pAi, 
  VisibilityGraphWithObjects* pVgo,
  float* pResult)
{
  // Score lower the more recently the piece has been used by the AI's
  // group. Score zero if the piece is not used by the AI's group.
  Assert(pAi->GetCooperationGroup());

  int pieceId = rm.m_pCrate->Takeable::GetId();
  if (pAi->GetCooperationGroup()->IsPieceUsed(pieceId))
  {
    int pieceOrder = pAi->GetCooperationGroup()->GetPieceOrder(pieceId);
    float f = (float)pieceOrder / 20.0f; // TODO CONFIG
    if (f > 1.0f)
    {
      f = 1.0f;
    }
    // Most recently used piece scores -1; less recently used pieces score
    // more positively (but still < 0).
    *pResult = -1.0f + f;
    return EvalOk;
  }

  // TODO
  *pResult = 0;
  return EvalOk; 
}
}
