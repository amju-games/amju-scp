/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: RotateHelper.h,v $
Revision 1.1  2004/09/08 15:42:56  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_ROTATE_HELPER_H_INCLUDED)
#define SCHMICKEN_ROTATE_HELPER_H_INCLUDED

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
  float* pXAdd, float* pZAdd);
}

#endif

