/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: AngleCompare.cpp,v $
Revision 1.1  2004/09/08 15:43:17  jay
Added to repository
  
*/

// This source code originally written by JASON COLMAN 2000-2003. 
// You may use it as you wish, at your own risk!  jason@amju.com.

#include "Common.h"
#include "AngleCompare.h"

namespace Amju
{
namespace AngleCompare
{
// Get the difference between two angles, in degrees.
// Returns the angle which, added to a1, results in a2.
float DegDiff(float a1, float a2)
{
  // Ensure angles are >= 0 and < 360
  while (a1 < 0)
  {
    a1 += 360.0f;
  }
  while (a1 >= 360.0f)
  {
    a1 -= 360.0f;
  }
  while (a2 < 0)
  {
    a2 += 360.0f;
  }
  while (a2 >= 360.0f)
  {
    a2 -= 360.0f;
  }

  float d = a2 - a1;

  // Get smallest value for d.
  if (d <= 180.0f && d >= -180.0f)
  {
    return d;
  }
  if (d > 180.0f)
  {
    d -= 360.0f; 
  }
  else if (d < -180.0f)
  {
    d += 360.0f;
  }

  return d;
}

// As for DegDiff, but all 3 angles are in Radians.
float RadDiff(float a1, float a2)
{
  float a1degs = RadToDeg(a1); //a1 * 180.0f / pi; 
  float a2degs = RadToDeg(a2); //a2 * 180.0f / pi; 
  float ddegs = DegDiff(a1degs, a2degs);
  float drads = DegToRad(ddegs); //ddegs / 180.0f * pi;
  return drads;
}

float TurnToFace(float x, float z, float targetX, float targetZ)
{
  float dx = x - targetX;
  float dz = z - targetZ;
  float r = atan2(dx, dz);
  return r;
}

}

float DegToRad(float degs)
{
  float rads = degs / 180.0f * pi;
  return rads;
}

float RadToDeg(float rads)
{
  float degs = rads * 180.0f / pi; 
  return degs;
}
}

