/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStatePoolShotInPlayDemo.h,v $
Revision 1.1.2.1  2006/08/14 17:50:17  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:29  jay
Moved Pool code from Pool/ to Pool/

Revision 1.2.6.2  2005/09/23 19:40:09  jay
Remove unused code

Revision 1.2.6.1  2005/04/01 06:01:28  jay
Pool Demo mode

Revision 1.2  2004/09/17 13:59:11  jay
Added demo mode

Revision 1.1  2004/09/08 15:43:05  jay
Added to repository
  
*/

#if !defined(ENGINE_STATE_GOLF_SHOT_IN_PLAY_DEMO_H_INCLUDED)
#define ENGINE_STATE_GOLF_SHOT_IN_PLAY_DEMO_H_INCLUDED

#include "EngineStatePoolShotInPlay.h"

namespace Amju
{
// Subclass shot-in-play state for Demo mode.
// Simply go to the next player, or back to Title mode.
class EngineStatePoolShotInPlayDemo : public EngineStatePoolShotInPlay
{
public:
  static const char* Name;

  virtual void SetActive(bool);
  virtual void DrawOverlays();
  virtual void MouseButton(bool down, bool ctrl, bool shift);
  virtual void MousePos(int x, int y);
};
}

#endif

