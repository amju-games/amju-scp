/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Endian.h,v $
Revision 1.1  2004/09/08 15:43:18  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_ENDIAN_H_INCLUDED)
#define SCHMICKEN_ENDIAN_H_INCLUDED

#include "Common.h"

// These functions swap the order of bytes. This would be useful when a 
// binary file saved under one Endianness was loaded under the other.
namespace Amju
{
int16 Endian(int16 i);

uint16 Endian(uint16 u);

int32 Endian(int32 i);

uint32 Endian(uint32 u);
// TODO ?? Is float format a portable standard ?
float32 Endian(float32 f);
}
#endif
