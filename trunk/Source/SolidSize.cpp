/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: SolidSize.cpp,v $
Revision 1.1  2004/09/08 15:42:57  jay
Added to repository
  
*/

#include "SolidSize.h"
#include "HeightServer.h"
#include "SolidComponent.h"

namespace Amju
{
void GetSolidSize(SolidComponent* p, float* pX, float* pY, float* pZ)
{
  Matrix m;
  m.SetIdentity();
  p->CreateBoundingSphere(m);
  p->StoreHeights(m);
  HeightServer hs;
  p->AddHeights(&hs, BoundingSphere(Vec3f(0, 0, 0), 1000.0f));
  BoundingBox bb = hs.GetBoundingBox();
  float dx = bb.m_xmax - bb.m_xmin;
  if (dx < 0)
  {
    dx = 0;
  }
  float dy = bb.m_ymax - bb.m_ymin;
  if (dy < 0)
  {
    dy = 0;
  }
  float dz = bb.m_zmax - bb.m_zmin;
  if (dz < 0)
  {
    dz = 0;
  }
  *pX = dx;
  *pY = dy;
  *pZ = dz;
}

float GetSolidXSize(SolidComponent* p)
{
  Matrix m;
  m.SetIdentity();
  p->CreateBoundingSphere(m);
  p->StoreHeights(m);
  HeightServer hs;
  p->AddHeights(&hs, BoundingSphere(Vec3f(0, 0, 0), 1000.0f));
  BoundingBox bb = hs.GetBoundingBox();
  float dx = bb.m_xmax - bb.m_xmin;
  if (dx < 0)
  {
    dx = 0;
  }
  return dx;
}

float GetSolidYSize(SolidComponent* p)
{
  Matrix m;
  m.SetIdentity();
  p->CreateBoundingSphere(m);
  p->StoreHeights(m);
  HeightServer hs;
  p->AddHeights(&hs, BoundingSphere(Vec3f(0, 0, 0), 1000.0f));
  BoundingBox bb = hs.GetBoundingBox();
  float dy = bb.m_ymax - bb.m_ymin;
  if (dy < 0)
  {
    dy = 0;
  }
  return dy;
}

float GetSolidZSize(SolidComponent* p)
{
  Matrix m;
  m.SetIdentity();
  p->CreateBoundingSphere(m);
  p->StoreHeights(m);
  HeightServer hs;
  p->AddHeights(&hs, BoundingSphere(Vec3f(0, 0, 0), 1000.0f));
  BoundingBox bb = hs.GetBoundingBox();
  float dz = bb.m_zmax - bb.m_zmin;
  if (dz < 0)
  {
    dz = 0;
  }
  return dz;
}

}

