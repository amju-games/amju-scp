/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: AStarPathFinder.cpp,v $
Revision 1.1  2004/09/08 15:42:44  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "AStarPathFinder.h"
#include "SchAssert.h"
#include "Common.h"
#include <algorithm>
#include <math.h>

#if defined(_DEBUG)
//#define ASTAR_TEST
#endif

// This A* implementation borrows lots of the great ideas in
// Game Programming Gems I. But not the bug-ridden code :-)

namespace Amju
{
AStarPathFinder::AStarPathFinder(
  PVisibilityGraph pVisgraph) :
  m_pVisgraph(pVisgraph),
  m_nodepool(1000) // pool size  
{
  // Some reasonable default line-of-sight width.
  m_losWidth = 0.5f; 
}

void AStarPathFinder::SetLineOfSightWidth(float w)
{
  m_losWidth = w;
}

AStarPathFinder::iterator AStarPathFinder::Begin()
{
  return m_path.begin();
}

AStarPathFinder::iterator AStarPathFinder::End()
{
  return m_path.end();
}

void AStarPathFinder::CreatePath(const PNode& n)
{
  Node* p = n.GetPtr();
  while (p)
  {
    m_path.push_front(p->loc);
    p = p->pParent.GetPtr();
  }
}

bool AStarPathFinder::PathExists(
  const VertexBase& begin, 
  const VertexBase& end, 
  float radius)
{
  return m_pVisgraph->PathExists(begin, end, radius);
}

SearchResult
AStarPathFinder::FindPath(const VertexBase& begin, const VertexBase& end)
{
  m_goal = end;

  // Check if end is visible from begin.
  // This line-of-sight test uses a width argument so the goal only counts as
  // visible if the path to it is wide enough.
  if (PathExists(begin, end, m_losWidth))
  {
    m_path.push_back(end);
    return SearchOk;
  }
  
  // No direct line of sight, so we really have to work out the best path.

  PriorityQueue  open;

  PNode startNode = m_nodepool.GetNode(begin);
  startNode->costFromStart = 0;
  startNode->totalCost = PathCostEstimate(begin, end);
  startNode->isInOpen = true;
  startNode->isInClosed = false;
  open.Push(startNode);

  while (!open.IsEmpty())
  {
    PNode n = open.Top();
    open.Pop();

    if (ReachedGoal(n->loc, end))
    {
      CreatePath(n);
      return SearchOk;
    } 

    // Get child nodes of n
    NodeList nlist = GetChildren(n);

    for (NodeList::iterator it = nlist.begin(); it != nlist.end(); ++it)
    {
      Node tempNode = *((*it).GetPtr());
      if (n->pParent.GetPtr() != 0 && 
          n->pParent->loc == tempNode.loc)
      {
        // Ignore parent
        continue;
      }

      // Use temp copy of the current node we're considering, so we
      // don't trash the real Node until we know it's better.
      tempNode.pParent = n;
      float newCost = n->costFromStart + TraverseCost(n->loc, tempNode.loc);
      tempNode.costFromStart = newCost;
      tempNode.totalCost = newCost + PathCostEstimate(tempNode.loc, end);

      PNode actualNode = *it;

      if ((actualNode->isInOpen && tempNode.totalCost > actualNode->totalCost) ||
          (actualNode->isInClosed && tempNode.totalCost > actualNode->totalCost))
      {
        // We have already considered this node, and got a better cost doing it
        // another way. So don't update it.
        continue;
      }

      // This node looks good. Store new or improved info.
      actualNode->pParent = n;
      actualNode->costFromStart = tempNode.costFromStart; 
      actualNode->totalCost = tempNode.totalCost; 

      actualNode->isInClosed = false;

      if (actualNode->isInOpen)
      { 
        // Adjust location in open - i.e. re-sort 
        open.Update(actualNode);
      }
      else
      {
        open.Push(actualNode);
        actualNode->isInOpen = true;
      }
    }
    n->isInClosed = true; // finished with this node.
  }
  // No path found, open is empty
  return SearchFail;
}

float DistanceSquaredXZ(const VertexBase& begin, const VertexBase& end)
{
  const float xdiff = end.x - begin.x;
  const float zdiff = end.z - begin.z;
  const float dsq = xdiff * xdiff + zdiff * zdiff;
  return dsq;
}

float DistanceSquaredXYZ(const VertexBase& begin, const VertexBase& end)
{
  const float xdiff = end.x - begin.x;
  const float ydiff = end.y - begin.y;
  const float zdiff = end.z - begin.z;
  const float dsq = xdiff * xdiff + ydiff * ydiff + zdiff * zdiff;
  return dsq;
}

// This function is virtual, but a reasonable default is to return the
// 3D distance between the points.
float AStarPathFinder::PathCostEstimate(const VertexBase& begin, const VertexBase& end)
{
  float d = sqrt(DistanceSquaredXYZ(begin, end));
  return d;
}

bool AStarPathFinder::ReachedGoal(const VertexBase& begin, const VertexBase& end)
{
  // Return true if begin is close enough to end to count as the goal.

  // TODO TEMP TEST: get distance from begin to end. Return true if it's < 1.
  float d = DistanceSquaredXYZ(begin, end);
  return (d <= 1.0f);
}

AStarPathFinder::NodeList AStarPathFinder::GetChildren(const AStarPathFinder::PNode& pn)
{
  NodeList result;

  // Check if goal is visible. 
  // This line-of-sight test uses a width argument so the goal only counts as
  // visible if the path to it is wide enough.
  if (PathExists(pn->loc, m_goal, m_losWidth))
  {
    result.push_back(m_nodepool.GetNode(m_goal));
    return result;
  }

  // Using the visibility graph, this is easy!
  // The only slight complication is that the start node won't be in the
  // visibility graph. In this case we need to create a list of neighbours.
  if (!m_pVisgraph->IsNode(pn->loc))
  {
    // Current location isn't in the graph, so create a list of neighbours.
    const VisibilityGraph::VisibilityList vislist = 
      m_pVisgraph->CreateNeighbours(pn->loc, m_losWidth);

    for (VisibilityGraph::VisibilityList::const_iterator it = vislist.begin();
         it != vislist.end();
         ++it)
    {
      PNode pNode = m_nodepool.GetNode(*it);
      result.push_back(pNode);
    }
  }
  else
  {
    // Node is in the graph, so we can simply look up its neighbours.
    const VisibilityGraph::VisibilityList& vislist = 
      m_pVisgraph->LookupNeighbours(pn->loc);

    for (VisibilityGraph::VisibilityList::const_iterator it = vislist.begin();
         it != vislist.end();
         ++it)
    {
      PNode pNode = m_nodepool.GetNode(*it);
      result.push_back(pNode);
    }
  }

  return result;
}

float AStarPathFinder::TraverseCost(const VertexBase& v1, const VertexBase& v2)
{
  float dsq = DistanceSquaredXYZ(v1, v2);
  float d = sqrt(dsq);
  return d; 
}

// ----------------------------------------------------------------------------
// Node Pool

AStarPathFinder::NodePool::NodePool(int poolSize)
{
}

AStarPathFinder::PNode AStarPathFinder::NodePool::GetNode(const VertexBase& loc)
{
  NodeMap::iterator it = m_nodemap.find(loc);
  if (it != m_nodemap.end())
  {
    return it->second;
  }
  PNode pn = new Node(loc); // TODO use pool
  pn->isInClosed = false;
  pn->isInOpen = false;
  m_nodemap[loc] = pn;
  return pn;
}

// ----------------------------------------------------------------------------
// Closed List
/*
bool AStarPathFinder::ClosedList::Contains(const VertexBase& v)
{
  for (iterator it = begin(); it != end(); ++it)
  {
    if ((*it)->loc == v)
    {
      return true;
    }
  }
  return false;
}
*/

// ----------------------------------------------------------------------------
// Priority Queue

// Predicate for STL heap sort
struct NodeOrder
{
  bool operator()(
    const AStarPathFinder::PNode& pn1, 
    const AStarPathFinder::PNode& pn2) const
  {
    // We want the node with the lowest total at the root of the heap, so we
    // need the opposite of 'less-than'.
    return (pn1->totalCost > pn2->totalCost);
  }
};   

#if defined(_DEBUG)
// Make sure front item has the lowest cost
void CheckHeap(const AStarPathFinder::NodeList& n)
{
  if (n.empty())
  {
    return;
  }
  float t = n[0]->totalCost;
  for (AStarPathFinder::NodeList::const_iterator it = n.begin(); it != n.end(); ++it)
  {
    if (!((*it)->totalCost >= t))
    {
      t = 0;
    }
  }
}
#endif

bool AStarPathFinder::PriorityQueue::Contains(const VertexBase& v)
{
  for (NodeList::iterator it = m_nodes.begin(); it != m_nodes.end(); ++it)
  {
    if ((*it)->loc == v)
    {
      return true;
    }
  }
  return false;
}

AStarPathFinder::PNode AStarPathFinder::PriorityQueue::Top() const
{
#if defined(_DEBUG)
  CheckHeap(m_nodes);
#endif

  return m_nodes.front();
}

void AStarPathFinder::PriorityQueue::Pop()
{
#if defined(_DEBUG)
  CheckHeap(m_nodes);
#endif

  std::pop_heap(m_nodes.begin(), m_nodes.end(), NodeOrder());
  m_nodes.pop_back();

#if defined(_DEBUG)
  CheckHeap(m_nodes);
#endif
}

void AStarPathFinder::PriorityQueue::Push(AStarPathFinder::PNode pn)
{
#if defined(_DEBUG)
  CheckHeap(m_nodes);
#endif

  m_nodes.push_back(pn);
  std::push_heap(m_nodes.begin(), m_nodes.end(), NodeOrder());

#if defined(_DEBUG)
  CheckHeap(m_nodes);
#endif
}

bool AStarPathFinder::PriorityQueue::IsEmpty() const
{
  return m_nodes.empty();
}

void AStarPathFinder::PriorityQueue::Update(PNode pn)
{
  // Don't call CheckHeap here - it may well be wrong! That's why 
  // we're updating it!

  // Node pn is now probably in the wrong place in the heap, making m_nodes 
  // not a heap any more. So we need to make m_nodes back into a heap.
  std::make_heap(m_nodes.begin(), m_nodes.end(), NodeOrder());

#if defined(_DEBUG)
  CheckHeap(m_nodes);
#endif
}


}
