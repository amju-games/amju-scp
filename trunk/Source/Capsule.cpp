/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: Capsule.cpp,v $
Revision 1.1.2.4  2005/09/06 17:19:52  jay
Added IsSphere()

Revision 1.1.2.3  2005/08/26 21:07:38  jay
Tidy-up code

Revision 1.1.2.2  2005/07/30 12:10:38  jay
Capsule-capsule and capsule-sphere tests now work reliably

Revision 1.1.2.1  2005/05/08 08:22:07  jay
New bounding volume class for better collision detection.
Initially used for POOL to catch collisions between fast balls, where
sphere-sphere intersection test fails before and after collision.

*/

#include <iostream>
#include "Capsule.h"
#include "BoundingSphere.h"
#include "Mgc/MgcDist3DLinLin.h"
#include "Mgc/MgcDist3DVecLin.h"
#include "StringUtils.h"

namespace Amju
{
std::string ToString(const Capsule& c)
{
  std::string s;

  s += ToString(c.m_v1);
  s += " - ";
  s += ToString(c.m_v2);
  s += " r: ";
  s += ToString(c.m_radius);

  return s;
}

Capsule::Capsule(const Vec3f& v1, const Vec3f& v2, float radius) :
  m_v1(v1), m_v2(v2), m_radius(radius)
{
}

//#define CAPSULE_DEBUG
//#define CAP_SPHERE

bool Capsule::IsSphere() const
{
  return (m_v1 == m_v2);
}

bool Capsule::Intersects(const BoundingSphere& rhs) const
{
  Mgc::Segment3 line1;
  line1.Origin() = Mgc::Vector3(m_v1.x, m_v1.y, m_v1.z);
  float d1x = m_v2.x - m_v1.x;
  float d1y = m_v2.y - m_v1.y;
  float d1z = m_v2.z - m_v1.z;
  line1.Direction() = Mgc::Vector3(d1x, d1y, d1z);
  // Line length zero ? => sphere, not capsule
  bool c1Sphere = (d1x == 0 && d1y == 0 && d1z == 0);

  float rsq = (m_radius + rhs.GetRadius());
  rsq *= rsq;
  float dsq = 0;
  if (c1Sphere)
  {
    dsq = (m_v1 - rhs.GetCentre()).SqLen();
  }
  else 
  {
    const Vec3f& v = rhs.GetCentre();
    dsq = Mgc::SqrDistance(Mgc::Vector3(v.x, v.y, v.z), line1);
#ifdef CAP_SPHERE
//std::cout << " MGC: Dist SQ between lines: " << dsq << ": dist: " << sqrt(dsq) << "\n";
#endif
  }

  bool b = dsq < rsq;

#ifdef CAP_SPHERE
if (b)
std::cout << " Capsule intersect: Capsule (" 
  << ToString(*this).c_str() << ") \n   and sphere (" 
  << ToString(rhs.GetCentre()).c_str() << ") "
  << (b ? " *do* " : " DO NOT ") << " intersect.\n";
#endif

  return b;
}

bool Capsule::Intersects(const Capsule& rhs) const
{
  Mgc::Segment3 line1;
  line1.Origin() = Mgc::Vector3(m_v1.x, m_v1.y, m_v1.z);
  float d1x = m_v2.x - m_v1.x;
  float d1y = m_v2.y - m_v1.y;
  float d1z = m_v2.z - m_v1.z;
  line1.Direction() = Mgc::Vector3(d1x, d1y, d1z);
  // Line length zero ? => sphere, not capsule
  bool c1Sphere = (d1x == 0 && d1y == 0 && d1z == 0);

  Mgc::Segment3 line2;
  line2.Origin() = Mgc::Vector3(rhs.m_v1.x, rhs.m_v1.y, rhs.m_v1.z);
  float d2x = rhs.m_v2.x - rhs.m_v1.x;
  float d2y = rhs.m_v2.y - rhs.m_v1.y;
  float d2z = rhs.m_v2.z - rhs.m_v1.z;
  line2.Direction() = Mgc::Vector3(d2x, d2y, d2z);
  // Line length zero ? => sphere, not capsule
  bool c2Sphere = (d2x == 0 && d2y == 0 && d2z == 0);

  float rsq = (m_radius + rhs.m_radius);
  rsq *= rsq;
  float dsq = 0;
  if (!c1Sphere && !c2Sphere)
  {
    dsq = Mgc::SqrDistance(line1, line2);
  }
  else if (c1Sphere && !c2Sphere)
  {
    dsq = Mgc::SqrDistance(line1.Origin(), line2);
  }
  else if (!c1Sphere && c2Sphere)
  {
    dsq = Mgc::SqrDistance(line2.Origin(), line1);
  }
  else // c1Sphere && c2Sphere
  {
    dsq = (m_v1 - rhs.m_v1).SqLen();
  }

#ifdef CAPSULE_DEBUG
std::cout << "MGC: Dist SQ between lines: " << dsq << ": dist: " << sqrt(dsq) << "\n";
#endif

  bool b = dsq < rsq;

#ifdef CAPSULE_DEBUG
std::cout << "Capsule intersect: Capsules (" 
  << ToString(*this).c_str() << ") \n   and (" << ToString(rhs).c_str() << ") "
  << (b ? " *do* " : " DO NOT ") << " intersect.\n";
#endif

  return b;
}

}



