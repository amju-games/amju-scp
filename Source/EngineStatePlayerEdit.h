/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: EngineStatePlayerEdit.h,v $
Revision 1.1.2.2  2006/08/14 17:50:12  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:31  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.1  2005/10/21 17:43:08  jay
This state lets user edit a player name

*/

#ifndef ENGINE_STATE_PLAYER_EDIT_H_INCLUDED
#define ENGINE_STATE_PLAYER_EDIT_H_INCLUDED

#include "EngineStateEnterText.h"
#include "GuiElement.h"

namespace Amju
{
class EngineStatePlayerEdit : public EngineStateEnterText
{
public:
  static const char* Name;

  EngineStatePlayerEdit();

  virtual void SetActive(bool);
  virtual void Draw();
  virtual void DrawOverlays();
  virtual void OnKey(char key, bool down);
  virtual bool Load();
  // Handle the problem that the space bar cannot normally be used
  // in name.
  virtual void Red(bool b);

  virtual void MouseButton(bool down, bool ctrl, bool shift);
  virtual void MousePos(int x, int y);
  virtual void Finished();
  virtual void Back();

  void SetName(const std::string& name);

protected:
  PGuiElement m_pOkButton;
  PGuiElement m_pBackButton;
  std::string m_errorText;
  std::string m_oldName; // name we are changing
};
}

#endif


