/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Colour.cpp,v $
Revision 1.1  2004/09/08 15:42:46  jay
Added to repository
  
*/

#include "Colour.h"
#include "Common.h"

namespace Amju
{
bool operator<(const Colour& c1, const Colour& c2)
{
  if (c1.m_r < c2.m_r) return true;
  if (c1.m_r > c2.m_r) return false;

  if (c1.m_g < c2.m_g) return true;
  if (c1.m_g > c2.m_g) return false;
 
  if (c1.m_b < c2.m_b) return true;
  if (c1.m_b > c2.m_b) return false;

  return (c1.m_a < c2.m_a);
}

bool operator!=(const Colour& c1, const Colour& c2)
{
  return !(c1 == c2);
}

bool operator==(const Colour& c1, const Colour& c2)
{
  return (
    fabs(c1.m_r - c2.m_r) < SMALLEST && 
    fabs(c1.m_g - c2.m_g) < SMALLEST && 
    fabs(c1.m_b - c2.m_b) < SMALLEST && 
    fabs(c1.m_a - c2.m_a) < SMALLEST );
}
}
