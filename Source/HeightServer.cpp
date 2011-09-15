/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: HeightServer.cpp,v $
Revision 1.1.10.4  2007/07/15 21:56:56  Administrator
DEBUG mode: Each HSTri stores the name of the leaf which created it.
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

#include <algorithm>
#include "HeightServer.h"
#include "Common.h"
#include "Geometry.h"
#include "BoundingSphere.h"
// Magic Software triangle-sphere intersect test
#include "Mgc/MgcIntr3DTriSphr.h"
// Mgc point-in-poly test
#include "Mgc/MgcCont2DPointInPolygon.h"
// Mgc distance from line to triangle
#include "Mgc/MgcDist3DLinTri.h"
// Mgc dist from point to triangle
#include "Mgc/MgcDist3DVecTri.h"
#include "SchAssert.h"

using namespace std;

namespace Amju
{
const float HeightServer::BadHeight = -1000.0f;

HeightServer::HeightServer() 
{
}

HeightServer::~HeightServer()
{
}

const HSTri* HeightServer::GetWallPoly(int i) const
{
  Assert(i < GetNumWallPolys());
  return &m_wallPolys[i];
}

const HSTri* HeightServer::GetHeightPoly(int i) const
{
  Assert(i < GetNumHeightPolys());
  return &m_heightPolys[i];
}

const HeightPoly* HeightServer::GetHeightPoly(
  float x, float y, float z) const
{
  return GetHeightPoly(x, y, BadHeight, z);
}

const HeightPoly* HeightServer::GetHeightPoly(
  float x, float ymax, float ymin, float z) const
{
  // Get the HeightPoly which is highest, in range (ymax, ymin).
  const HeightPoly* pResult = 0;
  float hmax = BadHeight;
  for (unsigned int i = 0; i < m_heightPolys.size(); i++)
  {
    if (m_heightPolys[i].IsIn(x, z))
    {
      // Get height at (x, z) for this poly..
      float h = m_heightPolys[i].GetY(x, z);
      if (h > hmax && h <= ymax && h >= ymin)
      {
        hmax = h;
        pResult = &m_heightPolys[i];
      }
    }
  }
  return pResult;
}

void HeightServer::GetAllHeightPolys(
  float x, 
  float ymax, 
  float ymin, 
  float z, 
  std::list<const HeightPoly*> * pResult) const
{
  float hmax = BadHeight;
  for (unsigned int i = 0; i < m_heightPolys.size(); i++)
  {
    if (m_heightPolys[i].IsIn(x, z))
    {
      // Get height at (x, z) for this poly..
      float h = m_heightPolys[i].GetY(x, z);
      if (h >= hmax && h <= ymax && h >= ymin)
      {
        hmax = h;
        pResult->push_back(&m_heightPolys[i]);
      }
    }
  }
}

void HeightServer::GetAllHeightPolys(
  float xmin,
  float xmax, 
  float ymax, 
  float ymin, 
  float zmin,
  float zmax, 
  std::list<const HeightPoly*> * pResult) const
{
  // TODO This isn't accurate, as we use the bounding rect. of each height
  // poly, rather than an exact test. But it's probably better to do it quickly
  // and maybe throw in some extra polys.
  float x = (xmax + xmin) * 0.5f;
  float z = (zmax + zmin) * 0.5f;
  for (unsigned int i = 0; i < m_heightPolys.size(); i++)
  {
    if (m_heightPolys[i].IsInBox(xmin, xmax, zmin, zmax))
    {
      // Get height at (x, z) for this poly..
      float h = m_heightPolys[i].GetY(x, z);
      if (h <= ymax && h >= ymin)
      {
        pResult->push_back(&m_heightPolys[i]);
      }
    }
  }
}

bool HeightServer::GetHeight(float x, float z, float* pResultY) const
{
  // TODO Currently finds the highest poly covering (x, z) - 
  // specify whether you want the lowest, first, biggest area, etc.
  Assert(pResultY);
  bool found = false;
  float hmax = BadHeight;

  for (unsigned int i = 0; i < m_heightPolys.size(); i++)
  {
    if (m_heightPolys[i].IsIn(x, z))
    {
      found = true;
      float h = m_heightPolys[i].GetY(x, z);
      if (h > hmax)
          hmax = h;
    }
  }
  if (found)
  {
    *pResultY = hmax;
  }
  return found;
}

bool HeightServer::QuickIntersectReject(
  const BoundingSphere& bsBefore, 
  const BoundingSphere& bsAfter) const
{
  const Vec3f& vBefore = bsBefore.GetCentre();
  float rBefore = bsBefore.GetRadius();

  const Vec3f& vAfter = bsAfter.GetCentre();
  float rAfter = bsAfter.GetRadius();

//  Assert(fabs(rBefore - rAfter) < SMALLEST);

  // Both spheres on same side ?
  if (vBefore.x + rBefore < m_boundingBox.m_xmin &&
      vAfter.x + rAfter < m_boundingBox.m_xmin)
  {
    return true; // => can reject
  }
  if (vBefore.x - rBefore > m_boundingBox.m_xmax &&
      vAfter.x - rAfter > m_boundingBox.m_xmax)
  {
    return true;
  }

  // Both spheres below bounding box ?
  if (vBefore.y + rBefore < m_boundingBox.m_ymin &&
      vAfter.y + rAfter < m_boundingBox.m_ymin)
  {
    return true; 
  }
  // Both spheres above ?
  if (vBefore.y - rBefore > m_boundingBox.m_ymax &&
      vAfter.y - rAfter > m_boundingBox.m_ymax)
  {
    return true;
  }

  // Both spheres at front or back ?
  if (vBefore.z + rBefore < m_boundingBox.m_zmin &&
      vAfter.z + rAfter < m_boundingBox.m_zmin)
  {
    return true; 
  }

  if (vBefore.z - rBefore > m_boundingBox.m_zmax &&
      vAfter.z - rAfter > m_boundingBox.m_zmax)
  {
    return true;
  }

  return false;
}

// Returns the first intersecting poly
const WallPoly* HeightServer::Intersects(
  const BoundingSphere& bsBefore, 
  const BoundingSphere& bsAfter) const
{
  // TODO test heightserver bounding box against the capsule made by the two
  // spheres, for quick rejection
  // TODO Profile this! Make sure it really is a quick reject!
  if (QuickIntersectReject(bsBefore, bsAfter))
  {
    return 0;
  }

  for (unsigned int i  = 0; i < m_wallPolys.size(); i++)
  {
    if (m_wallPolys[i].Intersects(bsBefore, bsAfter))
    {
      return &(m_wallPolys[i]);
    }
  }
  return 0;
}

// Return all intersecting polys
bool HeightServer::Intersects(
  const BoundingSphere& bsBefore, 
  const BoundingSphere& bsAfter,
  vector<WallPoly>* pResult) const
{
  Assert(pResult->empty());

  // TODO test heightserver bounding box against the capsule made by the two
  // spheres, for quick rejection
  // TODO Profile this! Make sure it really is a quick reject!
  if (QuickIntersectReject(bsBefore, bsAfter))
  {
    return false;
  }

  for (unsigned int i  = 0; i < m_wallPolys.size(); i++)
  {
    if (m_wallPolys[i].Intersects(bsBefore, bsAfter))
    {
      pResult->push_back(m_wallPolys[i]);
    }
  }
  return (!pResult->empty());
}

bool HeightServer::Intersects(
  const Vec3f& v1, 
  const Vec3f& v2, 
  float radius) const
{
  BoundingSphere begin(v1, radius);
  BoundingSphere end(v2, radius);

  bool b = (Intersects(begin, end) != 0);
  return b;
}

bool HeightServer::Intersects(const Mgc::Segment3& seg, float radius) const
{
  // Quick implementation: use the overload of Intersects() which
  // takes 2 bounding spheres.
  // The line seg can be represented by 2 spheres with tiny radii, at the
  // ends of the seg.
  // The advantage of doing it this way (apart from being quick to code)
  // is that the results are totally consistent with the spheres versions
  // of Intersects().
  // TODO undoubtedly faster to use Magic code.

  Vec3f v1(seg.Origin().x, seg.Origin().y, seg.Origin().z);
  Vec3f v2(
    seg.Origin().x + seg.Direction().x, 
    seg.Origin().y + seg.Direction().y, 
    seg.Origin().z + seg.Direction().z);

  return Intersects(v1, v2, radius);

/*
  BoundingSphere begin(
    Vec3f(seg.Origin().x, seg.Origin().y, seg.Origin().z), radius);

  BoundingSphere end(
    Vec3f(
      seg.Origin().x + seg.Direction().x, 
      seg.Origin().y + seg.Direction().y, 
      seg.Origin().z + seg.Direction().z), radius);

  bool b = (Intersects(begin, end) != 0);
  return b;
*/
}

bool HeightServer::IntersectsCeiling(
  const BoundingSphere& bsBefore, 
  const BoundingSphere& bsAfter) const
{
  // TODO test heightserver bounding box against the capsule made by the two
  // spheres, for quick rejection
  // TODO Profile this! Make sure it really is a quick reject!
  if (QuickIntersectReject(bsBefore, bsAfter))
  {
    return false;
  }

  int numWallPolys = m_wallPolys.size();
  for (int i  = 0; i < numWallPolys; i++)
  {
    const WallPoly& w = m_wallPolys[i];
    if (w.IsCeiling() && w.IntersectsCeiling(bsBefore, bsAfter))
    {
      return true;
    }
  }
  return false;
}

void HeightServer::AddHeightServer(const HeightServer& hs)
{
  // Add the entire contents of hs to this HeightServer.
  // We don't Union polys, for speed.
  //std::copy(hs.m_heightPolys.begin(), hs.m_heightPolys.end(), HeightPolyList::back_inserter(m_heightPolys));
  //std::copy(hs.m_wallPolys.begin(), hs.m_wallPolys.end(), HeightPolyList::back_inserter(m_wallPolys));
  
  m_heightPolys.insert(m_heightPolys.end(), hs.m_heightPolys.begin(), hs.m_heightPolys.end());
  m_wallPolys.insert(m_wallPolys.end(), hs.m_wallPolys.begin(), hs.m_wallPolys.end());

  m_boundingBox.Union(hs.m_boundingBox);
}

void HeightServer::AddHeightPoly(const HeightPoly& hp)
{
  m_heightPolys.push_back(hp);
}

void HeightServer::InsertPoly(const HSTri& p)
{
  // Decide which vector the plane should go in.
  if (p.IsHeightPoly())
  {
    HeightPoly hp(p);
    hp.CreateBoundingBox();

    AddHeightPoly(hp);
  }
  else
  {
    WallPoly wp(p);
    m_wallPolys.push_back(wp);
  }

  // Update bounding cuboid
  for (int i = 0; i < 3; i++)
  {
    const float x = p.GetVertex(i).x;
    const float y = p.GetVertex(i).y;
    const float z = p.GetVertex(i).z;
    m_boundingBox.SetIf(x, y, z);
  }
}

#if defined(HS_DEBUG)
void HeightServer::DrawHeights() const
{
  for (unsigned int i = 0; i < m_heightPolys.size(); i++)
  {
    m_heightPolys[i].Draw();
  }
}

void HeightServer::DrawWalls() const
{
  for (unsigned int i = 0; i < m_wallPolys.size(); i++)
  {
    m_wallPolys[i].Draw();
  }
}
#endif

void HeightServer::Clear()
{
  m_heightPolys.clear();
  m_wallPolys.clear();

  m_boundingBox = BoundingBox();
}

// ----------------------------------------------------------------------------

HeightPoly::HeightPoly(const HSTri& p) : HSTri(p)
{
  m_xmin = m_zmin = m_xmax = m_zmax = 0.0;
}

void HeightPoly::CreateBoundingBox()
{
  // This is a good place to rebuild the vector of Mgc verts.
  m_mgcVerts.clear();

  int i = 0;
  // Iterate over vertices.
  for (int j = 0; j < 3; j++)
  {
    const Vec3f& v = m_vertices[j];

    m_mgcVerts.push_back(Mgc::Vector2(v.x, v.z));

    if (i == 0)
    {
        m_xmin = m_xmax = v.x;
        m_zmin = m_zmax = v.z;
        i++;
    }

    if (v.x < m_xmin)
        m_xmin = v.x;

    if (v.x > m_xmax)
        m_xmax = v.x;

    if (v.z < m_zmin)
        m_zmin = v.z;

    if (v.z > m_zmax)
        m_zmax = v.z;
  }
}

float HeightPoly::GetY(float x, float z) const
{
  // This is a plane poly - if it isn't, then this function won't work - but 
  // then opengl won't display it properly either.

  // Equation of plane is ax + by + cz + d = 0. We know a, b, c and d, and
  // are given x and z, so we can solve for y.

  // If m_b were zero, this poly would be vertical - not a HeightPoly.
  Assert(fabs(m_b) > 0);
  return ((m_a * x + m_c * z + m_d) / -m_b);
}

bool HeightPoly::IsIn(float x, float z) const
{
  // Quick bounding box test.
  // TODO If this doesn't speed things up, we may as well get rid of the bounding rectangle.
  if (!IsInBox(x, z))
  {
      return false;
  }

  // Use Mgc point-in-poly test. This should hopefully let us use concave polys.
  Mgc::Vector2 point(x, z);

  int q = m_mgcVerts.size();
  return Mgc::PointInPolygon(q, &m_mgcVerts[0], point);
}

bool HeightPoly::IsInBox(float x, float z) const
{
  // Quickly reject (x, z) if it falls outside the bounding rectangle for the 
  // plan view of this poly.
  return (x >= m_xmin &&
          x <= m_xmax &&
          z >= m_zmin &&
          z <= m_zmax);
}

bool HeightPoly::IsInBox(float xmin, float xmax, float zmin, float zmax) const
{
  // If any of the corners are in the box, we can return true immediately.
  if  (IsInBox(xmin, zmin) ||
       IsInBox(xmin, zmax) ||
       IsInBox(xmax, zmax) ||
       IsInBox(xmax, zmin))
  {
    return true;
  }

  // If the four points given fall outside the box it may still intersect the
  // box!
  if (xmin < m_xmin && xmax > m_xmax && (zmax > m_zmin && zmax < m_zmax || zmin > m_zmin && zmin < m_zmax))
  {
    return true;
  }

  if (zmin < m_zmin && zmax > m_zmax && (xmax > m_xmin && xmax < m_xmax || xmin > m_xmin && xmin < m_xmax))
  {
    return true;
  }

  if (xmin < m_xmin && xmax > m_xmax && zmin < m_zmin && zmax > m_zmax)
  {
    return true;
  }
  return false;
}

bool HeightPoly::BoxesIntersect(const HeightPoly& hp) const
{
  return IsInBox(hp.m_xmin, hp.m_xmax, hp.m_zmin, hp.m_zmax);
}

// ----------------------------------------------------------------------------

WallPoly::WallPoly(const HSTri& p) : HSTri(p)
{
}

bool WallPoly::IsCeiling() const
{
  return m_b < 0.7;
}

bool WallPoly::IntersectsCeiling(
  const BoundingSphere& bsBefore, 
  const BoundingSphere& bsAfter) const
{
  // Special case because the regular intersection code tends to ignore 
  // y-values ?

  return Intersects(bsBefore, bsAfter); // TODO TEMP TEST
}

bool WallPoly::Intersects(
  const BoundingSphere& bsBefore, 
  const BoundingSphere& bsAfter) const
{
  // Make a line from the sphere centres. 
  // Get the closest distance from the line to this wall (triangle).
  // If the distance is less than the sphere radius, the sphere intersects the
  // wall as it moves from the 'before' to the 'after' position.

  // Avoid doing distance test when the spheres can't possibly intersect the plane,
  // or when the first point of contact would be on the back of the plane.
  PositionType posBefore = ClassifyPoint(bsBefore.GetCentre());
  if (posBefore == BACK)
  {
    // If the sphere is initially behind the plane, don't bother with any more tests.
    return false;
  }

  // We can also do a bit more pruning. If the sphere is in front of the
  // plane before and after, and the after position doesn't intersect the plane,
  // it won't intersect, or will only do so from behind.
  // This test makes collision detection/response easier, as you can, e.g. move
  // away from walls, but not into them.

  PositionType posAfter = ClassifyPoint(bsAfter.GetCentre());
  if (posAfter == FRONT)
  {
    if (!Intersects(bsAfter))
    {
      return false;
    }
  }

  float squareRadius = bsBefore.GetRadius() * bsBefore.GetRadius();

  // Make a line segment from the two sphere centres
  Mgc::Segment3 seg;
  seg.Origin() = Mgc::Vector3(bsBefore.x(), bsBefore.y(), bsBefore.z());
  seg.Direction() = Mgc::Vector3(
    bsAfter.x() - bsBefore.x(), 
    bsAfter.y() - bsBefore.y(), 
    bsAfter.z() - bsBefore.z());

  // Make Mgc triangle from wall vertices.
  // TODO Do this once up front!
  Mgc::Triangle3 tri;
  const Vec3f& a = m_vertices[0];
  const Vec3f& b = m_vertices[1];
  const Vec3f& c = m_vertices[2];
  
  tri.Origin() = Mgc::Vector3(a.x, a.y, a.z);
  tri.Edge0() = Mgc::Vector3(b.x - a.x, b.y - a.y, b.z - a.z);
  tri.Edge1() = Mgc::Vector3(c.x - a.x, c.y - a.y, c.z - a.z);

  float squareDist = Mgc::SqrDistance(seg, tri);
  return (squareDist < squareRadius);
}

bool WallPoly::IntersectGetPoint(
  const Vec3f& v1, 
  const Vec3f& v2,
  Vec3f* pIntersectPoint)
{
  // Get the point on the wall poly where the line v1-v2 intersects.
  Mgc::Vector3 pt1(v1.x, v1.y, v1.z);
  Mgc::Vector3 pt2(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
  Mgc::Segment3 seg;
  seg.Origin() = pt1;
  seg.Direction() = pt2;

  Mgc::Triangle3 tri;
  const Vec3f& a = m_vertices[0];
  const Vec3f& b = m_vertices[1];
  const Vec3f& c = m_vertices[2];
  
  tri.Origin() = Mgc::Vector3(a.x, a.y, a.z);
  tri.Edge0() = Mgc::Vector3(b.x - a.x, b.y - a.y, b.z - a.z);
  tri.Edge1() = Mgc::Vector3(c.x - a.x, c.y - a.y, c.z - a.z);

  // Intersect point
  float x = 0, y = 0, z = 0;
 
  float squareDist = Mgc::SqrDistance(seg, tri, &x, &y, &z);
  // We would expect the dist to be zero for an intersect.
  // But the line v1-v2 may not intersect, it may just get close.
  *pIntersectPoint = Vec3f(x, y, z);
#ifdef _DEBUG
std::cout << "WALLPOLY: Get Intersect Point: sq dist: " << squareDist << "\n";
#endif
  return squareDist == 0;
}

float WallPoly::SqDist(const Vec3f& v) const
{
  // Get square dist from point v to this wall.

  Mgc::Vector3 point(v.x, v.y, v.z);

  Mgc::Triangle3 tri;
  const Vec3f& a = m_vertices[0];
  const Vec3f& b = m_vertices[1];
  const Vec3f& c = m_vertices[2];
  
  tri.Origin() = Mgc::Vector3(a.x, a.y, a.z);
  tri.Edge0() = Mgc::Vector3(b.x - a.x, b.y - a.y, b.z - a.z);
  tri.Edge1() = Mgc::Vector3(c.x - a.x, c.y - a.y, c.z - a.z);

  float squareDist = Mgc::SqrDistance(point, tri);
  return squareDist;
}

bool WallPoly::Intersects(const BoundingSphere& bs) const
{
/*
  // We have to work out if the sphere is 'behind' this wall.
  // The normal of this wall is (m_a, m_b, m_c) - but we will ignore m_b.
  Vec3f n(m_a, 0, m_c);
  // Get a vector for the sphere. Try the bounding sphere centre - a vertex of the wall. 
  Vec3f v(bs.GetCentre().x - m_vertices[0].x,
               0, //bs.GetCentre().y() - m_vertices[0].y(),
               bs.GetCentre().z - m_vertices[0].z );
  n.Normalise(); // TODO are these calls necessary ?
  v.Normalise();
  // Get Dot product.
  float dp = Geometry::DotProduct(v, n);
  // If the sphere is behind the poly, the dot product will be < 0.
  // (The reason is that if the sphere is behind, the angle between the plane normal
  // and vector 'v' will be > 90 degrees, so the cosine will be < 0.)

  if (dp <= 0 && ignoreBehind)
  {
    // We can ignore this collision (if the plane and sphere actually do intersect)
    // because the sphere is behind the poly.
    return false;
  }
*/

  // TODO rewrite this to avoid all the conversion from Schmicken types to Mgc types.
  Mgc::Triangle3 tri;
  const Vec3f& a = m_vertices[0];
  const Vec3f& b = m_vertices[1];
  const Vec3f& c = m_vertices[2];
  
  tri.Origin() = Mgc::Vector3(a.x, a.y, a.z);
  tri.Edge0() = Mgc::Vector3(b.x - a.x, b.y - a.y, b.z - a.z);
  tri.Edge1() = Mgc::Vector3(c.x - a.x, c.y - a.y, c.z - a.z);
  
  Mgc::Sphere sphere;
  sphere.Center() = Mgc::Vector3(bs.GetCentre().x, bs.GetCentre().y, bs.GetCentre().z);
  sphere.Radius() = bs.GetRadius();
  
  return Mgc::TestIntersection(tri, sphere);
}

// ----------------------------------------------------------------------------

HSTri::HSTri()
{
  m_a = m_b = m_c = m_d = 0;
}

void HSTri::GetNormal(float pResult[3]) const
{
  pResult[0] = m_a;
  pResult[1] = m_b;
  pResult[2] = m_c;
}

Vec3f HSTri::GetNormal() const
{
  return Vec3f(m_a, m_b, m_c);
}

Geom2d::Line2d HSTri::GetEdge(unsigned int i) const
{
  Assert(i < 3);

  if (i < 2)
  {
    return Geom2d::Line2d(Geom2d::Point2d(m_vertices[i].x, m_vertices[i].z), 
                          Geom2d::Point2d(m_vertices[i + 1].x, m_vertices[i + 1].z));
  }
  return Geom2d::Line2d(Geom2d::Point2d(m_vertices[i].x, m_vertices[i].z), 
                        Geom2d::Point2d(m_vertices[0].x, m_vertices[0].z));
}

void HSTri::AddVertex(const Vec3f& v, unsigned int i)
{
  Assert(i < 3);
  m_vertices[i] = v; 

  // If we now have three points, we can work out the coefficents for the 
  // equation of the plane.
  if (i == 2)
  {
    CreateCoefficients();
  }
}

void HSTri::CreateCoefficients()
{
  //Assert(m_vertices.size() >= 3);
  // Get a, b, c of plane equation by taking the normal given by the first 
  // 3 vertices.
  float v1[] = { m_vertices[0].x, m_vertices[0].y, m_vertices[0].z };
  float v2[] = { m_vertices[1].x, m_vertices[1].y, m_vertices[1].z };
  float v3[] = { m_vertices[2].x, m_vertices[2].y, m_vertices[2].z };
  float r[] = { 0.0, 0.0, 0.0 };

  Geometry::MakeNormal(v1, v2, v3, r);

  m_a = r[0];
  m_b = r[1];
  m_c = r[2];
  // Solve for d:  d = -(ax + by + cz).
  m_d = - m_a * m_vertices[0].x - 
          m_b * m_vertices[0].y - 
          m_c * m_vertices[0].z;
}

bool HSTri::IsHeightPoly() const
{
  // We want to return true if the plane faces upwards, and is more horizontal 
  // than vertical.
  // This is easily found from the y-component of the plane normal.
  return (m_b > 0.7f); // TODO Config
}

#if defined(HS_DEBUG)
void HSTri::Draw() const
{
  AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);
  glBegin(GL_LINES);

  for (int i = 0; i < 3; i++)
  {
    //glColor3f(1.0f, 1.0f, 1.0f);
    float x = m_vertices[i].x;
    float y = m_vertices[i].y;
    float z = m_vertices[i].z;

    if (i < 2)
    {
      // Line from one vertex to the next
      float xb = m_vertices[i + 1].x;
      float yb = m_vertices[i + 1].y;
      float zb = m_vertices[i + 1].z;

      glVertex3f(x, y, z);
      glVertex3f(xb, yb, zb);
    }
    else
    {
      // Line from last vertex to first  
      float xb = m_vertices[0].x;
      float yb = m_vertices[0].y;
      float zb = m_vertices[0].z;

      glVertex3f(x, y, z);
      glVertex3f(xb, yb, zb);
    }
  }

  glEnd();
  AmjuGL::Enable(AmjuGL::AMJU_TEXTURE_2D);
}
#endif

bool HSTri::operator==(const HSTri& rhs) const
{
  // HSTri equality means the coefficients of the plane equations are the same.
  return (fabs(m_a - rhs.m_a) < SMALLEST  &&  
          fabs(m_b - rhs.m_b) < SMALLEST  &&  
          fabs(m_c - rhs.m_c) < SMALLEST  &&  
          fabs(m_d - rhs.m_d) < SMALLEST);
}

HSTri::PositionType HSTri::ClassifyPoint(const Vec3f& v) const
{
  // From Flipcode collision detection tutorial
  float f = Geometry::DotProduct(GetNormal(), v) + m_d;
  if (f > 0)
  {
    return FRONT;
  }
  else if (f < 0)
  {
    return BACK;
  }
  return COINCIDE;
}

bool HSTriLessThan::operator()(const HSTri& lhs, const HSTri& rhs)
{
  if (lhs.m_a < (rhs.m_a - SMALLEST))
  {
    return true;
  }

  if (lhs.m_a > (rhs.m_a + SMALLEST))
  {
    return false;
  }

  if (lhs.m_b < (rhs.m_b - SMALLEST))
  {
    return true;
  }

  if (lhs.m_b > (rhs.m_b + SMALLEST))
  {
    return false;
  }
  
  if (lhs.m_c < (rhs.m_c - SMALLEST))
  {
    return true;
  }

  if (lhs.m_c > (rhs.m_c + SMALLEST))
  {
    return false;
  }

  return lhs.m_d < (rhs.m_d - SMALLEST);
}

}


