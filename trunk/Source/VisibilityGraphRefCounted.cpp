/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: VisibilityGraphRefCounted.cpp,v $
Revision 1.1  2004/09/08 15:43:00  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <algorithm>
#include "VisibilityGraphRefCounted.h"
#include "Level.h"
#include "Engine.h"
#include "TakeableCrate.h"
#include "SchAssert.h"

namespace Amju
{
void VisibilityGraphRefCounted::InitRefcount(
  const VertexBase& v1, 
  const VertexBase& v2)
{
  Assert(m_refcounts.find(Link(v1, v2)) == m_refcounts.end() ||
    m_refcounts[Link(v1, v2)] == 0);

  m_refcounts[Link(v1, v2)] = 1;
}

void VisibilityGraphRefCounted::IncRefcount(
  const VertexBase& v1, 
  const VertexBase& v2)
{
  Assert(m_refcounts.find(Link(v1, v2)) != m_refcounts.end());
  m_refcounts[Link(v1, v2)]++;
}

int VisibilityGraphRefCounted::DecRefcount(
  const VertexBase& v1, 
  const VertexBase& v2)
{
  Assert(m_refcounts.find(Link(v1, v2)) != m_refcounts.end());
  int r = --m_refcounts[Link(v1, v2)];
  return r;
}

void VisibilityGraphRefCounted::AddLink(
  const VertexBase& v1, 
  const VertexBase& v2)
{
  // Does the link exist ? If so, increment the ref count.
  // Otherwise initialise the ref count for the link (to 1),
  //  and add it to the graph.
  VisibilityList& vlist1 = m_vismap[v1];
  if (std::find(vlist1.begin(), vlist1.end(), v2) != vlist1.end())
  {
    // Link exists, so simply increment the ref count.
    IncRefcount(v1, v2);
    // ensure symmetrical link exists
    VisibilityList& vlist2 = m_vismap[v2];
    Assert(std::find(vlist2.begin(), vlist2.end(), v1) != vlist2.end());
    IncRefcount(v2, v1);
  }
  else
  {
    vlist1.push_back(v2);
    // Add symmetrical link 
    VisibilityList& vlist2 = m_vismap[v2];
    Assert(std::find(vlist2.begin(), vlist2.end(), v1) == vlist2.end());
    vlist2.push_back(v1);

    // Initialise the ref count to 1.
    InitRefcount(v1, v2);
    InitRefcount(v2, v1);
  }
}

// Decs the ref count for a link, erases it if ref count is zero.
void VisibilityGraphRefCounted::DecrementRemoveLink(
  const VertexBase& v1, 
  const VertexBase& v2)
{
  // Decrement the ref count.
  // If the count hits zero, remove the link from the graph.

  int rc1 = DecRefcount(v1, v2);
  int rc2 = DecRefcount(v2, v1);
  Assert(rc1 == rc2);
  if (rc1 == 0)
  {
    RemoveLink(v1, v2);
  }
}

void VisibilityGraphRefCounted::RemoveLink(
  const VertexBase& v1, 
  const VertexBase& v2)
{
  // Erase v2 from vlist1
  // Allow for thepossibility of the link no longer existing, which happens 
  // if the link from A-B has been deleted, followed by B-A, say.
  VisibilityList& vlist1 = m_vismap[v1];
  if (std::find(vlist1.begin(), vlist1.end(), v2) != vlist1.end())
  {
    vlist1.erase(std::find(vlist1.begin(), vlist1.end(), v2));
  }

  // If list is empty, remove it from the map, it's not a valid visibility point.
  if (vlist1.empty())
  {
    m_vismap.erase(v1);
  }
  
  // remove symmetrical link, i.e. erase v1 from vlist2
  VisibilityList& vlist2 = m_vismap[v2];
  if (std::find(vlist2.begin(), vlist2.end(), v1) != vlist2.end())
  {
    vlist2.erase(std::find(vlist2.begin(), vlist2.end(), v1));
  }

  // If list is empty, remove it from the map, it's not a valid visibility point.
  if (vlist2.empty())
  {
    m_vismap.erase(v2);
  }

  // Reset the ref counts to zero.
  Assert(m_refcounts.find(Link(v1, v2)) != m_refcounts.end());
  m_refcounts[Link(v1, v2)] = 0;

  Assert(m_refcounts.find(Link(v2, v1)) != m_refcounts.end());
  m_refcounts[Link(v2, v1)] = 0;
}

void VisibilityGraphRefCounted::AddPiece(Takeable* p, float losWidth)
{
  // TODO Reset any existing list of links added for this piece..?

  // Alter cube map, so LOS tests below are correct.
  CubeMapAdd(p);

  VisibilityList::iterator it;

  // Create all links for this piece:
  // For each vis point on the piece, test the LOS to every other vis point
  // in the graph.
  // Get visibility points on the takeable.
  
  VisibilityList verts;
  PSolidComponent pSolid = p->GetSolid();
  FindVisibilityPoints(pSolid.GetPtr(), &verts);

  // Add the visibility points to the graph. 
  // We have to add all the new points, _then_ get their neighbours.
  for (it = verts.begin(); it != verts.end(); ++it)
  {
    VertexBase& v = *it;
    AddPoint(v); // (checks if vertex is already in map)
  }

  for (it = verts.begin(); it != verts.end(); ++it)
  {
    VertexBase& v = *it;
    // Create the list of visible neighbours for this new vertex.
    VisibilityList vlist = CreateNeighbours(v, losWidth);
    for (VisibilityList::iterator jt = vlist.begin(); jt != vlist.end(); ++jt)
    {
      const VertexBase& u = *jt;
      // For each link, create or inc ref count.
      AddLink(v, u);    
      // Additionally we must keep track of what piece added what links.
      // This is because the order in which pieces are added will affect
      // the links which are created.
      // When we remove a piece, we want to reduce the ref count on only
      // the links which were created when the piece was added.
      AddPieceLink(p, Link(v, u));
    }
  }

  // Now check links which did exist but which are now blocked by the new piece.
  // These links can be removed from the vis graph.
  // Any link which is removed should have the reference from all pieces removed.
  // TODO!
}

void VisibilityGraphRefCounted::CreateAddPiece(Takeable* p, float losWidth)
{
  // This is called when graph is being created; not when minimal
  // changes are being made.

  // TODO Reset any existing list of links added for this piece..?

  // Alter cube map, so LOS tests below are correct.
  CubeMapAdd(p);

  VisibilityList::iterator it;

  // Create all links for this piece:
  // For each vis point on the piece, test the LOS to every other vis point
  // in the graph.
  // Get visibility points on the takeable.
  
  VisibilityList verts;
  PSolidComponent pSolid = p->GetSolid();
  FindVisibilityPoints(pSolid.GetPtr(), &verts);

  /*
  // Add the visibility points to the graph. 
  // We have to add all the new points, _then_ get their neighbours.
  for (it = verts.begin(); it != verts.end(); ++it)
  {
    VertexBase& v = *it;
    AddPoint(v); // (checks if vertex is already in map)
  }
  */
  for (it = verts.begin(); it != verts.end(); ++it)
  {
    VertexBase& v = *it;
    // Create the list of visible neighbours for this new vertex.
    VisibilityList vlist = CreateNeighbours(v, losWidth);
    for (VisibilityList::iterator jt = vlist.begin(); jt != vlist.end(); ++jt)
    {
      const VertexBase& u = *jt;
      // For each link, create or inc ref count.
      AddLink(v, u);    
      // Additionally we must keep track of what piece added what links.
      // This is because the order in which pieces are added will affect
      // the links which are created.
      // When we remove a piece, we want to reduce the ref count on only
      // the links which were created when the piece was added.
      AddPieceLink(p, Link(v, u));
    }
  }

  // Now check links which did exist but which are now blocked by the new piece.
  // These links can be removed from the vis graph.
  // Any link which is removed should have the reference from all pieces removed.
  // TODO!
}

void VisibilityGraphRefCounted::RemovePiece(Takeable* p, float losWidth)
{
  CubeMapRemove(p);

  // Get the links for this piece.
  const LinkVector& lv = GetPieceLinks(p);
  int links = lv.size();
  for (LinkVector::const_iterator it = lv.begin(); it != lv.end(); ++it)
  {
    const Link& link = *it;
    DecrementRemoveLink(link.m_v1, link.m_v2);    
    DecrementRemoveLink(link.m_v2, link.m_v1);    /// ?????
  }
  ResetPieceLinks(p);
}

void VisibilityGraphRefCounted::CreateGraph(float losWidth)
{
  // NB Do NOT call Clear() here, the Scene vis points have already been set.

  // NB We must build the cube map first, so link-finding routines which
  // use the cube map will work!
  BuildCubeMap();

  // Create the graph for the static scene.
  Assert(m_pScene.GetPtr());

  m_refcounts.clear();

  // Each link created for the scene should up the ref count for the link.
  // That way the link will never be deleted, because the static scene is never
  // deleted either.
  VisibilityGraph::CreateGraph(losWidth);

  // Add pieces, incrementing ref counts on each link found for the piece.
  Assert(m_pLevel);
  int levelId = m_pLevel->GetId();
  int roomId = m_pLevel->GetRoomId();
  // Get the game objects which are in the same room as the player.
  GameObjectMap& objs = Engine::Instance()->GetGameObjects(levelId, roomId);
  
  // Iterate over the objects TWICE: first to add the vertices, then to 
  // create the links.
  GameObjectMap::iterator it;

  for (it = objs.begin(); it != objs.end(); ++it)
  {
    GameObjectId gameObjId = it->first;
    PGameObject pGo = it->second;
    // TODO should this be SolidGameObject ?
    TakeableCrate* pTc = dynamic_cast<TakeableCrate*>(pGo.GetPtr());
    if (pTc &&
        pTc->Takeable::GetState() == UNKNOWN)
    {
      VisibilityList verts;
      PSolidComponent pSolid = pTc->Takeable::GetSolid();
      FindVisibilityPoints(pSolid.GetPtr(), &verts);

      // Add the visibility points to the graph. 
      VisibilityList::iterator jt;
      for (jt = verts.begin(); jt != verts.end(); ++jt)
      {
        VertexBase& v = *jt;
        AddPoint(v); // (checks if vertex is already in map)
      }
    }
  }
  
  for (it = objs.begin(); it != objs.end(); ++it)
  {
    GameObjectId gameObjId = it->first;
    PGameObject pGo = it->second;
    // TODO should this be SolidGameObject ?
    TakeableCrate* pTc = dynamic_cast<TakeableCrate*>(pGo.GetPtr());
    if (pTc &&
        pTc->Takeable::GetState() == UNKNOWN)
    {
      CreateAddPiece(pTc, losWidth);
    }
  }


}

void VisibilityGraphRefCounted::UpdatePieceTaken(Takeable* p, float losWidth)
{

  // Dec ref count on links to/from vis points on the piece.
  RemovePiece(p, losWidth);

//  SweepUp(); // TODO TEMP TEST does this help ?

#if defined(_DEBUG)
  // Output graph to file before and after, for debugging
  std::ofstream dump1("fastgraph_before.txt");
  dump1 << *this;
#endif


  // Create links between points which used to be blocked by the piece.
  const BoundingSphere& bs = *(p->GetBoundingSphere());
  UpdateAddLinksThroughSphere(bs, losWidth);

//  SweepUp();

#if defined(_DEBUG)
  // Output graph to file before and after, for debugging
  std::ofstream dump2("fastgraph_after.txt");
  dump2 << *this;
#endif

}

void VisibilityGraphRefCounted::UpdatePieceDropped(Takeable* p, float losWidth)
{
  // Remove links which are now blocked by the piece.
  const BoundingSphere& bs = *(p->GetBoundingSphere());
  UpdateRemoveLinksThroughSphere(bs, losWidth);

//  SweepUp(); // TODO TEMP TEST does this help ?

  // Add/in ref count of links with points on piece
  AddPiece(p, losWidth);

//  SweepUp();
}

void VisibilityGraphRefCounted::AddPieceLink(Takeable* p, const Link& link)
{
  m_pieceLinkMap[p].push_back(link);
}

void VisibilityGraphRefCounted::ResetPieceLinks(Takeable* p)
{
  Assert(m_pieceLinkMap.find(p) != m_pieceLinkMap.end());

  m_pieceLinkMap.erase(p);
}

const VisibilityGraphRefCounted::LinkVector& 
VisibilityGraphRefCounted::GetPieceLinks(Takeable* p) const
{
  Assert(m_pieceLinkMap.find(p) != m_pieceLinkMap.end());

  PieceLinkMap::const_iterator it = m_pieceLinkMap.find(p);
  return it->second;
}


#if defined(_DEBUG)
void VisibilityGraphRefCounted::Print(std::ostream& o)
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
        << ") [rc "
        << m_refcounts[Link(v, u)]
        << "]  ";
    }

    o << std::endl;
  }
}

void VisibilityGraphRefCounted::SweepUpZeroRefCounts()
{
  LinkRefCount::iterator it = m_refcounts.begin();

  for ( ; it != m_refcounts.end();  )
  {
    int refcount = it->second;
    if (refcount == 0)
    {
      m_refcounts.erase(it++);
    }
    else
    {
      ++it;
    }
  }
}

bool VisibilityGraphRefCounted::EqualRefCounts(
  const VisibilityGraphRefCounted& rhs) const
{
  if (m_refcounts.size() != rhs.m_refcounts.size())
  {
    return false;
  }
  int size = m_refcounts.size();

  int count = 0;

  LinkRefCount::const_iterator it = m_refcounts.begin();
  LinkRefCount::const_iterator jt = rhs.m_refcounts.begin();

  for ( ; it != m_refcounts.end(); ++it, ++jt)
  {
    const Link& link1 = it->first;
    const Link& link2 = jt->first;

    if (link1 != link2)
    {
      return false;
    }

    int refcount1 = it->second;
    int refcount2 = jt->second;

    if (refcount1 != refcount2)
    {
      return false;
    }
    count++;
  }
  return true;
}

#endif
}


