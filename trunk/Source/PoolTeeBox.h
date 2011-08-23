/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: PoolTeeBox.h,v $
Revision 1.1.2.1  2006/08/14 17:50:25  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:35  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1  2004/09/08 15:43:06  jay
Added to repository
  
*/

#if !defined(GOLF_TEE_BOX_H_INCLUDED)
#define GOLF_TEE_BOX_H_INCLUDED

#include "SolidGameObject.h"
#include "BoundingSphere.h"

namespace Amju
{
// For a golf game, this is where the players start from
// for a particular hole.
class PoolTeeBox : public SolidGameObject
{
public:
  PoolTeeBox();

  virtual const char* GetTypeName() const;

  virtual void Draw();
  virtual bool Load(File* pFile);
  virtual void CreateShadow();
  virtual const BoundingSphere* GetBoundingSphere() const;
  virtual void CreateBoundingSphere(const Matrix& cm);
  virtual void DrawShadow() const;
  virtual void SetOrientation(const Orientation&);

public:
#if defined(SCENE_EDITOR)
  virtual bool Save(File*);
  virtual PropertyMap GetProperties() const;
  virtual void SetProperties(const PropertyMap&);
#endif

protected:
  VertexBase m_teeVertex;
  BoundingSphere m_bs;
};
}

#endif

