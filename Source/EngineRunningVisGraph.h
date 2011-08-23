/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineRunningVisGraph.h,v $
Revision 1.1  2004/09/08 15:42:36  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_ENGINE_RUNNING_VIS_GRAPH_H_INCLUDED)
#define SCHMICKEN_ENGINE_RUNNING_VIS_GRAPH_H_INCLUDED

#include "EngineRunningBase.h"
#include "VisibilityGraph.h"

namespace Amju
{
class Takeable;
class Level;

// Base class for Running states which require a visibility graph.
// All the functionality of this class is static so it can be accessed
// from any object. We only want one copy of the vis. graph in memory!
class EngineRunningVisGraph : public EngineRunningBase
{
public:
  EngineRunningVisGraph();

  // Recalculate the whole visibility graph for the Level.
  static void RecalcVisGraph(Level* pLevel);

  // Selectively recalculate the vis. graph when a piece is taken
  // (i.e. is OUT_OF_PLAY) and can be ignored.
  static void RecalcVisGraphPieceTaken(Takeable*);

  // Selectively recalculate the vis. graph when a piece is dropped
  // and must be taken into account.
  static void RecalcVisGraphPieceDropped(Takeable*);

  // Selectively rebuild graph when a piece is rotated.
  static void RecalcVisGraphPieceRotated(Takeable*);

  // Piece states
  // ------------

  // Returns true if the given piece intersects scene or another GameObject.
  static bool PieceIntersects(Takeable*, Level*);

  // Return true if all corners of bounding cuboid of piece are supported
  // at the same height.
  static bool AllPieceCornersSupported(Takeable* pTakeable, Level*);

  // Returns true if piece is holding up other pieces.
  static bool PieceSupportsOthers(Takeable* pTakeable, Level*);

  // Returns true if the given piece is holding up the given object.
  // E.g. used to test if the player is on the given piece.
  static bool PieceSupportsObject(Takeable* pTakeable, Level*, VisibleGameObject*);

  static PVisibilityGraph GetVisibilityGraph();

  // This constant should be used in all calls to update any
  // Visibility Graph.
  static const float LINE_OF_SIGHT_RADIUS;

  // Debug Draw
  // ----------
  virtual void Draw();

  static void ClearVisGraph();

protected:
  void ResetTakeables();
  void ResetAis();

protected:
  // The one and only visibility graph, used for all pathfinding.
  static PVisibilityGraph s_pVisibilityGraph;

#if defined(_DEBUG)
  // Actually there are two other graphs!
  // This is for debugging.
  // Both are always trashed and rebuilt when any object is moved.
  // Then both are compared with the minimally-updated graph.
  static PVisibilityGraph s_pTESTVisGraph;
  static PVisibilityGraph s_pTESTVisGraphRc; // ref-counted version

  static void RebuildTestGraph(Level*); // Rebuild the above graph
  static void CompareVisGraphs(); // Compare - dump then assert if not equal.

#endif
};
}

#endif
