/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Geometry.cpp,v $
Revision 1.1.10.1  2006/01/20 13:45:43  jay
Gcc 4.0 fix: remove unnecessary namespace names

Revision 1.1  2004/09/08 15:43:18  jay
Added to repository
  
*/

// This source code originally written by JASON COLMAN 2000-2003. 
// You may use it as you wish, at your own risk!  jason@amju.com.

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "Common.h"
#include "Geometry.h"
#include "Vertex.h"
#include <math.h>

namespace Amju
{
namespace Geometry
{
float GetXZAngle(const VertexBase& v1, const VertexBase& v2)
{
  float dx = v2.x - v1.x;
  float dz = v2.z - v1.z;
  float r = atan2(dx, dz); // TODO do we need to check if z = 0 ?
  float rDegs = r / (float)(pi * 180.0); // convert to degs
  rDegs += 180.0f; // need this to fix z direction 
  return rDegs;
}

void Normalize(float r[3])
{
  const float len = (float) sqrt(r[0]*r[0]+r[1]*r[1]+r[2]*r[2]);
  if (len != 0.0f)
  {
    r[0]/=len;
    r[1]/=len;
    r[2]/=len;
  }
}

void CrossProduct(float v1[3], float v2[3], float result[3])
{
  result[0] = v1[1]*v2[2]-v2[1]*v1[2];
  result[1] = v2[0]*v1[2]-v1[0]*v2[2];
  result[2] = v1[0]*v2[1]-v2[0]*v1[1];
}

float DotProduct(const VertexBase& v1, const VertexBase& v2)
{
  return v1.x * v2.x + v1.y * v2.y  + v1.z * v2.z;
}

void MakeNormal(const VertexBase& v1,
                          const VertexBase& v2,
                          const VertexBase& v3,
                          VertexBase* pResult)
{
  float t1[3];
  float t2[3];

  t1[0] = v1.x - v2.x;
  t1[1] = v1.y - v2.y;
  t1[2] = v1.z - v2.z;

  t2[0] = v2.x - v3.x;
  t2[1] = v2.y - v3.y;
  t2[2] = v2.z - v3.z;

  float r[3];
  CrossProduct(t1, t2, r);
  Normalize(r);
  *pResult = VertexBase(r[0], r[1], r[2]);
}

void MakeNormal(const float v1[3], 
                          const float v2[3], 
                          const float v3[3], 
                          float r[3])
{
    float t1[3];
    float t2[3];

    t1[0] = v1[0]-v2[0];
    t1[1] = v1[1]-v2[1];
    t1[2] = v1[2]-v2[2];

    t2[0] = v2[0]-v3[0];
    t2[1] = v2[1]-v3[1];
    t2[2] = v2[2]-v3[2];

    CrossProduct(t1, t2, r);
    Normalize(r);
}
} // Geometry
} // Schmicken
