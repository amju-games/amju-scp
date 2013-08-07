/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: SolidLeafCharacterMesh.h,v $
Revision 1.1  2004/09/08 15:42:57  jay
Added to repository
  
*/

#if !defined(SOLID_LEAF_MD2_H_INCLUDED)
#define SOLID_LEAF_MD2_H_INCLUDED

#include "SolidComponent.h"
#include "Character.h"

namespace Amju
{
// This kind of leaf uses a character model.
// This is to make it easy to import, say, an MD2 model and use it as scenery.
// Other character formats may be used too.
class SolidLeafCharacterMesh : public SolidComponent
{
public:
  virtual ~SolidLeafCharacterMesh();

  virtual PSolidComponent Clone();
  virtual void Draw();
  virtual void DrawSphere();
  virtual int CullDraw(const Frustum& f);
  virtual bool Load(const std::string& filename);
  virtual int GetNumberOfPolygons() const;
  virtual void StoreHeights(const Matrix& cm);
  virtual void AddHeights(HeightServer* pResult, const BoundingSphere& bs);
  virtual bool IsLeaf() const;
  virtual void CreateBoundingSphere(const Matrix& cm);
  virtual bool LineIntersects(const Mgc::Segment3& seg, float r) const;
  virtual void RecalculateAbsoluteCoords(const Matrix& cm);
  virtual bool BoxIntersects(const BoundingBox& b) const;

#if defined(SCENE_EDITOR)
public:
  virtual bool Save(bool recursive = false);
#endif

protected:
  RCPtr<Character> m_pMesh;
  std::string m_meshName;

  // A simple bounding volume. This is used for collision detection, 
  // rather than testing every face of the character model.
  PSolidComponent m_pSimpleSolid;
};
}

#endif

