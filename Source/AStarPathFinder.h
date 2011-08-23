/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: AStarPathFinder.h,v $
Revision 1.1  2004/09/08 15:42:44  jay
Added to repository
  
*/

#if !defined(A_STAR_PATH_FINDER_H_INCLUDED)
#define A_STAR_PATH_FINDER_H_INCLUDED

#include "Vertex.h"
#include "SharedPtr.h"
#include "SolidComponent.h"
#include "VisibilityGraph.h"
#include "SharedPtr.h"
#include <list>
#include <map>
#include <vector>

namespace Amju
{
// Path-finding implementation, using the A* algorithm.
// Uses a visibility graph to get neighbours of nodes, so the
// scenery data needs VisbilityPoints built in.
//
// This implementation is based on the ideas in Game Programming Gems I.
//

enum SearchResult
{
  SearchOk, SearchFail, SearchPending
};

class AStarPathFinder : public Shareable
{
public:

  // Construct path finder giving the graph of Visibility Points.
  AStarPathFinder(PVisibilityGraph pVisgraph);

  virtual ~AStarPathFinder() {}

  // Returns SearchOk if path exists from begin to end.
  // Once this has been called, you can get the list of nodes
  // by iterating over the, using Begin()/End().
  // Returns SearchFail if no path.
  // Returns SearchPending if path (or vis graph) is still being calculated.
  SearchResult FindPath(const VertexBase& begin, const VertexBase& end);

  typedef std::list<VertexBase> Path;
  typedef Path::iterator iterator;

  // Iterate over points in path.
  iterator Begin();
  iterator End();

  // For line-of-sight tests, this is the minimum radius of a cylinder between
  // nodes.
  // The idea is, if a character can see a node, it must be able to reach it -
  // it must not get stuck because it's too fat!
  void SetLineOfSightWidth(float w);

//private:

  struct Node;
  typedef SharedPtr<Node> PNode;
  struct Node : public Shareable // don't know about this
  {
    VertexBase loc;
    float costFromStart;
    //float costToGoal;
    float totalCost; // cost from start + heuristic cost to goal
    PNode pParent;

    Node() : costFromStart(0), pParent(0) {}
    Node(const VertexBase& v) : loc(v), costFromStart(0), pParent(0) {}
    Node(const Node& rhs) : 
      loc(rhs.loc), 
      costFromStart(rhs.costFromStart), 
      pParent(rhs.pParent) {}

    bool isInOpen;
    bool isInClosed;

  };

  //friend class PriorityQueue; // to allow access to...
  typedef std::vector<PNode> NodeList;

  // Priority Queue of Node for the A* "Open" list
  struct PriorityQueue
  {
    PNode Top() const;
    void Pop();
    void Push(PNode);
    bool IsEmpty() const;
    void Update(PNode); 
    bool Contains(const VertexBase& v);

  private:
    NodeList m_nodes;
  };

  class NodePool
  {
  public:
    NodePool(int poolSize);

    // Get the node corresponding to the given location.
    PNode GetNode(const VertexBase& loc);

  private:
    // TODO we want to use a hash_map
    typedef std::map<VertexBase, PNode> NodeMap;
    NodeMap m_nodemap;    
  };

protected:
  // The heuristic function: estimates cost from begin to end.
  virtual float PathCostEstimate(const VertexBase& begin, const VertexBase& end);

  // Returns true if a capsule of given radius from begin to end collides
  // with something.
  virtual bool PathExists(
    const VertexBase& begin, 
    const VertexBase& end, 
    float radius);

  // Return true if begin is close enough to end to count as the goal.
  bool ReachedGoal(const VertexBase& begin, const VertexBase& end);

  // Return cost of travelling from v1 to v2 (this is precise, not estimated).
  float TraverseCost(const VertexBase& v1, const VertexBase& v2);

  // Get successor (child) Nodes for the given node.
  NodeList GetChildren(const PNode& pn);

  // Called once path is found, to fill m_path with the locations.
  void CreatePath(const PNode& n);

  // Result path of positions, in order, from start position to goal.
  Path m_path;

  // Visibility Graph for the scene.
  // This tells us what Visibility Points in the scene are visible from any
  // position or other node.
  PVisibilityGraph m_pVisgraph;

  // Memory management optimisation: the nodepool stores all Nodes.
  // TODO optimise this.
  NodePool m_nodepool;

  // The goal position which we are trying to reach in the Scene.
  VertexBase m_goal;

  // Line-of-sight width: a node is reachalbe only if the path to it is at least
  // this wide. In fact this is like the radius of a cylinder between nodes.
  float m_losWidth;
};
}
#endif

