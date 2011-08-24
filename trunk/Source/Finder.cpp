/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Finder.cpp,v $
Revision 1.1  2004/09/08 15:43:03  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <algorithm>
#include <iostream> // for debug output
#include <iterator>
#include "Finder.h"
#include "TakeableCrate.h"
#include "Orientation.h"
#include "Ai.h"
#include "CubeMap.h"
#include "Level.h"
#include "Engine.h"
#include "File.h"
#include "EngineRunningVisGraph.h"
#include "VisibilityGraphWithObjects.h"
#include "CubeMap.h"
#include "EvaluatorFactory.h"
#include "AStarPathFinder.h"
#include "AngleCompare.h"
#include "SchAssert.h"

using namespace std;
namespace Amju
{
bool Move::Load(File* pf)
{
  /*
  if (!pf->GetFloat(&m_yrot))
  {
    pf->ReportError("Expected move y-rotation.");
    return false;
  }
  */
  if (!pf->GetDataLine(&m_pieceType)) 
  {
    pf->ReportError("Expected piece type.");
    return false;
  }
  if (!m_or.Load(pf))
  {
    pf->ReportError("Failed to load piece orientation for move.");
    return false;
  }
  return true;
}

RankedMove::RankedMove(const Move& m) : Move(m)
{
  m_pCrate = 0;
  m_rank = 0;
}

struct SameSorter
{
bool operator() (const RankedMove& lhs, const RankedMove& rhs)
{
  // Two moves are the same if the piece type is the same and the orientation
  // is also the same.
  if (lhs.m_pieceType < rhs.m_pieceType)
  {
    return true;
  }
  if (lhs.m_pieceType > rhs.m_pieceType)
  {
    return false;
  }

  return lhs.m_or < rhs.m_or;
}
};

bool RankedMove::operator==(const RankedMove& rhs) const
{
  // Define RankedMove equality in terms of SameSorter.
  // That's because we use SameSorter to sort a list, then std::unique()
  // to remove duplicates, which relies on this operator==().
  // Both sort and unique must use the same criteria to sort lists,
  // or we get undefined behaviour.

  SameSorter ss;
  return !ss(*this, rhs) && !ss(rhs, *this);
}

#if defined(_DEBUG)
std::ostream& operator<<(std::ostream& o, const RankedMove& rm)
{
  rm.Print(o);
  return o;
}

void RankedMove::SetEvaluatorScore(const std::string& evaluatorName, float score)
{
  m_scoreMap[evaluatorName] = score;
}

void RankedMove::Print(std::ostream& o) const
{
  o   << " piece ID: " << m_pCrate->Takeable::GetId() 
      << " (type " << m_pieceType.c_str() 
      << ")  move to: (" 
      << m_or.GetX() << ", " 
      << m_or.GetY() << ", "
      << m_or.GetZ() << ") "
      << "[ry=" << m_or.GetYRot() << "]"
      << " Score: " << m_rank;

  o << "  Scores for evaluators:" << std::endl;
  for (ScoreMap::const_iterator it = m_scoreMap.begin(); it != m_scoreMap.end(); ++it)
  {
    o << "  " 
      << it->first
      << ": "
      << it->second
      << std::endl;
  }
}

#endif

// ----------------------------------------------------------------------------

// This helpful utility function rotates and translates a bounding box.
void Finder::BoundingBoxTransform(
  BoundingBox* pBb,  
  const VertexBase& posBefore,
  const VertexBase& posAfter,
  float yRotBefore,
  float yRotAfter)
{
  Assert(pBb);

  //Orientation savedOr = *(pCrate->GetOrientation());
  //State savedState = pCrate->GetState();
  //pCrate->SetState(UNKNOWN); // to get collision vol

  float diff = yRotAfter - yRotBefore; //o.GetYRot() - savedOr.GetYRot();
  // TODO maybe we should use AngleCompare ?

//  Assert(pCrate->GetCollisionVol());
//  BoundingBox bb = pCrate->GetCollisionVol()->GetBoundingBox();
  // If we can transform the bounding box directly, we don't have to 
  // do the expensive SetOrientation on the piece.
  // Rotate by the y-rotation diff.
  // But we want to rotate about the bounding box centre, not the world origin.
  // So translate to origin.
//  BoundingBox bb1 = bb; // TODO TEMP TEST for debugging
//  bb.Translate(-savedOr.GetX(), -savedOr.GetY(), -savedOr.GetZ());
  pBb->Translate(-posBefore.x, -posBefore.y, -posBefore.z);

//  BoundingBox bb2 = bb; // TODO TEMP TEST for debugging
//  bb.RotateY(diff);
  pBb->RotateY(diff);
//  BoundingBox bb3 = bb; // TODO TEMP TEST for debugging
  // Now translate to new orientation
//  bb. Translate(o.GetX(), o.GetY(), o.GetZ());
  pBb->Translate(posAfter.x, posAfter.y, posAfter.z);

//  bool result = pVgo->GetCubeMap().Intersects(bb);
//  pCrate->SetState(savedState); // restore real piece state

}

// ----------------------------------------------------------------------------

Finder::Finder()
{
  m_pLastPieceChosen = 0;
}

void Finder::SetAi(Ai* pAi)
{
  m_pAi = pAi;
}

Ai* Finder::GetAi()
{
  return m_pAi;
}

void Finder::SetHeight(int h)
{
  m_height = h;
}

float Finder::GetGoalRadius()
{
  static float r = atof(Engine::Instance()->GetConfigValue("finder_goal_radius").c_str()); 
  return r;
}

Finder::PieceList Finder::GetPiecesToConsider()
{
  PieceList results;
  
  // Get goal position of AI. Use this to choose pieces to consider.
  Assert(m_pAi);
  Assert(m_pAi->GetCurrentGoal().GetPtr());
  Assert(m_pAi->GetCurrentGoal()->GetBoundingSphere());

  const VertexBase& goalPos = m_pAi->GetCurrentGoal()->GetBoundingSphere()->GetCentre();
  float radius = GetGoalRadius(); 
  BoundingSphere bs(goalPos, radius); 

  // Include pieces which are within some radius around the goalPos.
  int levelId = m_pAi->GetLevel()->GetId();
  int roomId =  m_pAi->GetLevel()->GetRoomId();

  // Get the game objects which are in the same room as the AI.
  GameObjectMap& objs = Engine::Instance()->GetGameObjects(levelId, roomId);

  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    PGameObject pGo = it->second;
    // Consider the piece if it's a TakeableCrate and intersects the b.sphere.
    TakeableCrate* pTakeableCrate = dynamic_cast<TakeableCrate*>(pGo.GetPtr());
    if (pTakeableCrate && 
        pTakeableCrate->Takeable::GetState() == UNKNOWN &&
        pTakeableCrate->Takeable::GetBoundingSphere()->Intersects(bs) 
       )
    {
      results.push_back(pTakeableCrate);
    }
  }

  return results;
}

bool Finder::Load(File* pf)
{
  // Get the filename for the moves
  std::string movesFilename;
  if (!pf->GetDataLine(&movesFilename))
  {
    pf->ReportError("Expected moves file name.");
    return false;
  }
  File movesFile;
  if (!movesFile.OpenRead(movesFilename))
  { 
    return false;
  }

  bool b = LoadMoves(&movesFile);
  if (!b)
  {
    return false;
  }

  // Get filename for evaluators
  std::string evalFilename;
  if (!pf->GetDataLine(&evalFilename))
  {
    pf->ReportError("Expected evaluators file name.");
    return false;
  }
  File evalFile;
  if (!evalFile.OpenRead(evalFilename))
  {
    return false;
  }

  b = LoadEvaluators(&evalFile);
  if (!b)
  {
    return false;
  }

  b = LoadSeeds(pf);

  return b;
}

bool Finder::LoadMoves(File* pf)
{
  // Get the number of possible move entries.
  int numMoves = 0;
  if (!pf->GetInteger(&numMoves))
  {
    pf->ReportError("Expected number of possible moves.");
    return false;
  }

  int i;
  for (i = 0; i < numMoves; i++)
  {
    // Each move has a number of piece types, not just one.
    // For a move to be valid, all piece types must be in the appropriate
    // relative positions. Then the new piece may be added in its 
    // relative position.
    PossibleMove pm;
    
    int numPieceTypes;
    if (!pf->GetInteger(&numPieceTypes))
    {
      pf->ReportError("Expected number of piece types.");
      return false;
    }
    for (int j = 0; j < numPieceTypes; j++)
    {
      PieceType pt;
      // Load the piece type.
      // TODO This depends on what a PieceType really is!
      if (!pf->GetDataLine(&pt))
      {
        pf->ReportError("Expected piece type.");
        return false;
      }
      // Get orientation (relative to first piece - so first orientation should
      // always be the origin.)
      Orientation o;
      if (!o.Load(pf))
      {
        pf->ReportError("Expected piece type orientation.");
        return false;
      }

      PossibleMove::PossMoveItem pmi(o, pt);
      pm.m_itemList.push_back(pmi);
    }

    // Load the possible move for the situation. The move is another piece type
    //  and a (relative) orientation.
    Move m; 
    if (!m.Load(pf))
    {
      return false;
    }
    
    pm.m_response = m;

    m_possibleMoves.push_back(pm);

    //m_possibleMoves.insert(PossibleMoves::value_type(pt, m));

    // TODO At this point the move should be 'transposed', i.e. rotated
    //  by 90 degrees three times, to avoid having to do this in the config file.

  }
  return true;
}

bool Finder::LoadEvaluators(File* pf)
{
  // Get the evaluators for this finder.
  // Get the number of evaluators
  int numEvaluators = 0;
  if (!pf->GetInteger(&numEvaluators))
  {
    pf->ReportError("Expected number of evaluators.");
    return false;
  }
  // Get each evaluator.
  for (int i = 0; i < numEvaluators; i++)
  {
    // Create the evaluator
    std::string evaluatorName;
    if (!pf->GetDataLine(&evaluatorName))
    {
      pf->ReportError("Expected evaluator name.");
      return false;
    }
    SharedPtr<Evaluator> pEvaluator = EvaluatorFactory::Instance()->Create(evaluatorName);
    if (!pEvaluator.GetPtr())
    {
      std::string err = "Failed to create evaluator ";
      err += evaluatorName;
      pf->ReportError(err);
      return false;
    }
    // Get weighting for evaluator
    float weight = 0;
    if (!pf->GetFloat(&weight))
    {
      pf->ReportError("Expected evaluator weighting.");
      return false;
    }
    pEvaluator->SetWeight(weight);

    // Load evaluator-specific data
    if (!pEvaluator->Load(pf))
    {
      std::string err = "Failed to load evaluator ";
      err += evaluatorName;
      pf->ReportError(err);
      return false;
    }
    // Add it to m_evaluators.
    m_evaluators.push_back(pEvaluator);
  }
  
  return true;
}

bool Finder::LoadSeeds(File* pf)
{
  int numSeeds = 0;
  if (!pf->GetInteger(&numSeeds))
  {
    pf->ReportError("Exoected number of seed types.");
    return false;
  }
  for (int i = 0; i < numSeeds; i++)
  {
    PieceType pt;
    if (!pf->GetDataLine(&pt))
    {
      pf->ReportError("Expected seed type.");
      return false;
    }

    m_seedPieceTypes.push_back(pt);
  }

  return true;
}

PieceType Finder::GetTypeForPiece(TakeableCrate* p)
{
  return p->GetType();
}

Orientation Finder::GetOrientationForPiece(TakeableCrate* p)
{
  return *(p->Takeable::GetOrientation());
}

Finder::PieceList Finder::GetAvailablePiecesOfType(
  PieceType pt, 
  VisibilityGraphWithObjects* pVgo)
{
  PieceList results;

  int levelId = m_pAi->GetLevel()->GetId();
  int roomId =  m_pAi->GetLevel()->GetRoomId();

  // Get the game objects which are in the same room as the AI.
  GameObjectMap& objs = Engine::Instance()->GetGameObjects(levelId, roomId);

  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    PGameObject pGo = it->second;
    // Consider the piece if it's a TakeableCrate of the correct type,
    // desn't support anything, and isn't used by us or anyone in our group.
    TakeableCrate* pTakeableCrate = dynamic_cast<TakeableCrate*>(pGo.GetPtr());
    if (pTakeableCrate && 
        pTakeableCrate->GetType() == pt &&
        pTakeableCrate->Takeable::GetState() == UNKNOWN &&
        pTakeableCrate != m_pLastPieceChosen &&
        !PieceSupportsOthers(pVgo, pTakeableCrate)
    ) 
        // TODO more conditions 
    {
      results.push_back(pTakeableCrate);
    }
  }

  return results;
}

bool Finder::IsPieceTypeAt(
  PieceType pt, 
  const Orientation& o, 
  VisibilityGraphWithObjects* pVgo)
{
  // First test could be to check the cube map - if it's empty at the
  // given point, there can't be a piece there of any type.
  // TODO


  // Check orientaiton of each piece of type pt
  int levelId = m_pAi->GetLevel()->GetId();
  int roomId =  m_pAi->GetLevel()->GetRoomId();

  // Get the game objects which are in the same room as the AI.
  GameObjectMap& objs = Engine::Instance()->GetGameObjects(levelId, roomId);

  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    PGameObject pGo = it->second;
    // Consider the piece if it's a TakeableCrate of the correct type,
    // desn't support anything, and isn't used by us or anyone in our group.
    TakeableCrate* pTakeableCrate = dynamic_cast<TakeableCrate*>(pGo.GetPtr());
    if (pTakeableCrate && 
        pTakeableCrate->GetType() == pt &&
        pTakeableCrate->Takeable::GetState() == UNKNOWN &&
        *(pTakeableCrate->Takeable::GetOrientation()) == o
    ) 
    {
      return true;
    }
  }
  return false;
}

Finder::RankedMoveList Finder::GetPossibleNeighboursForPiece(
  TakeableCrate* pCrate, 
  VisibilityGraphWithObjects* pVgo)
{
  // Finding 'neighbours for a piece' means finding other pieces which can be
  // placed adjacent to the piece. The piece itself (pCrate) isn't moved.

  RankedMoveList results;

  PieceType pt = GetTypeForPiece(pCrate);

  Orientation crateOr = GetOrientationForPiece(pCrate);

  // Get possible moves for the given piece type.
  // The possible moves consist of a list of pieces and orientations.
  // If we find a configuration of pieces which matches the possible
  // move, then the 'response' for the possible move is valid.

  // Iterator over the possible moves.
  PossibleMoves::iterator it = m_possibleMoves.begin();
  PossibleMoves::iterator upper = m_possibleMoves.end();
  for (; it != upper; ++it)
  {
    const PossibleMove& possmove = *it;
    Assert(!possmove.m_itemList.empty());

    // First piece matches the crate for which we are trying to find neighbours ?
    if (possmove.m_itemList[0].m_pieceType != pt)
    {
      continue; 
    }
    // Iterate over the remainder of the itemlist for this possible move.
    PossibleMove::PossMoveItemList::const_iterator kt = possmove.m_itemList.begin();
    ++kt; // this is ok, there's at least one item.
    for (; kt != possmove.m_itemList.end(); ++kt)
    {
      // Check there's a piece of the corrrect type at the specified orientation.
      // If not, this possible move fails, so we go on to the next one.
      PieceType pt = kt->m_pieceType;
      Orientation o = kt->m_or;
      o += crateOr;
      // Is there a piece of type pt at location o ?
      // If not, this possible move doesn't match.
      if (!IsPieceTypeAt(pt, o, pVgo))
      {
        continue;
      }
    }
    
    // Create a 'concrete' move from the 'template' moves in the possible list.
    const Move& move = possmove.m_response;


    Orientation newOr = move.m_or;
    newOr.SetX(newOr.GetX() + crateOr.GetX());
    newOr.SetY(newOr.GetY() + crateOr.GetY());
    newOr.SetZ(newOr.GetZ() + crateOr.GetZ());

    // Check ONE piece of the given type, to see if it would intersect anything.
    // If not, ALL pieces of the type are possible moves.
    PieceType thisPt = move.m_pieceType; 

    // Get all available pieces of type thisPt. 'Available' here means not used 
    // by this AI or any others in its cooperation group, and not supporting
    // anything.
    PieceList pieces = GetAvailablePiecesOfType(thisPt, pVgo);

    // If there are no available pieces of this type, reject the move.
    // Also we can prune all other moves which use a piece of this type!
    if (pieces.empty())
    {
      // TODO add type to "prune list" - list of unuseable types ?
      continue;
    }

    // Try the first one to see if it would intersect something.
    TakeableCrate* pFirstPiece = pieces[0].GetPtr(); 

    if (PieceWouldIntersectCubeMap(pVgo, pFirstPiece, newOr))
    {
      // No piece of this type will fit here.
      continue;
    }

    // Try the first piece to make sure it will be supported by something.
    // If not, then no piece of the type (thisPt) can go here.
    // An evaluator can refine this by seeing if the piece is totally 
    // supported, etc.
    if (!PieceWouldBeSupported(pVgo, pFirstPiece, newOr))
    {
      continue;
    }

    // All pieces of this type will fit here.
    for (PieceList::iterator jt = pieces.begin(); jt != pieces.end(); ++jt)
    {
      TakeableCrate* pThisCrate = jt->GetPtr();
      if (pThisCrate != pCrate)
      {
        // Must test if piece is holding something else up - if so, 
        // we can't move it.
        if (!PieceSupportsOthers(pVgo, pThisCrate))
        {
          RankedMove rm(move);
          rm.m_or = newOr;
          rm.m_pCrate = pThisCrate;
          results.push_back(rm);
        }
      }
    }
  }

  return results;
}

bool Finder::PieceSupportsOthers(
  VisibilityGraphWithObjects* pVgo, 
  TakeableCrate* pCrate)
{
  //Orientation savedOr = *(pCrate->GetOrientation());
  State savedState = pCrate->Takeable::GetState();
  pCrate->Takeable::SetState(UNKNOWN);
  //Assert(savedState == UNKNOWN); // ?
  // Change the orientation to ABOVE the piece.
  // Try the new orientation. Get the new bounding box and check if it
  // intersects the cube map. If it does, then something is above
  // the piece.

  Assert(pCrate->Takeable::GetCollisionVol());

  BoundingBox bb = pCrate->Takeable::GetCollisionVol()->GetBoundingBox();
  bb.m_ymin = bb.m_ymax;
  bb.m_ymax += 1.0f;

  bool result = pVgo->GetCubeMap().Intersects(bb);
  pCrate->Takeable::SetState(savedState);
  //pCrate->SetOrientation(savedOr);

  return result;
}

bool Finder::PieceWouldBeSupported(
  VisibilityGraphWithObjects* pVgo, 
  TakeableCrate* pCrate, 
  Orientation o)
{
  Orientation savedOr = *(pCrate->Takeable::GetOrientation());
  State savedState = pCrate->Takeable::GetState();
  pCrate->Takeable::SetState(UNKNOWN);
  // Change the orientation to BELOW the piece.
  // Try the new orientation. Get the new bounding box and check if it
  // intersects the cube map. If it does, then something is underneath
  // where we want the piece to go.
//  o.SetY(o.GetY() - 1.0f);
//pCrate->SetOrientation(o);
  // Translate bounding box to new position, with extra height adjustment
  // as described above.
  // NB We don't set the orientation of the crate, as this is an
  // expensive operation - we only need to change the bounding box.
  BoundingBox bb = pCrate->Takeable::GetCollisionVol()->GetBoundingBox();
  VertexBase vAfter = o.GetVertex();
  vAfter.y -= 1.0f;  // make the destination height adjustment
  BoundingBoxTransform(&bb, savedOr.GetVertex(), vAfter, savedOr.GetYRot(), o.GetYRot());

  bool result = pVgo->GetCubeMap().Intersects(bb);
  pCrate->Takeable::SetState(savedState);
//pCrate->SetOrientation(savedOr);

  return result;
}

bool Finder::PieceWouldIntersectCubeMap(
  VisibilityGraphWithObjects* pVgo, 
  TakeableCrate* pCrate, 
  Orientation o)
{
/*
#if defined(_DEBUG)
  bool correctResult;
  BoundingBox correctBB;

  { 
    Orientation savedOr = *(pCrate->GetOrientation());
    State savedState = pCrate->GetState();
    pCrate->SetState(UNKNOWN); 
    // Try the new orientation. Get the new bounding box and check if it
    // intersects the cube map.
    pCrate->SetOrientation(o);
    Assert(pCrate->GetCollisionVol());
    correctBB = pCrate->GetCollisionVol()->GetBoundingBox();
    correctResult = pVgo->GetCubeMap().Intersects(correctBB);
    pCrate->SetState(savedState);
    pCrate->SetOrientation(savedOr);
  }
#endif
*/

  // The above works fine, but kills performance. The reason is that
  // setting the Crate's orientation recalcs the height server etc.
  // All we need to do is get the bounding box for the piece, and  
  // traslate/rotate it to the new orientation.

  Orientation savedOr = *(pCrate->Takeable::GetOrientation());
  State savedState = pCrate->Takeable::GetState();
  pCrate->Takeable::SetState(UNKNOWN); // to get collision vol

  Assert(pCrate->Takeable::GetCollisionVol());
  BoundingBox bb = pCrate->Takeable::GetCollisionVol()->GetBoundingBox();
  // If we can transform the bounding box directly, we don't have to 
  // do the expensive SetOrientation on the piece.
  // Rotate by the y-rotation diff.
  // But we want to rotate about the bounding box centre, not the world origin.
  // So translate to origin.

  BoundingBoxTransform(&bb, savedOr.GetVertex(), o.GetVertex(), savedOr.GetYRot(), o.GetYRot());

  bool result = pVgo->GetCubeMap().Intersects(bb);
  pCrate->Takeable::SetState(savedState); // restore real piece state

/*
#if defined(_DEBUG)
  Assert(result == correctResult);
#endif
*/
  return result;
}

Finder::RankedMoveList Finder::GetAllPossibleMoves(VisibilityGraphWithObjects* pVgo)
{
  RankedMoveList results;

  // Get all pieces to consider for the AI.
  PieceList pieces = GetPiecesToConsider();

  // Get valid moves for each piece
  for (PieceList::iterator it = pieces.begin(); it != pieces.end(); ++it)
  {
    RankedMoveList rml = GetPossibleNeighboursForPiece(it->GetPtr(), pVgo);
    std::copy(rml.begin(), rml.end(), std::back_inserter(results));
  }

  // It is also possible to remove any of the pieces under consideration - 
  // except ones that support other pieces, or are unreachable.
  // TODO add 'remove a piece' - type moves to the list

  // Remove duplicates - we must first sort the vector of moves
  //  to get rid of _all_ duplicates. (If we don't sort we just
  //  get rid of consecutive duplicates!)
  // The 'sameness' criterion is the piece type + orientation.
  SameSorter sorter; 
  std::sort(results.begin(), results.end(), sorter);
  results.erase(std::unique(results.begin(), results.end()), results.end());

  return results;
}

float Finder::Evaluate(const RankedMove& rm, VisibilityGraphWithObjects* pVgo)
{
  // Apply each 'evaluator' to the move. 
  // Each has a weighting.
 
  float total = 0;
  bool pending = false;

  for (EvaluatorList::iterator it = m_evaluators.begin(); it != m_evaluators.end(); ++it)
  {
    Evaluator* pEvaluator = it->GetPtr();
    float score = 0;
    Evaluator::EvalResult evalResult = pEvaluator->Score(rm, m_pAi, pVgo, &score);

    if (evalResult == Evaluator::EvalOk)
    {
//      Assert(score >= -1.0f);
//      Assert(score <= 1.0f);
      float weight = pEvaluator->GetWeight();
      float weighted = score * weight;
      total += weighted;

#if defined(_DEBUG)
      // For debugging, store the individual components of the score.
      if (true) //(weighted != 0)
      {
        const_cast<RankedMove&>(rm).SetEvaluatorScore(pEvaluator->GetName(), weighted);
      }
#endif
    }
    else if (evalResult == Evaluator::EvalPending)
    {
      // Not all evaluators have finished yet.
      // TODO handle this!
      pending = true;
    }  
  }
  return total;
}

// Sort ranked moves
struct RankSorter
{
bool operator() (const RankedMove& lhs, const RankedMove& rhs)
{
  // We want the highest rank to go first in the list.
  return lhs.m_rank > rhs.m_rank;
}
};

void Finder::SortList(
  Finder::RankedMoveList* pMoveList, 
  VisibilityGraphWithObjects* pVgo)
{
  // Reset all evaluators.
  // This is done once per evaluator before evaluating a set of positions.
  // It _Isn't_ called before evaluating every move.
  for (EvaluatorList::iterator jt = m_evaluators.begin(); jt != m_evaluators.end(); ++jt)
  {
    Evaluator* pEvaluator = jt->GetPtr();
    pEvaluator->Reset(this);
  }

  // Rank each move. Then sort the list, with the ranking as the sort criterion.
  for (RankedMoveList::iterator it = pMoveList->begin(); it != pMoveList->end(); ++it)
  {
    RankedMove& rm = *it;
    float score = Evaluate(rm, pVgo);
    rm.m_rank = score;
  }

  RankSorter sorter;
  std::sort(pMoveList->begin(), pMoveList->end(), sorter);
}

bool Finder::GetMove(
  Orientation* pResultOr, 
  TakeableCrate** ppResultPiece,
  VisibilityGraphWithObjects* pVgo)
{
#if defined(_DEBUG)
  cout << "Finder::GetMove()\n";
#endif

  RankedMoveList movelist = GetAllPossibleMoves(pVgo);

  // Add the Seed moves even the list is non-empty, as a seed
  // move may get the best score.
  RankedMoveList seedmovelist = GetAllPossibleSeedMoves(pVgo);

  std::copy(seedmovelist.begin(), seedmovelist.end(), back_inserter(movelist));

  if (movelist.empty())
  {
    return false;
  }

  // Sort moves, so best is first, next best is second, etc.
  SortList(&movelist, pVgo);

  // Look-ahead: test moves to see if a complete path to the goal would
  // exist. Any winning move is placed at the beginning of the movelist.
  //bool winMove = LookAhead(&movelist, pVgo);

#if defined(_DEBUG)
  /*
  int topn = atoi(Engine::Instance()->GetConfigValue("topn").c_str());

  cout << "Finder results: number of moves: " << movelist.size() << endl;
  
  for (int i = 0; i < movelist.size() && (topn?(i < topn):true); i++)
  {
    const RankedMove& rm = movelist[i];
    cout 
      << "Move " << i 
      << " piece ID: " << rm.m_pCrate->GetId() 
      << " (type " << rm.m_pieceType.c_str() 
      << ")  move to: (" 
      << rm.m_or.GetX() << ", " 
      << rm.m_or.GetY() << ", "
      << rm.m_or.GetZ() << ") "
      << " Score: " << rm.m_rank << endl;

    rm.Print(cout);
  }
  */
#endif

  // Get first move TODO
  // Check that the best move is reachable. If not, go to the next one.
  // Reachable means the AI can reach the piece, and can then get to the
  // destination.
  for (RankedMoveList::iterator it = movelist.begin(); it != movelist.end(); ++it)
  {
    RankedMove& rm = *it;

#if defined(_DEBUG)
    /*
    cout 
      << "  checking move (" << rm.m_pCrate->GetId()
      << " to "
      << rm.m_or.GetX() << ", " 
      << rm.m_or.GetY() << ", "
      << rm.m_or.GetZ() << ")\n";
    */
#endif

    if (true) ///// TODO TEMP TEST  IsReachable(rm))
    {
#if defined(_DEBUG)
      
      cout 
        << "FINDER HEIGHT: " << m_height
        << " Best move: " 
        << " piece ID: " << rm.m_pCrate->Takeable::GetId() 
        << " (" << rm.m_pieceType.c_str() 
        << ")  move to: (" 
        << rm.m_or.GetX() << ", " 
        << rm.m_or.GetY() << ", "
        << rm.m_or.GetZ() << ") "
        << "\nScore: " << rm.m_rank << endl;
      rm.Print(cout);
      
#endif
      *pResultOr = rm.m_or;
      *ppResultPiece = rm.m_pCrate;
      m_pLastPieceChosen = rm.m_pCrate;
      return true;
    }
    else
    {
#if defined(_DEBUG)
      cout << "One move found to be unreachable.. trying next one..\n";
#endif
    }
  }
  // None of the moves in the list are reachable!

#if defined(_DEBUG)
  cout << "No moves are reachable, Finder has FAILED!\n";
#endif
  return false;
}

Finder::RankedMoveList Finder::GetAllPossibleSeedMoves(
  VisibilityGraphWithObjects* pVgo)
{
  RankedMoveList results;

  // Try to place a suitable seed piece.
  // For each seed piece type
  for (PieceTypes::iterator it = m_seedPieceTypes.begin(); it != m_seedPieceTypes.end(); ++it)
  {
    PieceType pt = *it;

    // Get available pieces of the chosen type
    PieceList pieces = GetAvailablePiecesOfType(pt, pVgo);

    // Get valid moves for each piece
    for (PieceList::iterator it = pieces.begin(); it != pieces.end(); ++it)
    {
      RankedMoveList rml = GetPossibleSeedPositions(it->GetPtr(), pVgo);
      std::copy(rml.begin(), rml.end(), std::back_inserter(results));
    }
  }
  return results;
}

Finder::RankedMoveList Finder::GetPossibleSeedPositions(
  TakeableCrate* pCrate, 
  VisibilityGraphWithObjects* pVgo)
{
  static const float PIECE_GRID = 2.0f; // TODO CONFIG piece grid size

  RankedMoveList results;

  // TODO
  Assert(m_pAi);
  Assert(m_pAi->GetCurrentGoal().GetPtr());
  Assert(m_pAi->GetCurrentGoal()->GetBoundingSphere());

  const VertexBase& goalPos = 
    m_pAi->GetCurrentGoal()->GetBoundingSphere()->GetCentre();

  // Choose a position: 
  // Ideal location is AI target, so move outwards from there.
  // Height should be lower, but not too low - check for ledges.
  // Location must be within GoalRadius!
  // The space for the piece must be empty - use Cube Map

  Orientation left(-1.0f, 0, 0, 0, 0, 0);
  Orientation right(1.0f, 0, 0, 0, 0, 0);
  Orientation fwd(0, 0, 1.0f, 0, 0, 0);
  Orientation back(0, 0, -1.0f, 0, 0, 0);

  GetSeedPosOneDirection(pCrate, left, &results, goalPos, pVgo);
  GetSeedPosOneDirection(pCrate, right, &results, goalPos, pVgo);
  GetSeedPosOneDirection(pCrate, fwd, &results, goalPos, pVgo);
  GetSeedPosOneDirection(pCrate, back, &results, goalPos, pVgo);

  VertexBase goalPos1(goalPos);
  goalPos1.x += PIECE_GRID;
  goalPos1.z += PIECE_GRID;

  GetSeedPosOneDirection(pCrate, right, &results, goalPos1, pVgo);
  GetSeedPosOneDirection(pCrate, fwd, &results, goalPos1, pVgo);

  VertexBase goalPos2(goalPos);
  goalPos2.x -= PIECE_GRID;
  goalPos2.z += PIECE_GRID;

  GetSeedPosOneDirection(pCrate, left, &results, goalPos2, pVgo);
  GetSeedPosOneDirection(pCrate, fwd, &results, goalPos2, pVgo);

  VertexBase goalPos3(goalPos);
  goalPos3.x -= PIECE_GRID;
  goalPos3.z -= PIECE_GRID;

  GetSeedPosOneDirection(pCrate, left, &results, goalPos3, pVgo);
  GetSeedPosOneDirection(pCrate, back, &results, goalPos3, pVgo);

  VertexBase goalPos4(goalPos);
  goalPos4.x += PIECE_GRID;
  goalPos4.z -= PIECE_GRID;

  GetSeedPosOneDirection(pCrate, right, &results, goalPos4, pVgo);
  GetSeedPosOneDirection(pCrate, back, &results, goalPos4, pVgo);
  
  return results;
}

bool Finder::GetSeedPosOneDirection(
  TakeableCrate* pCrate,
  const Orientation& step,
  RankedMoveList* pResults,
  const VertexBase& goalPos,
  VisibilityGraphWithObjects* pVgo)
{
  float radius = GetGoalRadius(); 
  BoundingSphere bs(goalPos, radius); 

  // Why subtract 3 from height ?
  // We subtract 1 becuase goal targets are at height 1 w.r.t. piece heights.
  // Then we want to place the piece below the goal, not at the same level,
  // so subtract 2 more.
  Orientation o(goalPos.x, goalPos.y - 3.0f, goalPos.z, 0, 0, 0);

  int count = 0;

  while (count < 4)
  {
    Move move;
    move.m_pieceType = GetTypeForPiece(pCrate);
    // Find the floor height.
    // Check if piece intersects something. 
    // If not, we have found the position closest to the target in the current
    // direction.

    // Try each way the piece can be rotated.
    for (int i = 0; i < 4; i++) // 4 right angles
    {
      // Copy o, recrease height until floor is found.
      Orientation o1 = o;
      o1.SetYRot(i * 90.0f);
      //o1.SetY(0); // TODO TEMP TEST
      // Get floor height. Decrement height until piece is supported by something.
      while (!PieceWouldBeSupported(pVgo, pCrate, o1))
      {
        o1.SetY(o1.GetY() - 1.0f);
      }

      bool collides = PieceWouldIntersectCubeMap(pVgo, pCrate, o1);
      if (!collides)
      {
        bool validMove = true; // TODO more tests ?
        if (validMove)
        {
          move.m_or = o1;
          RankedMove rm(move);
          rm.m_pCrate = pCrate;

#if defined(_DEBUG)
          //cout << "Seed move: " << rm << endl;
#endif

          pResults->push_back(rm);

          count++;
        }
        // TODO check that position is within goal radius. 
      }
    }
    o += step;
  }
  return true;
}

bool Finder::IsReachable(const RankedMove& rm)
{
#if defined(_DEBUG)
  // Disable 'reachable' test if we don't want it for debugging
  if (Engine::Instance()->GetConfigValue("finder_always_reachable") == "y")
  {
    return true;
  }
#endif

  // Check that the piece can be reached from the current AI pos.
  Assert(m_pAi);

  PGameObject pTemp = rm.m_pCrate; 
  State tempState = pTemp->GetState();
  // Temporarily disable the piece, so it won't obstruct the path to its
  // own centre.
  pTemp->SetState(OUT_OF_PLAY);

  // TODO shared path finder object ?
  AStarPathFinder pf(EngineRunningVisGraph::GetVisibilityGraph());
  VertexBase v1 = m_pAi->GetPosition();
  v1.y += 1.0f; // increase height from on ground to same height as piece
                // vis points.
  VertexBase v2 = rm.m_pCrate->GetPosition(); 
  v2.y += 1.0f;
  SearchResult sr = pf.FindPath(v1, v2);

  // State of piece should be UNKNOWN already.. if it isn't, Update() will fail.
  
  if (sr == SearchFail)
  {
    pTemp->SetState(tempState);
    return false;
  }

  // Now check that piece could be moved to the target location.

  // Change v1 to destination pos and search the other way.
  v1 = rm.m_or.GetVertex();
  v1.y += 1.0f; // increase height from on ground to same height as piece
                // vis points.

  AStarPathFinder pf2(EngineRunningVisGraph::GetVisibilityGraph());

  sr = pf2.FindPath(v2, v1); // deliberately the other way round

  pTemp->SetState(tempState);

  if (sr == SearchFail)
  {
    return false;
  }
  
  return true; 
}

bool Finder::LookAhead(RankedMoveList* pMovelist, VisibilityGraphWithObjects* pVgo)
{
  // TODO rules for when look ahead should be used. E.g. after a certain no of
  // pieces have been placed.

  std::cout << "STARTING LOOK AHEAD (" << pMovelist->size() << " moves)\n";

  for (RankedMoveList::iterator it = pMovelist->begin(); it != pMovelist->end(); ++it)
  {
    std::cout << ".";

    // Make the move, by setting the Orientation of the piece.
    // Work out what the character position would be - i.e. just above the 
    // piece coord.
    // Do a path find from the character pos to the goal.
    // Replace the piece in its current position.
    RankedMove& rm = *it;

    TakeableCrate* pCrate = rm.m_pCrate;
    Orientation savedOr = *(pCrate->Takeable::GetOrientation());
    Orientation o = rm.m_or;
    pCrate->SetOrientation(o);

    //VertexBase charPos = o.GetVertex();
    //o.SetY(o.GetY() + 2.0f);
    VertexBase charPos(0, 1.0f, 0); // TODO TEMP TEST

    VertexBase goalPos = m_pAi->GetCurrentGoal()->GetBoundingSphere()->GetCentre();
    AStarPathFinder pf(pVgo);

    bool pathExists = (pf.FindPath(charPos, goalPos) == SearchOk);

    pCrate->SetOrientation(savedOr);

    if (pathExists)
    {
      // Move this move to the front of the movelist.

      std::cout << "GOOD LORD! LOOK AHEAD FOUND A WINNING MOVE!!!\n";

      return true;
    }
  }

  std::cout << "LOOK AHEAD FINISHED\n";

  return false;
}

}
