/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: SolidComponent.h,v $
Revision 1.1  2004/09/08 15:42:56  jay
Added to repository
  
*/

#if !defined(SOLID_COMPONENT_H_INCLUDED)
#define SOLID_COMPONENT_H_INCLUDED

#include <string>
#include <vector>
#include "Orientation.h"
#include "Matrix.h"
#include "BoundingSphere.h"
#include "SharedPtr.h"
#include "Mgc/MgcSegment3.h"

namespace Amju
{
class File;
class HeightServer;
class Frustum;
class HeightServer;
class VisibilityGraph;
class SolidComponent;
class BoundingBox;

// Smart pointer to Solid Component
typedef SharedPtr<SolidComponent> PSolidComponent;
typedef std::vector<SharedPtr<SolidComponent> > CompVector;

class SolidComponent : public Shareable
{
public:
  SolidComponent();
  virtual ~SolidComponent();

  // Pure virtuals
  // -------------

  // Create a deep copy of this object on the heap.
  virtual PSolidComponent Clone() = 0;

  virtual void Draw() = 0;

  // Draw, but cull either this object or any child objects which are outside
  // of the given view frustum. Returns the number of polygons drawn, to 
  // compare with total number of polys in scene. 
  // NB Release version can simply return 0.
  virtual int CullDraw(const Frustum& f) = 0;

  virtual bool Load(const std::string& filename) = 0;

  virtual int GetNumberOfPolygons() const = 0;

  // Virtuals
  // --------

  // Update the solid, whether or not it is drawn.
  virtual void Update() {}

  // Store heights of polygons of leaves, using absolute coords.
  virtual void StoreHeights(const Matrix& cm) {}

  // Add heights to the given HeightServer if we intersect the given
  // bounding sphere.
  virtual void AddHeights(HeightServer* pResult, const BoundingSphere& bs) {}

  // Call this if an orientation is changed. Used by SceneEditor.
  virtual void RecalculateAbsoluteCoords(const Matrix& cm) {}

  // Create a Bounding Sphere for the Solid. This uses absolute coords,
  // so uses a transformation matrix. Pass in the Identity matrix at
  // the top level call. This is transformed and passed to each child
  // recursively.
  virtual void CreateBoundingSphere(const Matrix& cm) {}

  // Get Bounding Sphere of Solid. Override for Decorators, where you need
  // to return the Bounding Sphere of the contained Solid.
  virtual BoundingSphere* GetBoundingSphere() { return &m_bsphere; }

  // Used by Scene Editor. TODO get rid of this ?
  virtual bool IsLeaf() const { return false; }

  virtual void DrawSphere() {}; 

  // Returns true if the given line segment intersects one or more polygons.
  // r is the 'radius' of the line seg, i.e. it's really a capsule.
  // This is useful because a point can be reachable for a small object, but
  // not reachable for a larger object.
  //  
  // Does a rough bounding sphere test first, but is then polygon-accurate.
  virtual bool LineIntersects(const Mgc::Segment3& seg, float r) const { return false; }

  // Add visibility points to graph. This is used for path finding.
  virtual void AddVisibilityPoint(VisibilityGraph* ) const {}

  // Returns true if the given bounding box intersects a bounding box in this
  // tree.
  virtual bool BoxIntersects(const BoundingBox&) const { return false; }


  // Non-virtuals
  // ------------

  // Calls virtual LineIntersects() above. This just converts 2 vertices
  // to a Line Seg.
  bool LineIntersects(const VertexBase& v1, const VertexBase& v2, float r) const;


  // Issue gl commands to translate and rotate.
  void Orientate() { m_or.Draw(); }

  bool LoadOrientation(File* jf) { return m_or.Load(jf); }
  
  void SetOrientation(const Orientation& orientation) { m_or = orientation; }

  Orientation* GetOrientation() { return &m_or; }


  // Helper function to load a Solid from a filename. This is used in a lot
  // of places: this function should be used each time to avoid copied code.
  // Also allows some optimisations, like caching.
  static PSolidComponent LoadSolid(const std::string& name);

protected:
  // Orientation (translation and rotation) for this component
  Orientation m_or;

  // Each component has a bounding sphere. For a leaf, the sphere contains all 
  // the vertices. For a composite, the sphere contains all the child spheres.
  BoundingSphere m_bsphere;

public:
   // Get name
  std::string GetName() const { return m_name; }
  // Set - used by SceneEditor.
  void SetName(const std::string& name) { m_name = name; }

protected:
  std::string m_name;


#if defined(SCENE_EDITOR)
public:
  // Save composite/leaf to a file. The filename is set by SetName().
  // If recursive is true, then child Components are saved, etc.
  virtual bool Save(bool recursive = false) { return false; }

  bool IsDirty() const { return m_isDirty; }
  void SetDirty(bool b) { m_isDirty = b; }

  bool SaveOrientation(File* jf) { return m_or.Save(jf); }

protected:
  // For editing, this flag is set when the Component should be saved.
  bool m_isDirty;

#endif
};
}
#endif

