/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Target.h,v $
Revision 1.1.10.1  2005/05/08 08:03:43  jay
Fix GetOrientation - now returns const

Revision 1.1  2004/09/08 15:42:57  jay
Added to repository
  
*/

#if !defined(TARGET_H_INCLUDED)
#define TARGET_H_INCLUDED

#include "GameObject.h"
#include "Orientation.h"

namespace Amju
{
// A GameObject which doesn't do anything, but, because it has a Bounding Sphere,  
// can be used by other Game Objects to aim for.
class Target : public GameObject
{
public:
  Target();

  virtual void Draw();
  virtual const BoundingSphere* GetBoundingSphere() const;
  virtual void CreateBoundingSphere(const Matrix& cm);
  virtual bool Load(File* pf);
  virtual const char* GetTypeName() const;
  virtual void SetOrientation(const Orientation&);
  virtual const Orientation* GetOrientation();

  void SetSphere(const BoundingSphere& b) { m_bsphere = b; }

protected:
  BoundingSphere m_bsphere;
  Orientation m_or; // for get/set orientation

public:
#if defined(SCENE_EDITOR)
  virtual bool Save(File*);
  virtual PropertyMap GetProperties() const;
  virtual void SetProperties(const PropertyMap&);
#endif
};
}

#endif

