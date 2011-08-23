/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: AiBehaviourBase.h,v $
Revision 1.1  2004/09/08 15:43:01  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_AI_BEHAVIOUR_BASE_H_INCLUDED)
#define SCHMICKEN_AI_BEHAVIOUR_BASE_H_INCLUDED

#include "SeekBehaviourBase.h"

namespace Amju
{
class AiBehaviourBase : public SeekBehaviourBase
{
protected:
  AiBehaviourBase();

  virtual void Update();
  virtual PVisibilityGraph GetVisibilityGraph();
  virtual void AvoidObstacles();

protected:
  // If true, this behaviour has finished. We would expect another behaviour
  // to take over control of the Character, but this may not happen.
  // In this case the behaviour should simply make the character Idle.
  bool m_isTaskCompleted;

};
}

#endif
