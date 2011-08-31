/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: SolidComposite.h,v $
Revision 1.1  2004/09/08 15:42:56  jay
Added to repository
  
*/

#if !defined(SOLID_COMPOSITE_H_INCLUDED)
#define SOLID_COMPOSITE_H_INCLUDED

#include "SolidComponent.h"
#include <string>

namespace Amju
{
class File;

class SolidComposite : public SolidComponent
{
public:
  SolidComposite();
  virtual ~SolidComposite();

  virtual PSolidComponent Clone();

  virtual void Update();

  virtual void Draw();
  virtual void DrawSphere();
  virtual int CullDraw(const Frustum& f);

  virtual bool Load(const std::string& filename);

  virtual int GetNumberOfPolygons() const;

  virtual void StoreHeights(const Matrix& cm);

  // Add heights to the given HeightServer if we intersect the given
  // bounding sphere.
  virtual void AddHeights(HeightServer* pResult, const BoundingSphere& bs);

  virtual void CreateBoundingSphere(const Matrix& cm);

  virtual void RecalculateAbsoluteCoords(const Matrix& cm);

  virtual bool LineIntersects(const Mgc::Segment3& seg, float r) const;

  virtual bool BoxIntersects(const BoundingBox&) const;


  // Non-virtuals: manage collection of children
  // -------------------------------------------
  // Add the given pointer to the back of the vector of pointers.
  void AddComponent(PSolidComponent pComp);

  // Delete the pointer from the vector of components - doesn't delete
  // the object pointed to. Returns false if the pointer is not in the vector.
  bool DeleteComponent(SolidComponent* pComp);

  // Return child given by (zero-based) index, or 0 if index is invalid.
  PSolidComponent GetChild(int index);

  int GetNumChildren();
 
protected:
  // Copy ctor, used by Clone()
  SolidComposite(const SolidComposite& rhs);

protected:
  CompVector m_children;

#if defined(SCENE_EDITOR)
public:
  virtual bool Save(bool recursive = false);
#endif
};
}
#endif
