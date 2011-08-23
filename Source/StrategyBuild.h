/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: StrategyBuild.h,v $
Revision 1.1  2004/09/08 15:43:03  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_STRATEGY_BUILD_H_INCLUDED)
#define SCHMICKEN_STRATEGY_BUILD_H_INCLUDED

#include <set>
#include <vector>
#include "Strategy.h"
#include "Building.h"
#include "GameObject.h"

namespace Amju
{
class File;
class TakeableCrate;

class StrategyBuild : public Strategy
{
public:
  StrategyBuild(Strategizer* pS);

  virtual bool IsValid() const;
  virtual void OnActivate();
  virtual void OnTaskFinished();
  virtual void OnTaskFailed();
  virtual bool Load(File*); // Load any data required by the Strategy.
  virtual void Reset();
  virtual void RecalculatePath();

  bool IsPieceUsed(int id) const;

protected:
  TakeableCrate* FindPiece(const std::string& pieceType);

  // Decide what kind of building to make, and where to make it.
  void DecideNewBuilding();

  // Age-out or otherwise decide to not care about some old buildings any more.
  // I.e. delete selected buildings from m_finishedBuildings.
  // Any buildings deleted from m_finishedBuildings can be cannibalized for 
  // pieces.
  void DeleteOldBuildings();

protected:
  // Vector of buildings which this Strategy will try to build.
  typedef std::vector<PBuilding> Buildings;
  Buildings m_buildings;

  // Origin of current building
  Orientation m_origin;
  // Index into m_buildings of the current building.
  int m_index;
  // Piece number of the building we are making.
  int m_pieceNumber;

  // Keep track of pieces we have moved to make a building.
  // TODO associate each piece with a particular building.
  typedef std::set<int> UsedPieceSet;
  // Too basic - can't age-out old buildings
  //UsedPieceSet m_usedPieces;

  // Pieces used in the building we are currently making.
  UsedPieceSet m_currentBuildingPieces; 

  // If true, the current building we are trying to build is still good.
  // So we know what to do next: get the next piece of the building.
  bool m_isLastGood;

  // Keep track of the buildings we have made up till now.
  // This lets us age-out the oldest buildings. 
  struct FinishedBuilding
  {
    int m_buildingIndex;
    Orientation m_origin;
    UsedPieceSet m_pieces;
  };
  typedef std::vector<FinishedBuilding> FinishedBuildings;
  FinishedBuildings m_finishedBuildings;

  // Predicate for sorting vector of Crates by distance from AI.
  struct SortPred
  {
    SortPred(const VertexBase& v) : m_v(v) {}
    // Sort two crates based on both their distances from m_v.
    // If lhs is closer, returns true.
    bool operator() (TakeableCrate* lhs, TakeableCrate* rhs) const;

  private:
    VertexBase m_v;
  };
};
}

#endif
