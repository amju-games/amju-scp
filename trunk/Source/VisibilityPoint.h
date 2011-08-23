/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: VisibilityPoint.h,v $
Revision 1.1  2004/09/08 15:43:00  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_VISIBILITY_POINT_H_INCLUDED)
#define SCHMICKEN_VISIBILITY_POINT_H_INCLUDED

#include "SolidComponent.h"

namespace Amju
{
// A non-visible marker used to create a visibility graph.
// VisibilityPoints in the composite scene for a room are stripped out
// once the graph has been created.
// By including them in this class heirarchy, we can use Scene Editor
// to add them to a scene.
class VisibilityPoint : public SolidComponent
{
public:
  virtual void AddVisibilityPoint(VisibilityGraph* ) const;

  virtual PSolidComponent Clone();
  virtual void Draw();
  virtual int CullDraw(const Frustum& f);

  virtual bool Load(const std::string& filename);

#ifdef SCENE_EDITOR
  virtual bool Save(bool recursive = false);
#endif

  virtual int GetNumberOfPolygons() const  { return 0; }

  virtual void StoreHeights(const Matrix& cm);
  virtual void RecalculateAbsoluteCoords(const Matrix& cm);
  virtual void CreateBoundingSphere(const Matrix& cm);

protected:
  VertexBase m_v; // untransformed vertex
}; 
}

#endif
