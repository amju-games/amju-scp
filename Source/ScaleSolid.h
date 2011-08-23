/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ScaleSolid.h,v $
Revision 1.1  2004/09/08 15:42:41  jay
Added to repository
  
*/


#if !defined(SCALE_SOLID_H_INCLUDED)
#define SCALE_SOLID_H_INCLUDED

namespace Amju
{
class SolidComponent;

void ScaleRecursive(SolidComponent* pNode, float x, float y, float z);
}

#endif
