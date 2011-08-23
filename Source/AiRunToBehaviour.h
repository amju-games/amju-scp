/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: AiRunToBehaviour.h,v $
Revision 1.1  2004/09/08 15:43:01  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_AI_RUN_TO_BEHAVIOUR_H_INCLUDED)
#define SCHMICKEN_AI_RUN_TO_BEHAVIOUR_H_INCLUDED

#include "AiBehaviourBase.h"

namespace Amju
{
class AiRunToBehaviour : public AiBehaviourBase
{
public:
  virtual void Update();
  virtual void OnActivated();

protected:
  virtual void ReachedTarget();
  virtual void ContinueActivating();

};
}

#endif