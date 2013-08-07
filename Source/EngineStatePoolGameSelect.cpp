/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStatePoolGameSelect.cpp,v $
Revision 1.1.2.3  2007/07/18 22:15:07  jay
Update PoolOnline

Revision 1.1.2.2  2006/08/26 21:28:36  jay
Ongoing changes for Online game; all Online features are off unless
POOL_ONLINE is defined

Revision 1.1.2.1  2006/08/14 17:50:13  jay
Rename "Pool"

Revision 1.1.2.2  2006/08/07 20:42:49  jay
Online Pool - not working yet

Revision 1.1.2.1  2006/07/24 18:20:26  jay
Moved Pool code from Pool/ to Pool/

Revision 1.2.6.12  2006/04/11 17:03:14  jay
Centre main menu

Revision 1.2.6.11  2006/03/24 22:58:17  jay
Disable Reg/Buy buttons if not unlockable

Revision 1.2.6.10  2006/01/20 13:43:32  jay
Rearranged buttons

Revision 1.2.6.9  2005/11/06 09:39:42  jay
Make buttons less wide; auto-centre.

Revision 1.2.6.8  2005/10/28 12:47:20  jay
Fix code to start 2-player online game

Revision 1.2.6.7  2005/10/21 17:06:48  jay
Added extra buttons for Options and Editing player names.

Revision 1.2.6.6  2005/08/26 21:26:29  jay
Add Pool Background.. general fixes

Revision 1.2.6.5  2005/07/17 23:02:39  jay
Add quit button; configurable button sizes

Revision 1.2.6.4  2005/06/13 22:19:46  jay
Ongoing improvements to course/game/player select screens

Revision 1.2.6.3  2005/06/04 22:30:31  jay
Cosmetic changes; remove some dead code

Revision 1.2.6.2  2005/05/24 20:39:21  jay
Now with 4 player possibilities

Revision 1.2.6.1  2005/05/15 17:13:38  jay
Don't draw characters

Revision 1.2  2004/09/15 09:25:00  Administrator
Changes to enable the Player Stats state. "Skill levels" of each player
are shown before each Hole starts.

Revision 1.1  2004/09/08 15:43:04  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "EngineStatePoolGameSelect.h"
#include "EngineStateTitle.h"
#include "Engine.h"
#include "TextMenu.h"
#include "StringUtils.h"
#include "PoolGameState.h"
#include "EngineStatePoolCourseSelect.h"
#include "PoolCourseManager.h"
#include "LevelServer.h"
#include "TextWriter.h"
#include "PoolMisc.h"
#include "Charactermanager.h"
#include "GuiButton.h"
#include "TextFactory.h"
#include "TextColourizerColourList.h"
#include "EngineStatePoolChoosePlayer.h"
#include "PoolBg.h"
#include "EngineStatePoolOnlineTable.h"
#include "EngineStateOptions.h"
#include "EngineStatePoolPlayerMaint.h"
#include "PoolOnline.h"

extern void OnExitClicked();
extern void OnRegisterClicked();
extern void OnBuyButton();

namespace Amju
{
void ReportError(const std::string&);

bool IsRegistered();

const char* EngineStatePoolGameSelect::Name = "golf_game_select"; 
// Register this State with the Engine.
static const bool registered = Engine::Instance()->
  RegisterEngineState(EngineStatePoolGameSelect::Name, new EngineStatePoolGameSelect);

void OnEditNames()
{
  Engine::Instance()->ChangeState(EngineStatePoolPlayerMaint::Name,
    Engine::IMMEDIATE);
}

void OnOptions()
{
  Engine::Instance()->ChangeState(EngineStateOptions::Name,
    Engine::IMMEDIATE);
}

void OnOnePlayerPractise()
{
#ifdef _DEBUG
std::cout << "** START one player practice game\n";
#endif

  SetIsOnlineGame(false);
  StartGame(1, true /* friendly */);

  ((EngineStatePoolChoosePlayer*)
    Engine::Instance()->GetEngineState(EngineStatePoolChoosePlayer::Name).GetPtr())->SetNumberOfNamesToGet(1);

  Engine::Instance()->ChangeState(
    EngineStatePoolChoosePlayer::Name, Engine::IMMEDIATE);
}

void OnOnePlayer()
{
#ifdef _DEBUG
std::cout << "** START one player game\n";
#endif

  SetIsOnlineGame(false);
  StartGame(1, false /* not friendly */);

  ((EngineStatePoolChoosePlayer*)
    Engine::Instance()->GetEngineState(EngineStatePoolChoosePlayer::Name).GetPtr())->SetNumberOfNamesToGet(1);

  Engine::Instance()->ChangeState(
    EngineStatePoolChoosePlayer::Name, Engine::IMMEDIATE);
}

void OnTwoPlayer()
{
#ifdef _DEBUG
std::cout << "** START TWO player game\n";
#endif

  SetIsOnlineGame(false);
  StartGame(2, false /* not friendly */);

  ((EngineStatePoolChoosePlayer*)
    Engine::Instance()->GetEngineState(EngineStatePoolChoosePlayer::Name).GetPtr())->SetNumberOfNamesToGet(2);

  Engine::Instance()->ChangeState(
    EngineStatePoolChoosePlayer::Name, Engine::IMMEDIATE);
}

void OnTwoPlayerOnline()
{
#ifdef _DEBUG
std::cout << "** START TWO player ONLINE game\n";
#endif

  SetIsOnlineGame(true);
  StartGame(2, false /* not friendly */);

  // Only need to get one player name, then go to online lobby page.
  ((EngineStatePoolChoosePlayer*)
    Engine::Instance()->GetEngineState(EngineStatePoolChoosePlayer::Name).GetPtr())->SetNumberOfNamesToGet(1);

  Engine::Instance()->ChangeState(
    EngineStatePoolChoosePlayer::Name, Engine::IMMEDIATE);
}

void EngineStatePoolGameSelect::Update()
{
  EngineStateText::Update();
#ifdef POOL_ONLINE
  ThePoolOnlineManager::Instance()->Update();
#endif
}

void EngineStatePoolGameSelect::SetActive(bool active)
{
  if (active)
  {
#ifdef POOL_ONLINE
    // Reset any online game state
    ThePoolOnlineManager::Instance()->Reset();
#endif

    std::string song = GetEngine()->GetConfigValue("golf_song_ball_in_hole");
    GetEngine()->PlaySong(song);

    m_time = 0;
    m_maxTime = 30.0f; // back to title after this delay

    // Set positions of buttons
    float y = Engine::Instance()->GetConfigFloat("golf_gs_b_y");
    
    static const float SIZE_W = Engine::Instance()->GetConfigFloat(
      "pool_main_button_w");
    static const float x1 = 12.5f - 0.5f * SIZE_W;
    
    static const float SPACE_H = Engine::Instance()->GetConfigFloat(
      "pool_main_button_h_space"); 

    bool registered = IsRegistered();
    bool onePlayerPractice = true; 
    bool twoPlayer = true; 
#ifdef POOL_ONLINE
    bool twoPlayerOnline = true; 
#else
    bool twoPlayerOnline = false; 
#endif
    
    // Move up if we are going to show extra buttons
    if (registered)
    {
      y += SPACE_H;
    }
    if (onePlayerPractice)
    {
      y -= SPACE_H * 0.5f;
    }
    if (twoPlayer)
    {
      y -= SPACE_H * 0.5f;
    }
    if (twoPlayerOnline)
    {
      y -= SPACE_H * 0.5f;
    }
    if (onePlayerPractice)
    {
      m_pOnePlayerPractiseButton->SetRelPos(y, x1);
      y += SPACE_H;
    }
    m_pOnePlayerButton->SetRelPos(y, x1);
    y += SPACE_H;
    if (twoPlayer)
    {
      m_pTwoPlayerButton->SetRelPos(y, x1);
      y += SPACE_H;
    }
    if (twoPlayerOnline)
    {
      m_pTwoPlayerOnlineButton->SetRelPos(y, x1);
      y += SPACE_H;
    }
    m_pOptionsButton->SetRelPos(y, x1);
    y += SPACE_H;
    m_pEditNamesButton->SetRelPos(y, x1);
    y += SPACE_H;
    if (!registered)
    {
      m_pRegButton->SetRelPos(y, x1);
      y += SPACE_H;
      m_pBuyButton->SetRelPos(y, x1);
      y += SPACE_H;
    }
    m_pQuitButton->SetRelPos(y, x1);

  }
}

void EngineStatePoolGameSelect::TimerExpired()
{
  Engine::Instance()->ChangeState(
    EngineStateTitle::Name, Engine::IMMEDIATE);
}

void EngineStatePoolGameSelect::DrawOverlays()
{
  m_pOnePlayerButton->Draw();
  m_pTwoPlayerButton->Draw();
  m_pOnePlayerPractiseButton->Draw();

  // TODO TEMP TEST -No online game yet ???
  m_pTwoPlayerOnlineButton->Draw();

  m_pQuitButton->Draw();
  m_pOptionsButton->Draw(); 
  m_pEditNamesButton->Draw();

#ifndef AMJU_NOT_UNLOCKABLE
  // If not registered, draw reg and buy now buttons.
  bool registered = IsRegistered();
  if (!registered)
  {
    m_pRegButton->Draw();
    m_pBuyButton->Draw();
  }
#endif
}

void EngineStatePoolGameSelect::MouseButton(bool down, bool, bool)
{
  m_pQuitButton->MouseButton(down, false, false);
  m_pOnePlayerButton->MouseButton(down, false, false);
  m_pTwoPlayerButton->MouseButton(down, false, false);
  m_pOnePlayerPractiseButton->MouseButton(down, false, false);
  m_pTwoPlayerOnlineButton->MouseButton(down, false, false);
  m_pOptionsButton->MouseButton(down, false, false);
  m_pEditNamesButton->MouseButton(down, false, false);

#ifndef AMJU_NOT_UNLOCKABLE
  m_pRegButton->MouseButton(down, false, false);
  m_pBuyButton->MouseButton(down, false, false);
#endif
}

void EngineStatePoolGameSelect::Draw()
{
  AmjuGL::PushMatrix();
  // Paint background
  DrawBg();

  // Do camera work 
  m_pCam->Update();
  m_pCam->Draw();

  // Lighting
  m_lighting.DrawLight();
  
  // Move text according to Function
  //m_pFunc->Recalc(&m_or);
  // Position the text
  m_or.Draw();

//  m_pComp->Draw(); // draw "player select" text
  ThePoolBg::Instance()->Draw();

  AmjuGL::PopMatrix();
}

bool EngineStatePoolGameSelect::LoadCharacter(const std::string& charname)
{
/*
  RCPtr<CharacterGameObject> pChar = new CharacterGameObject;
  pChar->AddMesh(CharacterManager::Instance()->GetCharacter(charname));
  m_characters.push_back(pChar);
*/
  return true;
}

bool EngineStatePoolGameSelect::Load()
{
  if (!EngineStateText::Load())
  {
    return false;
  }

  std::string bg = GetEngine()->GetConfigValue("title_bg");
  if (!m_bg.Load(bg, ""))
  {
    return false;
  }

  m_pOnePlayerButton = new PoolGuiButton;
  std::string onePlayerFile = GetEngine()->GetConfigValue("golf_1p_button");
  if (!m_pOnePlayerButton->Load(onePlayerFile))
  {
    ReportError("Failed to load one player button.");
    return false;
  }

  static const float y = Engine::Instance()->GetConfigFloat("golf_gs_b_y");
//  static const float x2 = Engine::Instance()->GetConfigFloat("golf_gs_b_x2");
 
  static const float SIZE_W = Engine::Instance()->GetConfigFloat(
    "pool_main_button_w");
  static const float SIZE_H = Engine::Instance()->GetConfigFloat(
    "pool_main_button_h"); 
  static const float SPACE_H = Engine::Instance()->GetConfigFloat(
    "pool_main_button_h_space"); 
  
  static const float x1 = 12.5f - 0.5f * SIZE_W;
    //Engine::Instance()->GetConfigFloat("golf_gs_b_x1");

  m_pOnePlayerButton->SetSize(SIZE_W, SIZE_H);
  m_pOnePlayerButton->SetRelPos(y + SPACE_H, x1); 
  m_pOnePlayerButton->SetCommand(&OnOnePlayer); 

  m_pTwoPlayerButton = new PoolGuiButton;
  std::string twoPlayerFile = GetEngine()->GetConfigValue("golf_2p_button");
  if (!m_pTwoPlayerButton->Load(twoPlayerFile)) 
  {
    ReportError("Failed to load two player button.");
    return false;
  }
  m_pTwoPlayerButton->SetSize(SIZE_W, SIZE_H);
  m_pTwoPlayerButton->SetRelPos(y + SPACE_H * 2.0f, x1); 
  m_pTwoPlayerButton->SetCommand(&OnTwoPlayer); 

  m_pOnePlayerPractiseButton = new PoolGuiButton;
  std::string practiseFile = GetEngine()->GetConfigValue("pool_prac_button");
  if (!m_pOnePlayerPractiseButton->Load(practiseFile))
  {
    ReportError("Failed to load one player practise button.");
    return false;
  }
  m_pOnePlayerPractiseButton->SetSize(SIZE_W, SIZE_H);
  m_pOnePlayerPractiseButton->SetRelPos(y, x1); 
  m_pOnePlayerPractiseButton->SetCommand(&OnOnePlayerPractise); 
 
  m_pTwoPlayerOnlineButton = new PoolGuiButton;
  std::string twoPlayerOnlineFile = 
    GetEngine()->GetConfigValue("pool_2p_online_button");
  if (!m_pTwoPlayerOnlineButton->Load(twoPlayerOnlineFile)) 
  {
    ReportError("Failed to load two player online button.");
    return false;
  }
  m_pTwoPlayerOnlineButton->SetSize(SIZE_W, SIZE_H);
  m_pTwoPlayerOnlineButton->SetRelPos(y + SPACE_H * 3.0f, x1); 
  m_pTwoPlayerOnlineButton->SetCommand(&OnTwoPlayerOnline); 

  m_pQuitButton = new PoolGuiButton;
  std::string menuQuitFile = GetEngine()->GetConfigValue("menu_quit_button");
  if (!m_pQuitButton->Load(menuQuitFile))
  {
    ReportError("Failed to load menu quit button.");
    return false;
  }
  m_pQuitButton->SetSize(SIZE_W, SIZE_H);
  m_pQuitButton->SetRelPos(y + SPACE_H * 5.0f, x1); 
  m_pQuitButton->SetCommand(&OnExitClicked); // TODO + confirm ?

  m_pOptionsButton = new PoolGuiButton;
  if (!m_pOptionsButton->Load("menu-options-button.txt"))
  {
    ReportError("Failed to load main menu options button");
    return false;
  }
  m_pOptionsButton->SetSize(SIZE_W, SIZE_H);
  m_pOptionsButton->SetRelPos(y + SPACE_H * 3.0f, x1);
  m_pOptionsButton->SetCommand(&OnOptions);

  m_pEditNamesButton = new PoolGuiButton;
  if (!m_pEditNamesButton->Load("menu-edit-names-button.txt"))
  {
    ReportError("Failed to load main menu edit names button");
    return false;
  }
  m_pEditNamesButton->SetSize(SIZE_W, SIZE_H);
  m_pEditNamesButton->SetRelPos(y + SPACE_H * 4.0f, x1);
  m_pEditNamesButton->SetCommand(&OnEditNames);

  // "Register" button
  m_pRegButton = new PoolGuiButton;
  if (!m_pRegButton->Load("menu-reg-button.txt")) 
  {
    return false;
  }
  m_pRegButton->SetSize(SIZE_W, SIZE_H);
  m_pRegButton->SetRelPos(y + SPACE_H * 7.0f, x1);
  m_pRegButton->SetCommand(&OnRegisterClicked);

  // Buy now button
  m_pBuyButton = new PoolGuiButton;
  if (!m_pBuyButton->Load("menu-buy-button.txt"))
  {
    return false;
  }
  m_pBuyButton->SetSize(SIZE_W, SIZE_H);
  m_pBuyButton->SetRelPos(y + SPACE_H * 6.0f, x1);
  m_pBuyButton->SetCommand(&OnBuyButton);

  return true;
}

void EngineStatePoolGameSelect::Red(bool down)
{
}
}

