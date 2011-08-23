/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Intersection.cpp,v $
Revision 1.1  2004/09/08 15:42:51  jay
Added to repository
  
*/

#include "Intersection.h"
#include "HeightServer.h"

namespace Amju
{
// Return true if the height polys of the two height servers intersect,
// (i.e. they intersect in x-z) and the range of heights also intersects.
bool Intersects(const HeightServer& hs1, const HeightServer& hs2)
{
  // Get the min and max heights of each H.S. If these ranges don't
  // overlap, no intersection.
  return false;
}
}

