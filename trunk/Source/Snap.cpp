/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Snap.cpp,v $
Revision 1.1  2004/09/08 15:43:20  jay
Added to repository
  
*/

#include "Snap.h"
#include "SchAssert.h"

namespace Amju
{
float Snap(float x, float snap)
{
  Assert(snap > 0);
  if (x < 0)
  {
    x = snap * float(int((x - snap / 2.0)/ snap));
  }
  else
  {
    x = snap * float(int((x + snap / 2.0)/ snap));
  }
  return x;
}
}

