/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: EngineStatePoolChoosePlayer.cpp,v $
Revision 1.1.2.6  2007/07/21 23:42:36  Administrator
Store Online ID in player names: only request ID from server when uploading
stats for playing an online game for the first time.

Revision 1.1.2.5  2007/05/26 21:04:39  jay
Fixes for Table state after one game, etc.

Revision 1.1.2.4  2006/08/30 21:12:13  Administrator
Get Pool to build in MSVC

Revision 1.1.2.3  2006/08/26 21:28:36  jay
Ongoing changes for Online game; all Online features are off unless
POOL_ONLINE is defined

Revision 1.1.2.2  2006/08/14 17:50:13  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:31  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.10  2005/10/28 12:48:02  jay
Remove dead edit/delete classes; change to different state if Online game.

Revision 1.1.2.9  2005/10/21 17:07:55  jay
Removed edit and delete buttons, they are not used here.
Make sure player names are updated on SetActive, in case they have
been edited/deleted.

Revision 1.1.2.8  2005/09/20 13:16:23  jay
Show stars + hearts

Revision 1.1.2.7  2005/09/05 20:06:43  jay
Show player stats on button

Revision 1.1.2.6  2005/08/26 21:34:16  jay
Add Pool Background, new Font for buttons, cancel button

Revision 1.1.2.5  2005/08/02 18:32:18  jay
Match button pos/size to Player Select buttons.

Revision 1.1.2.4  2005/06/24 05:38:23  Administrator
Fixes to get Scene Editor to compile

Revision 1.1.2.3  2005/06/22 21:40:39  jay
New buttons

Revision 1.1.2.2  2005/06/13 22:19:47  jay
Ongoing improvements to course/game/player select screens

Revision 1.1.2.1  2005/06/04 22:41:15  jay
Choose player from list. Needs work because 3D menus are not very useable.

*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "EngineStatePoolChoosePlayer.h"
#include "EngineStatePoolGetPlayerName.h"
#include "EngineStatePoolCourseSelect.h"
#include "EngineStatePoolGameSelect.h"
#include "Engine.h"
#include "TextMenu.h"
#include "Directory.h"
#include "TextFactory.h"
#include "PlayerNames.h"
#include "PoolPlayerStats.h"
#include "StringUtils.h"
#include "TextWriter.h"
#include "GuiButton.h"
#include "Font.h"
#include "PoolBg.h"
#include "PoolMisc.h"
#include "EngineStatePoolOnlineTable.h"

namespace Amju
{
static const int MAX_PLAYERS = 4;

const char* EngineStatePoolChoosePlayer::Name = "pool-choose-player";
// Register this State with the Engine.
static const bool registeredtitle = Engine::Instance()->
  RegisterEngineState(EngineStatePoolChoosePlayer::Name, new EngineStatePoolChoosePlayer);

void OnPlayerChooseCancel()
{
  Engine::Instance()->ChangeState(EngineStatePoolGameSelect::Name,
    Engine::IMMEDIATE); 
}

void EngineStatePoolChoosePlayer::SetNumberOfNamesToGet(int n)
{
  m_numNamesToGet = n;
  m_currentPlayer = 0;
  
  // Set the number of players in the Name Entry state, so from there we know
  // whether to come back here or go on to the next state.
  ((EngineStatePoolGetPlayerName*)
    Engine::Instance()->GetEngineState(EngineStatePoolGetPlayerName::Name).GetPtr())->SetNumberOfPlayers(n);
}

void EngineStatePoolChoosePlayer::SetActive(bool active)
{
#ifdef _DEBUG
std::cout << "** Setting up buttons: m_currentPlayer = " << m_currentPlayer << "\n";
#endif

  if (!active)
  {
    return;
  }

  ThePlayerNames::Instance()->Load();
  int numNames = ThePlayerNames::Instance()->GetNumNames();
/*
  // If there are no existing player names, go to the name entry state.
  if (numNames == 0)
  {
    Engine::Instance()->ChangeState(EngineStatePoolGetPlayerName::Name,
      Engine::IMMEDIATE);
    return;
  }
*/

  // Set names and stats in each button
  if (numNames > MAX_PLAYERS)
  {
    Assert("Too many players!" && 0);
    numNames = MAX_PLAYERS;
  }
  Assert((int)m_buttons.size() == MAX_PLAYERS);
  int i;
  // Reset names on each button, so if a name was deleted it will
  // no longer appear.
  for (i = 0; i < MAX_PLAYERS; i++)
  {
    std::string s = "new name ";
    s += ToString(i + 1);
    m_buttons[i].m_name = s;
    m_buttons[i].m_stats = "";
  }

  for (i = 0; i < numNames; i++)
  {
    std::string name = ThePlayerNames::Instance()->GetName(i);
    m_buttons[i].m_name = name; 
    // Set stats string for this player
    PoolPlayerStats stats(name);
    stats.Load();
    int won, lost, d;
    stats.GetStats(&won, &lost, &d);
    std::string s = "Won: ";
    s += ToString(won);
    s += " Lost: ";
    s += ToString(lost);
    s += " \" ";
    s += ToString(stats.GetNumBonuses(1)); 
    s += " $ "; // star
    s += ToString(stats.GetNumBonuses(2));
    // TODO other types

    m_buttons[i].m_stats = s;

    m_buttons[i].m_nameIsSet = true;
    // TODO Set disabled for name chosen by player 1
    m_buttons[i].m_pButton->SetEnabled(true);
  }
  // Enable first 'new name' button, disable the rest
  if (numNames < MAX_PLAYERS)
  {
    m_buttons[numNames].m_pButton->SetEnabled(true);
  }
  for (i = numNames + 1; i < MAX_PLAYERS; i++)
  {
    m_buttons[i].m_pButton->SetEnabled(false);
  }
  if (m_currentPlayer > 0)
  {
    // Disable previous player's choice
#ifdef _DEBUG
std::cout << "Disable player 1 choice: " << m_player1Choice << "\n";
#endif
    m_buttons[m_player1Choice].m_pButton->SetEnabled(false);
  }

  // DON'T set this here, as we return to this state from
  //  entering player 1 name.
  // m_currentPlayer = 0;
}

class PlayerSelectCommand : public GuiCommand
{
public:
  PlayerSelectCommand(int i): m_num(i) {}
  virtual bool Do()
  {
    ((EngineStatePoolChoosePlayer*)
      Engine::Instance()->GetEngineState(EngineStatePoolChoosePlayer::Name
      ).GetPtr())->ChooseButton(m_num);
  
    return false; // no undo  
  }
private:
  // Button number
  int m_num;
};

bool EngineStatePoolChoosePlayer::Load()
{
  if (!m_bg.Load(GetEngine()->GetConfigValue("file_bg"), ""))
  {
    return false;
  }

//  static const float y = Engine::Instance()->GetConfigFloat("golf_gs_b_y");
//  static const float x2 = Engine::Instance()->GetConfigFloat("golf_gs_b_x2");

  static const float x1 = Engine::Instance()->GetConfigFloat(
    "pool_choose_button_x1");
  static const float SIZE_W = Engine::Instance()->GetConfigFloat(
    "pool_choose_button_w");
  static const float SIZE_H = Engine::Instance()->GetConfigFloat(
    "pool_choose_button_h");
  static const float SPACE_H = Engine::Instance()->GetConfigFloat(
    "pool_choose_button_h_space");

  m_pBackButton = new GuiButton;
  std::string backFile = "choose-player-cancel.txt"; // TODO CONFIG
  if (!m_pBackButton->Load(backFile))
  {
    return false;
  } 
  m_pBackButton->SetSize(SIZE_W, SIZE_H);
  m_pBackButton->SetRelPos(4.0 * SPACE_H + 3.0f, x1);
  m_pBackButton->SetCommand(&OnPlayerChooseCancel);

  // Load player buttons. The Textured quad is the same for each one,
  // but the text is different.
  for (int i = 0; i < MAX_PLAYERS; i++)
  {
    PlayerButton pb;
    GuiElement* pGuiButton = new GuiButton;
    if (!pGuiButton->Load(std::string("pool-player-button.txt")))
    {
      return false;
    } 
    pGuiButton->SetSize(SIZE_W, SIZE_H);
    pGuiButton->SetRelPos(i * SPACE_H + 3.0f, x1);
    pGuiButton->SetCommand(new PlayerSelectCommand(i));
    pb.m_pButton = pGuiButton;

    pb.m_y = i * SPACE_H + 3.2f; // TODO TEMP TEST
    pb.m_name = "New name ";
    pb.m_name += ToString(i + 1);
    pb.m_stats = ""; 
    pb.m_enabled = true;
    pb.m_nameIsSet = false; // will be set for some in SetActive()
    m_buttons.push_back(pb);
  }

  return true;
}

void EngineStatePoolChoosePlayer::Red(bool down)
{
}

void EngineStatePoolChoosePlayer::Draw()
{
  AmjuGL::PushMatrix();
  DrawBg();
  
  m_pCam->Update();
  m_pCam->Draw();
  
  m_lighting.DrawLight();
  
  m_or.Draw();

  ThePoolBg::Instance()->Draw();
  AmjuGL::PopMatrix();
}

void EngineStatePoolChoosePlayer::DrawOverlays()
{
  Font* pFont = TheFontManager::Instance()->GetFont("cheri-1.0");
  const float size = pFont->GetSize();

  AmjuGL::PushAttrib(AmjuGL::AMJU_BLEND);

  ////////glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  AmjuGL::Enable(AmjuGL::AMJU_BLEND);

  std::string s = "Player ";
  s += ToString(m_currentPlayer + 1);
  s += ", please choose your name";
  // Default font
  Engine::Instance()->GetTextWriter()->PrintCentre(1, s.c_str());

  for (int i = 0; i < MAX_PLAYERS; i++)
  {
    const PlayerButton& pb = m_buttons[i];

    pb.m_pButton->Draw();
    if (true) //pb.m_pButton->IsEnabled())
    {
      float scale = 1.0f;
      if (pb.m_pButton->IsMouseOver() || pb.m_pButton->IsSelected())
      {
        scale *= 1.1f;
      }
      float e = 0;
      if (pb.m_pButton->IsSelected())
      {
        e = 0.1f;
      }
      pFont->SetSize(size * scale);
      float centre = 12.0f - 2.5f * pFont->GetTextWidth(pb.m_name.c_str());
      pFont->PrintNoBlend(centre + e, pb.m_y + 0.3f + e, pb.m_name.c_str());

      // Draw stats etc string
      pFont->SetSize(size * scale * 0.5f);
      float centre2 = 12.0f - 1.5f * pFont->GetTextWidth(pb.m_stats.c_str());
      pFont->PrintNoBlend(centre2 + e, pb.m_y + 0.8f + e, pb.m_stats.c_str());

      pFont->SetSize(size);
    }
  }

  AmjuGL::PopAttrib(); // Blend

  m_pBackButton->Draw();
}

void EngineStatePoolChoosePlayer::MouseButton(bool down, bool, bool)
{
  if (down)
  {
    m_mouseDown = true;
  }

  m_pBackButton->MouseButton(down, false, false);

  for (int i = 0; i < MAX_PLAYERS; i++)
  {
    m_buttons[i].m_pButton->MouseButton(down, false, false);
//    m_buttons[i].m_pEditButton->MouseButton(down, false, false);
//    m_buttons[i].m_pDeleteButton->MouseButton(down, false, false);
  }

  if (!down)
  {
    m_mouseDown = false;
  }
}

void EngineStatePoolChoosePlayer::ChooseButton(int i)
{
  // Store this choice so we can disable it for the next player.
  m_player1Choice = i;
    
  // Set player name, then go to next state, or start this state again
  // if there is another player name to choose.
  if (i < ThePlayerNames::Instance()->GetNumNames())
  {
    std::string name = m_buttons[i].m_name; 
    PoolPlayerStats::SetPlayerStats(name, m_currentPlayer); 
    
    m_currentPlayer++;

#ifdef _DEBUG
std::cout << "CHOOSE: You chose " << name.c_str() << ": new m_currentPlayer: " << m_currentPlayer << " - m_numNamesToGet: " << m_numNamesToGet << "\n";
#endif

    // NB >= because m_currentPlayer is zero-based, m_numNamesToGet is 1-based
    if (m_currentPlayer >= m_numNamesToGet)
    {
      // If user chose online game, go to Lobby. Otherwise go right
      //  to the level/room select state.
      if (IsOnlineGame())
      {
#ifdef POOL_ONLINE
        // Get the ID for this player
        int playerId = ThePlayerNames::Instance()->GetId(i);

        ((EngineStatePoolOnlineTable*)
          (GetEngine()->GetEngineState(EngineStatePoolOnlineTable::Name).GetPtr()))->
          SetPlayerName(name, playerId);

        Engine::Instance()->ChangeState(EngineStatePoolOnlineTable::Name,
          Engine::IMMEDIATE);
#endif
      }
      else
      { 
        Engine::Instance()->ChangeState(EngineStatePoolCourseSelect::Name,
          Engine::IMMEDIATE);
      }
    }
    else
    {
      // Get the next name
      // Disable the choice of player 1, etc.
      SetActive(true);
    }
  }
  else
  {
    // Player has chosen "new name" button
    m_currentPlayer++;
    Engine::Instance()->ChangeState(EngineStatePoolGetPlayerName::Name,
      Engine::IMMEDIATE);
  
    ((EngineStatePoolGetPlayerName*)
      Engine::Instance()->GetEngineState(EngineStatePoolGetPlayerName::Name
      ).GetPtr())->SetPlayerToGet(m_currentPlayer - 1);
  }
}
}

