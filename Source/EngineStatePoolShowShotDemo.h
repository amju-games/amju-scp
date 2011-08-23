/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStatePoolShowShotDemo.h,v $
Revision 1.1.2.1  2006/08/14 17:50:18  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:30  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.6.2  2005/09/23 19:42:39  jay
Added include guards!!!

Revision 1.1.6.1  2005/04/01 06:01:53  jay
Pool Demo mode

Revision 1.1  2004/09/17 13:59:11  jay
Added demo mode

*/

#ifndef ENGINE_STATE_GOLF_SHOW_SHOT_DEMO_H_INCLUDED
#define ENGINE_STATE_GOLF_SHOW_SHOT_DEMO_H_INCLUDED

#include "EngineStatePoolShowShot.h"

namespace Amju
{
class EngineStatePoolShowShotDemo : public EngineStatePoolShowShot
{
public:
  static const char* Name;
  virtual void SetActive(bool);
  virtual void TimerExpired();

  // Demo mode - so draw buttons and respond to mouse clicks.
  virtual void DrawOverlays();
  virtual void MouseButton(bool down, bool ctrl, bool shift);
  virtual void MousePos(int x, int y);

};
}

#endif

