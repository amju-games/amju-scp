/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: EngineStatePoolChoosePlayer.h,v $
Revision 1.1.2.2  2006/08/14 17:50:13  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:31  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.3  2005/08/26 21:34:16  jay
Add Pool Background, new Font for buttons, cancel button

Revision 1.1.2.2  2005/06/13 22:19:47  jay
Ongoing improvements to course/game/player select screens

Revision 1.1.2.1  2005/06/04 22:41:15  jay
Choose player from list. Needs work because 3D menus are not very useable.

*/

#ifndef ENGINE_STATE_POOL_CHOOSE_PLAYER
#define ENGINE_STATE_POOL_CHOOSE_PLAYER

#include "EngineStateText.h"
#include "Menu.h"

namespace Amju
{
// Currrent player chooses name from list.
// If player clicks on "new player", go to enter name state.
// If player clicks on an existing name, show options to delete,
// continue, etc. 
class EngineStatePoolChoosePlayer : public EngineStateText
{
public:
  static const char* Name;

  virtual void SetActive(bool);
  virtual bool Load();
  virtual void Red(bool down);
  virtual void Draw();
  virtual void DrawOverlays();
  virtual void MouseButton(bool, bool, bool);

  void SetNumberOfNamesToGet(int num);

  // Called when one of the PlayerButtons is clicked.
  void ChooseButton(int i);

protected:
//  PMenu m_pPlayerNames; 
  int m_numNamesToGet;
  // Current player index - should be 0 or 1.
  int m_currentPlayer;

  // Back button - go back to select no. of players
  PPoolGuiElement m_pBackButton; 

  // 2D buttons rather than menu, with names and stats
  struct PlayerButton
  {
    PPoolGuiElement m_pButton; // select this player
    PPoolGuiElement m_pEditButton; // edit player name
    PPoolGuiElement m_pDeleteButton; // delete this player
    std::string m_name;
    std::string m_stats;
    float m_y; // text Y-pos
    bool m_enabled;
    // Use this flag to decide if player has to enter name or not
    bool m_nameIsSet;
  };
  typedef std::vector<PlayerButton> Buttons;
  Buttons m_buttons;

  // Remember choice for player 1, so we can disable it for player 2.
  int m_player1Choice;
};
}

#endif


