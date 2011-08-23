/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: SolidSize.h,v $
Revision 1.1  2004/09/08 15:42:57  jay
Added to repository
  
*/

#if !defined(SOLID_SIZE_H_INCLUDED)
#define SOLID_SIZE_H_INCLUDED

namespace Amju
{
class SolidComponent;

float GetSolidXSize(SolidComponent*);
float GetSolidYSize(SolidComponent*);
float GetSolidZSize(SolidComponent*);
void GetSolidSize(SolidComponent*, float* pX, float* pY, float* pZ);
}

#endif

