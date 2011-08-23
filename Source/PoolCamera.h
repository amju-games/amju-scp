/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: PoolCamera.h,v $
Revision 1.1.2.2  2006/08/14 17:50:22  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:38  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.5  2005/08/26 21:57:41  jay
Pull back further for AI players

Revision 1.1.2.4  2005/07/30 12:25:48  jay
Adjust camera to cue height.

Revision 1.1.2.3  2005/07/17 23:04:44  jay
Keep camera directly behind cue

Revision 1.1.2.2  2005/04/03 16:32:31  jay
Override FollowTarget() so camera does not chase after cue ball. The original
purpose was to make placing the cue ball work, but seems like a generally
good idea.

Revision 1.1.2.1  2005/03/31 22:30:49  jay
Added pool-specific camera: it pulls back to ensure that all moving balls
are in the view frustum.

*/

#ifndef POOL_CAMERA_H_INCLUDED
#define POOL_CAMERA_H_INCLUDED

#include "ThirdPersonCamera2.h"

namespace Amju
{
// For Pool, we may need to pull back further than the normal limit,
// so we can see all moving balls.
class PoolCamera : public ThirdPersonCamera2
{
public:
  PoolCamera();
  virtual void GetConfigValues();
  virtual void Update();
  virtual void Reset();

  void SetPoolPullBackRequired(bool);

  void GoDirectlyBehindCueNow(float degs);

  // Set when user changes cue height
  void SetAdjustToCueHeight(bool);

  // Set player to human or AI, so we pull out to the appropriate distance.
  void SetIsAiPlayer(bool);

protected:
  // We override this to stop the camera following the cue ball.
  virtual bool FollowTarget() const;

  // True if we need to pull back to view all moving balls.
  bool m_poolPullBackRequired;

  // Vel at which we pull back to view all moving balls.
  float m_poolPullBackVel;

  // Max value for above vel
  float m_poolMaxPullBackVel;

  // Deceleration for pool pull-back.
  // If m_poolPullBackRequired flag is not set, we slow down to zero pull-back vel.
  float m_poolPullBackAccel;

  // If this flag is set, move up or down to match cue elevation.
  bool m_adjustToCueHeight;
};
}

#endif


