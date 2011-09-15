/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: BoundingBox.cpp,v $
Revision 1.1  2004/09/08 15:42:45  jay
Added to repository
  
*/

// This source code originally written by JASON COLMAN 2000-2003. 
// You may use it as you wish, at your own risk!  jason@amju.com.

#if defined(_DEBUG)
#if defined(WIN32)
#pragma warning(disable: 4786)
#endif
#include "Common.h" // for drawing
#include "Engine.h" 
#endif

#include "BoundingBox.h"
#include "BoundingSphere.h"
#include "SolidComposite.h"
#include "SolidLeaf.h"
#include "Matrix.h"
#include "AngleCompare.h"
#include "SchAssert.h"

namespace Amju
{
BoundingBox::BoundingBox(
  float xmin, float xmax, float ymin, float ymax, float zmin, float zmax) :
  m_xmin(xmin), m_xmax(xmax), 
  m_ymin(ymin), m_ymax(ymax), 
  m_zmin(zmin), m_zmax(zmax)
{
}

void BoundingBox::DrawSolid() const
{
/*
  glBegin(GL_QUADS);
    //Engine::Instance()->PushColour(0, 1.0f, 0,1.0f);
    glVertex3f(m_xmax, m_ymin, m_zmin); // 3
    glVertex3f(m_xmax, m_ymin, m_zmax); // 2
    glVertex3f(m_xmin, m_ymin, m_zmax); // 1
    glVertex3f(m_xmin, m_ymin, m_zmin); // 0
    //Engine::Instance()->PopColour();
    //Engine::Instance()->PushColour(1.0f, 1.0f, 1.0f, 1.0f);
    glVertex3f(m_xmin, m_ymax, m_zmin); // 0
    glVertex3f(m_xmin, m_ymax, m_zmax); // 1
    glVertex3f(m_xmax, m_ymax, m_zmax); // 2
    glVertex3f(m_xmax, m_ymax, m_zmin); // 3
    //Engine::Instance()->PopColour();
    //Engine::Instance()->PushColour(0, 0, 1.0f, 1.0f);
    glVertex3f(m_xmin, m_ymin, m_zmin); // 0
    glVertex3f(m_xmin, m_ymin, m_zmax); // 1
    glVertex3f(m_xmin, m_ymax, m_zmax); // 2
    glVertex3f(m_xmin, m_ymax, m_zmin); // 3
    //Engine::Instance()->PopColour();
    //Engine::Instance()->PushColour(1.0f, 0, 1.0f, 1.0f);
    glVertex3f(m_xmax, m_ymax, m_zmin); // 3
    glVertex3f(m_xmax, m_ymax, m_zmax); // 2
    glVertex3f(m_xmax, m_ymin, m_zmax); // 1
    glVertex3f(m_xmax, m_ymin, m_zmin); // 0
    //Engine::Instance()->PopColour();
    //Engine::Instance()->PushColour(1.0f, 0, 0, 1.0f);
    glVertex3f(m_xmin, m_ymin, m_zmin); // 0
    glVertex3f(m_xmin, m_ymax, m_zmin); // 1
    glVertex3f(m_xmax, m_ymax, m_zmin); // 2
    glVertex3f(m_xmax, m_ymin, m_zmin); // 3
    //Engine::Instance()->PopColour();
    //Engine::Instance()->PushColour(1.0f, 1.0f, 0, 1.0f);
    glVertex3f(m_xmax, m_ymin, m_zmax); // 3
    glVertex3f(m_xmax, m_ymax, m_zmax); // 2
    glVertex3f(m_xmin, m_ymax, m_zmax); // 1
    glVertex3f(m_xmin, m_ymin, m_zmax); // 0
    //Engine::Instance()->PopColour();
  glEnd();
*/
}

#if defined(_DEBUG)
void BoundingBox::Draw() const
{
/*
  glBegin(GL_LINES);
    // bottom
    glVertex3f(m_xmin, m_ymin, m_zmin);
    glVertex3f(m_xmax, m_ymin, m_zmin);

    glVertex3f(m_xmax, m_ymin, m_zmin);
    glVertex3f(m_xmax, m_ymin, m_zmax);

    glVertex3f(m_xmax, m_ymin, m_zmax);
    glVertex3f(m_xmin, m_ymin, m_zmax);  

    glVertex3f(m_xmin, m_ymin, m_zmax);
    glVertex3f(m_xmin, m_ymin, m_zmin);
    // top
    glVertex3f(m_xmin, m_ymax, m_zmin);
    glVertex3f(m_xmax, m_ymax, m_zmin);

    glVertex3f(m_xmax, m_ymax, m_zmin);
    glVertex3f(m_xmax, m_ymax, m_zmax);

    glVertex3f(m_xmax, m_ymax, m_zmax);
    glVertex3f(m_xmin, m_ymax, m_zmax);

    glVertex3f(m_xmin, m_ymax, m_zmax);
    glVertex3f(m_xmin, m_ymax, m_zmin);
    // sides
    glVertex3f(m_xmin, m_ymin, m_zmin);
    glVertex3f(m_xmin, m_ymax, m_zmin);

    glVertex3f(m_xmax, m_ymin, m_zmin);
    glVertex3f(m_xmax, m_ymax, m_zmin);

    glVertex3f(m_xmax, m_ymin, m_zmax);
    glVertex3f(m_xmax, m_ymax, m_zmax);

    glVertex3f(m_xmin, m_ymin, m_zmax);
    glVertex3f(m_xmin, m_ymax, m_zmax);

  glEnd();
*/
}
#endif

void BoundingBox::Set(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax)
{
  m_xmin = xmin;
  m_xmax = xmax;
  m_ymin = ymin;
  m_ymax = ymax;
  m_zmin = zmin;
  m_zmax = zmax;
}

// Set values which are lower than the mins or greater than maxes.
void BoundingBox::SetIf(float x, float y, float z)
{
  if (x < m_xmin) m_xmin = x;
  if (x > m_xmax) m_xmax = x;

  if (y < m_ymin) m_ymin = y;
  if (y > m_ymax) m_ymax = y;

  if (z < m_zmin) m_zmin = z;
  if (z > m_zmax) m_zmax = z;
}

bool BoundingBox::Intersects(const BoundingBox& b) const
{
/*
  // These asserts aren't true when this box is constructed,
  // i.e. its volume is zero. 
  Assert(m_xmin <= m_xmax);
  Assert(m_ymin <= m_ymax);
  Assert(m_zmin <= m_zmax);
*/
  // Sometimes returns true when boxes touch but don't intersect.
  // So use SMALLEST offset to stop this happening.
  return (
    m_xmax > (b.m_xmin + SMALLEST) &&
    b.m_xmax > (m_xmin + SMALLEST) &&
    m_ymax > (b.m_ymin + SMALLEST) &&
    b.m_ymax > (m_ymin + SMALLEST) &&
    m_zmax > (b.m_zmin + SMALLEST) &&
    b.m_zmax > (m_zmin + SMALLEST) );
}

bool BoundingBox::Intersects(const BoundingSphere& bs) const
{
  // Quick, inaccurate: make a cube around the sphere.
  const Vec3f v = bs.GetCentre();
  const float x = v.x;
  const float y = v.y;
  const float z = v.z;
  const float r = bs.GetRadius();
  BoundingBox bb(x - r, x + r, y - r, y + r, z - r, z + r);
  return Intersects(bb);
}

BoundingSphere BoundingBox::GetBoundingSphere() const
{
  float ax = (m_xmin + m_xmax) * 0.5f;
  float ay = (m_ymin + m_ymax) * 0.5f;
  float az = (m_zmin + m_zmax) * 0.5f;
  BoundingSphere bs;
  bs.SetCentre(Vec3f(ax, ay, az));
  float r = m_xmin - ax;
  if (fabs(m_ymax - ay) > r)
  {
    r = fabs(m_ymax - ay);
  }
  if (fabs(m_zmax - az) > r)
  {
    r = fabs(m_zmax - az);
  }
  bs.SetRadius(r);
  return bs;
}

void BoundingBox::SetFromSphere(const BoundingSphere& s)
{
  const Vec3f v = s.GetCentre();
  float x = v.x;
  float y = v.y;
  float z = v.z;
  float r = s.GetRadius();
  m_xmin = x - r;
  m_xmax = x + r;
  m_ymin = y - r;
  m_ymax = y + r;
  m_zmin = z - r;
  m_zmax = z + r;
}

void BoundingBox::Union(const BoundingBox& b)
{
  if (b.m_xmin < m_xmin) m_xmin = b.m_xmin;
  if (b.m_xmax > m_xmax) m_xmax = b.m_xmax;

  if (b.m_ymin < m_ymin) m_ymin = b.m_ymin;
  if (b.m_ymax > m_ymax) m_ymax = b.m_ymax;

  if (b.m_zmin < m_zmin) m_zmin = b.m_zmin;
  if (b.m_zmax > m_zmax) m_zmax = b.m_zmax;
}

void BoundingBox::SetFromLeaf(SolidLeaf* pLeaf)
{
  if (!pLeaf->IsSolid())
  {
    return;
  }

  int numPolys = pLeaf->GetNumberOfPolygons();
  for (int i = 0; i < numPolys; i++)
  {
    Polygon* pPoly = pLeaf->GetPolygon(i);
    for (int j = 0; j < 3; j++)
    {
      const SceneVertex& sv = const_cast<const Polygon*>(pPoly)->GetVertex(j);
      // **** NB ****
      // We must use rel. coords to set the box.
      // Don't forget, lots of leaves point to the same leaf data.
      // We can't rely on the absolute coords being what we'd like them to be.
      SetIf(sv.x, sv.y, sv.z);
    }
  }
}

void BoundingBox::SetFromComposite(SolidComposite* pComp)
{
  int numChildren = pComp->GetNumChildren();
  for (int i = 0; i< numChildren; i++)
  {
    SolidComponent* pChild = pComp->GetChild(i).GetPtr();
    // Work out the absolute coords for this child
    SetFromSolid(pChild);
  }
}

void BoundingBox::SetFromSolid(SolidComponent* p)
{
  SolidLeaf* pLeaf = dynamic_cast<SolidLeaf*>(p);
  if (pLeaf)
  {
    SetFromLeaf(pLeaf);
  } 
  else
  {
    SolidComposite* pComp = dynamic_cast<SolidComposite*>(p);
    if (pComp)
    {
      SetFromComposite(pComp);
    }
  }

#if defined(_DEBUG)
  // Check for badly set up Solid.
  if (m_xmin == 0 && m_xmax == 0 && m_ymin == 0 && m_ymax == 0 && m_zmin == 0 && m_zmax == 0)
  {
    // Rel. coords not set up for solid ? 
    Assert(0);
  }
#endif
}

/*
void BoundingBox::Transform(const Matrix& m)
{
  Vec3f vmin(m_xmin, m_ymin, m_zmin);
  Vec3f vmax(m_xmax, m_ymax, m_zmax);
  vmin = vmin * m;
  vmax = vmax * m;
  m_xmin = vmin.x;
  m_ymin = vmin.y;
  m_zmin = vmin.z;
  m_xmax = vmax.x;
  m_ymax = vmax.y;
  m_zmax = vmax.z;
}
*/

void BoundingBox::Translate(float dx, float dy, float dz)
{
  m_xmin += dx;
  m_xmax += dx;
  m_ymin += dy;
  m_ymax += dy;
  m_zmin += dz;
  m_zmax += dz;
}

void BoundingBox::RotateY(float ydegs)
{
  // TODO speed up with 2*2 matrix
  Matrix m;
  m.SetIdentity();
  m.RotateY(DegToRad(ydegs));

  Vec3f v1(m_xmin, 0, m_zmin);
  Vec3f v2(m_xmax, 0, m_zmin);
  Vec3f v3(m_xmax, 0, m_zmax);
  Vec3f v4(m_xmin, 0, m_zmax);

  v1 = v1 * m;
  v2 = v2 * m;
  v3 = v3 * m;
  v4 = v4 * m;

  // Get new x, z min/max from v1-4.
  m_xmin = 1000.0f;
  m_xmax = -1000.0f;
  m_zmin = 1000.0f;
  m_zmax = -1000.0f;
  SetIf(v1.x, m_ymin, v1.z); 
  SetIf(v2.x, m_ymin, v2.z); 
  SetIf(v3.x, m_ymin, v3.z); 
  SetIf(v4.x, m_ymin, v4.z); 
}

}

