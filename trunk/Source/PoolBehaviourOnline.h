/*
Amju Games source code (c) Copyright Jason Colman 2000-2006
$Log: PoolBehaviourOnline.h,v $
Revision 1.1.2.2  2007/03/16 08:50:40  jay
Online changes: each ball state change/collision is sent to the server, and
used in the replay to keep consistency between clients.

Revision 1.1.2.1  2006/08/07 20:42:50  jay
Online Pool - not working yet

*/

#ifndef POOL_BEHAVIOUR_ONLINE_H_INCLUDED
#define POOL_BEHAVIOUR_ONLINE_H_INCLUDED

//#include "PoolBehaviour.h"

namespace Amju
{
class PoolBehaviourOnline //: public PoolBehaviour
{
public:
  PoolBehaviourOnline();

  virtual void Update();
  virtual void OnEsActivated();
  virtual void OnNewRoom();

  // Must override pure virtual
//  virtual ValidBalls GetValidBalls();

private:
  float m_pollTime;

  int m_lastShotId;

  // Reset when activated, set when we have a new shot but must wait for
  //  all object state changes before starting the shot.
  bool m_waitingForObjects;
};
}

#endif

