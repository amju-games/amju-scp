/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: AngleCompare.h,v $
Revision 1.1  2004/09/08 15:43:17  jay
Added to repository
  
*/

#if !defined(ANGLE_COMPARE_H_INCLUDED)
#define ANGLE_COMPARE_H_INCLUDED

#include <DegRad.h>

namespace Amju
{
namespace AngleCompare
{
// Get the difference between two angles, in degrees.
// Returns the angle which, added to a1, results in a2.
float DegDiff(float a1, float a2);

// As for DegDiff, but all 3 angles are in Radians.
float RadDiff(float a1, float a2);

// Return angle (radians) which an object should turn to to face
// a target at (targetX, targetZ).
// Used to point characters to face relevant objects, etc.
float TurnToFace(float x, float z, float targetX, float targetZ);
}
}

#endif

