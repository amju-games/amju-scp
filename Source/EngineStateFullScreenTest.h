/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: EngineStateFullScreenTest.h,v $
Revision 1.1.2.1  2005/10/21 17:04:39  jay
Added test screen for when full screen mode is first entered.

*/

#ifndef ENGINE_STATE_FULL_SCREEN_TEST_H_INCLUDED
#define ENGINE_STATE_FULL_SCREEN_TEST_H_INCLUDED

#include "EngineState.h"
#include "GuiElement.h"

namespace Amju
{
// The first time the game goes full screen, show this state.
// A message asks the user to click OK if full screen has worked,
// The state counts down to 0 secs then times out.
// If we time out the full screen flag will be reset.
class EngineStateFullScreenTest : public EngineState
{
public:
  static const char* Name;

  EngineStateFullScreenTest(); 
  virtual void SetActive(bool);
  virtual bool Load();
  virtual void DrawOverlays();
  virtual void Draw();
  virtual void MouseButton(bool, bool, bool);
  virtual void Update();
  virtual void TimerExpired();

protected:
  PGuiElement m_pOkButton;
};
}

#endif

