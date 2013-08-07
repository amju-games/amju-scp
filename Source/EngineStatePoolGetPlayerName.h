/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: EngineStatePoolGetPlayerName.h,v $
Revision 1.1.2.3  2007/07/27 08:43:57  jay
Factor out copied code into function: the "copied" code was actually
 different, causing a bug

Revision 1.1.2.2  2006/08/14 17:50:14  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:32  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.3  2005/08/26 21:40:28  jay
Add OK and Cancel buttons

Revision 1.1.2.2  2005/06/04 22:32:46  jay
Remove menu, add to ChoosePlayer state

Revision 1.1.2.1  2005/05/24 20:46:40  jay
New state to get player names

*/

#ifndef ENGINE_STATE_POOL_GET_PLAYER_NAME_H_INCLUDED
#define ENGINE_STATE_POOL_GET_PLAYER_NAME_H_INCLUDED

#include "EngineStateEnterText.h"
#include "Menu.h"
#include "TexturedQuad.h"
#include "GuiElement.h"

namespace Amju
{
class EngineStatePoolGetPlayerName : public EngineStateEnterText
{
  friend void OnGetPlayerOk();

public:
  static const char* Name;

  EngineStatePoolGetPlayerName();

  virtual void SetActive(bool);
  virtual void Draw();
  virtual void DrawOverlays();
  virtual void OnKey(char key, bool down);
  virtual bool Load();
  // Handle the problem that the space bar cannot normally be used
  // in name.
  virtual void Red(bool b);

  // User clicks menu of previous names
  virtual void MouseButton(bool down, bool ctrl, bool shift);

  // Set player index - should be 0 or 1.
  void SetPlayerToGet(int n);
  // Set number of players chosen - should be 1 or 2.
  void SetNumberOfPlayers(int n);

protected:
  virtual void AlertMaxTextSize();
  virtual void Finished();
  virtual void Back();

  void AddMenuText(const std::string&);

  void GoToGameSelectState();

protected:
  int m_numPlayers;
  int m_currentPlayer;

  std::string m_errorText;

  // Text entry box 
  TexturedQuad m_textboxTq;

  PPoolGuiElement m_pOkButton;
  PPoolGuiElement m_pBackButton;
};
}

#endif



