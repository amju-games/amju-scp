/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineRunningVisGraph.cpp,v $
Revision 1.1  2004/09/08 15:42:36  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "EngineRunningVisGraph.h"
#include "Level.h"
#include "Engine.h"
#include "VisibilityGraphRefCounted.h"
#include "VisibilityGraphRc2.h"
#include "VisibilityGraphNull.h"
#include "TakeableCrate.h"
#include "SchAssert.h"

// If TEST_VIS_GRAPH is defined, the vis graph is tested against a "control"
// version - it's very slow! (This only happens in DEBUG build.)
//#define TEST_VIS_GRAPH

namespace Amju
{
const float EngineRunningVisGraph::LINE_OF_SIGHT_RADIUS = 0.5f; // TODO CONFIG ?

PVisibilityGraph EngineRunningVisGraph::s_pVisibilityGraph;

#if defined(_DEBUG)
PVisibilityGraph EngineRunningVisGraph::s_pTESTVisGraph; // non-ref-counted version
PVisibilityGraph EngineRunningVisGraph::s_pTESTVisGraphRc; // ref-counted version
#endif

EngineRunningVisGraph::EngineRunningVisGraph()
{
  float maxLinkLength = 1000.0f;

  // NB If no vis. graph is required, use VisibilityGraphNull.
  //s_pVisibilityGraph = new VisibilityGraphNull; //VisibilityGraphRc2;//VisibilityGraphRefCounted;
  
  // Use this kind of visibility graph for normal path finding.
  s_pVisibilityGraph = new VisibilityGraphRc2;//VisibilityGraphRefCounted;
  s_pVisibilityGraph->SetMaxLinkLength(maxLinkLength); 

#if defined(_DEBUG)
  // This test graph is non-ref counted, and is rebuilt after every change.
  // It's the 'control' graph, that is always considered to be the correct
  // one when there are discrepancies.
  s_pTESTVisGraph = new VisibilityGraphWithObjects;
  s_pTESTVisGraph->SetMaxLinkLength(maxLinkLength); 

  // Another test vis graph - this one is ref counted, like the real, non-test
  // graph. This is to make sure rebuilding the ref-counted and non-ref-counted
  // versions give the same graph. Then we compare the rebuilt ref-counted
  // graph with the minimally-updated graph, to find discrepancies.
  s_pTESTVisGraphRc = new VisibilityGraphRc2;//VisibilityGraphRefCounted;
  s_pTESTVisGraph->SetMaxLinkLength(maxLinkLength); 
#endif
}

void EngineRunningVisGraph::Draw()
{
  EngineRunningBase::Draw();

#if defined(_DEBUG)

  AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);
  GetEngine()->PushColour(0, 0, 0, 1);
  // TODO TEMP TEST
  // Draw the two graphs in different colours ?
  // In game.cfg, set "draw_vis_graph" to 1 to see the FAST graph,
  // set to 2 to see the SLOW/CONTROL graph, or 0 for no graph.
  if (GetEngine()->GetConfigValue("draw_vis_graph") == "1")
  {
    s_pVisibilityGraph->Draw();
  }
  else if (GetEngine()->GetConfigValue("draw_vis_graph") == "2")
  {
    s_pTESTVisGraph->Draw();
  }
  else if (GetEngine()->GetConfigValue("draw_vis_graph") == "3")
  {
    s_pTESTVisGraphRc->Draw();
  }

  GetEngine()->PopColour();
  AmjuGL::Enable(AmjuGL::AMJU_TEXTURE_2D);
#endif
}

bool EngineRunningVisGraph::PieceIntersects(Takeable* pObj, Level* pLevel)
{
  // Return true if pObj intersects the static scene or any GameObjects.

  const BoundingBox& bbox = pObj->GetCollisionVol()->GetBoundingBox();

  // Compare the collision volume of pObj with the HeightServer of each
  // leaf in the scene. 
  // NB we can walk the scene and only compare leaves whose b.sphere
  // intersects the piece b.sphere.
  if (pLevel->GetScene()->BoxIntersects(bbox))
  {
    return true;
  }

  // Compare the collision volume of pObj with the collision volume of
  // each GameObject.
  // TODO use collision/interaction manager to mark possible colliders.
  // NB Check b.spheres first ? 
  // Iterate over the game objects which are in the current level/room.
  int levelId = pLevel->GetId();
  int roomId = pLevel->GetRoomId();

  // Get the game objects which are in the same room as the player.
  GameObjectMap& objs = GetEngine()->GetGameObjects(levelId, roomId);

  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    PGameObject pGo = it->second;
    //VisibleGameObject* pVis = dynamic_cast<VisibleGameObject*>(pGo.GetPtr());
    // We don't want to have to worry about CharacterGameobejcts for now.
    Takeable* pVis = dynamic_cast<Takeable*>(pGo.GetPtr());
    // TODO Check state
    if (pVis &&
        pVis != pObj &&
        pVis->GetCollisionVol() &&
        pVis->GetCollisionVol()->GetBoundingBox().Intersects(bbox))
    {
      return true;
    }
  }

  return false;
}

bool EngineRunningVisGraph::AllPieceCornersSupported(Takeable* pTakeable, Level*)
{
  return true; // TODO
}

bool EngineRunningVisGraph::PieceSupportsObject(
  Takeable* pTakeable, 
  Level* pLevel, 
  VisibleGameObject* pVgo)
{
  // Use the takeable's bounding box. Add the box's height to its y-coord.
  // Then see if the box intersects the given visible game object.

  Assert(pVgo);
  Assert(pTakeable->GetCollisionVol());

  BoundingBox bb = pTakeable->GetCollisionVol()->GetBoundingBox();
  float h = bb.m_ymax - bb.m_ymin;
  bb.m_ymin = bb.m_ymax;
  bb.m_ymax += h;
  // Now check bb for intersection with pVgo.
  if (pVgo->GetCollisionVol() &&
      pVgo->GetCollisionVol()->GetBoundingBox().Intersects(bb))
  {
    return true;
  }
  return false; 
}

bool EngineRunningVisGraph::PieceSupportsOthers(Takeable* pTakeable, Level* pLevel)
{
  // TODO just use cube map

  // Use the takeable's bounding box. Add the box's height to its y-coord.
  // Then see if the box intersects another piece.
  Assert(pTakeable->GetCollisionVol());

  BoundingBox bb = pTakeable->GetCollisionVol()->GetBoundingBox();
  float h = bb.m_ymax - bb.m_ymin;
  bb.m_ymin = bb.m_ymax;
  bb.m_ymax += h;
  // Now check bb for intersection with another piece.

  int levelId = pLevel->GetId();
  int roomId = pLevel->GetRoomId();

  // Get the game objects which are in the same room as the player.
  GameObjectMap& objs = GetEngine()->GetGameObjects(levelId, roomId);

  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    PGameObject pGo = it->second;

    // TODO
    // Is it ok to take a piece out from under a Character ?

    //VisibleGameObject* pVis = dynamic_cast<VisibleGameObject*>(pGo.GetPtr());
    // We don't want to have to worry about CharacterGameobejcts for now.
    Takeable* pVis = dynamic_cast<Takeable*>(pGo.GetPtr());
    // TODO Check state
    if (pVis &&
        pVis != pTakeable &&
        pVis->GetCollisionVol() &&
        pVis->GetCollisionVol()->GetBoundingBox().Intersects(bb))
    {
      return true;
    }
  }

  return false; 
}

void EngineRunningVisGraph::RecalcVisGraphPieceTaken(Takeable* pTakeable)
{
#if defined(_DEBUG)
  CompareVisGraphs();
#endif

  // We want the state to be UNKNOWN at first, to get the b.sphere etc.
  pTakeable->SetState(UNKNOWN);
  s_pVisibilityGraph->UpdatePieceTaken(pTakeable, LINE_OF_SIGHT_RADIUS);
  // To rebuild graphs entirely, the takeable should be out of action.
  pTakeable->SetState(TAKEN);

#if defined(_DEBUG)
  RebuildTestGraph(pTakeable->GetLevel());
  CompareVisGraphs();
#endif
}

void EngineRunningVisGraph::RecalcVisGraphPieceDropped(Takeable* pTakeable)
{
  // We want the state to be UNKNOWN at first, to get the b.sphere etc.
  pTakeable->SetState(UNKNOWN);

  s_pVisibilityGraph->UpdatePieceDropped(pTakeable, LINE_OF_SIGHT_RADIUS);

#if defined(_DEBUG)
  RebuildTestGraph(pTakeable->GetLevel());
  CompareVisGraphs();
#endif
}

#if defined(_DEBUG)
void EngineRunningVisGraph::RebuildTestGraph(Level* pLevel)
{
#if defined(TEST_VIS_GRAPH)
  // For testing, we have TWO vis graphs! This one is rebuilt each time. 
  s_pTESTVisGraph->Clear();
  s_pTESTVisGraph->SetScene(pLevel->GetScene());
  s_pTESTVisGraph->SetLevel(pLevel); 
  s_pTESTVisGraph->CreateGraph(LINE_OF_SIGHT_RADIUS);
  //s_pTESTVisGraph->SweepUp(); // clear vertices with no neightbours

  s_pTESTVisGraphRc->Clear();
  s_pTESTVisGraphRc->SetScene(pLevel->GetScene());
  s_pTESTVisGraphRc->SetLevel(pLevel); 
  s_pTESTVisGraphRc->CreateGraph(LINE_OF_SIGHT_RADIUS);
#endif
}

void EngineRunningVisGraph::CompareVisGraphs()
{
#if defined(TEST_VIS_GRAPH)
  Assert(s_pVisibilityGraph->IsSymmetrical());
  Assert(s_pTESTVisGraph->IsSymmetrical());
  Assert(s_pTESTVisGraphRc->IsSymmetrical());
  
  // Make sure the two control graphs are the same.
  // They should be - both are always rebuilt after every change.
  if (*(s_pTESTVisGraphRc.GetPtr()) != (*s_pTESTVisGraph.GetPtr()))
  {
    { // scope so dtors close files
      std::ofstream dump1("rctestgraph.txt");
      dump1 << *(s_pTESTVisGraphRc.GetPtr());

      std::ofstream dump2("nonrctestgraph.txt");
      dump2 << (*s_pTESTVisGraph.GetPtr());
    }
    Assert(0);
  }
 
  // Check the rebuilt ref-counted graph with the minimally-updated
  // ref-counted graph. Dump them if they are not equal.
  //s_pVisibilityGraph->SweepUp();
  //s_pTESTVisGraphRc->SweepUp();
  if (*(s_pVisibilityGraph.GetPtr()) != (*s_pTESTVisGraphRc.GetPtr()))
  {
    { // scope so dtors close files
      std::ofstream dump1("rebuilt.txt");
      dump1 << *(s_pTESTVisGraphRc.GetPtr());

      std::ofstream dump2("minimal.txt");
      dump2 << (*s_pVisibilityGraph.GetPtr()); // dump the "real" graph
    }
    Assert(0);
  }
/*
  VisibilityGraphRefCounted& vgrc1 = 
    *(dynamic_cast<VisibilityGraphRefCounted*>(s_pVisibilityGraph.GetPtr()));

  VisibilityGraphRefCounted& vgrc2 = 
    *(dynamic_cast<VisibilityGraphRefCounted*>(s_pTESTVisGraphRc.GetPtr()));

  vgrc1.SweepUpZeroRefCounts();
  vgrc2.SweepUpZeroRefCounts();

  if (!vgrc1.EqualRefCounts(vgrc2))
  {
    { // scope so dtors close files
      std::ofstream dump1("rebuilt.txt");
      dump1 << *(s_pTESTVisGraphRc.GetPtr());

      std::ofstream dump2("minimal.txt");
      dump2 << (*s_pVisibilityGraph.GetPtr()); // dump the "real" graph
    }
    Assert(0);
  }
*/
#endif
}
#endif

void EngineRunningVisGraph::RecalcVisGraph(Level* pLevel)
{
  // Normally we don't rebuild from scratch, we just redo what changed.
  // So this should just be called at the start of a level.

  s_pVisibilityGraph->Clear();
  s_pVisibilityGraph->SetScene(pLevel->GetScene());
  s_pVisibilityGraph->SetLevel(pLevel); 
  s_pVisibilityGraph->CreateGraph(LINE_OF_SIGHT_RADIUS);

#if defined(_DEBUG)
  RebuildTestGraph(pLevel);
  CompareVisGraphs();
#endif
}

void EngineRunningVisGraph::ResetTakeables()
{
  GameObjectMap& objs = Engine::Instance()->GetGameObjects();
  // Iterate through map of Game Objects. 
  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    PGameObject pGo = it->second;

    Takeable* pTakeable = dynamic_cast<Takeable*>(pGo.GetPtr());
    if (pTakeable)
    {
      pTakeable->ResetPosition();
    }
  }
}

void EngineRunningVisGraph::ResetAis()
{
  GameObjectMap& objs = Engine::Instance()->GetGameObjects();
  // Iterate through map of Game Objects. 
  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    PGameObject pGo = it->second;
/*
    Ai* pAi = dynamic_cast<Ai*>(pGo.GetPtr());
    if (pAi)
    {
      pAi->ResetPieceMover();
      pAi->SetState(OUT_OF_PLAY); // to force state change below  
      pAi->SetState(UNKNOWN); // TODO TEMP TEST  
      pAi->OnRoomEntry(); // TODO TEMP TEST - do we need the test below ?
  
      // 30/9/2002 This seems reasonable - needed to kickstart the action in 
      // Intro state - but make sure it doesn't break Running state!
      if (pAi->GetState() != OUT_OF_PLAY)
      {
        pAi->OnRoomEntry();
      }
    }
*/
  }
}

PVisibilityGraph EngineRunningVisGraph::GetVisibilityGraph()
{
  return s_pVisibilityGraph;
/*
  // TODO TEMP TEST
  // does the slow graph work better than the fast graph ?
  return s_pTESTVisGraph;
*/
}

void EngineRunningVisGraph::ClearVisGraph()
{
  s_pVisibilityGraph->Clear();
#if defined(_DEBUG) && defined (TEST_VIS_GRAPH)
  s_pTESTVisGraph->Clear();
#endif
}
}

