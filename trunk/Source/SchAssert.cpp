/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: SchAssert.cpp,v $
Revision 1.1  2004/09/08 15:43:19  jay
Added to repository
 
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <iostream>
#include "SchAssert.h"

namespace Amju
{
#if defined(_DEBUG)
// Only called if the assert() macro has failed.
void sch_assert(const void* exp, const void* file, unsigned line)
{
  std::cout 
    << "Assertion failed: "
    << (const char *)exp
    << std::endl
    << "File: "
    << (const char *)file
    << " line: "
    << line
    << std::endl;

#if defined(WIN32) && defined(MSVC)
  __asm
  {
    int 3 // break
  }
#endif
}
#endif
}
