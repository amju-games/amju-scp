/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Intersection.h,v $
Revision 1.1  2004/09/08 15:42:51  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_INTERSECTION_H_INCLUDED)
#define SCHMICKEN_INTERSECTION_H_INCLUDED

namespace Amju
{
class HeightServer;
// Decide whether two solids intersect. This is used to place
// takeables only where they do not intersect the scene, or
// each other.
//namespace Intersection
//{
// Return true if the height polys of the two height servers intersect,
// (i.e. they intersect in x-z) and the range of heights also intersects.
bool Intersects(const HeightServer& hs1, const HeightServer& hs2);
//}
}

#endif

