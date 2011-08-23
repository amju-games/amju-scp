/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: VisibilityGraphRc2.cpp,v $
Revision 1.1  2004/09/08 15:43:00  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <algorithm>
#include "VisibilityGraphRc2.h"
#include "Level.h"
#include "Engine.h"
#include "TakeableCrate.h"

namespace Amju
{
void VisibilityGraphRc2::Clear()
{
  m_vertexRcs.clear();
  VisibilityGraphWithObjects::Clear();
}

void VisibilityGraphRc2::AddPoint(const VertexBase& v)
{
  if (m_vismap.find(v) == m_vismap.end())
  {
    m_vismap[v] = VisibilityList();
    // Initialise ref count for this vertex
    m_vertexRcs[v] = 1;
  }
  else
  {
    m_vertexRcs[v]++;
  }
}

void VisibilityGraphRc2::RemovePoint(const VertexBase& v)
{
  // Decrement the ref count for the vertex.
  // If the count hits zero, delete the point and any links to it.
  Assert(m_vertexRcs.find(v) != m_vertexRcs.end());
  int rc = --m_vertexRcs[v];
  Assert(rc >= 0);
  if (rc > 0)
  {
    return;
  }
  // Remove all links to/from v
  ClearVisPoint(v);
}

void VisibilityGraphRc2::CreateGraph(float losWidth)
{
  // NB Do NOT call Clear() here, the Scene vis points have already been set.

  // NB We must build the cube map first, so link-finding routines which
  // use the cube map will work!
  BuildCubeMap();

  // Create the graph for the static scene.
  Assert(m_pScene.GetPtr());

  m_vertexRcs.clear();

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
      // Add piece to cube map 
      CubeMapAdd(pTc);

      VisibilityList verts;
      PSolidComponent pSolid = pTc->Takeable::GetSolid();
      FindVisibilityPoints(pSolid.GetPtr(), &verts);

      // Add the visibility points to the graph. 
      VisibilityList::iterator jt;
      for (jt = verts.begin(); jt != verts.end(); ++jt)
      {
        VertexBase& v = *jt;
        AddPoint(v); // incrementing ref count as required
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

void VisibilityGraphRc2::UpdatePieceTaken(Takeable* p, float losWidth)
{
  BoundingSphere bs = *(p->GetBoundingSphere());

  p->SetState(TAKEN);

  RemovePiece(p, losWidth);

  // Create links between points which used to be blocked by the piece.
  // Increase radius so we don't miss any links
  bs.SetRadius(bs.GetRadius() + 4.0f); // TODO CONFIG
  UpdateAddLinksThroughSphere(bs, losWidth);
  // We must also remove any links which existed due to the floor provided
  // by the piece!
  UpdateRemoveLinksThroughSphere(bs, losWidth);
}

void VisibilityGraphRc2::UpdatePieceDropped(Takeable* p, float losWidth)
{
  // Remove links which are now blocked by the piece.
  BoundingSphere bs = *(p->GetBoundingSphere());
  // Increase radius so we don't miss any links
  bs.SetRadius(bs.GetRadius() + 4.0f); // TODO CONFIG
  UpdateRemoveLinksThroughSphere(bs, losWidth);

  CubeMapAdd(p);

  // Add links which can use the newly-dropped piece as a floor.
  UpdateAddLinksThroughSphere(bs, losWidth);

  AddPiece(p, losWidth);
}

void VisibilityGraphRc2::AddLink(const VertexBase& v1, const VertexBase& v2)
{
  VisibilityGraph::AddLink(v1, v2);
}

void VisibilityGraphRc2::RemoveLink(const VertexBase& v1, const VertexBase& v2)
{
  // Erase v2 from vlist1
  // Allow for thepossibility of the link no longer existing, which happens 
  // if the link from A-B has been deleted, followed by B-A, say.
  VisibilityList& vlist1 = m_vismap[v1];
  if (std::find(vlist1.begin(), vlist1.end(), v2) != vlist1.end())
  {
    vlist1.erase(std::find(vlist1.begin(), vlist1.end(), v2));
  }

  /*
  // If list is empty, remove it from the map, it's not a valid visibility point.
  if (vlist1.empty())
  {
    m_vismap.erase(v1);
  }
  */
  // remove symmetrical link, i.e. erase v1 from vlist2
  VisibilityList& vlist2 = m_vismap[v2];
  if (std::find(vlist2.begin(), vlist2.end(), v1) != vlist2.end())
  {
    vlist2.erase(std::find(vlist2.begin(), vlist2.end(), v1));
  }
  /*
  // If list is empty, remove it from the map, it's not a valid visibility point.
  if (vlist2.empty())
  {
    m_vismap.erase(v2);
  }
  */
}

void VisibilityGraphRc2::CreateAddPiece(Takeable* p, float losWidth)
{
  // This is called when graph is being created; not when minimal
  // changes are being made.
  VisibilityList::iterator it;

  // Create all links for this piece:
  // For each vis point on the piece, test the LOS to every other vis point
  // in the graph.
  // Get visibility points on the takeable.
  
  VisibilityList verts;
  PSolidComponent pSolid = p->GetSolid();
  FindVisibilityPoints(pSolid.GetPtr(), &verts);

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
    }
  }
}

void VisibilityGraphRc2::AddPiece(Takeable* p, float losWidth)
{
  // TODO Reset any existing list of links added for this piece..?

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
    }
  }
}

void VisibilityGraphRc2::RemovePiece(Takeable* p, float losWidth)
{
  CubeMapRemove(p);

  // Get vertices on piece. Dec ref count on each one.
  VisibilityList verts;
  PSolidComponent pSolid = p->GetSolid();
  FindVisibilityPoints(pSolid.GetPtr(), &verts);

  for (VisibilityList::iterator it = verts.begin(); it != verts.end(); ++it)
  {
    VertexBase& v = *it;
    RemovePoint(v); // (checks if vertex is already in map)
  }
}


#if defined(_DEBUG)
void VisibilityGraphRc2::Print(std::ostream& o)
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
      << ") "
      << "refcount: "
      << m_vertexRcs[v]
      << " number of links:" 
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
        << ")  ";
    }

    o << std::endl;
  }

}
#endif

}

