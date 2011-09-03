/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: VisibleGameObject.cpp,v $
Revision 1.1.10.2  2006/08/14 17:37:30  jay
Rename "Pool"

Revision 1.1.10.1  2005/05/08 08:07:32  jay
Fix calls which rely on non-const GetOrientation()

Revision 1.1  2004/09/08 15:43:00  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <iostream>
#include "VisibleGameObject.h"
#include "Level.h"
#include "Frustum.h"
#include "BasicShadow.h"
#include "RotateHelper.h"

namespace Amju
{
VisibleGameObject::VisibleGameObject()
{
  m_oldx = -1000.0f; // So we initially populate the HeightServer.
  m_oldy = -1000.0f;
  m_oldz = -1000.0f;
  // Default: should be overwritten on Load.
  m_shadowSize = 0.5;

  // Do this again in Load() for special shadows
  CreateShadow(); 
}

void VisibleGameObject::Update()
{
  RefreshHeightServer();
/*
  // Platforms
  if (m_pActivePlatform)
  {
    // Get delta of platform position. Then add the delta to player position,
    // so player moves with platform.
    Orientation deltaO;
    m_pActivePlatform->GetOrientationDelta(&deltaO);
    // Add the change in position to the Character's position.
    // If the platform is rotating, we must add the rotation delta
    // to the player too.
    
    if (deltaO.GetYRot() != 0)
    {
      // Special case for platforms rotating about y-axis. We need to move the
      // character position about the platform's centre.
      // Get the platform centre of rotation
      // Get diff in (x,z) between character coords and platform centre.
      // Use trig to move character (x,z) position.
      const float charx = GetOrientation()->GetX();
      const float charz = GetOrientation()->GetZ();

      const float platx = m_pActivePlatform->GetRotationCentre().x;
      const float platz = m_pActivePlatform->GetRotationCentre().z;
    
      float addx = 0;
      float addz = 0;

      RotateHelper(platx, platz, charx, charz, deltaO.GetYRot(), &addx, &addz);
      deltaO.SetX(deltaO.GetX() + addx);
      deltaO.SetZ(deltaO.GetZ() + addz);
    }
    Orientation o = *(GetOrientation());
    o += deltaO;
    SetOrientation(o);
  }
*/
}

/*
void VisibleGameObject::SetActivePlatform(Platform* p)
{
  // j.c. 10/5/2004 - Pool - improved so that the platform is only
  // set if we are really on it, not just within the b.sphere.
  if (!p)
  {
    m_pActivePlatform = 0;
    return;
  }

  // Check that this object is actually ON the platform,
  // not just within the b. sphere.

  const HeightServer* pHs = p->GetCollisionVol();
  if (!pHs)
  {
    m_pActivePlatform = 0;
    return;
  }
  if (!GetOrientation())
  {
    m_pActivePlatform = 0;
    return;
  }
  const float x = GetOrientation()->GetX();
  const float y = GetOrientation()->GetY() + 1.0f; // TODO desperate
  const float z = GetOrientation()->GetZ();

  const HeightPoly* pHp = pHs->GetHeightPoly(x, y, z);
  if (pHp)
  {
    m_pActivePlatform = p;
  }
  else
  {
    m_pActivePlatform = 0; // ? Causes problems ?
  }
}
*/

void VisibleGameObject::CreateShadow()
{
  m_pShadow = new BasicShadow;
  m_pShadow->SetCaster(this);
}

void VisibleGameObject::RefreshShadow()
{
  m_pShadow->Reset();
  // Reset previous position so the HeightServer will really be refreshed.
  m_oldx = 1000;
  // ..that should do it. 
  RefreshHeightServer();
}

void VisibleGameObject::CullDraw(Frustum* pFrustum)
{
  if (pFrustum->Contains(*(GetBoundingSphere())))
  {
    Draw();
  }
}

const BoundingSphere* VisibleGameObject::GetHeightServerSphere()
{
  return GetBoundingSphere();
}

void VisibleGameObject::RefreshHeightServer()
{
  RefreshHeightServer(&m_heightServer);
}

void VisibleGameObject::RefreshHeightServer(HeightServer* pHs)
{
  Assert(pHs);
  // Level may not be set for objects which are loaded first and added
  // to a level later.
  if (!m_pLevel)
  {
    return;
  }

  const BoundingSphere* bs = GetHeightServerSphere();
  Assert(bs);

  if (!bs)
  {
    return;
  }

  const float x = bs->x();
  const float y = bs->y();
  const float z = bs->z();
  if (x == m_oldx && y == m_oldy && z == m_oldz)
  {
    return; // we haven't moved - don't recalculate.
  }
  m_oldx = x;
  m_oldy = y;
  m_oldz = z;
  pHs->Clear();
  m_pLevel->GetHeightServer(pHs, *bs);
}

bool VisibleGameObject::AddToHeightServer(const HeightServer& hs)
{
  m_heightServer.AddHeightServer(hs);
  return true;
}

void VisibleGameObject::DrawShadow() const
{
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

  m_pShadow->Draw(x, y, z, GetShadowSize(), m_heightServer);
}

const HeightServer* VisibleGameObject::GetCollisionVol() const
{
  return &m_collisionVolume;
}

}

