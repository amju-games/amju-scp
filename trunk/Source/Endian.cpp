/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Endian.cpp,v $
Revision 1.1.10.1  2006/03/21 19:15:17  jay
Fix for Mac Intel

Revision 1.2  2006/01/16 13:34:19  jay
Add test for Mac Intel

Revision 1.1  2004/09/08 15:43:18  jay
Added to repository
  
*/

#include "Endian.h"

namespace Amju
{
// If Mac on Intel, no swapping is required.
#if defined(WIN32) || defined(__i386__)
int16 Endian(int16 i) { return i; }
uint16 Endian(uint16 u) { return u; }
int32 Endian(int32 i) { return i; }
uint32 Endian(uint32 u) { return u; }
float32 Endian(float32 f) { return f; }
#else

int16 Endian(int16 i) 
{
  return (int16)(Endian((uint16)(i)));
}

uint16 Endian(uint16 u) 
{
  uint16 r1 = ((u & 0x00ff) << 8);
  uint16 r2 = ((u & 0xff00) >> 8); 
  
  uint16 r = r1 + r2;
  return r;
}

int32 Endian(int32 i) 
{
  return (int32)(Endian((uint32)(i)));
}

uint32 Endian(uint32 u) 
{
  return ((u & 0x000000ff) << 24) | 
         ((u & 0x0000ff00) << 8) | 
         ((u & 0x00ff0000) >> 8) |
         ((u & 0xff000000) >> 24);
}

float32 Endian(float32 f) 
{ 
  uint32 r = Endian(*((uint32*)&f));
  float32 rf = *((float32*)&r);
  return rf; 
}
#endif
}

