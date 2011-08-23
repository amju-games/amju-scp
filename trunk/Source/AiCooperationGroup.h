/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: AiCooperationGroup.h,v $
Revision 1.1  2004/09/08 15:42:33  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_AI_COOPERATION_GROUP_H_INCLUDED)
#define SCHMICKEN_AI_COOPERATION_GROUP_H_INCLUDED

#include <deque>
#include <vector>
#include "SharedPtr.h"

namespace Amju
{
class PieceMover;
class Takeable;

// Piece Movers which are cooperating with each other belong to one of these groups.
// Members of a group share info about the current building under construction,
// and finished buildings.
class CooperationGroup : public Shareable
{
public:
  void Join(PieceMover*);

  // Returns true if an AI in this cooperation group last placed the piece.
  bool IsPieceUsed(int id) const;

  // Get the order in which the pieces were used. If the piece was last used,
  // this fn returns 0. If one piece was used after this one, 1 is returned;
  // if 2 pieces have been used since this one, 2 is returned... etc.
  int GetPieceOrder(int id) const;

  void SetPieceUsed(int id, bool inUse = true);

  void Clear();

  int GetNumPiecesPlacedAtHeight(float h);

  // TODO generalise for any type of piece - use codes for types
  int GetNumRampsPlacedAtHeight(float h);

protected:
  // List of members
  typedef std::vector<PieceMover*> Pms;
  Pms m_pms;

  // List of pieces and their order, inferred from position in
  // container.
  typedef std::deque<int> TakeableDeque;
  TakeableDeque m_takeables;
};

typedef SharedPtr<CooperationGroup> PCooperationGroup;
}

#endif
