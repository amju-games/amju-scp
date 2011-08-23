/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: MovingSolid.h,v $
Revision 1.1.10.1  2005/05/15 17:12:33  jay
Set child/function in code, not just Scene Ed

Revision 1.1  2004/09/08 15:42:53  jay
Added to repository
  
*/

#if !defined(MOVING_SOLID_H_INCLUDED)
#define MOVING_SOLID_H_INCLUDED

#include "HeightServer.h"
#include "SolidComponent.h"
#include "Function.h"
#include <string>

namespace Amju
{
class File;
class Matrix;

// Decorates a SolidComponent to move it using a Function.
class MovingSolid : public SolidComponent
{
public:
  MovingSolid();
  MovingSolid(const MovingSolid&);

  virtual ~MovingSolid();
  virtual PSolidComponent Clone();

  virtual bool Load(const std::string& filename);
  virtual void StoreHeights(const Matrix& cm);
  virtual void Draw();
  virtual void Update();
  virtual void DrawSphere();
  virtual int CullDraw(const Frustum& f);
  virtual int GetNumberOfPolygons() const;
  virtual void CreateBoundingSphere(const Matrix& cm);
  virtual BoundingSphere* GetBoundingSphere();
  virtual void RecalculateAbsoluteCoords(const Matrix& cm);
  virtual void AddHeights(HeightServer* pResult, const BoundingSphere& bs);
  // Test the line segment against the current position of the moving solid.
  // Obviously the result will change as the solid moves.
  virtual bool LineIntersects(const Mgc::Segment3& seg, float r) const;

  PSolidComponent GetInnerSolid();

  // Get the difference in Orientation between Update()s.
  void GetOrientationDelta(Orientation* pOrientation);

  // Get the function controlling movement.
  PFunction GetFunction() { return m_pFunc; }
  
  void SetChildSolid(PSolidComponent pChild) { m_pSolid = pChild; }

  void SetFunction(PFunction pFunc) { m_pFunc = pFunc; }

protected:
  bool Load(File* pf);

protected:
  // The visible solid.
  PSolidComponent m_pSolid;

  // Function determining the movement. 
  PFunction m_pFunc;

  // Difference in orientation between Update()s.
  Orientation m_orientationDelta;

#if defined(SCENE_EDITOR)
public:
  virtual bool Save(bool recursive);
#endif
};
}
#endif

