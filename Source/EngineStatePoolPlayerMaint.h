/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: EngineStatePoolPlayerMaint.h,v $
Revision 1.1.2.2  2006/08/14 17:50:15  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:32  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.3  2005/10/21 19:53:45  Administrator
Fix constants EDIT and DELETE so we can compile on windows - presumably
they are #defined somewhere in windows.h.

Revision 1.1.2.2  2005/10/21 17:09:15  jay
Edit and delete work ok now.

Revision 1.1.2.1  2005/08/26 22:03:08  jay
Added Player Maintenance state, not used yet.

*/

#ifndef POOL_PLAYER_MAIN_T_H_INCLUDED
#define POOL_PLAYER_MAIN_T_H_INCLUDED

#include "EngineStatePoolChoosePlayer.h"
#include "GuiElement.h"

namespace Amju
{
// When player selects a name, go to this State. This allows user
// to edit name or delete name
class EngineStatePoolPlayerMaint : public EngineStatePoolChoosePlayer
{
public:
  static const char* Name;

  EngineStatePoolPlayerMaint();

  virtual void SetActive(bool);
  virtual bool Load();
  virtual void Draw();
  virtual void DrawOverlays();
  virtual void MouseButton(bool, bool, bool);

  // Go back to main menu state if nothing happens for a while
  virtual void TimerExpired();

  // Called from button handlers:

  // Set requested operation to edit, andchanges mode to PlayerSelect mode.
  void Edit();
 
  // Set requested operation to delete, andchanges mode to PlayerSelect mode.
  void Delete();

  // Call to change mode from edit/delete/cancel selection to player name
  // selection.
  void SetPlayerSelectMode(bool isPlayerSelectMode);

  void DoOperationOnPlayer(int playerId);

protected:
  PGuiElement m_pEditButton;
  PGuiElement m_pDeleteButton;
  PGuiElement m_pCancelButton; // cancel last selection
  PGuiElement m_pBackButton;  // go back to main menu

  // If true, user must select a player name or cancel.
  // NB if cancel is chosen when in this mode, the mode should revert to
  //  operation select.
  // If false, user must select an operation: edit, delete or back.
  // NB If back is chosen when in this state, the EngineState reverts to
  //  the main menu.
  bool m_isPlayerSelectMode;

  std::string m_text;

  enum Operation { AMJU_EDIT, AMJU_DELETE };
  Operation m_op;
};
}

#endif


