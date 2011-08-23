/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: AiJumpDownBehaviour.h,v $
Revision 1.1  2004/09/08 15:43:01  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_AI_JUMP_DOWN_BEHAVIOUR_H_INCLUDED)
#define SCHMICKEN_AI_JUMP_DOWN_BEHAVIOUR_H_INCLUDED

#include "AiBehaviourBase.h"
#include "Target.h"

namespace Amju
{
// Get to floor level from on top of a piece.
// Try to find a path to loor level, otherwise just jump off.
class AiJumpDownBehaviour : public AiBehaviourBase
{
public:
  virtual void OnActivated();
  virtual void Update();

protected:
  virtual void ReachedTarget();
  virtual void ContinueActivating();
  virtual void AvoidObstacles();

  // Set direction to walk in  - towards the next target, but not if there's
  // an obstacle nearby.
  void SetDirection();

  // Return true if path in front of character is obstructed in 
  // direction (degrees) a.
  bool IsObstructed(float a);

  // Called when no unobstructed path was found.
  void NoPath();

protected:
  // Height we want to get down to.
  float m_floor;

  // m_pTarget is set to the piece/location we are trying to get to.
  // This target is used by the path finder whatever the 'real' target is.
  SharedPtr<Target> m_pInnerTarget;

  // Set to true when direction to walk in has been decided. Reset when
  // OnActivated is called.
  bool m_directionIsSet;
};
}

#endif
