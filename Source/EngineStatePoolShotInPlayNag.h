/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: EngineStatePoolShotInPlayNag.h,v $
Revision 1.1.2.1  2006/08/14 17:50:17  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:29  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.1  2005/09/23 19:58:05  jay
Add NagMode states

*/

#ifndef ENGINE_STATE_GOLF_SHOT_IN_PLAY_NAG_H_INCLUDED
#define ENGINE_STATE_GOLF_SHOT_IN_PLAY_NAG_H_INCLUDED

#include "EngineStatePoolShotInPlayDemo.h"

namespace Amju
{
class EngineStatePoolShotInPlayNag : public EngineStatePoolShotInPlayDemo
{
public:
  static const char* Name;

  virtual void DrawOverlays();
};
}

#endif

