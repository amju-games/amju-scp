/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: VisibilityGraphWithObjects.cpp,v $
Revision 1.1  2004/09/08 15:43:00  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <algorithm>
#include <iostream>
#include "Common.h"
#include "VisibilityGraphWithObjects.h"
#include "Takeable.h"
#include "Level.h"
#include "Engine.h"
#include "VisibilityPoint.h"
#include "SolidComposite.h"
#include "Snap.h"
#include "TakeableCrate.h"
#include "Engine.h"

using namespace std;
namespace Amju
{
void VisibilityGraphWithObjects::CreateGraph(float losWidth)
{
  // Add vis points in scene to vis graph.
  m_pScene->AddVisibilityPoint(this);

  BuildCubeMap();

  // Add vis points on each piece to the vis graph.
  int levelId = m_pLevel->GetId();
  int roomId = m_pLevel->GetRoomId();
  // Get the game objects which are in the same room as the player.
  GameObjectMap& objs = Engine::Instance()->GetGameObjects(levelId, roomId);
  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    GameObjectId gameObjId = it->first;
    PGameObject pGo = it->second;
    // TODO should this be SolidGameObject ?
    TakeableCrate* pTc = dynamic_cast<TakeableCrate*>(pGo.GetPtr());
    if (pTc &&
        pTc->Takeable::GetState() == UNKNOWN)
    {
      AddVisibilityPointsFromTakeable(pTc);

      // Add piece to cube map 
      CubeMapAdd(pTc);
   
    }
  }

  VisibilityGraph::CreateGraph(losWidth);
}

// Get the vis. points in a Solid Composite.
void VisibilityGraphWithObjects::FindVisibilityPoints(
  SolidComponent* p, 
  VisibilityGraph::VisibilityList* pResults)
{
  if (dynamic_cast<VisibilityPoint*>(p))
  {
    pResults->push_back(p->GetBoundingSphere()->GetCentre());
  }
  else if (dynamic_cast<SolidComposite*>(p))
  {
    SolidComposite* pComp = dynamic_cast<SolidComposite*>(p);
    int numChildren = pComp->GetNumChildren();
    for (int i = 0; i < numChildren; i++)
    {
      PSolidComponent pChild = pComp->GetChild(i);
      FindVisibilityPoints(pChild.GetPtr(), pResults);
    }
  }
}

void VisibilityGraphWithObjects::AddVisibilityPointsFromTakeable(Takeable* pT)
{
  PSolidComponent pSolid = pT->GetSolid();
  pSolid->AddVisibilityPoint(this);
  // Recurse over the Solid, looking for Visibility Points. Add them to
  // a map, so later we can quickly look up the visibility points we added 
  // from the solid.
  VisibilityList verts;
  FindVisibilityPoints(pSolid.GetPtr(), &verts);

  m_SolidToVisLists[pSolid.GetPtr()] = verts;
}

VisibilityGraph::VisibilityList VisibilityGraphWithObjects::GetVisiblityPoints(
  PSolidComponent pSolid)
{
  return m_SolidToVisLists[pSolid.GetPtr()];
}

void VisibilityGraphWithObjects::UpdateAddLinksThroughSphere(
  const BoundingSphere& bs,
  float losWidth)
{
#if defined(_DEBUG)
  int numVisPoints = m_vismap.size();
  int comparisons = 0;
  int newLinks = 0;
#endif

  // We want to check all the pairs of points which were
  // blocked by a takeable, because they may now be visible to each other.
  // Unfortunately this means an n^2 search, but we can prune a lot of pairs
  // if the line between them doesn't intersect the takeable's b. sphere. 
  for (VisMap::iterator it = m_vismap.begin(); it != m_vismap.end();  /* */  )
  {
    const VertexBase& v1 = it->first;
    VisibilityList& vlist1 = it->second;

    ++it;

    // Gah, now we must check the line from v1 to every other point!

    // NB We create a new iterator from 'it'. So we compare 
    // n^2 / 2 pairs.
    // TODO Make sure we don't try to use an invalid iterator!
    for (VisMap::iterator jt = it; 
         jt != m_vismap.end(); 
         ++jt)
    {
      const VertexBase& v2 = jt->first;
      VisibilityList& vlist2 = jt->second;

      Assert(v1 != v2);

#if defined(_DEBUG)
      comparisons++;
#endif

      // TODO see if line v1-v2 OR v2-v1 is cached as blocked. ???
      
      // if line v1-v2 intersects bs, we should check it as it may now
      // be visible. We can't just add it because something else may 
      // block the line.

      if (bs.Intersects(v1, v2, losWidth))
      {
        // Bah, the line from v1 to v2 intersects the sphere. Now we
        // must test it, and add it to the graph if the points really 
        // are visible to each other.
        if (PathExists(v1, v2, losWidth) &&
            PathExists(v2, v1, losWidth) )
        {
          AddLink(v1, v2);
#if defined(_DEBUG)
          newLinks++;
#endif
        }
      }
    }
  }
#if defined(_DEBUG)
  /*
  cout << "Comparisons: " << comparisons << " new links: " << newLinks << endl;
  */
#endif
}

void VisibilityGraphWithObjects::UpdatePieceTaken(
  Takeable* pTakeable, 
  float losWidth)
{
  Clear();
  CreateGraph(losWidth);
}

void VisibilityGraphWithObjects::UpdateRemoveLinksThroughSphere(
  const BoundingSphere& bs,
  float losWidth)
{
  // Check existing pairs which intersect the bounding sphere. 
  // For pairs which intersect, thoroughly test as visibility may now be blocked.

  // List of links to erase. We erase them at the end when we have finished
  // iterating!
  std::vector<Link> linksToErase;

  for (VisMap::iterator it = m_vismap.begin(); it != m_vismap.end(); ++it)
  {
    const VertexBase& v1 = it->first;
    VisibilityList& vlist = it->second;

    for (VisibilityList::iterator jt = vlist.begin(); jt != vlist.end(); ++jt)
    {
      const VertexBase& v2 = *jt;
      if (bs.Intersects(v1, v2, losWidth))
      {
        if (!PathExists(v1, v2, losWidth) ||
            !PathExists(v2, v1, losWidth))
        {
          // v2 is no longer visible from v1.
          linksToErase.push_back(Link(v1, v2));
        }
      }
    }
  }

  // Now erase the links in linksToErase
  for (std::vector<Link>::iterator kt = linksToErase.begin(); 
       kt != linksToErase.end(); 
       ++kt)
  { 
    const VertexBase& v1 = kt->m_v1;
    const VertexBase& v2 = kt->m_v2;

    RemoveLink(v1, v2);
  }
}

void VisibilityGraphWithObjects::UpdatePieceDropped(
  Takeable* pTakeable, 
  float losWidth)
{
  Clear();
  CreateGraph(losWidth);
}

void VisibilityGraphWithObjects::UpdatePieceRotated(
  Takeable* pTakeable, 
  float losWidth)
{
  Clear();
  CreateGraph(losWidth);
}

bool VisibilityGraphWithObjects::ObstructionExists(
  const VertexBase& v1, 
  const VertexBase& v2, 
  float r) const
{
  if (!VisibilityGraph::PathExists(v1, v2, r) )
  {
    return true;
  }
  // Find out if the capsule intersects any GameObjects' collision vol.
  int levelId = m_pLevel->GetId();
  int roomId = m_pLevel->GetRoomId();

  // Get the game objects which are in the same room as the player.
  GameObjectMap& objs = Engine::Instance()->GetGameObjects(levelId, roomId);

  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    GameObjectId gameObjId = it->first;
    PGameObject pGo = it->second;
    Takeable* pTakeable = dynamic_cast<Takeable*>(pGo.GetPtr());

    // We must check that visibility works in both directions. An example of
    // why this is: a vis. point attached to one solid may be _inside_ a 
    // nearby solid.
    if (pTakeable && 
        pTakeable->GetState() == UNKNOWN &&
        pTakeable->GetCollisionVol() &&
        (pTakeable->GetCollisionVol()->Intersects(v1, v2, r) ||
        pTakeable->GetCollisionVol()->Intersects(v2, v1, r)))
    {
      return true;
    }
  }
  
  return false;
}

bool VisibilityGraphWithObjects::PathExists(
  const VertexBase& v1, 
  const VertexBase& v2, 
  float r) const
{
  // Work out if there is a path from v1 to v2 for a sphere of radius r.
  //

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
  
  // Standard intersection test
  if (ObstructionExists(v1, v2, r))
  {
    return false;
  }

  // Use cube map to check that there is an unbroken floor between the points.
  // We want to check the layer of cubes below the two vertices, because we
  // want to make sure there's a decent floor underneath the path between them.

  CubeMap::Coord c1((int)v1.x, (int)(v1.y - 2.0f), (int)v1.z); // TODO SNAP CONFIG
  CubeMap::Coord c2((int)v2.x, (int)(v2.y - 2.0f), (int)v2.z); // TODO SNAP CONFIG

  bool traversable = m_cubemap.IsTraversable(c1, c2);

  return traversable;
}

void VisibilityGraphWithObjects::BuildCubeMap()
{
  // This rebuilds the whole cube map.
  // TODO just unset/set the thing that has changed
  Assert(m_pLevel);
  m_cubemap.Clear();
  m_cubemap.Create(m_pLevel);
}

void VisibilityGraphWithObjects::CubeMapAdd(Takeable* p)
{
  Assert(p);
  Assert(p->GetCollisionVol());
  m_cubemap.Set(*(p->GetCollisionVol()));
}

void VisibilityGraphWithObjects::CubeMapRemove(Takeable* p)
{
  Assert(p);
  // Set the State so we can access the collision vol
  State s = p->GetState();
  p->SetState(UNKNOWN);
  Assert(p->GetCollisionVol());
  m_cubemap.Unset(*(p->GetCollisionVol()));
  p->SetState(s);
}

const CubeMap& VisibilityGraphWithObjects::GetCubeMap() const
{
  return m_cubemap;
}

#if defined(_DEBUG)
void VisibilityGraphWithObjects::Draw()
{
  VisibilityGraph::Draw();
  DrawCubeMap();
}

void VisibilityGraphWithObjects::DrawCubeMap()
{
  m_cubemap.Draw();
}
#endif

}

