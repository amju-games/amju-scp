/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: SolidGameObject.cpp,v $
Revision 1.1.10.2  2005/07/17 22:13:55  Administrator
Fixed bad function sig for GetOrientation()

Revision 1.1.10.1  2005/06/24 05:37:42  Administrator
Fix GetOrientation() - added const

Revision 1.1  2004/09/08 15:42:56  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "SolidGameObject.h"
#include "Matrix.h"
#include "SolidComponent.h"
#include "Common.h"
#include "File.h"
#include "Engine.h"
#include "SchAssert.h"

namespace Amju
{
SolidGameObject::SolidGameObject() 
{
#if defined(SCENE_EDITOR)
/*
  // Assign some default solid, to make this object saveable. 
  std::string defaultComp = Engine::Instance()->GetConfigValue("default_comp_name");
  if (defaultComp.empty())
  {
    defaultComp = "block1.comp";
  }
  m_pSolid = SolidComponent::LoadSolid(defaultComp);
  Assert(m_pSolid.GetPtr());
*/
#endif
}

SolidGameObject::~SolidGameObject()
{
}

#ifdef SCENE_EDITOR
bool SolidGameObject::Save(File* pf)
{
  // Save Solid name
  pf->WriteComment("// Solid name");
  pf->Write(m_pSolid->GetName());
  // Save orientation
  pf->WriteComment("// Orientation");
  m_pSolid->GetOrientation()->Save(pf);
  
  return true;
}
#endif

void SolidGameObject::Draw()
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

void SolidGameObject::Update()
{
  m_pSolid->Update();

  RefreshHeightServer();
}

void SolidGameObject::CreateBoundingSphere(const Matrix& cm)
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

const BoundingSphere* SolidGameObject::GetBoundingSphere() const
{
  return m_pSolid->GetBoundingSphere();
}

void SolidGameObject::SetOrientation(const Orientation& o)
{
  Assert(m_pSolid.GetPtr() && "No solid set for SolidGameObject");
  m_pSolid->SetOrientation(o);
  // Also we must reset the bounding sphere of the object.
  // Assume we are not currently transformed by anything.
  Matrix m;
  m.SetIdentity();
  CreateBoundingSphere(m);

  // Some solid Game Objects may be solid, at least to NPCs.
  RecalcCollisionVol();
}

const Orientation* SolidGameObject::GetOrientation() 
{
  Assert(m_pSolid.GetPtr());
  Assert(m_pSolid->GetOrientation());

  return m_pSolid->GetOrientation();
}

void SolidGameObject::SetSolid(PSolidComponent pSolid)
{
  m_pSolid = pSolid;

  Matrix m;
  m.SetIdentity();
  CreateBoundingSphere(m);

  RecalcCollisionVol();
  RefreshHeightServer();
  // Work out shadow size - average x,z size
  float dx = m_collisionVolume.GetBoundingBox().m_xmax - 
    m_collisionVolume.GetBoundingBox().m_xmin;
  float dz = m_collisionVolume.GetBoundingBox().m_zmax - 
    m_collisionVolume.GetBoundingBox().m_zmin;
  float s = (dx + dz) / 2.0f;

  SetShadowSize(s);
}

void SolidGameObject::RecalcCollisionVol() 
{
  Matrix m;
  m.SetIdentity();
  GetOrientation()->TransformMatrix(&m); 
  m_pSolid->StoreHeights(m);

  m_collisionVolume.Clear();
  BoundingSphere bs(Vec3f(0, 0, 0), 1000); // big B.S.
  m_pSolid->AddHeights(&m_collisionVolume, bs);
}

void SolidGameObject::DrawEditSolid()
{
  Draw();
}
}

