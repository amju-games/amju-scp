/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: AiPlaceBehaviour.h,v $
Revision 1.1  2004/09/08 15:43:01  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_AI_PLACE_BEHAVIOUR_H_INCLUDED)
#define SCHMICKEN_AI_PLACE_BEHAVIOUR_H_INCLUDED

#include "AiBehaviourBase.h"

namespace Amju
{
// This behaviour instructs its owner Character to place a piece
// at a specified Orientation.
// (The Orientation is given by a Target object.)
// Expected to be preceded by AiCollectBehaviour, which collects 
// a piece.
class AiPlaceBehaviour : public AiBehaviourBase
{
public:
  virtual void OnActivated();

  // Override so we can check when we are in the right spot - it has to
  // be more accurate than usual or we will place the piece in the wrong
  // spot.
  virtual void Update();

protected:
  virtual void ReachedTarget();
  virtual void ContinueActivating();
};
}

#endif