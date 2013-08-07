/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: EngineStatePlayerDelete.h,v $
Revision 1.1.2.2  2006/08/14 17:50:12  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:31  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.1  2005/10/21 17:42:47  jay
This state is for the user to confirm player delete

*/

#ifndef ENGINE_STATE_PLAYER_DELETE_H_INCLUDED
#define ENGINE_STATE_PLAYER_DELETE_H_INCLUDED

#include "EngineStateText.h"
#include "GuiElement.h"

namespace Amju
{
// Simply shows text, and yes/no confirm buttons.
// Would probably be a good idea to have EngineStateConfirm..???
class EngineStatePlayerDelete : public EngineStateText
{
public:
  static const char* Name;

  virtual void SetActive(bool);
  virtual bool Load();
  virtual void Draw();
  virtual void DrawOverlays();
  virtual void MouseButton(bool, bool, bool);

  // Set name of player to delete
  void SetName(const std::string& name);

  // Called when delete is confirmed.
  void DeletePlayer();

protected:
  // Name of player to delete
  std::string m_name;

  // Confirm buttons - ok/cancel
  PPoolGuiElement m_pOkButton;
  PPoolGuiElement m_pCancelButton;
};
}

#endif

