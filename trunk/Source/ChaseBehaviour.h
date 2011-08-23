/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ChaseBehaviour.h,v $
Revision 1.1  2004/09/08 15:42:34  jay
Added to repository
  
*/

#if !defined(CHASE_BEHAVIOUR_H_INCLUDED)
#define CHASE_BEHAVIOUR_H_INCLUDED

#include "SeekBehaviourBase.h"
#include "CrocDecideBehaviour.h"

namespace Amju
{
class ChaseBehaviour : public SeekBehaviourBase
{
public:
  static void Init();

  ChaseBehaviour();

  // Call base class for seeking, and occasionally decide if character should 
  // enter a new state 
  virtual void Update();

  virtual void OnActivated();

  // Force an immediate rethink. 
  void ForceDecision();

protected:
  virtual void ReachedTarget();

protected:
  // A sub-behaviour, called only every so often, by Update().
  // This looks for chicks nearby, and decide whether to continue chasing, or what.
  CrocDecideBehaviour m_decideBehaviour;

  // Make character go this fast when chasing.
  static float s_runVel;
};
}

#endif
