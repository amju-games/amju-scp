/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: FightBehaviour.h,v $
Revision 1.1  2004/09/08 15:42:38  jay
Added to repository
  
*/

#if !defined(FIGHT_BEHAVIOUR_H_INCLUDED)
#define FIGHT_BEHAVIOUR_H_INCLUDED

#include "Behaviour.h"

namespace Amju
{
class FightBehaviour : public Behaviour
{
public:
  static void Init();

  FightBehaviour();
  virtual void Update();

protected:
  // Timer, so we can't shoot missiles faster than some rate.
  float m_shootTimer;

  //static float s_farDistance;
};
}

#endif
