/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: VisibilityGraph.cpp,v $
Revision 1.1  2004/09/08 15:42:59  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <algorithm> // std::remove
#include "VisibilityGraph.h"
#if defined(_DEBUG)
#include "Common.h" // debug drawing
#endif
#include "SchAssert.h"

namespace Amju
{
float VisibilityGraph::s_maxSqDist = 0;

void VisibilityGraph::SetMaxLinkLength(float dist)
{
  s_maxSqDist = dist * dist;
}

bool VisibilityGraph::Link::operator<(
  const VisibilityGraph::Link& rhs) const
{
  if (m_v1 < rhs.m_v1)
  {
    return true;
  }
  if (rhs.m_v1 < m_v1)
  {
    return false;
  }

  return m_v2 < rhs.m_v2;
}

bool VisibilityGraph::Link::operator==(
  const VisibilityGraph::Link& rhs) const
{
  return (m_v1 == rhs.m_v1 && m_v2 == rhs.m_v2);
}

bool VisibilityGraph::Link::operator!=(const VisibilityGraph::Link& rhs) const
{
  return !(operator==(rhs));
}

void VisibilityGraph::AddLink(const VertexBase& v1, const VertexBase& v2)
{
  // TODO maybe vlist should be a set ??
  VisibilityList& vlist = m_vismap[v1];
  if (std::find(vlist.begin(), vlist.end(), v2) == vlist.end())
  {
    m_vismap[v1].push_back(v2);
  }

  VisibilityList& vlist2 = m_vismap[v2];
  if (std::find(vlist2.begin(), vlist2.end(), v1) == vlist2.end())
  {
    m_vismap[v2].push_back(v1);
  }
}

void VisibilityGraph::RemoveLink(const VertexBase& v1, const VertexBase& v2)
{
  // Not implemented here because we simply rebuild the whole graph when
  // anything changes; we don't fiddle around removing links.
  Assert(0);
}

void VisibilityGraph::SetScene(PSolidComponent pScene)
{
  Assert(pScene.GetPtr());
  m_pScene = pScene;
  // Recurse over scene, adding visbility points to this graph.
  m_pScene->AddVisibilityPoint(this);
}

void VisibilityGraph::SetLevel(Level* pLevel)
{
  m_pLevel = pLevel;
}

void VisibilityGraph::AddPoint(const VertexBase& v)
{
  if (m_vismap.find(v) != m_vismap.end())
  {
    return;
  }
  m_vismap[v] = VisibilityList();
}

void VisibilityGraph::Clear()
{
  m_vismap.clear();
}

#if defined(_DEBUG)
bool VisibilityGraph::operator==(const VisibilityGraph& rhs) const
{
  int thisSize = m_vismap.size();
  int rhsSize = rhs.m_vismap.size();
  if (thisSize != rhsSize)
  {
    // Maps are different sizes.
    return false;
  }

  // Check that the keys in both maps are the same. I.e. both maps
  // have the same list of vertices.
  VisMap::const_iterator it;
  VisMap::const_iterator jt;
  for (it = m_vismap.begin(), jt = rhs.m_vismap.begin(); 
       it != m_vismap.end(); 
       ++it, ++jt)
  {
    const VertexBase& thisV = it->first;
    const VertexBase& rhsV = jt->first;
    if (thisV != rhsV)
    {
      // Maps contain different vertex keys.
      return false;
    }
  }

  // Check that the list of visible points from each key vertex is the same.
  for (it = m_vismap.begin(), jt = rhs.m_vismap.begin(); 
       it != m_vismap.end(); 
       ++it, ++jt)
  {
    VisibilityList list1 = it->second;
    VisibilityList list2 = jt->second;
    // Compare the 2 lists
    int size1 = list1.size();
    int size2 = list2.size();
    if (size1 != size2)
    {
      // The vis. lists for a vertex are different sizes in the two maps.
      return false;
    }
    // Compare each element in the lists.
    std::sort(list1.begin(), list1.end());
    std::sort(list2.begin(), list2.end());
    if (!std::equal(list1.begin(), list1.end(), list2.begin()))
    {
      // The lists are the same length but have different things in them.
      return false;
    }
  }
  return true;
}

bool VisibilityGraph::operator!=(const VisibilityGraph& rhs) const
{
  return !operator==(rhs);
}
#endif

void VisibilityGraph::ClearVisPoint(const VertexBase& v)
{
  m_vismap.erase(v);
  // Remove all references to v in the other lists
  for (VisMap::iterator it = m_vismap.begin(); it != m_vismap.end(); ++it)
  {
    //const VertexBase& v = it->first;
    VisibilityList& vlist = it->second;
    vlist.erase(std::remove(vlist.begin(), vlist.end(), v), vlist.end());
  }
}

void VisibilityGraph::UpdatePieceTaken(Takeable*, float losWidth)
{
  // Default behaviour: rebuild the graph. 
  // In fact we probably don't need to do anything here, because Game Objects
  // don't affect visibility.
  Clear();
  CreateGraph(losWidth);
}

void VisibilityGraph::UpdatePieceDropped(Takeable*, float losWidth)
{
  Clear();
  CreateGraph(losWidth);
}

void VisibilityGraph::CreateGraph(float losWidth)
{
  // Nested loop over map (inner loop is in call to CreateNeighbours())
  for (VisMap::iterator it = m_vismap.begin(); it != m_vismap.end(); ++it)
  {
    const VertexBase& v = it->first;
    VisibilityList vlist = CreateNeighbours(v, losWidth);
    for (VisibilityList::iterator jt = vlist.begin(); jt != vlist.end(); ++jt)
    {
      AddLink(v, *jt);    
    }
  }
#ifdef _DEBUG
  Assert(IsSymmetrical());
#endif
}

bool VisibilityGraph::IsNode(const VertexBase& v) const
{
  return (m_vismap.find(v) != m_vismap.end());
}

const VisibilityGraph::VisibilityList& 
VisibilityGraph::LookupNeighbours(const VertexBase& v)
{
  Assert(IsNode(v));
  return m_vismap[v];
}

VisibilityGraph::VisibilityList 
VisibilityGraph::CreateNeighbours(const VertexBase& v, float losWidth)
{
  // Create list of nodes which are visible from v.
  Assert(m_pScene.GetPtr());

  VisibilityList result;
  for (VisMap::iterator it = m_vismap.begin(); it != m_vismap.end(); ++it)
  {
    const VertexBase& v1 = it->first;
    if (v != v1 && 
        PathExists(v, v1, losWidth)  && 
        PathExists(v1, v, losWidth))
    {
      // Line-of-sight exists between the points.
      result.push_back(v1);
    }
  }
  return result;
}

bool VisibilityGraph::PathExists(
  const VertexBase& v1, 
  const VertexBase& v2, 
  float r) const
{
  // Nasty hack: cull bad lines when they are unfeasibly long
  // This is brutal but effective: the graph is cut down a lot!
  const float dx = v1.x - v2.x;
  const float dy = v1.y - v2.y;
  const float dz = v1.z - v2.z; 
  float sqdist = dx * dx + dy * dy + dz * dz;
  if (sqdist > s_maxSqDist) 
  {
    return false;
  }

  Assert(m_pScene.GetPtr());
  return !m_pScene->LineIntersects(v1, v2, r);
}

#if defined(_DEBUG)
void VisibilityGraph::Draw()
{
  for (VisMap::iterator it = m_vismap.begin(); it != m_vismap.end(); ++it)
  {
    const VertexBase& v = it->first;
    Draw(v);
  }
}

void VisibilityGraph::Draw(const VertexBase& v)
{
  const VisibilityList& vlist = m_vismap[v];
  // Draw a line from v to each item in vlist.
  for (VisibilityList::const_iterator it = vlist.begin(); it != vlist.end(); ++it)
  {
    const VertexBase& v2 = *it;
    //glBegin(GL_LINES);
    //  glVertex3f(v.x, v.y, v.z);
    //  glVertex3f(v2.x, v2.y, v2.z);
    //glEnd();
    AmjuGL::DrawLine(AmjuGL::Vec3(v.x, v.y, v.z), AmjuGL::Vec3(v2.x, v2.y, v2.z));
  }
}

// Dump vis graph to file, for debug comparisons
std::ostream& operator<<(std::ostream& o, VisibilityGraph& vg)
{
  vg.Print(o);
  return o;
}

void VisibilityGraph::Print(std::ostream& o)
{
  int size = m_vismap.size();
  o << "Number of visbility points: " << size << std::endl;

  VisibilityGraph::VisMap::iterator it;

  for (it = m_vismap.begin(); it != m_vismap.end(); ++it)
  {
    const VertexBase& v = it->first;
    VisibilityGraph::VisibilityList vlist = it->second;
    o << "Visibility Point: (" 
      << v.x 
      << ", " 
      << v.y 
      << ", " 
      << v.z 
      << ")  number of links :" 
      << vlist.size()
      << ": ";


    // Sort the vlist. This is so we can diff this output, and the order
    // of vertices won't make a difference.
    std::sort(vlist.begin(), vlist.end());
    for (VisibilityList::iterator jt = vlist.begin(); jt != vlist.end(); ++jt)
    {
      const VertexBase& u = *jt;

      o << "("
        << u.x
        << ", "
        << u.y
        << ", "
        << u.z
        << ") ";
    }

    o << std::endl;
  }
}

void VisibilityGraph::SweepUp()
{
  // Delete nodes which have no neighbours.
  // This should be a no op.
  for (VisMap::iterator it = m_vismap.begin(); it != m_vismap.end(); /* nothing */ )
  {
    VisibilityList& vlist = it->second;
    if (vlist.empty())
    {
      //Assert(0);
      m_vismap.erase(it++);
    }
    else
    {
      ++it;
    }
  }
}

bool VisibilityGraph::IsSymmetrical() const
{
  // Make sure every link v1-v2 is matched by a link v2-v1
  for (VisMap::const_iterator it = m_vismap.begin(); it != m_vismap.end(); ++it )
  {
    const VertexBase& v1 = it->first;
    const VisibilityList& vlist = it->second;
    for (VisibilityList::const_iterator jt = vlist.begin(); jt != vlist.end(); ++jt)
    {
      const VertexBase& v2 = *jt;
      // v2 belongs to v1's list.
      // Check that v1 belongs to v2's list
      VisMap::const_iterator kt = m_vismap.find(v2);
      if (kt == m_vismap.end())
      {
        return false;
      }

      Assert(kt->first == v2);
      const VisibilityList& vlist2 = kt->second;
      // Check v1 exists in v2.
      if (std::find(vlist2.begin(), vlist2.end(), v1) == vlist2.end())
      {
        return false;
      }
    }
  }
  return true;
}

#endif
}

