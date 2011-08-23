/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Geometry.h,v $
Revision 1.1  2004/09/08 15:43:18  jay
Added to repository
  
*/

#if !defined(GEOM_H_INCLUDED)
#define GEOM_H_INCLUDED

namespace Amju
{
class VertexBase;

namespace Geometry
{
  void MakeNormal(const float v1[3], 
                         const float v2[3], 
                         const float v3[3], 
                         float result[3]);

  void MakeNormal(const VertexBase& v1,
                         const VertexBase& v2,
                         const VertexBase& v3,
                         VertexBase* pResult);

  void CrossProduct(float v1[3], float v2[3], float result[3]);

  VertexBase CrossProduct(const VertexBase& v1, const VertexBase& v2);

  float DotProduct(const VertexBase& v1, const VertexBase& v2);

  void Normalize(float r[3]);

  // Get the angle in the x-z plane between the first vertex and the
  // second.
  float GetXZAngle(const VertexBase& v1, const VertexBase& v2);
};
}
#endif
