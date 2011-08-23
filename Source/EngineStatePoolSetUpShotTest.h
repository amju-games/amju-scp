/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStatePoolSetUpShotTest.h,v $
Revision 1.1.2.1  2006/08/14 17:50:16  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:28  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1  2004/09/08 15:43:05  jay
Added to repository
  
*/

#if !defined(ENGINE_STATE_GOLF_SET_UP_SHOT_TEST_H_INCLUDED)
#define ENGINE_STATE_GOLF_SET_UP_SHOT_TEST_H_INCLUDED

#include "EngineStatePoolSetUpShot.h"

namespace Amju
{
// Subclass set-up-shot state for Level Design Testing state.
// In this state the user is allowed to simply hit the ball around
// wherever he likes.
class EngineStatePoolSetUpShotTest : public EngineStatePoolSetUpShot
{
public:
  static const char* Name;

  virtual void SetActive(bool active);
  virtual void Red(bool);
  virtual void DrawOverlays();
  virtual void TakeShotNow(float yRot, float verticalVel, float horVel, float accel);
};
}

#endif

