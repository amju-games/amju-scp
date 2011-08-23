/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: IdleBehaviour.h,v $
Revision 1.1  2004/09/08 15:42:38  jay
Added to repository
  
*/

#if !defined(IDLE_BEHAVIOUR_H_INCLUDED)
#define IDLE_BEHAVIOUR_H_INCLUDED

#include "Behaviour.h"
#include "CrocDecideBehaviour.h"

namespace Amju
{
class IdleBehaviour : public Behaviour
{
public:
  IdleBehaviour();

  virtual void Update();

protected:
  // A sub-behaviour, called only every so often, by Update().
  // This looks for chicks nearby, and decide whether to chase them, or what.
  // (ChaseBehaviour, RunHomeBehaviour also have one)
  CrocDecideBehaviour m_decideBehaviour;

};
}

#endif

