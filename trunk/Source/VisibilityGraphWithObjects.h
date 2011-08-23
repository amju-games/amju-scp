/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: VisibilityGraphWithObjects.h,v $
Revision 1.1  2004/09/08 15:43:00  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_VISIBILITY_GRAPH_WITH_OBJECTS_H_INCLUDED)
#define SCHMICKEN_VISIBILITY_GRAPH_WITH_OBJECTS_H_INCLUDED

#include "VisibilityGraph.h"
#include "CubeMap.h"

namespace Amju
{
class Level;

// This kind of visibility graph takes into account GameObjects
// as well as the static scene.
class VisibilityGraphWithObjects : public VisibilityGraph
{
public:
  // Override to take Game Objects into account.
  virtual bool PathExists(
    const VertexBase& v1, 
    const VertexBase& v2, 
    float r) const;

  // Rebuild graph from scratch
  virtual void CreateGraph(float losWidth);

  // Update graph due to a piece being taken
  virtual void UpdatePieceTaken(Takeable*, float losWidth);

  // Update graph due to a piece being placed
  virtual void UpdatePieceDropped(Takeable*, float losWidth);

  virtual void UpdatePieceRotated(Takeable*, float losWidth);

  void AddVisibilityPointsFromTakeable(Takeable*);

  // Remove a list of points from a graph.
  void ClearVisPointsForObject(const VisibilityList& vlist);

//private:
  // This shouldn't have to be made available ideally
  const CubeMap& GetCubeMap() const;

public:

#if defined(_DEBUG)
  // Override base implementation to draw the cube map too.
  virtual void Draw();
  void DrawCubeMap();
#endif

protected:
  // Trash cube map and build it from scratch.
  void BuildCubeMap();

  // Add an object to the cube map.
  void CubeMapAdd(Takeable* p);
 
  // Remove an object from the cube map.
  void CubeMapRemove(Takeable* p);

  // Selectively add/subtract volumes to/from cube map
  void AddToCubeMap(BoundingBox&);
  void SubtractFromCubeMap(BoundingBox&);

  // Get list of visibility points we got from a solid.
  VisibilityList GetVisiblityPoints(PSolidComponent pSolid);

  // Check all pairs of points to see if they intersect the sphere.
  // If so, do a thorough test and add the pair if they are visible 
  // to each other.
  void UpdateAddLinksThroughSphere(
    const BoundingSphere& bs,
    float losWidth);

  void UpdateRemoveLinksThroughSphere(
    const BoundingSphere& bs,
    float losWidth);

  // Find the visibility points on a Solid.
  void FindVisibilityPoints(
    SolidComponent* p, 
    VisibilityList* pResults);

protected:
  bool ObstructionExists(
  const VertexBase& v1, 
  const VertexBase& v2, 
  float r) const;

protected:
  // Hold state of unit cubes. This is to simplify path tests.
  CubeMap m_cubemap;

  // Map Solids to the list of visbility points on them.
  // The idea is to uise this to quickly find visibility points which are
  // due to a particular game object.
  std::map<SolidComponent*, VisibilityList> m_SolidToVisLists;
};
}

#endif
