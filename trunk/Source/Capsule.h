/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: Capsule.h,v $
Revision 1.1.2.3  2005/09/06 17:19:52  jay
Added IsSphere()

Revision 1.1.2.2  2005/07/30 12:09:57  jay
Add capsule-sphere intersect test

Revision 1.1.2.1  2005/05/08 08:22:09  jay
New bounding volume class for better collision detection.
Initially used for POOL to catch collisions between fast balls, where
sphere-sphere intersection test fails before and after collision.

*/

#ifndef CAPSULE_H_INCLUDED
#define CAPSULE_H_INCLUDED

#include <string>
#include "Vertex.h"

namespace Amju
{
class BoundingSphere;

// Capsule is formed by moving a Sphere along a straight line.
class Capsule
{
friend std::string ToString(const Capsule&);

public:
  Capsule(const VertexBase& v1, const VertexBase& v2, float radius);

  bool IsSphere() const;

  // Capsule-capsule intersection test
  bool Intersects(const Capsule&) const;

  bool Intersects(const BoundingSphere&) const;

  // TODO Intersection with other solids

protected:
  VertexBase m_v1;
  VertexBase m_v2;
  float m_radius; 
};
}

#endif


