/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Text3dLeaf.h,v $
Revision 1.1  2004/09/08 15:42:57  jay
Added to repository
 
*/

#if !defined (SCHMIEKN_TEXT_3D_LEAF_H_INCLUDED)
#define SCHMIEKN_TEXT_3D_LEAF_H_INCLUDED

#include <string>
#include "TextLeaf.h"
#include "SolidComposite.h"

namespace Amju
{
class Text3dLeaf : public SolidComposite
{
public:
  Text3dLeaf();
  Text3dLeaf(const char *); // create from string

  virtual PSolidComponent Clone();

  virtual bool Load(File* pf);
  virtual bool Load(const std::string& filename);
};

/*
class Text3dLeaf : public TextLeaf
{
public:
  Text3dLeaf();
  Text3dLeaf(const char *); // create from string
  Text3dLeaf(const Text3dLeaf&);

  virtual PSolidComponent Clone();
  virtual void Draw();
  virtual int CullDraw(const Frustum& f);
  virtual int GetNumberOfPolygons() const;
  virtual bool Load(File* pf);

  
  virtual void Update() { m_pSolid->Update(); }
  virtual bool Save(bool recursive = false) { return false; }
  virtual void StoreHeights(const Matrix& cm) { m_pSolid->StoreHeights(cm); }
  virtual void AddHeights(HeightServer* pResult, const BoundingSphere& bs) { m_pSolid->AddHeights(pResult, bs); }
  virtual void RecalculateAbsoluteCoords(const Matrix& cm) { m_pSolid->RecalculateAbsoluteCoords(cm); }
  virtual void CreateBoundingSphere(const Matrix& cm) { m_pSolid->CreateBoundingSphere(cm); }
  virtual BoundingSphere* GetBoundingSphere() { return m_pSolid->GetBoundingSphere(); }
  virtual bool IsLeaf() const { return false; }
  virtual void DrawSphere() { m_pSolid->DrawSphere(); } 

  PSolidComponent GetChild() { return m_pSolid; }

protected:
  PSolidComponent m_pSolid;
};
*/
}

#endif

