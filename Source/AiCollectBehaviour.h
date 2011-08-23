/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: AiCollectBehaviour.h,v $
Revision 1.1  2004/09/08 15:43:01  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_AI_COLLECT_BEHAVIOUR_H_INCLUDED)
#define SCHMICKEN_AI_COLLECT_BEHAVIOUR_H_INCLUDED

#include "AiBehaviourBase.h"
#include "Target.h"

namespace Amju
{
// This behaviour instructs its owner Character to collect a piece.
class AiCollectBehaviour : public AiBehaviourBase
{
public:
  virtual void OnActivated();
  virtual void Update();

protected:
  virtual void ReachedTarget();
  virtual void ContinueActivating();

protected:
  // m_pTarget is set to the piece we are trying to collect.
  // This target is used by the path finder - it's a Target inside the 
  // m_pTarget piece.
  SharedPtr<Target> m_pInnerTarget;
};
}
#endif
