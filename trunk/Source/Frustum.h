/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Frustum.h,v $
Revision 1.1  2004/09/08 15:42:49  jay
Added to repository
  
*/

#if !defined(FRUSTUM_H_INCLUDED)
#define FRUSTUM_H_INCLUDED

namespace Amju
{
class BoundingSphere;
class Frustum
{
public:
  static Frustum* Instance();

  // Creates frustum from openGL matrices.
  void Create();

  // Returns true if sphere bs is in the frustum.
  bool Contains(const BoundingSphere& bs) const;

private:
  Frustum() {}
  
};
}
#endif

