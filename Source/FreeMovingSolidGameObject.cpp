/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: FreeMovingSolidGameObject.cpp,v $
Revision 1.1.10.3  2006/04/13 18:21:51  jay
Added RecalcCollisionVol for low-poly collision vol if one is available.

Revision 1.1.10.2  2005/09/23 20:01:44  jay
DrawShadow - only if in UNKNOWN state.

Revision 1.1.10.1  2005/05/08 08:10:20  jay
Fix GetOrientation() - now returns const

Revision 1.1  2004/09/08 15:42:49  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <algorithm>
#include <math.h>
#include <iostream>
#include "FreeMovingSolidGameObject.h"
#include "CharacterControl.h"
#include "Engine.h"
#include "Level.h"
#include "PoolGameState.h"
#include "PlayerCodes.h"
#include "PlayerMessage.h"
#include "Matrix.h"
#include "Player.h"
#include "RotateHelper.h"
#include "SchAssert.h"

namespace Amju
{
const char* FreeMovingSolidGameObject::GetTypeName() const
{
  return "freesolid";
}

const BoundingSphere* FreeMovingSolidGameObject::GetBoundingSphere() const
{
  return m_pSolid->GetBoundingSphere();
}

void FreeMovingSolidGameObject::CreateBoundingSphere(const Matrix& cm)
{
  // Required for subclasses where the orientation is not necessarily
  // the one in m_pSolid. Make sure all representations of the orientation
  // are the same.
  m_pSolid->SetOrientation(*GetOrientation());

  Matrix m = cm;
  // Get initial transformation matrix
  GetOrientation()->TransformMatrix(&m); 
  m_pSolid->CreateBoundingSphere(m);
}

void FreeMovingSolidGameObject::Draw()
{
  if (GetState() == OUT_OF_PLAY)
  {
    return;
  }

  AmjuGL::PushMatrix();
  GetOrientation()->Draw();
  m_pSolid->Draw();
  AmjuGL::PopMatrix();
}

void FreeMovingSolidGameObject::DrawShadow() const
{
  // Put this in the base class because it is almost certainly more useful
  // than drawing the shadow whatever the state.
  if (GetState() != UNKNOWN)
  {
    return;
  }

  // NB This relies on us calling RefreshHeightServer().
  const BoundingSphere* bs = GetBoundingSphere();
  if (!bs)
  {
    return;
  }

  // Get coords for shadow.
  const float x = bs->x();
  const float y = bs->y();
  const float z = bs->z();

  m_pShadow->Draw(x, y, z, GetShadowSize(), m_heightServerWithPlatforms);
}

void FreeMovingSolidGameObject::SetOrientation(const Orientation& o)
{
  FreeMovingGameObject::SetOrientation(o);
}

const Orientation* FreeMovingSolidGameObject::GetOrientation()
{
  return FreeMovingGameObject::GetOrientation();
}

void FreeMovingSolidGameObject::SetSolid(PSolidComponent pSolid)
{
  m_pSolid = pSolid;

  Matrix m;
  m.identity();
  CreateBoundingSphere(m);

  RecalcCollisionVol();
  RefreshHeightServer();

/*
  // Work out shadow size - average x,z size
  float dx = m_collisionVolume.GetBoundingBox().m_xmax - 
    m_collisionVolume.GetBoundingBox().m_xmin;
  float dz = m_collisionVolume.GetBoundingBox().m_zmax - 
    m_collisionVolume.GetBoundingBox().m_zmin;
  float s = (dx + dz) / 2.0f;
  SetShadowSize(s);
*/
}

void FreeMovingSolidGameObject::Update()
{
  FreeMovingGameObject::Update();

  m_pSolid->Update();

  ControlUpdateThis();

  Matrix m;
  m.identity();
  CreateBoundingSphere(m);
}

#if defined(SCENE_EDITOR)
bool FreeMovingSolidGameObject::Save(File*)
{
  return true; // TODO
}

FreeMovingSolidGameObject::PropertyMap 
FreeMovingSolidGameObject::GetProperties() const
{
  return PropertyMap(); // TODO
}

void FreeMovingSolidGameObject::SetProperties(
  const FreeMovingSolidGameObject::PropertyMap&)
{
}
#endif

void FreeMovingSolidGameObject::RecalcCollisionVol()
{
  Matrix m;
  m.identity();
  m_collisionVolume.Clear();
  BoundingSphere bs(VertexBase(0, 0, 0), 1000); // big B.S.

  // Load m_pCollisionVol to get a low-poly volume. If this is not
  // set up, use the Solid as the collision vol - this can be expensive
  // as there may be a lot more polys than is necessary for collision.
  if (m_pCollisionVol.GetPtr())
  {
    GetOrientation()->TransformMatrix(&m);
    // Set orientation of collision solid so it encloses this character.
    m_pCollisionVol->SetOrientation(m_orientation);
    m_pCollisionVol->StoreHeights(m);
    m_pCollisionVol->AddHeights(&m_collisionVolume, bs);
  } 
  else
  {
    GetOrientation()->TransformMatrix(&m);
    m_pSolid->StoreHeights(m);
    m_pSolid->AddHeights(&m_collisionVolume, bs);
  }
}
}

