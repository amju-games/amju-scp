/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: BoundingBox.h,v $
Revision 1.1  2004/09/08 15:42:45  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_BOUNDING_BOX_H_INCLUDED)
#define SCHMICKEN_BOUNDING_BOX_H_INCLUDED

namespace Amju
{
class BoundingSphere;
class SolidComponent;
class SolidComposite;
class SolidLeaf;
class Matrix;

// Axis-aligned cuboid, for intersection tests where a box is more
// suitable than a sphere.
class BoundingBox
{
public:
  BoundingBox(
    float xmin = 1000.0f, 
    float xmax = -1000.0f, 
    float ymin = 1000.0f, 
    float ymax = -1000.0f, 
    float zmin = 1000.0f, 
    float zmax = -1000.0f);

  void Set(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax);
  
  // Set values which are lower than the mins or greater than maxes.
  // I.e. enlarge the box to contain (x, y, z) if necessary.
  void SetIf(float x, float y, float z);

  bool Intersects(const BoundingBox&) const;
  bool Intersects(const BoundingSphere&) const;

  // Enlarge this box to contain b, if necessary.
  void Union(const BoundingBox& b);

  // Fit the box around the given Solid.
  void SetFromSolid(SolidComponent*);
 
  void SetFromSphere(const BoundingSphere&);

  // Get sphere enclosing box
  BoundingSphere GetBoundingSphere() const;

  void Translate(float dx, float dy, float dz);

  void RotateY(float ydegs);

  // Draw box using quads. Used for picking, so not just DEBUG.
  void DrawSolid() const;

#if defined(_DEBUG)
  void Draw() const;
#endif

protected:
  void SetFromLeaf(SolidLeaf* pLeaf);
  void SetFromComposite(SolidComposite* pComp);

public:
  // No point in making this data private
  float m_xmin, m_xmax, m_ymin, m_ymax, m_zmin, m_zmax;
};
}

#endif

