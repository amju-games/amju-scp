/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: EngineStatePoolShowShotNag.h,v $
Revision 1.1.2.1  2006/08/14 17:50:18  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:30  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.1  2005/09/23 19:58:05  jay
Add NagMode states

*/

#ifndef ENGINE_STATE_SHOW_SHOT_NAG_H_INCLUDED
#define ENGINE_STATE_SHOW_SHOT_NAG_H_INCLUDED

#include "EngineStatePoolShowShotDemo.h"

namespace Amju
{
class EngineStatePoolShowShotNag : public EngineStatePoolShowShotDemo
{
public:
  static const char* Name;

  virtual void DrawOverlays();
};
}

#endif

