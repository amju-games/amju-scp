/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: PoolCue.h,v $
Revision 1.1.2.2  2006/08/14 17:50:22  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:38  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.5  2005/07/09 10:43:13  Administrator
Added flag for 'auto take shot'. When this is set the user no longer
controls the cue swing vel.

Revision 1.1.2.4  2005/06/13 22:13:43  jay
"Pinball" control - pull back and release

Revision 1.1.2.3  2005/05/24 20:44:55  jay
Return value from SetContactPos so we know if cue intersected anything.

Revision 1.1.2.2  2005/05/15 17:22:03  jay
Allow user to swing cue using mouse; show cue contact pos;
detect collision with table or balls (ongoing)

Revision 1.1.2.1  2005/05/08 08:25:31  jay
Show user the cue position

*/

#ifndef AMJU_POOL_CUE_H_INCLUDED
#define AMJU_POOL_CUE_H_INCLUDED

#include "SolidComponent.h"

namespace Amju
{
// Cue - shows where player is aiming.
// TODO What to derive from ?
class PoolCue 
{
public:
  PoolCue();
  void Draw();
  void Update();
  bool Load();

  // Reset swing mode, hit ball flag, etc.
  void Reset();
  // Reset swing position, but not elevation etc.
  void ResetSwingPos();

  // Set mode to swing back and forwards, or aim mode.
  void SetSwingMode(bool swing);

  bool IsSwingMode() const;

  // Get cue vel on contact with cue ball.
  float GetSwingVel(); // not sure we need this
  // Set vel - for when user pulls back and releases mouse button.
  void SetSwingVel(float v);

  // Returns true if cue has just hit the cue ball.
  bool HitsCueBall();

  // For swing mode, mouse up/down movement swings the cue forward and
  // back.
  void MousePos(int x, int y);

  // Set the cue ball contact pos: (0, 0) is cue ball centre.
  // Returns true if new position is valid, i.e. no intersection.
  bool SetContactPos(float x, float y);

  // Return true if cue intersects table or any ball except cue ball
  bool Intersects();

  // Return power of shot, between 0 and 1.0.
  float GetSwingPower() const;

  // When set, cue swings forward, user is not controlling the vel
  // any more.
  void SetAutoTakeShot(bool b);

  bool GetAutoTakeShot() const;
 
protected:
  PSolidComponent m_pSolid;

  bool m_swingMode;
  float m_swingVel;
  int m_oldy; // old mouse Y 
  bool m_hitBall;
  float m_swingPos; // 0 is default position
  float m_x, m_y; // contact pos: (0, 0) is cue ball centre.

  // Endpoints of line - v1 is centre of ball.
  VertexBase m_v1, m_v2;

  bool m_autoTakeShot;
};
}

#endif

