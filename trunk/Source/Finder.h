/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Finder.h,v $
Revision 1.1  2004/09/08 15:43:03  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_FINDER_H_INCLUDED)
#define SCHMICKEN_FINDER_H_INCLUDED

#include <map>
#include <string>
#include <vector>
#include "Orientation.h"
#include "Evaluator.h"
#include "SharedPtr.h"

namespace Amju
{
class TakeableCrate;
class Takeable;
class Ai;
class VisibilityGraphWithObjects;
class File;
class BoundingBox;

typedef std::string PieceType;

struct Move
{ 
  //Move(const PieceType& pt, const Orientation& o) : m_pieceType(pt), m_or(o) {}

  PieceType m_pieceType; // ..we can place this kind of piece..
  Orientation m_or; // ..at this relative position

  bool Load(File*);
};

struct RankedMove : public Move
{
  RankedMove(const Move& m);
  float m_rank;
  TakeableCrate* m_pCrate;

  // Required for unique()
  bool operator==(const RankedMove&) const;

#if defined(_DEBUG)
  friend std::ostream& operator<<(std::ostream&, const RankedMove&);

    // For debugging, store the individual components of the score.
  void SetEvaluatorScore(const std::string& evaluatorName, float score);
  void Print(std::ostream&) const;
  typedef std::map<std::string, float> ScoreMap;
  ScoreMap m_scoreMap;
#endif
};

class Finder
{
public:
// Find the best move.
// A move is a piece ID and a destination orientation.

  typedef std::vector<Move> MoveList;
  typedef std::vector<RankedMove> RankedMoveList;
  typedef std::vector<SharedPtr<TakeableCrate> > PieceList;
  typedef std::vector<SharedPtr<Evaluator> > EvaluatorList;

public:
  // This helpful utility function rotates and translates a bounding box.
  static void BoundingBoxTransform(
    BoundingBox* pBb,  
    const VertexBase& posBefore,
    const VertexBase& posAfter,
    float yRotBefore,
    float yRotAfter);

public:
  Finder();

  // Choose the next move. This is a piece ID/ptr and target orientation.
  // Use the sorted possible move list to decide.
  // Usually the best move is the first move in the list, but it may be
  // possible to combine two moves, i.e. a 'place' and a 'remove'.
  // TODO Signature
  //void SelectMove(const RankedMoveList&);

  bool GetMove(
    Orientation* pResultOr, 
    TakeableCrate** ppResultPiece,
    VisibilityGraphWithObjects* pVgo);

  /*
    Sort the moves using heuristics. 
    Each rule is an object ?
    The rules have weights. Diff. AI types can use diff. weights.
     - Height: greater than target height is no good. Same level is best, then lower score
        for lower heights. (But building up base should take precedence)
     - number of connected (touching) blocks wrt height. This is to get a larger base
        than top. Once base is a certain size, increasing it further gets a lower score.
     - Mobility: i.e. reachable surface ? I.e. does the move increase traversable area ?

     - Only one ramp per height per connected bunch of blocks
     - rule to encourage AI to place ramp last, i.e. increase x-z size using other blocks
       first.
     - Place blocks far away from the ramp on the lower height
     - select for/against straightness
     - select for/against using particular block types, e.g. colours

     - remove a piece blocking a ramp or above a ramp
     - remove a piece blocking a spare space further from the ramp on lower level.

     NB score each move, then call std::sort. Don't evaluate each move in operator<() !!!
  */
  void SortList(RankedMoveList* pMovelist, VisibilityGraphWithObjects* pVgo);

  // Get all moves for the AI. To do this:
  // - get all pieces to consider
  // - get movelist for each piece. Prune illegal moves from each list.
  // - merge the lists
  // - remove duplicates in the list!
  // - sort the list.
  // But there are more moves, namely to remove every piece under consideration.
  // (We don't care where they go). Only add this kind of move where a piece 
  //  - doesn't support anything
  //  - is reachable
  //  - is not Immovable
  RankedMoveList GetAllPossibleMoves(VisibilityGraphWithObjects* pVgo);

  // For the given piece, get the pieces which could be placed next to it.
  // List is pruned of illegal moves, using cubemap to reject piece/position pairs
  // which would intersect something.
  // TODO for recursive search, also supply vis graph/cube map.
  // (No need to supply scene as this is static.)
  // The orientation of each Move is absolute, i.e. has been adjusted to fit
  //  the current location of the piece under consideration.
  // NB For recursive search, don't copy CubeMap and pass by value - do/undo
  //  boxes. CubeMap should have Undo stack.
  RankedMoveList GetPossibleNeighboursForPiece(
    TakeableCrate* pCrate, 
    VisibilityGraphWithObjects* pVgo);

  // Get the pieces for which it's worth constructing a MoveList.
  // E.g. We don't need to consider pieces we can't reach, or we just moved, etc.
  PieceList GetPiecesToConsider();

  // Called to get a sensible start move when there is nothing to build on.
  RankedMoveList GetPossibleSeedPositions(
    TakeableCrate* pCrate, 
    VisibilityGraphWithObjects* pVgo);

  // Returns true if move is reachable from the AI's position, and the 
  // destination.
  bool IsReachable(const RankedMove& rm);


  bool Load(File*);

  void SetAi(Ai*);

  Ai* GetAi();

  static float GetGoalRadius();
 
  void SetHeight(int h);
 
protected:

  // Look ahead to spot any winnng moves (i.e. would create a full path to 
  //  the current goal)
  bool LookAhead(RankedMoveList* pMovelist, VisibilityGraphWithObjects* pVgo);

  // Load up list of possible pieces to put next to other pieces.
  bool LoadMoves(File*);
  
  bool LoadEvaluators(File*);

  bool LoadSeeds(File* pf);


  PieceType GetTypeForPiece(TakeableCrate* p);

  // Return true if the given crate, _moved to the given orientation_,
  // would intersect something in the cube map.
  bool PieceWouldIntersectCubeMap(
    VisibilityGraphWithObjects* pVgo, 
    TakeableCrate* pCrate, 
    Orientation o);

  // Returns true if the given crate, moved to the given orientation,
  // would be supported by something - i.e. the scenery or another crate.
  bool PieceWouldBeSupported(
    VisibilityGraphWithObjects* pVgo, 
    TakeableCrate* pCrate, 
    Orientation o);

  // Returns true if there is something (i.e. a piece) above the
  // given crate.
  bool PieceSupportsOthers(
    VisibilityGraphWithObjects* pVgo, 
    TakeableCrate* pCrate);

  Orientation GetOrientationForPiece(TakeableCrate* p);

  float Evaluate(const RankedMove& rm, VisibilityGraphWithObjects* pVgo);

  // Get all pieces of type pt. We use the cubemap to find if any are holding 
  // up other pieces - if so they are unavailable.
  PieceList GetAvailablePiecesOfType(
    PieceType pt, 
    VisibilityGraphWithObjects* pVgo);

  RankedMoveList GetAllPossibleSeedMoves(VisibilityGraphWithObjects* pVgo);

  // Return true if there is a piece of the given type at the given
  // orientation.
  bool IsPieceTypeAt(
    PieceType pt, 
    const Orientation& o, 
    VisibilityGraphWithObjects* pVgo);

  bool GetSeedPosOneDirection(
    TakeableCrate* pCrate,
    const Orientation& step,
    RankedMoveList* pResults,
    const VertexBase& goalPos,
    VisibilityGraphWithObjects* pVgo);

protected:
  // Represent possible moves in a way which makes it easy to create MoveLists.
  // Map each piece type to possible moves.
  // TODO this could just as well be a map of piece type to vector, say.
  // Each move in this list of possibles has an orientation relative to the
  // origin of the PieceType. I.e. the Piece being considered is at (0, 0, 0)
 
  struct PossibleMove
  {
    struct PossMoveItem
    {
      PossMoveItem(const Orientation& o, const PieceType& pt) : 
        m_or(o), m_pieceType(pt) {}

      Orientation m_or;
      PieceType m_pieceType;
    };
    typedef std::vector<PossMoveItem> PossMoveItemList;
    PossMoveItemList m_itemList;
    Move m_response;
  };

  typedef std::vector<PossibleMove> PossibleMoves;

  PossibleMoves  m_possibleMoves;

  // The AI for which we are finding the best move.
  Ai* m_pAi;

  EvaluatorList m_evaluators;

  // Remember the last thing we selected.
  Takeable* m_pLastPieceChosen;

  // The types of pieces which may be used as Seeds.
  typedef std::vector<PieceType> PieceTypes;
  PieceTypes m_seedPieceTypes;

  // Relative Height of goal for which this finder applies.
  int m_height;
};
}
#endif