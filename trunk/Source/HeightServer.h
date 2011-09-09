/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: HeightServer.h,v $
Revision 1.1.10.4  2007/07/15 21:56:57  Administrator
DEBUG mode: Each Plane stores the name of the leaf which created it.
This is for debugging HeightServer issues, where you want to know where
a poly has come from.

Revision 1.1.10.3  2006/04/13 17:36:10  jay
Repaced _DEBUG with HS_DEBUG

Revision 1.1.10.2  2005/07/30 12:14:48  jay
Added IntersectGetPoint() - don't think it works though.

Revision 1.1.10.1  2005/04/09 23:07:44  jay
Added function to get distance from a vertex to a WallPoly.
This is so we can accurately extract Pool balls from walls, but may be
generally useful.

Revision 1.1  2004/09/08 15:42:51  jay
Added to repository
  
*/

#if !defined(HEIGHT_SERVER_H_INCLUDED)
#define HEIGHT_SERVER_H_INCLUDED

#include "BoundingBox.h"
#include "Geom2d.h"
#include "Vertex.h"
#include "Mgc/MgcVector2.h"
#include "Mgc/MgcSegment3.h"
#include <list>
#include <vector>

#ifdef _DEBUG
//#define HS_DEBUG
#endif

namespace Amju
{
class BoundingSphere;

class Plane
{
  friend struct PlaneLessThan;

public:

#ifdef _DEBUG
  // Store name of leaf contributing this poly for debugging
  Plane(const char* leaf);
#else
  Plane();
#endif

  ~Plane() {}

  // Add a vertex to the plane. 0 <= i < 3
  void AddVertex(const Vec3f& v, unsigned int i);

  // Get values for coeffiecients a, b, c, d in plane equation.
  void CreateCoefficients();

#if defined(HS_DEBUG)
  void Draw() const; // for testing, draw the polys on screen.
#endif

  // Returns true if this poly should be used to generate height info. If false, 
  // this poly should be used for wall info.
  bool IsHeightPoly() const;

  void GetNormal(float pResult[3]) const;
  Vec3f GetNormal() const;

  const Vec3f& GetVertex(unsigned int i) const { return m_vertices[i]; }

  // Get an edge. There are as many edges as vertices.
  Geom2d::Line2d GetEdge(unsigned int i) const;

  // Equality: two planes are equal if their coefficients are equal.
  bool operator==(const Plane& rhs) const;

  // Classify Point - from Flipcode collision detection tutorial
  // -----------------------------------------------------------
  // Possible positions of a point in relation to the Plane.
  enum PositionType
  {  
    FRONT, BACK, COINCIDE
  };

  // Get the position type for a point in relation to this Plane.
  PositionType ClassifyPoint(const Vec3f& v) const;

  float a() const { return m_a; }
  float b() const { return m_b; }
  float c() const { return m_c; }
  float d() const { return m_d; }

protected:
  // Vertices making up the poly.
  Vec3f m_vertices[3];

  // Plane equation is ax + by + cz + d = 0 - these are the coefficients.
  // We get them using vertex (x, y, z) info.
  // Normal to plane is (m_a, m_b, m_c).
  float m_a, m_b, m_c, m_d;

#ifdef _DEBUG
  std::string m_leafName;
#endif
};

struct PlaneLessThan
{
  bool operator()(const Plane& lhs, const Plane& rhs);
};

class WallPoly : public Plane
{
public:
#ifdef _DEBUG
  WallPoly(const char* leaf) : Plane(leaf) {}
#else
  WallPoly() : Plane() {}
#endif

  WallPoly(const Plane& p);

  ~WallPoly() {}

  // Return true if a bounding sphere moving from 'before' to 'after' 
  // collides with the wall.
  bool Intersects(const BoundingSphere& bsBefore, const BoundingSphere& bsAfter) const;

  bool IsCeiling() const;

  bool IntersectsCeiling(
    const BoundingSphere& bsBefore, 
    const BoundingSphere& bsAfter) const;

  // Get squared dist from given point to this wall.
  float SqDist(const Vec3f&) const;

  // Find out if line intersects this wall, and if so the intersect point.
  bool IntersectGetPoint(
    const Vec3f& v1,
    const Vec3f& v2,
    Vec3f* pIntersectPoint);

protected:
  // Returns true if this wall polygon intersects the sphere. This is used to
  // detect when a moving object collides with a wall.
  // We ignore a collision if the sphere is behind the wall, as this is not
  // normally relevant. This can be turned off so a collision from either
  // side counts.
  bool Intersects(const BoundingSphere& b) const;
};

class HeightPoly : public Plane
{
public:
  HeightPoly(const Plane& p);

  // Call once all vertices have been added to create a rectangle which 
  // encloses the plan view of the plane poly.
  void CreateBoundingBox();

  float GetY(float x, float z) const;

  // Return true if point (x, z) is inside the plan view of this poly.
  bool IsIn(float x, float z) const;

  // Return true if point is in the bounding box for the plan of this poly.
  bool IsInBox(float x, float z) const;

  // Return true if given bounding rect in x-z intersects the bounding
  // rect for this height poly.
  bool IsInBox(float xmin, float xmax, float zmin, float zmax) const;

  // Returns true if the bounding rectangle for the argument poly
  // intersects the rectangle for this poly.
  bool BoxesIntersect(const HeightPoly& hp) const;

private:
  // Returns true if point (x, z) is on the inside of the edge 
  // (x1, z1) -> (x2, z2).
  bool IsIn(float x, float z, float x1, float z1, float x2, float z2) const;

  // Bounding box around the plan of the poly.
  // This is used for a quick rejection of points outside the plan box.
  float m_xmin, m_zmin, m_xmax, m_zmax;

  // Vector of Mgc 2D vertices. This is used by the Mgc point-in-poly code.
  std::vector<Mgc::Vector2> m_mgcVerts;
};

// The Height server serves up a height (y-value) given an (x, z)
// coord. This lets us keep the camera a constant distance from 
// the floor when it slopes etc.
// Also supports collision detection with walls.
class HeightServer
{
public:
  HeightServer();
  ~HeightServer();

  // Get the height at (x, z), if it's available from this height server.
  bool GetHeight(float x, float z, float* pResultY) const;

  // Get the HeightPoly at (x, z) if available.
  // Returns 0 if (x, z) is not covered by a HeightPoly by this HeightServer.
  // The poly chosen is the highest which is under the given y value.
  // If there is more than one matching poly, the choice is random.
  const HeightPoly* GetHeightPoly(float x, float y, float z) const;

  // As above, gets highest Height Poly in height range.
  const HeightPoly* GetHeightPoly(float x, float ymax, float ymin, float z) const;

  // As above, but all height polys which match are inserted into the list.
  // Polys must contain the (x, z) coord, and lie between ymax and ymin.
  // NB result list is added to - it isn't initially cleared.
  void GetAllHeightPolys(float x, 
                         float ymax, 
                         float ymin, 
                         float z, 
                         std::list<const HeightPoly*> * pResult) const;

  // As above, but height polys must intersect bounding rectangle in x-z
  // plane, and lie between ymax and ymin.
  // NB result list is added to - it isn't initially cleared.
  void GetAllHeightPolys(float xmin,
                         float xmax, 
                         float ymax, 
                         float ymin, 
                         float zmin,
                         float zmax, 
                         std::list<const HeightPoly*> * pResult) const;


  // Intersection test
  // -----------------
  // Find if a bounding sphere moving from the 'before' position to the 'after' position
  // intersects one or more of the Walls owned by this HeightServer.

  // Find if a bounding sphere moving from 'before' to 'after' positions goes
  // through a wall. Returns a pointer to the wall if so, else zero.
  // NB If sphere intersects more than one wall, only the first to be
  // found is returned.
  const WallPoly* Intersects(
    const BoundingSphere& bsBefore, 
    const BoundingSphere& bsAfter) const;

  // As above, but find all intersecting polys. All polys which are intersected are
  // pushed back into the result vector. Returns true if vector is then non-empty.
  bool Intersects(
    const BoundingSphere& bsBefore, 
    const BoundingSphere& bsAfter,
    std::vector<WallPoly>* pResult) const;

  // Returns true if the given line segment intersects with one or more walls.
  // Only front-facing walls are considered, with the segment origin the 'front' end
  // of the line seg.
  // radius is the radius of the line segment - i.e. it's really a capsule.
  bool Intersects(const Mgc::Segment3& seg, float radius) const;

  // As above, but uses two vertices instead of Line Segment.
  bool Intersects(const Vec3f& v1, const Vec3f& v2, float radius) const;

  // Check for collision with (near-)horizontal walls which are not floors.
  // This is a special case as the normal collision detection works in x-z plane.
  bool IntersectsCeiling(
    const BoundingSphere& bsBefore, 
    const BoundingSphere& bsAfter) const;

  // Insert a plane polygon.
  void InsertPoly(const Plane& p); 

  // for testing, draw the polys on screen.
#if defined(HS_DEBUG)
  void DrawHeights() const; 
  void DrawWalls() const;
#endif

  // Get the number of polys stored
  int GetNumHeightPolys() const { return m_heightPolys.size(); }
  int GetNumWallPolys() const { return m_wallPolys.size(); }

  // Get wall and height polys.
  const Plane* GetWallPoly(int i) const;
  const Plane* GetHeightPoly(int i) const;

  void Clear();

  static const float BadHeight;

  // Add the entire contents of another HeightServer.
  void AddHeightServer(const HeightServer& hs);

  const BoundingBox& GetBoundingBox() const { return m_boundingBox; }

protected:
  // Returns false if capsule made from the two spheres intersects bounding box.
  // True means the capsule doesn't intersect, so can be rejected for collision
  // testing.
  bool QuickIntersectReject(
    const BoundingSphere& bsBefore, 
    const BoundingSphere& bsAfter) const;

  void AddHeightPoly(const HeightPoly& hp);

  // Get union of two height polys. The first argument is set to the result.
  // The bounding box is recalculated.
  // The two arguments should be polys on the same plane with one shared edge.
  //void HeightPolyUnion(HeightPoly* pPoly1, const HeightPoly& poly2);

  typedef std::vector<HeightPoly> HeightPolyList;
  HeightPolyList m_heightPolys;
  
  typedef std::vector<WallPoly> WallPolyList;
  WallPolyList m_wallPolys;

  // Bounding cuboid. Used for rough intersection testing.
  BoundingBox m_boundingBox;
};
} // namespace
#endif
