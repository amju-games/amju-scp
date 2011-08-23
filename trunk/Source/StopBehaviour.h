/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: StopBehaviour.h,v $
Revision 1.1  2004/09/08 15:42:41  jay
Added to repository
  
*/

#if !defined(STOP_BEHAVIOUR_H_INCLUDED)
#define STOP_BEHAVIOUR_H_INCLUDED

#include "Behaviour.h"

namespace Amju
{
class StopBehaviour : public Behaviour
{
public:
  virtual void Update();
};
}

#endif

