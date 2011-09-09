/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: BoundingSphere.cpp,v $
Revision 1.1.10.3  2007/03/06 19:29:06  jay
Glut fix

Revision 1.1.10.2  2005/06/13 22:09:15  jay
Fix capsule intersect test

Revision 1.1.10.1  2005/03/17 20:48:01  jay
Get Draw() to work

Revision 1.1  2004/09/08 15:42:45  jay
Added to repository
  
*/

// This source code originally written by JASON COLMAN 2000-2003. 
// You may use it as you wish, at your own risk!  jason@amju.com.

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <math.h>
#include "BoundingSphere.h"
#include "SolidLeaf.h"
#include "Common.h"
#include "File.h"
#include "Mgc/MgcCont3DSphere.h" // merge 2 spheres
#include "Engine.h" // needed for debug drawing
#include "Mgc/MgcIntr3DLinSphr.h" // line seg/sphere intersect
#include "SchAssert.h"

namespace Amju
{
bool BoundingSphere::s_draw = false;

BoundingSphere::BoundingSphere() : m_r(0.0f)
{
}

BoundingSphere::BoundingSphere(Vec3f v, float radius) : m_v(v), m_r(radius)
{
}

bool BoundingSphere::Load(File* pf)
{
  float x = 0;
  float y = 0;
  float z = 0;
  float r = 0;
  if (!pf->GetFloat(&x) || !pf->GetFloat(&y) || !pf->GetFloat(&z))
  {
    pf->ReportError("Failed to load bounding sphere centre");
    return false;
  }
  if (!pf->GetFloat(&r))
  {
    pf->ReportError("Failed to load bounding sphere radius");
    return false;
  }
  SetRadius(r);
  SetCentre(Vec3f(x, y, z));
  return true;
}

#ifdef SCENE_EDITOR
bool BoundingSphere::Save(File* pf)
{
  pf->WriteComment("// Sphere x, y, z, r");
  pf->WriteFloat(m_v.x);
  pf->WriteFloat(m_v.y);
  pf->WriteFloat(m_v.z);
  pf->WriteFloat(m_r);
  return true;
}
#endif

void BoundingSphere::Draw()
{
#if defined(_DEBUG)
  if (s_draw)
  {
    AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);
    // Save current colour
    Engine::Instance()->PushColour(1.0f, 1.0f, 1.0f, 1.0f);
    //glColor3f(1.0f, 1.0f, 1.0f);
    //glColor3f(0, 0, 0);

    AmjuGL::PushMatrix();
    AmjuGL::Translate(m_v.x, m_v.y, m_v.z);

#if defined(USE_GLUT)
    // Use glut wire sphere function.
    glutWireSphere(m_r, 16, 16); // radius, slices, stacks
#else
    // TODO non-GLUT write sphere
#endif

    AmjuGL::PopMatrix();

    // Restore current colour
    Engine::Instance()->PopColour();
    AmjuGL::Enable(AmjuGL::AMJU_TEXTURE_2D);
  }
#endif
}

void BoundingSphere::SetCentre(const Vec3f& v)
{
  m_v = v;
}

void BoundingSphere::SetRadius(float r)
{
  m_r = r;
}
 
const Vec3f& BoundingSphere::GetCentre() const 
{
  return m_v;
}

float BoundingSphere::GetRadius() const
{
  return m_r;
}

bool BoundingSphere::Intersects(
  const Vec3f& v1, 
  const Vec3f& v2, 
  float r) const
{
  Mgc::Segment3 seg;
  seg.Origin() = Mgc::Vector3(v1.x, v1.y, v1.z);
  seg.Direction() = Mgc::Vector3(v2.x - v1.x, v2.y - v1.y, v2.z - v1.z);
  return Intersects(seg, r);
}

bool BoundingSphere::Intersects(const Mgc::Segment3& seg, float r) const
{
  // r is radius of line seg - i.e. it's really a capsule.
  // take account of r, i.e. we want sphere-capsule intersection test.
  // Add r to the sphere radius. The intersection test is that the distance
  // between line and sphere centre is less than the combined radii....
  // so this should work, right ?

  Mgc::Sphere s;
  s.Center() = Mgc::Vector3(m_v.x, m_v.y, m_v.z);
  s.Radius() = m_r + r;
  return Mgc::TestIntersection(seg, s); 
}

bool BoundingSphere::Intersects(const BoundingSphere& s) const
{
  // Get distance between vertices. If it is less than the sum of the radii,
  // the spheres intersect.
  if (fabs(m_r) < SMALLEST || fabs(s.m_r) < SMALLEST)
  {
    return false;
  } 

  float x = s.m_v.x - m_v.x;
  float y = s.m_v.y - m_v.y;
  float z = s.m_v.z - m_v.z;
  float dsquared = x * x + y * y + z * z;
  float rsquared = (s.m_r + m_r) * (s.m_r + m_r);
  bool intersects = dsquared < rsquared;
  return intersects;
}

BoundingSphere& BoundingSphere::operator+=(const BoundingSphere& s) 
{
  // TODO this is the wrong way around!
  // operator+ should be defined in terms of op+= 
  *this = *this + s;
  return *this;
}

BoundingSphere operator+(const BoundingSphere& lhs, const BoundingSphere& rhs)
{
  if (rhs.GetRadius() == 0)
  {
    return lhs;
  }

  Mgc::Sphere s1;
  s1.Radius() = lhs.GetRadius();
  s1.Center() = Mgc::Vector3(lhs.GetCentre().x, lhs.GetCentre().y, lhs.GetCentre().z);
  Mgc::Sphere s2;
  s2.Radius() = rhs.GetRadius();
  s2.Center() = Mgc::Vector3(rhs.GetCentre().x, rhs.GetCentre().y, rhs.GetCentre().z);
  
  Mgc::Sphere res = Mgc::MergeSpheres(s1, s2);

  Vec3f centre(res.Center().x, res.Center().y, res.Center().z);
  return BoundingSphere(centre, res.Radius());
}

void BoundingSphere::SetFromLeaf(const SolidLeaf& leaf)
{
  // Iterate through the polygons for this leaf. Find the min x, y, z and 
  // max x, y, z. This gets a bounding box. The sphere centre is the centre
  // of the box. The radius is the distance from the centre to the furthest 
  // vertex.
  // This algorithm is from Game Programming Gems.
  // NB We use absolute coords here. So we depend on RecalculateAbsoluteCoords()
  // to multiply the relative dcoords by the current transformation matrix
  // first.
  int numPolys = leaf.GetNumberOfPolygons();
  if (numPolys < 1)
  {
    return;
  }

  float minx = 10000.0f;
  float miny = 10000.0f;
  float minz = 10000.0f;
  float maxx = -10000.0f;
  float maxy = -10000.0f;
  float maxz = -10000.0f;

  // Iterate through all vertices to get the bounding box.
  int i;
  for (i = 0; i < numPolys; i++)
  {
    Polygon* pPoly = const_cast<SolidLeaf&>(leaf).GetPolygon(i);
    int numVertices =  pPoly->Vertices();
    for (int j = 0; j < numVertices; j++)
    {
      const SceneVertex& sv = const_cast<const Polygon*>(pPoly)->GetVertex(j);

      if (sv.GetAbsX() < minx)
        minx = sv.GetAbsX();

      if (sv.GetAbsX() > maxx)
        maxx = sv.GetAbsX();
      
      if (sv.GetAbsY() < miny)
        miny = sv.GetAbsY();
      
      if (sv.GetAbsY() > maxy)
        maxy = sv.GetAbsY();

      if (sv.GetAbsZ() < minz)
        minz = sv.GetAbsZ();

      if (sv.GetAbsZ() > maxz)
        maxz = sv.GetAbsZ();
    }
  }

  // Sphere centre is centre of box.
  m_v = Vec3f((maxx + minx)/2.0f, (maxy + miny)/2.0f, (maxz + minz)/2.0f);

  // Iterate again to get the radius. Get distance from centre to each point.
  float rsquared = 0;

  for (i = 0; i < numPolys; i++)
  {
    const Polygon* pPoly = const_cast<SolidLeaf&>(leaf).GetPolygon(i);
    int numVertices =  pPoly->Vertices();
    for (int j = 0; j < numVertices; j++)
    {
      const SceneVertex& sv = pPoly->GetVertex(j);
      float x = sv.GetAbsX() - m_v.x;
      float y = sv.GetAbsY() - m_v.y;
      float z = sv.GetAbsZ() - m_v.z;
      float dsquared = x * x + y * y + z * z;
      if (dsquared > rsquared)
      {
        rsquared = dsquared;
      }
    }
  }
  m_r = sqrt(rsquared);
}

bool operator==(const BoundingSphere& lhs, const BoundingSphere& rhs)
{
  return (lhs.m_v == rhs.m_v && lhs.m_r == rhs.m_r);
}

}

