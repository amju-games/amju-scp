/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: RunHomeBehaviour.h,v $
Revision 1.1  2004/09/08 15:42:41  jay
Added to repository
  
*/

#if !defined(RUN_HOME_BEHAVIOUR_H_INCLUDED)
#define RUN_HOME_BEHAVIOUR_H_INCLUDED

#include "SeekBehaviourBase.h"
#include "CrocDecideBehaviour.h"

namespace Amju
{
class RunHomeBehaviour : public SeekBehaviourBase
{
public:
  static void Init();

  RunHomeBehaviour();

  virtual void Update();
  virtual void OnActivated();

protected:
  virtual void ReachedTarget();

protected:
  static float s_runHomeVel;

  // A sub-behaviour, called only every so often, by Update().
  // This looks for chicks nearby, and decide whether to chase them, or what.
  // (ChaseBehaviour, IdleBehaviour also have one)
  CrocDecideBehaviour m_decideBehaviour;
};
}

#endif


