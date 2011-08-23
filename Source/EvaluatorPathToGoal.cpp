/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EvaluatorPathToGoal.cpp,v $
Revision 1.1  2004/09/08 15:43:02  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "EvaluatorPathToGoal.h"
#include "EngineRunningVisGraph.h"
#include "Finder.h"
#include "Ai.h"
#include "VisibilityGraphWithObjects.h"
#include "AStarPathFinder.h"
#include "TakeableCrate.h"

namespace Amju
{
EvaluatorPathToGoal::EvaluatorPathToGoal()
{
#if defined(_DEBUG)
  SetName("Encourage path to goal.");
#endif
}

Evaluator::EvalResult EvaluatorPathToGoal::Score(
  const RankedMove& rm, 
  Ai* pAi, 
  VisibilityGraphWithObjects* pVgo,
  float* pResult)
{
  // Make the move, then try to get a path from the AI to its goal pos.
  // Then place the piece in its old position in the vis. graph.
  // NB This is an expensive operation!
  // Especially as no path means all nodes will be searched by A* :-(

  // TODO

  // Take away the crate in its current position
  pVgo->UpdatePieceTaken(rm.m_pCrate, EngineRunningVisGraph::LINE_OF_SIGHT_RADIUS);
  // Add the crate in its new position.
  pVgo->UpdatePieceDropped(rm.m_pCrate, EngineRunningVisGraph::LINE_OF_SIGHT_RADIUS);

  // Try to find a path from the AI position to its goal position.
  AStarPathFinder pf(pVgo);
  float radius = EngineRunningVisGraph::LINE_OF_SIGHT_RADIUS; // TODO or AI radius ?
  pf.SetLineOfSightWidth(radius);

  VertexBase begin = pAi->GetPosition();
  // Raise y-value, it's at floor level.
  begin.y += 1.0f;
  VertexBase end = pAi->GetCurrentGoal()->GetBoundingSphere()->GetCentre();

  SearchResult sr = pf.FindPath(begin, end);

  // TODO
  // Replace the piece in its old position, restoring visibility graph
  // to the state iw was in at the start of this function.
  // Alternatively we could clone the graph - TODO PROFILE

  if (sr == SearchOk) 
  {
    *pResult = 1.0f;
    return EvalOk; 
  }
  else if (sr == SearchPending)
  {
    return EvalPending; // must call again. TODO handle this
  }

  pResult = 0; // no path exists :-(
  return EvalOk; // "ok" in the sense that the result is known
}

}