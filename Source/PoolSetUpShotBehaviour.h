/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: PoolSetUpShotBehaviour.h,v $
Revision 1.1.2.1  2006/08/14 17:50:24  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:35  jay
Moved Pool code from Pool/ to Pool/

Revision 1.2.2.1  2004/10/03 17:01:50  jay
Player faces waypoint rather than hole if appropriate.

Revision 1.2  2004/09/20 13:35:53  Administrator
Ongoing improvements to AI for demo mode. Shot speed is now varied, but
is not accurate (distance travelled is not linear with shot speed ?).
Should aim at each waypoint in turn, or check for floor where the ball
will land.

Revision 1.1  2004/09/08 15:43:06  jay
Added to repository
  
*/

#if !defined(GOLF_SET_UP_SHOT_BEHAVIOUR_H_INCLUDED)
#define GOLF_SET_UP_SHOT_BEHAVIOUR_H_INCLUDED

#include "Behaviour.h"
#include "GameObject.h"

namespace Amju
{
class PoolHole;

class PoolSetUpShotBehaviour : public Behaviour
{
public:
  virtual void Update();
  virtual void OnActivated();

  GameObject* GetTargetObject() { return m_pTargetObject.GetPtr(); }

  void CalcShot();

protected:
  float m_time;
  float m_hVel;
  float m_vVel;
  float m_yRot;

  // The object we are aiming to hit with the ball.
  PGameObject m_pTargetObject;
};
}

#endif

