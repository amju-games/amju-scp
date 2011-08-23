/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStateGolfShotInPlayTest.h,v $
Revision 1.1.2.1  2006/07/24 18:20:29  jay
Moved Pool code from Golf/ to Pool/

Revision 1.1  2004/09/08 15:43:05  jay
Added to repository
  
*/

#if !defined(ENGINE_STATE_GOLF_SHOT_IN_PLAY_TEST_H_INCLUDED)
#define ENGINE_STATE_GOLF_SHOT_IN_PLAY_TEST_H_INCLUDED

#include "EngineStateGolfShotInPlay.h"

namespace Amju
{
// Subclass shot-in-play class for Level Design Testing.
// Once the ball comes to rest, simply go back to the previous state,
// (i.e. set up shot). If the ball goes OB, just put it back
// at the Tee. The user can exit the state and go back to Level Designing.
class EngineStateGolfShotInPlayTest : public EngineStateGolfShotInPlay
{
public:
  static const char* Name;

  virtual void SetActive(bool active);
  virtual void Red(bool);
  virtual void DrawOverlays();
};
}

#endif

