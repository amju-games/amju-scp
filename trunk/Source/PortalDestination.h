/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: PortalDestination.h,v $
Revision 1.1.10.1  2005/05/08 08:02:29  jay
Fix GetOrientation - now returns const

Revision 1.1  2004/09/08 15:42:40  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_PORTAL_DESTINATION_H_INCLUDED)
#define SCHMICKEN_PORTAL_DESTINATION_H_INCLUDED

#include "GameObject.h"
#include "Orientation.h"

namespace Amju
{
// When the Player intersects a portal, this is the destination.
class PortalDestination : public GameObject
{
public:
  virtual void Draw();
  virtual const BoundingSphere* GetBoundingSphere() const;
  virtual void CreateBoundingSphere(const Matrix& cm);
  virtual const char* GetTypeName() const;
  virtual bool Load(File*);
  virtual void SetOrientation(const Orientation&);
  virtual const Orientation* GetOrientation();

protected:
  // Position and direction to face.
  Orientation m_or;

#if defined(SCENE_EDITOR)
public:
  virtual bool Save(File*);
  virtual PropertyMap GetProperties() const;
  virtual void SetProperties(const PropertyMap&);

#endif
};
}

#endif

