/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: FreeMovingSolidGameObject.h,v $
Revision 1.1.10.2  2006/04/13 18:21:51  jay
Added RecalcCollisionVol for low-poly collision vol if one is available.

Revision 1.1.10.1  2005/05/08 08:10:20  jay
Fix GetOrientation() - now returns const

Revision 1.1  2004/09/08 15:42:49  jay
Added to repository
  
*/

#if !defined(FREE_MOVING_SOLID_GAME_OBJECT_H_INCLUDED)
#define FREE_MOVING_SOLID_GAME_OBJECT_H_INCLUDED

#include "FreeMovingGameObject.h"
#include "SolidComponent.h"

namespace Amju
{
class FreeMovingSolidGameObject : 
  public FreeMovingGameObject
{
public:
  FreeMovingSolidGameObject();

  virtual const char* GetTypeName() const;

  virtual void Draw();

    // Unfortunately these functions are defined in both base classes.
  // So define them here to avoid having to dis-ambiguate all the time.
  virtual void SetOrientation(const Orientation& o); 
  virtual const Orientation* GetOrientation();

  virtual void Update();
  virtual void DrawShadow() const;
  virtual const BoundingSphere* GetBoundingSphere() const;
  virtual void CreateBoundingSphere(const Matrix& cm);

  // Useful to have this here as it gives us Collision Vols in all subclasses
  //  but we should override to do nothing where it is not needed.
  virtual void RecalcCollisionVol();

  void SetSolid(PSolidComponent);
  PSolidComponent GetSolid() { return m_pSolid; }

protected:
  PSolidComponent m_pSolid;

  bool m_atRest;

public:
#if defined(SCENE_EDITOR)
  // Save object. Used by editing tool.
  virtual bool Save(File*);
  virtual PropertyMap GetProperties() const;
  virtual void SetProperties(const PropertyMap&);
#endif

};
}

#endif

