/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: AvoidBehaviourBase.h,v $
Revision 1.1  2004/09/08 15:42:44  jay
Added to repository
  
*/

#if !defined(AVOID_BEHAVIOUR_BASE_H_INCLUDED)
#define AVOID_BEHAVIOUR_BASE_H_INCLUDED

#include "Behaviour.h"
#include "Vertex.h"
#include "SolidComponent.h"

namespace Amju
{
class AvoidBehaviourBase : public Behaviour
{
public:
  AvoidBehaviourBase();

  virtual void Update();

protected:
  // Steers away from obstacles, e.g. other characters.
  virtual void AvoidObstacles();
 
  // If an obstacle is detected, this function is called to do something about it.
  virtual void ReactToObstacle();

protected:
  // Distance to look ahead for obstacle avoidance
  float m_lookahead;

  // Velocity at which to rotate to avoid an obstacle directly in front of
  // Character. 
  float m_obstructionRotateVel;

  // True if currently working around an obstacle.
  bool m_isAvoiding;

}; 
}

#endif


