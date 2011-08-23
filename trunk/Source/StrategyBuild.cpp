/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: StrategyBuild.cpp,v $
Revision 1.1  2004/09/08 15:43:03  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <algorithm>
#include "StrategyBuild.h"
#include "File.h"
#include "Ai.h"
#include "Engine.h"
#include "TakeableCrate.h"
#include "Level.h"
#include "EngineRunning.h"
#include "AiNotifier.h"

namespace Amju
{
StrategyBuild::StrategyBuild(Strategizer* pS) : Strategy(pS)
{
  m_index = 0;
  m_pieceNumber = 0;
  m_origin.SetZ(-50.0f);  // TODO TEMP TEST
  m_origin.SetX(40.0f);

  m_isLastGood = false;
}

bool StrategyBuild::IsValid() const
{
  // TODO
  return true;
}

bool StrategyBuild::Load(File* pf)
{
  // Load any data required by the Strategy.
  // In this case we want to load all valid Buildings.
  // Get number of buildings
  int numBuildings = 0;
  if (!pf->GetInteger(&numBuildings))
  {
    pf->ReportError("Expected number of buildings.");
    return false;
  }
  // Load each building
  for (int i = 0; i < numBuildings; i++)
  {
    PBuilding pB = new Building;
    if (!pB->Load(pf))
    {
      pf->ReportError("Failed to load building.");
      return false;
    }

    m_buildings.push_back(pB);
  }

  return true;
}

void StrategyBuild::DeleteOldBuildings()
{
  // Delete buildings we never walk on ?
  // Age-out buildings when we have made a certain number of them.
}

bool StrategyBuild::IsPieceUsed(int id) const
{
  if (m_currentBuildingPieces.find(id) != m_currentBuildingPieces.end())
  {
    return true;
  }
  // Now check old buildings we have made
  for (FinishedBuildings::const_iterator it = m_finishedBuildings.begin(); 
       it != m_finishedBuildings.end(); 
       ++it)
  {
    if (it->m_pieces.find(id) != it->m_pieces.end())
    {
      return true;
    }
  }
  return false;
}

void StrategyBuild::Reset()
{
  m_finishedBuildings.clear();
  m_isLastGood = false;
}

// SortPred is used by FindPiece() below to sort crates by distance.
bool StrategyBuild::SortPred::operator() (
  TakeableCrate* lhs, 
  TakeableCrate* rhs) const
{
  float distSq1 = SquareDist(lhs->Takeable::GetBoundingSphere()->GetCentre(), m_v);
  float distSq2 = SquareDist(rhs->Takeable::GetBoundingSphere()->GetCentre(), m_v);
  return (distSq1 < distSq2);
}

bool IsUsed(const TakeableCrate* p)
{
/*
  const SingleAiNotifier::Ais ais = AiNotifier::Instance()->GetAllAis();
  for (SingleAiNotifier::Ais::const_iterator it = ais.begin(); it != ais.end(); ++it)
  {
    if (p->GetId() == (*it)->GetPieceId())
    {
      // Piece is used by the AI (*it).
      return true;
    }
  }
  // Piece is not used by any AI.
*/
  return false;
}

TakeableCrate* StrategyBuild::FindPiece(const std::string& pieceType)
{
  // Find a piece that matches the type string and is unused.
  // A used piece is one that has another piece on it or was moved somewhere
  // by us. We don't care if someone else put it somewhere - in fact it
  // will be funny to demolish their building! Har har!
  // Well, not har har if AIs fight over a piece. So don't use a piece which is
  // part of the current building of any AI.
  // TODO This will be covered by 'cooperation groups'.

  int levelId = m_pAi->GetLevel()->GetId();
  int roomId =  m_pAi->GetLevel()->GetRoomId();

  // Get the game objects which are in the same room as the AI.
  GameObjectMap& objs = Engine::Instance()->GetGameObjects(levelId, roomId);

  // Vector of candidates - we will sort to get the best one.
  typedef std::vector<TakeableCrate*> Crates;
  Crates crates;

  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    GameObjectId gameObjId = it->first;
    PGameObject pGo = it->second;
    TakeableCrate* pTc = dynamic_cast<TakeableCrate*>(pGo.GetPtr());

    if (pTc && 
        pTc->Takeable::GetState() != TAKEN &&
        pTc->GetType() == pieceType &&
        !IsPieceUsed(pTc->Takeable::GetId()) &&
        !EngineRunningVisGraph::PieceSupportsOthers(pTc, m_pAi->GetLevel()))
    {
      // TODO other tests ?
      // TODO cache piece types ?
      crates.push_back(pTc);
    }
  }

  if (crates.empty())
  {
    return 0;
  }

  // Sort the Crates by distance from the AI.
  // This is not very accurate, it should really use path length.
  SortPred sortPred(m_pAi->GetPosition());
  std::sort(crates.begin(), crates.end(), sortPred);

  // Pick the first crate which isn't in use by another AI 
  // (we know we're not using it).
  // If all are used by AIs, we will just have to take it anyway.
  for (Crates::iterator jt = crates.begin(); jt != crates.end(); ++jt)
  {
    // TODO TEMP TEST
    TakeableCrate* pCrate = *jt;
    float distSq = SquareDist(
      pCrate->Takeable::GetBoundingSphere()->GetCentre(), 
      m_pAi->GetPosition());

    // Is this crate used by an AI ?
    if (!IsUsed(*jt))
    {
      // Not used, so this one will do!
      return *jt;
    }

  }

  // No crates are unused. 
  // TODO don't pick the first one, it's probably used by this AI ???
  return crates[0];
}

void StrategyBuild::DecideNewBuilding()
{
  // The thing we were building can't be finished for some reason. We want to 
  // find something else to build. We don't have to start from scratch though, 
  // as there may be a nearly-finished building we can finish to reach our goal.
  // If there's nothing suitable, start a new building.

  // NB A "nearly finished" building may have _extra_ pieces on it, not just
  // missing pieces. 

  // Look for a suitable unfinished building. The best choice is the
  // building that will be finished with fewest new pieces, and leads
  // to the next Home.
  // Best way to frig this would be "Building Seed" game objects. Each
  // Seed has a list of Building IDs, and a Home ID. This would mean
  // "you can build buildings with these IDs using this point as the
  //  origin, and if you build the whole thing you'll be able to reach
  //  the Home with this ID."
  // This would be much better than a horrible search, but maybe the
  // search is quick enough to not need Seeds.

  // Even with the seeds we have to do a bunch of searching:
  //  - What Home are we aiming for ?
  //    - answer is the lowest unreachable one.
  // This is m_pGoal.

  //  - What seeds lead to the above Home ?
  //  - Of these seeds, which ones are most complete,
  //    and have the pieces available ?
  //   (This last one is TWO searches - which one should be first ?)
  //  Maybe Home objects should have a list of seed origins and building IDs ?
  //  No - it depends on the scenery _around_ the Home.

  // Set position of new building.
  // (We expect all rotation values to be zero)
  float r1 = (float)rand() / (float)RAND_MAX;
  float x = (int)(r1 * 5.0f) * 10;
  float r2 = (float)rand() / (float)RAND_MAX;
  float z = (int)(r2 * 5.0f) * 10;

  m_origin = Orientation(x, 0, z, 0, 0, 0);

  // Set building index in our list of buildings.
  m_index = 1; // TODO

  // IF we are starting a new building, reset piece index. 
  // If we are working on an incomplete building, find the last correctly-
  // placed piece to get the next piece index.
  m_pieceNumber = 0; // because we are starting a new building.

}

void StrategyBuild::OnActivate()
{
  // Is current/last building still good ?
  if (!m_isLastGood)
  {
    // Get new building index, piece index and origin.
    DecideNewBuilding();
  }

  Assert(m_index < m_buildings.size());

  PBuilding& pCurrentBuilding = m_buildings[m_index];
  Assert(pCurrentBuilding.GetPtr());
  Assert(pCurrentBuilding->GetNumPieces() > m_pieceNumber);
  std::string pieceType = pCurrentBuilding->GetPiece(m_pieceNumber).m_type;

  // Find an unused piece of the right type.
  TakeableCrate* pCrate = FindPiece(pieceType);
  if (!pCrate)
  {
    // Can't find a piece.
    Assert(0);
  }

  m_pAi->SetPieceId(pCrate->Takeable::GetId()); 

  // Set up the position we want the piece to be in. This is the building's
  // orientation plus the current piece's relative orientation within the 
  // building.
  Orientation piecePos(m_origin);
  piecePos += pCurrentBuilding->GetPiece(m_pieceNumber).m_or;
  // Set Y-value so pathfinders can get to it
  piecePos.SetY(piecePos.GetY() + 1.0f);

  m_pAi->SetPiecePos(piecePos.GetVertex());
  m_pAi->SetPieceYRot(piecePos.GetYRot()); 

  m_pAi->SetState(UNKNOWN);  // Hmm, a bit hacky :-(

  // First job is jump down from a piece if we just placed one,
  // (although not if there's a path from the top of the piece.)
  //m_pAi->SetState(AI_COLLECT);
  m_pAi->SetState(AI_JUMP_DOWN);

}

void StrategyBuild::OnTaskFinished()
{
  // The current task for this strategy was successfully completed.

  int currentPieceId = m_pAi->GetPieceId();
  // Remember the current piece has been successfully placed by us.
  m_currentBuildingPieces.insert(currentPieceId);
  m_isLastGood = true; // it's ok to continue with the current building

  m_pieceNumber++;

  // Is the building finished ? 
  PBuilding& pCurrentBuilding = m_buildings[m_index];
  Assert(pCurrentBuilding.GetPtr());
  Assert(m_pieceNumber <= pCurrentBuilding->GetNumPieces());
  if (m_pieceNumber == pCurrentBuilding->GetNumPieces())
  {
    // Building is finished. 
    // Seed for the building should be marked as used.
    // Reset any cached info about the current building.
    FinishedBuilding fb; // TODO  on heap to avoid set copying ?
    fb.m_buildingIndex = m_index;
    fb.m_origin = m_origin;
    fb.m_pieces = m_currentBuildingPieces;
    m_finishedBuildings.push_back(fb);

    m_currentBuildingPieces.clear();
    m_isLastGood = false; // Current building finished, so is no good anymore.
  }

  // This call is needed to choose the next piece to collect.
  // It won't be called by the Strategizer because the current Strategy
  // will not have changed.
  //OnActivate(); 
}

void StrategyBuild::OnTaskFailed()
{
  // The current task could not be completed. A new strategy will be
  // chosen - although the new one may be this one again! 
  // Do any clearing up necessary.
  // Don't cache info about the building we were working on, as things have
  // obviously gone pear-shaped.

  m_currentBuildingPieces.clear();

  m_isLastGood = false;

  // If this Strategy is re-chosen the Strategizer will not call this function,
  // so we must do it here.
  //OnActivate(); 
}

void StrategyBuild::RecalculatePath()
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
/*
  if (m_pAi->GetState() == AI_COLLECT)
  {
    m_pAi->SetState(UNKNOWN);
    m_pAi->SetState(AI_COLLECT);
  }

  if (m_pAi->GetState() == AI_TAKE)
  {
    m_pAi->SetState(UNKNOWN);
    m_pAi->SetState(AI_TAKE);
  }

  if (m_pAi->GetState() == AI_DUMP)
  {
    m_pAi->SetState(UNKNOWN);
    m_pAi->SetState(AI_DUMP);
  }
*/
}

}
