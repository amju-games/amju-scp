/*
Amju Games source code (c) Copyright Jason Colman 2000-2006
$Log: EngineStatePoolOnlineWait.h,v $
Revision 1.1.2.5  2007/07/18 22:17:43  jay
Improvements to Table/Wait/Accept/Decline states

Revision 1.1.2.4  2007/05/23 18:45:42  jay
Send all object changes in one Http request (POST), not individually.
 + Some tidying up.

Revision 1.1.2.3  2007/03/09 21:50:41  jay
Online game - ongoing fixes

Revision 1.1.2.2  2006/08/26 21:28:36  jay
Ongoing changes for Online game; all Online features are off unless
POOL_ONLINE is defined

Revision 1.1.2.1  2006/08/07 20:42:50  jay
Online Pool - not working yet

*/

#ifndef ENGINE_STATE_POOL_ONLINE_WAIT_H_INCLUDED
#define ENGINE_STATE_POOL_ONLINE_WAIT_H_INCLUDED

#ifdef POOL_ONLINE

#include "EngineStateText.h"
#include "GuiButton.h"

namespace Amju
{
class EngineStatePoolOnlineWait : public EngineStateText
{
public:
  static const char* Name;

  virtual bool Load();
  virtual void Draw();
  virtual void DrawOverlays();
  virtual void Update();
  virtual void SetActive(bool);
  virtual void TimerExpired();
  virtual void MouseButton(bool down, bool ctrl, bool shift);

  void SetGameTypeChosenByOpponent(int levelId, int roomId);

protected:
  int m_chosenLevelId;
  int m_chosenRoomId;

  // Button to return to main menu
  PPoolGuiElement m_pCancelButton;

  // What we are currently waiting for:
  // 0 - reply
  // 1 - game choice
  int m_waitState;
};
}

#endif // POOL_ONLINE

#endif

