/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: RotateHelper.cpp,v $
Revision 1.1.10.1  2005/09/14 19:40:18  Administrator
Camera sometimes goes berzerk because RotateHelper() gets NAN.
This hopefully fixes it by making sure asin() never gets a bad argument.

Revision 1.1  2004/09/08 15:42:55  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <iostream>
#include "Common.h"
#include "RotateHelper.h"

namespace Amju
{
// Help for rotation functionality.
// E.g. characters on rotating platforms; character rotating about characters.
// xOrigin, zOrigin: centre of rotation.
// yDegs: degrees of rotation, about Y axis.
// x, z: point on x-z plane which rotating about (xOrigin, zOrigin)
// pXAdd, pZAdd: result: change to add to (x, z) to get rotated position.
// 
bool RotateHelper(
  float xOrigin, float zOrigin,
  float x, float z,
  float ydegs,
  float* pXAdd, float* pZAdd)
{
  const float diffx = xOrigin - x;
  const float diffz = zOrigin - z;

  const float dist = sqrt(diffx * diffx + diffz * diffz); 

  if (dist < SMALLEST)
  {
    *pXAdd = 0;
    *pZAdd = 0;
    return true;
  }

  // Get the old angle of character pos around the centre of rotation.
  // asinf always returns an angle getween -90 and 90 degrees.
  float s = (z - zOrigin) / dist;
  if (s >= 1.0f)
  {
    s = 1.0f;
  }
  if (s <= -1.0f)
  {
    s = -1.0f;
  }

  float b = asin(s);

  float bdegs = b * (float)(180.0 / pi); // for debugging
  // bdegs is in range -90 to 90 - i.e. only 180 degrees of values.
  bdegs += 90; // now in range 0 - 180
  // Get the other 180 degrees
  if (x > xOrigin)
  {
    bdegs = 360.0f - bdegs;
  }
  bdegs -= 90.0f;

  // Get the new angle in rads
  // Fix offset
  float adegs = ydegs;
  float abdegs = bdegs + adegs;
  abdegs -= 90.0f;

  const float abrads = abdegs / (float)(180.0 * pi);

  // New position: rotate about the platform centre.
  float newx = dist * sin(abrads) + xOrigin;
  float newz = dist * cos(abrads) + zOrigin;

  float addx = newx - x;
  float addz = newz - z;

  *pXAdd = addx;
  *pZAdd = addz;

  return true;
}
}

