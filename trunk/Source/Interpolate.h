/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Interpolate.h,v $
Revision 1.1  2004/09/08 15:42:51  jay
Added to repository
  
*/

#if !defined(INTERPOLATE_H_INCLUDED)
#define INTERPOLATE_H_INCLUDED

#include "Orientation.h"
#include <utility>

namespace Amju
{
// Time, point/orientation pair. This defines a point in space, (and a
// direction), and the time at which something should be at that point.
typedef std::pair<float, Orientation> TimePoint;

// Interpolate to get an Orientation. pPoints should be an array of
// four time/point pairs. Although we need four points, the result
// is always somewhere between points 1 and 2 (0-based).
// time should be a time value >= pPoints[1].first, and <= pPoints[2].first.
// The result vertex is a point on a spline curve which runs through the
// four points. The result angles are simply interpolated linearly.
void Interpolate(float time, TimePoint* pPoints, Orientation* pResult);

// NB Another Interpolation function can be found in Orientation class.
}

#endif

