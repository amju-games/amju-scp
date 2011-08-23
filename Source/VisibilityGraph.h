/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: VisibilityGraph.h,v $
Revision 1.1  2004/09/08 15:42:59  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_VISIBILITY_GRAPH_H_INCLUDED)
#define SCHMICKEN_VISIBILITY_GRAPH_H_INCLUDED

#include <map>
#include <set>
#include <vector>
#include "SolidComponent.h"
#include "Vertex.h"
#include "SharedPtr.h"

namespace Amju
{
class Takeable;
class Level;

// For a composite scene, this graph holds all VisibilityPoint vertices.
// For each given point, it creates the list of visible neighbours.
// This is used for path-finding.
class VisibilityGraph : public Shareable
{
#if defined(_DEBUG)
  friend std::ostream& operator<<(std::ostream& o, VisibilityGraph& v);
#endif

public:
  typedef std::vector<VertexBase> VisibilityList;

public:
  virtual ~VisibilityGraph() {}

  // Add a vertex PRIOR to building the graph. This simply adds the vertex to
  // m_vismap. 
  virtual void AddPoint(const VertexBase&);

  // Create a visibility graph given the minimum width of a line-of-sight path.
  virtual void CreateGraph(float losWidth);

  // Update graph due to a piece being taken
  virtual void UpdatePieceTaken(Takeable*, float losWidth);

  // Update graph due to a piece being placed
  virtual void UpdatePieceDropped(Takeable*, float losWidth);

  // Reset to empty
  virtual void Clear();
  // Remove points visible from vertex, and remove from all other vertices' lists.
  virtual void ClearVisPoint(const VertexBase&);

  // Returns true if a path exists from v1 to v2, for a sphere of radius r.
  // The default implementation checks for collisions with the current scene. 
  virtual bool PathExists(
    const VertexBase& v1, 
    const VertexBase& v2, 
    float r) const;

protected:
  // Add link between the two points to the graph.
  // NB both points should exist in the graph, i.e. have been added using
  // AddPoint().
  virtual void AddLink(const VertexBase& v1, const VertexBase& v2);

  // Remove link between the two points to the graph.
  // NB both points should exist in the graph, i.e. have been added using
  // AddPoint().
  virtual void RemoveLink(const VertexBase& v1, const VertexBase& v2);
 
public:
  // Non virtuals
  // ------------

  // Scene: used by default implementation of LineIntersects().
  void SetScene(PSolidComponent pScene);

  // Set the level for which we test Game Objects.
  void SetLevel(Level* pLevel);

  // Returns true if given vertex is a graph node.
  bool IsNode(const VertexBase&) const;

  // Get all points which are visible from the argument point.
  // The argument point should be one of the ones added using AddPoint().
  const VisibilityList& LookupNeighbours(const VertexBase&);

  // For points which are not graph nodes, this function creates a list of
  // visible neightbours on the fly.
  // To be visible, a node-node path must be at least as wide as the given
  // width (radius).
  VisibilityList CreateNeighbours(const VertexBase&, float losWidth);

  static void SetMaxLinkLength(float dist);

#if defined(_DEBUG)

  bool IsSymmetrical() const;

  // Delete nodes which have no neighbours.
  // This should be a no op.
  void SweepUp();

  // Draw the whole graph, i.e. draw a line between every pair of points
  // which are visible to each other.
  virtual void Draw();

  // Draw lines from the given point to all visible points.
  void Draw(const VertexBase&);

  bool operator==(const VisibilityGraph& rhs) const;
  bool operator!=(const VisibilityGraph& rhs) const;

  // called by operator<<
  virtual void Print(std::ostream& o);

#endif

protected:
  // Map of points to visible neighbours
  typedef std::map<VertexBase, VisibilityList> VisMap;
  VisMap m_vismap;

  // The scene for which this visiblity graph is valid.
  // Used by default implementation of LineIntersects().
  PSolidComponent m_pScene;

  Level* m_pLevel;

  struct Link 
  {
    Link(const VertexBase& v1, const VertexBase& v2) : m_v1(v1), m_v2(v2) {}
    VertexBase m_v1, m_v2;
    bool operator<(const Link&) const;
    bool operator==(const Link&) const;
    bool operator!=(const Link&) const;
  };

  // Max link length squared
  static float s_maxSqDist;
};

typedef SharedPtr<VisibilityGraph> PVisibilityGraph;
}
#endif

