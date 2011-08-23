/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: SolidGameObject.h,v $
Revision 1.1.10.2  2005/07/17 22:13:54  Administrator
Fixed bad function sig for GetOrientation()

Revision 1.1.10.1  2005/06/24 05:37:42  Administrator
Fix GetOrientation() - added const

Revision 1.1  2004/09/08 15:42:56  jay
Added to repository
  
*/

#if !defined(SOLID_GAME_OBJECT_H_INCLUDED)
#define SOLID_GAME_OBJECT_H_INCLUDED

#include "VisibleGameObject.h"
#include "SolidComponent.h"

namespace Amju
{
class File;
class Matrix;

// Common base for Visible Game Objects which have 'SolidComponent'-based solid.
class SolidGameObject : public VisibleGameObject
{
public:
  SolidGameObject();
  virtual ~SolidGameObject();

  virtual void Draw();
  virtual void Update();

  virtual void CreateBoundingSphere(const Matrix& cm);
  virtual const BoundingSphere* GetBoundingSphere() const;
  virtual void SetOrientation(const Orientation&);
  virtual const Orientation* GetOrientation();
  virtual void DrawEditSolid();

  PSolidComponent GetSolid() { return m_pSolid; }

  void SetSolid(PSolidComponent pSolid);

protected:
  virtual void RecalcCollisionVol();

protected:
  PSolidComponent m_pSolid;

#ifdef SCENE_EDITOR
public:
  virtual bool Save(File*);
#endif
};
}
#endif

