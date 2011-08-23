/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: EngineStatePoolTrickShotOver.h,v $
Revision 1.1.2.2  2006/08/14 17:50:18  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:33  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.1  2005/09/16 20:04:20  jay
Add Trick Shot rules

*/

#ifndef ENGINE_STATE_POOL_TRICK_SHOT_OVER_H_INCLUDED
#define ENGINE_STATE_POOL_TRICK_SHOT_OVER_H_INCLUDED

#include "EngineStatePoolText.h"

namespace Amju
{
class EngineStatePoolTrickShotOver : public EngineStatePoolText
{
public:
  static const char* Name;

  virtual void Draw();
  virtual void DrawOverlays();
  virtual void SetActive(bool active);
  virtual void Update();
  virtual bool Load();
  virtual void MouseButton(bool, bool, bool);

  // Trick Shot Rules calls this when Trick Shot is over, to
  // set Win or Lose text.
  // (Awarding hearts etc done in Rules ?)
  void SetSuccess(bool success);

protected:
  virtual void TimerExpired();

};
}

#endif

