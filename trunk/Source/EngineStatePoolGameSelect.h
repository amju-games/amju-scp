/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStatePoolGameSelect.h,v $
Revision 1.1.2.2  2007/07/18 22:14:59  jay
Add Update so we can update PoolOnline

Revision 1.1.2.1  2006/08/14 17:50:13  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:26  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.8.4  2005/10/21 17:06:48  jay
Added extra buttons for Options and Editing player names.

Revision 1.1.8.3  2005/08/26 21:26:29  jay
Add Pool Background.. general fixes

Revision 1.1.8.2  2005/07/17 23:02:39  jay
Add quit button; configurable button sizes

Revision 1.1.8.1  2005/05/24 20:39:21  jay
Now with 4 player possibilities

Revision 1.1  2004/09/08 15:43:04  jay
Added to repository
  
*/

#if !defined(ENGINE_STATE_GOLF_GAME_SELECT_H_INCLUDED)
#define ENGINE_STATE_GOLF_GAME_SELECT_H_INCLUDED

#include "EngineStateText.h"
#include "TextLeaf.h"
#include "CharacterGameObject.h"
#include "GuiElement.h"

namespace Amju
{
// POOL
// This is the "Main Menu" - the menu users get after clicking on Start.
// Select the number of players
// TODO Options
// TODO Buy Now button if not registered
class EngineStatePoolGameSelect : public EngineStateText
{
public:
  static const char* Name;

  virtual void SetActive(bool);
  virtual bool Load();
  virtual void Red(bool down);
  virtual void Draw();
  virtual void DrawOverlays();
  virtual void MouseButton(bool, bool, bool);
  virtual void Update();

  // Go back to title state if nothing happens for a while
  virtual void TimerExpired();

protected:
  bool LoadCharacter(const std::string&);

protected:
  PGuiElement m_pOnePlayerButton, m_pTwoPlayerButton, 
    m_pOnePlayerPractiseButton, m_pTwoPlayerOnlineButton;

  // As this is the "Main Menu" from a user perspective, we need other
  // menu items.
  PGuiElement m_pQuitButton;
  PGuiElement m_pOptionsButton;
  PGuiElement m_pRegButton;
  PGuiElement m_pBuyButton;
  PGuiElement m_pEditNamesButton; // edit/del/copy player names
  
  // Characters - one for one player, two for two player.
  std::vector<RCPtr<CharacterGameObject> > m_characters;

};
}

#endif

