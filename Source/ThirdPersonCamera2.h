/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ThirdPersonCamera2.h,v $
Revision 1.1  2004/09/08 15:42:59  jay
Added to repository
  
*/

#if !defined(THIRD_PERSON_CAMERA2_H_INCLUDED)
#define THIRD_PERSON_CAMERA2_H_INCLUDED

#include "ThirdPersonCameraBase.h"

namespace Amju
{
// Instead of swinging around the player, this camera 'resets'
// by pulling away behind the player if there's an obstruction.
// This seems to work relatively well: we don't get stuck in 
// obstacles, or rotate into a blind spot.
class ThirdPersonCamera2 : public ThirdPersonCameraBase
{
public:
  ThirdPersonCamera2();

  virtual void GetConfigValues();

  virtual void Update();
  virtual void Reset();

protected:
  virtual void RecalcNotIdle();
  virtual void RecalcIdle(); 

  // Return true if the camera should move to follow the target (player).
  virtual bool FollowTarget() const;

  void PullBack();

  // true when we are repositioning behind the player.
  bool m_isResetting;
  
  // Pull-back velocity: the faster the better, but too fast might miss
  // collisions.
  float m_pullbackVel;

  // Flag so we don't reset continuously. Only reset when it's ok to
  // pull back again.
  bool m_hasReset;

  // Enforce a minimum time period between resets.
  float m_resetTimer, m_minResetTime;

  // Previous distance between camera position and the player. 
  // I.e. copy of m_behind.
  // This is compared to updated distance to tell if the player is getting
  // closer to the camera or is moving away.
  float m_oldDistance;

  // If true, reset pulls in front of the player rather than behind.
  bool m_resetToFront;
};
}

#endif

