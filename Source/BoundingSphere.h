/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: BoundingSphere.h,v $
Revision 1.1  2004/09/08 15:42:45  jay
Added to repository
  
*/

#if !defined(BOUNDING_SPHERE_H_INCLUDED)
#define BOUNDING_SPHERE_H_INCLUDED

#include "Vertex.h"
#include "Mgc/MgcSegment3.h" // line seg, for intersection test

namespace Amju
{
class HSTri;
class SolidLeaf;
class File;

class BoundingSphere
{
public:
  // Set/get drawing flag. If the flag is set to true, ALL bounding spheres are
  // drawn.
  static void SetDraw(bool b) { s_draw = b; }
  static bool GetDraw() { return s_draw; }

  // Return a sphere which contains both sphere arguments.
  friend BoundingSphere operator+(const BoundingSphere& lhs, 
                                  const BoundingSphere& rhs);

  friend bool operator==(const BoundingSphere& lhs, const BoundingSphere& rhs);

  // Default sphere - unit radius at origin.
  BoundingSphere();
  // Sphere is defined by a vertex and a radius.
  BoundingSphere(Vec3f v, float radius);

  // Draw sphere - used for debugging.
  void Draw();

  // Given a pointer to a leaf data object, set centre and radius of sphere.
  void SetFromLeaf(const SolidLeaf&);

  // Enlarge the sphere to enclose the new sphere s as well as the original
  // volume.
  BoundingSphere& operator+=(const BoundingSphere& s);

  void SetCentre(const Vec3f& v);
  void SetRadius(float r);

  const Vec3f& GetCentre() const;
  float GetRadius() const;

  // Get the x, y, z of the centre.
  float x() const { return m_v.x; }
  float y() const { return m_v.y; }
  float z() const { return m_v.z; }

  // Return true if this sphere intersects the argument sphere.
  bool Intersects(const BoundingSphere& s) const;

  // Return true if this sphere intersects the given line segment.
  // r is the line segment radius - so this is really a capsule.
  bool Intersects(const Mgc::Segment3& seg, float r) const;

  // As above, but using 2 Vec3fs as the segment end points.
  bool Intersects(const Vec3f& v1, const Vec3f& v2, float r) const;

  // Load b. sphere from a file.
  bool Load(File* pf);

#ifdef SCENE_EDITOR
  bool Save(File* pf);
#endif

protected:
  Vec3f m_v;
  float m_r;

  static bool s_draw;
};
}
#endif

