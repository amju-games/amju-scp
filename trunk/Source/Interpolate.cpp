/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Interpolate.cpp,v $
Revision 1.1  2004/09/08 15:42:51  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "Interpolate.h"
#include "Vertex.h"

namespace Amju
{
void Interpolate(float time, TimePoint* pPoints, Orientation* pResult)
{
  // t is fraction of time elapsed between current and next, 0..1
  float t = (time - pPoints[1].first) / (pPoints[2].first - pPoints[1].first);
  if (t < 0)
  {
    return;
  }

  // Angles of rotation are interpolated linearly
  pResult->SetXRot(pPoints[1].second.GetXRot() + 
      t * (pPoints[2].second.GetXRot() - pPoints[1].second.GetXRot()));

  pResult->SetYRot(pPoints[1].second.GetYRot() + 
      t * (pPoints[2].second.GetYRot() - pPoints[1].second.GetYRot()));

  pResult->SetZRot(pPoints[1].second.GetZRot() + 
      t * (pPoints[2].second.GetZRot() - pPoints[1].second.GetZRot()));

  // Spline interpolation from Game Programming Gems p. 470
  const float t2 = t * t;
  const float t3 = t2 * t;
  const float alpha = 0; // usually 0, adjust as required
  const float a = (1.0f - alpha) / 2.0f;
  VertexBase v1(pPoints[0].second.GetX(), pPoints[0].second.GetY(), pPoints[0].second.GetZ());
  VertexBase v2(pPoints[1].second.GetX(), pPoints[1].second.GetY(), pPoints[1].second.GetZ());
  VertexBase v3(pPoints[2].second.GetX(), pPoints[2].second.GetY(), pPoints[2].second.GetZ());
  VertexBase v4(pPoints[3].second.GetX(), pPoints[3].second.GetY(), pPoints[3].second.GetZ());

  VertexBase m0 = v2;
  m0 -= v1;
  m0 *= a;
  m0 += v3; 
  m0 -= v2;

  VertexBase m1 = v3;
  m1 -= v2;
  m1 *= a;
  m1 += v4;
  m1 -= v3;

  VertexBase r = v2;
  r *= (2 * t3 - 3 * t2 + 1);
  
  VertexBase r1 = m0;
  r1 *= (t3 - 2 * t2 + t);

  VertexBase r2 = m1;
  r2 *= (t3 - t2);

  VertexBase r3 = v3;
  r3 *= (-2 * t3 + 3 * t2);

  r += r1;
  r += r2;
  r += r3;
  pResult->SetX(r.x);
  pResult->SetY(r.y);
  pResult->SetZ(r.z);
}
}
