/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStatePoolSetUpShotDemo.h,v $
Revision 1.1.2.2  2007/12/12 10:27:12  jay
Don't poll server when in demo mode!

Revision 1.1.2.1  2006/08/14 17:50:16  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:28  jay
Moved Pool code from Pool/ to Pool/

Revision 1.2.6.3  2005/09/23 19:38:47  jay
Remove unused code

Revision 1.2.6.2  2005/04/17 22:04:06  jay
Added english and draw/roll parameters to shot

Revision 1.2.6.1  2005/04/01 06:00:01  jay
Pool Demo mode

Revision 1.2  2004/09/17 13:59:10  jay
Added demo mode

Revision 1.1  2004/09/08 15:43:05  jay
Added to repository
  
*/

#if !defined(ENGINE_STATE_GOLF_SET_UP_SHOT_DEMO_H_INCLUDED)
#define ENGINE_STATE_GOLF_SET_UP_SHOT_DEMO_H_INCLUDED

#include "EngineStatePoolSetUpShot.h"

namespace Amju
{
// Subclass setting-up-shot state for demo mode.
// I.e. user input is never used.
// When the ball is struck, the next state is also a demo state.
class EngineStatePoolSetUpShotDemo : public EngineStatePoolSetUpShot
{
public:
  static const char* Name;

  virtual void SetActive(bool active);
  virtual void DrawOverlays();
  virtual void Red(bool);
  virtual void MouseButton(bool down, bool ctrl, bool shift);
  virtual void MousePos(int x, int y);
  virtual bool Load();

  // Set up players, course etc here, not in SetActive, which is called many
  // times in one Demo cycle.
//  void Init();

  // Static fn, called by all Demo states.
  static void SDrawOverlays();
  // Static mouse handlers, called by all demo states.
  static void SMouseButton(bool down, bool ctrl, bool shift);
  static void SMousePos(int x, int y);

protected:
  virtual void UpdateOnlineStatus() {}

protected:
  // Static buttons etc, drawn by all demo states.
  static PPoolGuiElement s_startButton;
  static PPoolGuiElement s_exitButton;
  static PPoolGuiElement s_registerButton;
  static PPoolGuiElement s_buynowButton;
  
};
}

#endif

