/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: StrategyFindTest.cpp,v $
Revision 1.1  2004/09/08 15:43:03  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "StrategyFindTest.h"
#include "Engine.h"
#include "Ai.h"
#include "AiNotifier.h"
#include "Takeable.h"
#include "EngineRunningVisGraph.h"

namespace Amju
{
StrategyFindTest::StrategyFindTest(Strategizer* pS) : StrategyFindBuild(pS)
{
}

void StrategyFindTest::OnActivate()
{
#if defined(_DEBUG)
  // The base class version uses the Finder to set the piece and
  // target location in the AI.
  StrategyFindBuild::OnActivate();

  // Put the target piece in its target location immediately.
  // So we don't rely on the Character movement to test the Finder.
  int pieceId = m_pAi->GetPieceId();
  VertexBase v = m_pAi->GetPiecePos();
  float yrot = m_pAi->GetPieceYRot();

  PGameObject pGo = Engine::Instance()->GetGameObject(pieceId);
  Takeable* pTakeable = dynamic_cast<Takeable*>(pGo.GetPtr());

  EngineRunningVisGraph::RecalcVisGraphPieceTaken(pTakeable);

  // NB we subtract 1 from y because 1 is added in the base class, 
  // (so pathfinding works)
  Orientation o(v.x, v.y - 1.0f, v.z, 0, yrot, 0);
  pGo->SetOrientation(o);

  EngineRunningVisGraph::RecalcVisGraphPieceDropped(pTakeable);

  // Not needed ordinarily, when the PieceMover actually drops the piece.
  // The last placed piece is used to group pieces together, etc.
  m_pAi->SetLastPiecePlaced(pTakeable);

  // Reset the AI's state so he does nothing.
  m_pAi->SetState(UNKNOWN);

  //Notifier::Instance()->NotifyPieceDropped(pTakeable, 0);

  // Sleep for some configurable time. Otherwise things may
  // happen too fast!
  // TODO

#endif
}

void StrategyFindTest::RecalculatePath()
{
  m_pAi->Succeeded();
}

}