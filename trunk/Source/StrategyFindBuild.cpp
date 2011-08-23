/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: StrategyFindBuild.cpp,v $
Revision 1.1  2004/09/08 15:43:03  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <algorithm>
#include "StrategyFindBuild.h"
#include "File.h"
#include "Ai.h"
#include "Engine.h"
#include "TakeableCrate.h"
#include "Level.h"
#include "EngineRunning.h"
#include "VisibilityGraphWithObjects.h"
#include "AiNotifier.h"

namespace Amju
{
StrategyFindBuild::StrategyFindBuild(Strategizer* pS) : Strategy(pS)
{
  m_isValid = true;
  m_pFinder = 0;
}

bool StrategyFindBuild::IsValid() const
{
  return m_isValid;
}

void StrategyFindBuild::SelectFinder()
{
  // Get the height from the goal we have just reached to the next one.
  // If we have not reached any goal yet (since being reset) the height
  // is from the floor (y == 0) to the first goal.

  float prevheight = 0;
  if (m_pAi->GetPrevGoal().GetPtr())
  {
    prevheight = m_pAi->GetPrevGoal()->GetBoundingSphere()->GetCentre().y;
  }

  float height = m_pAi->GetCurrentGoal()->GetBoundingSphere()->GetCentre().y;
  height -= prevheight;

  int iheight = (int)height;

  m_pFinder = m_pAi->GetFinder(iheight);
}

void StrategyFindBuild::OnActivate()
{
  // Select the Finder most suited to the current goal height.
  SelectFinder();
  Assert(m_pFinder);

  // Get the next piece to place.
  m_pFinder->SetAi(m_pAi);

  TakeableCrate* pCrate = 0; // The piece to place

  // The orientation at which we want to place the piece.
  Orientation piecePos;

  // Get the current Cube Map state.
  // This is passed through Finder so it's possible to recurse, for
  // evaluating a number of moves ahead.
  VisibilityGraph* pVg = EngineRunningVisGraph::GetVisibilityGraph().GetPtr();
  VisibilityGraphWithObjects* pVgwo = dynamic_cast<VisibilityGraphWithObjects*>(pVg);
  Assert(pVgwo);

  bool success = m_pFinder->GetMove(&piecePos, &pCrate, pVgwo);

  if (!success)
  {
    m_isValid = false;

#if defined(_DEBUG)
    std::cout << "Calling FAILED because Finder failed to get a move.\n";
#endif

    m_pAi->Failed();
    return;
  }

  if (!pCrate)
  {
    // Can't find a piece.
    Assert(0);
    m_isValid = false;
  }

  m_pAi->SetTarget(pCrate);

  // Make heights equal.
  piecePos.SetY(piecePos.GetY() + 1.0f);

  m_pAi->SetPieceId(pCrate->Takeable::GetId()); 
  m_pAi->SetPiecePos(piecePos.GetVertex());
  m_pAi->SetPieceYRot(piecePos.GetYRot()); 

  m_pAi->SetState(UNKNOWN);

  // First job is jump down from a piece if we just placed one,
  // (although not if there's a path from the top of the piece.)
  //m_pAi->SetState(AI_COLLECT);
  m_pAi->SetState(AI_JUMP_DOWN);
}

void StrategyFindBuild::OnTaskFinished()
{
}

void StrategyFindBuild::OnTaskFailed()
{
  // TODO
  // The last piece we tried to place didn't work out, so dump it at the
  // end of the finder list next time.
  // e.g. m_finder.SetBadPiece(m_piece)
  // or m_finder.SetBadOrientation(m_or);
}

bool StrategyFindBuild::Load(File* pf)
{
  return true;
}

void StrategyFindBuild::Reset()
{
  // TODO reset finder ?
  m_isValid = true;
}

void StrategyFindBuild::RecalculatePath()
{
  State s = m_pAi->GetState();
  if (s == OUT_OF_PLAY)
  {
    return;
  }
  // This is supposed to have the effect of reinitializing the current state,
  //  thus recalculating the path.
  m_pAi->SetState(OUT_OF_PLAY);
  m_pAi->SetState(s);
}
}
