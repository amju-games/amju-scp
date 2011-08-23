/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: StupidBehaviour.h,v $
Revision 1.1  2004/09/08 15:42:41  jay
Added to repository
  
*/

#if !defined(STUPID_BEHAVIOUR_H_INCLUDED)
#define STUPID_BEHAVIOUR_H_INCLUDED

#include "AvoidBehaviourBase.h"

namespace Amju
{
// This behaviour makes character stupidly walk in a straight line.
// We derive from SeekBehaviour so we can avoid obstacles.
class StupidBehaviour : public AvoidBehaviourBase
{
public:
  StupidBehaviour();

  virtual void Update();

protected:
  virtual void ReactToObstacle();

  float GetTurnDir() const;
 
  bool IsBlocked() const;

  bool IsDropAhead() const;
};
}

#endif
