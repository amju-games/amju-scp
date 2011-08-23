/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: EngineStatePoolSetUpShotNag.h,v $
Revision 1.1.2.1  2006/08/14 17:50:16  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:28  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.1  2005/09/23 19:58:05  jay
Add NagMode states

*/

#ifndef ENGINE_STATE_GOLF_SET_UP_SHOT_NAG_H_INCLUDED
#define ENGINE_STATE_GOLF_SET_UP_SHOT_NAG_H_INCLUDED

#include "EngineStatePoolSetUpShotDemo.h"
#include "HelpText.h"

namespace Amju
{
// Like Demo state, but nag player to buy.
class EngineStatePoolSetUpShotNag : public EngineStatePoolSetUpShotDemo
{
public:
  static const char* Name;

  // Override to load "help" (i.e. nag) text
  virtual bool Load();
  virtual void DrawOverlays();
  virtual void SetActive(bool);

public:
  static void SDrawOverlays();

protected:
  static HelpText s_helpText;
};
}

#endif

