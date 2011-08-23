/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: FrozenBehaviour.h,v $
Revision 1.1  2004/09/08 15:42:38  jay
Added to repository
  
*/

#if !defined(FROZEN_BEHAVIOUR_H_INCLUDED)
#define FROZEN_BEHAVIOUR_H_INCLUDED

#include "Behaviour.h"

namespace Amju
{
class FrozenBehaviour : public Behaviour
{
public:
  virtual void Update();
};
}

#endif

