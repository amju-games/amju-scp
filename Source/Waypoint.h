/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Waypoint.h,v $
Revision 1.1  2004/09/08 15:43:00  jay
Added to repository
  
*/

#if !defined(WAYPOINT_H_INCLUDED)
#define WAYPOINT_H_INCLUDED

#include "Target.h"

namespace Amju
{
class Waypoint : public Target
{
public:
  virtual const char* GetTypeName() const;

};
}

#endif


