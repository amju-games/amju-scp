/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: CrocDecideBehaviour.h,v $
Revision 1.1  2004/09/08 15:42:35  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_CROC_DECIDE_BEHAVIOUR_H_INCLUDED)
#define SCHMICKEN_CROC_DECIDE_BEHAVIOUR_H_INCLUDED

#include "Behaviour.h"

namespace Amju
{
// This behaviour decides what the croc should do next.
// Crocs switch to this behaviour periodically,
// then either revert to previous state or go to a new state.
class CrocDecideBehaviour : public Behaviour
{
public:
  static void Init();

  CrocDecideBehaviour();

  // This is called every frame. A decision is made only periodically.
  virtual void Update();

  // Call this to force a decision immediately, i.e. don't wait until
  // m_period expires.
  void ForceUpdate();

  // Updates only do anything periodically.
  void SetUpdatePeriod(float);

private:
  float m_period;
  float m_timer; // count up to m_period before doing anything.

  static float s_idleChaseDistance;
  static float s_runHomeChaseDistance;
};
}

#endif

