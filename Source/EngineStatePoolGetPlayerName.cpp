/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: EngineStatePoolGetPlayerName.cpp,v $
Revision 1.1.2.7  2007/07/27 08:43:56  jay
Factor out copied code into function: the "copied" code was actually
 different, causing a bug

Revision 1.1.2.6  2007/07/21 23:42:36  Administrator
Store Online ID in player names: only request ID from server when uploading
stats for playing an online game for the first time.

Revision 1.1.2.5  2007/05/26 21:04:39  jay
Fixes for Table state after one game, etc.

Revision 1.1.2.4  2007/05/16 22:03:54  Administrator
MSVC warning

Revision 1.1.2.3  2006/08/26 21:28:36  jay
Ongoing changes for Online game; all Online features are off unless
POOL_ONLINE is defined

Revision 1.1.2.2  2006/08/14 17:50:13  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:31  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.8  2005/10/28 12:48:24  jay
Change to different Engine State if Online game.

Revision 1.1.2.7  2005/10/21 17:08:20  jay
Removed unused Menu

Revision 1.1.2.6  2005/09/04 20:54:19  Administrator
Turn off key debug output

Revision 1.1.2.5  2005/08/26 21:40:28  jay
Add OK and Cancel buttons

Revision 1.1.2.4  2005/07/30 12:19:08  jay
Debug code - TODO handle backspace key

Revision 1.1.2.3  2005/07/06 20:18:30  jay
Check for existing name

Revision 1.1.2.2  2005/06/04 22:32:46  jay
Remove menu, add to ChoosePlayer state

Revision 1.1.2.1  2005/05/24 20:46:40  jay
New state to get player names

*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "Common.h"
#include "EngineStatePoolGetPlayerName.h"
#include "Engine.h"
#include "EngineStatePoolCourseSelect.h"
#include "TextWriter.h"
#include "PoolGameState.h"
#include "StringUtils.h"
#include "PoolPlayerStats.h"
#include "TextMenu.h"
#include "TextFactory.h"
#include "Directory.h"
#include "GuiButton.h"
#include "PlayerNames.h"
#include "EngineStatePoolChoosePlayer.h"
#include "EngineStatePoolGameSelect.h"
#include "PoolBg.h"
#include "PoolMisc.h"
#include "EngineStatePoolOnlineTable.h"

namespace Amju
{
const char* EngineStatePoolGetPlayerName::Name = "pool-get-name";
// Register this State with the Engine.
static const bool registeredtitle = Engine::Instance()->
  RegisterEngineState(EngineStatePoolGetPlayerName::Name, new EngineStatePoolGetPlayerName);

void OnGetPlayerCancel()
{
  Engine::Instance()->ChangeState(EngineStatePoolGameSelect::Name,
    Engine::IMMEDIATE);
}

void OnGetPlayerOk()
{
  ((EngineStatePoolGetPlayerName*)(Engine::Instance()->GetEngineState(
   EngineStatePoolGetPlayerName::Name).GetPtr()))->Finished(); 
}

EngineStatePoolGetPlayerName::EngineStatePoolGetPlayerName()
{
}

void EngineStatePoolGetPlayerName::Draw()
{
  AmjuGL::PushMatrix();
  DrawBg();
  m_pCam->Update();
  m_pCam->Draw();

  m_lighting.DrawLight();

//  AmjuGL::PushMatrix();
  m_or.Draw();
  ThePoolBg::Instance()->Draw();
//  AmjuGL::PopMatrix();


  // Box containing player name text
//  m_textboxTq.Draw(2.5, 2.0, 7.5, 24.0); // t, l, b, r

//  m_or.Draw();
  static const float nameY = Engine::Instance()->GetConfigFloat("pool_name_y");
  AmjuGL::Translate(0, nameY, 0); // above menu of old names
  static const float z = Engine::Instance()->GetConfigFloat("hi_score_text_z2");
  AmjuGL::Translate(0, 0, z);
  //m_pGreetText->Draw();
  AmjuGL::Translate(0, -3.0f, 0);
  AmjuGL::PushMatrix();
  // Draw the text being edited:
  AmjuGL::Translate(-m_totalSize/2.0f, 0, -4.0f); // TODO TEMP TEST was -m_totalSize);
  // Draw the text to the left of the cursor
  m_pComp->Draw();
  // Draw the text to the right of the cursor
  AmjuGL::Translate(m_leftSize + 0.5f, 0, 0);
  DrawCursor();
  m_pComp2->Draw();
  AmjuGL::PopMatrix();

  AmjuGL::PopMatrix();

  std::string s = "Player ";
  s += ToString(m_currentPlayer + 1);
  s += ", please type your name then hit return";
  Engine::Instance()->GetTextWriter()->PrintCentre(3.0f, s.c_str());
  Engine::Instance()->GetTextWriter()->PrintCentre(10.0f, m_errorText.c_str());
}

void EngineStatePoolGetPlayerName::DrawOverlays()
{
  m_pOkButton->Draw();
  m_pBackButton->Draw();
}

void EngineStatePoolGetPlayerName::Back()
{
  Engine::Instance()->ChangeState(EngineStatePoolChoosePlayer::Name,
    Engine::IMMEDIATE);
}

void EngineStatePoolGetPlayerName::AlertMaxTextSize()
{
  // TODO
}

void EngineStatePoolGetPlayerName::GoToGameSelectState()
{
    if (IsOnlineGame())
    {
#ifdef POOL_ONLINE
      ((EngineStatePoolOnlineTable*)(GetEngine()->GetEngineState(EngineStatePoolOnlineTable::Name).GetPtr()))->
        SetPlayerName(m_text, -1 /* ID not known yet */); 

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

void EngineStatePoolGetPlayerName::Finished()
{
  m_errorText = "";

  if (m_text.empty())
  {
    m_errorText = "Please enter your name!";
    return;
  }

  m_text = ToLower(m_text);

  // Check this name does not already exist
  if (ThePlayerNames::Instance()->NameExists(m_text))
  {
    m_errorText = "That name already exists! Please type a new name.";
    return;
  }

  ThePlayerNames::Instance()->AddName(m_text);
  ThePlayerNames::Instance()->Save();

  PoolPlayerStats::SetPlayerStats(m_text, m_currentPlayer);
 
  Assert(m_currentPlayer == 0 || m_currentPlayer == 1);  
  if (m_currentPlayer == 1)
  {
#ifdef _DEBUG
std::cout << "GetPlayerName: m_currentPlayer == 1, go to Course select\n";
#endif

    // No more player names to enter - go to game select
    GoToGameSelectState();
  }
  else if (m_numPlayers == 1) 
  {
#ifdef _DEBUG
std::cout << "GetPlayerName: m_numPlayers == 1, go to Course select\n";
#endif

    // No more player names to enter - go to game select
    GoToGameSelectState();
  }
  else
  {
#ifdef _DEBUG
std::cout << "GetPlayerName: m_numPlayers == 2, go back to Choose state\n";
#endif

    Assert(m_numPlayers == 2);
    // Another name to choose 
    Engine::Instance()->ChangeState(EngineStatePoolChoosePlayer::Name,
      Engine::IMMEDIATE);
  }
}

#define KEY_DEBUG
void EngineStatePoolGetPlayerName::OnKey(char key, bool down)
{
  // TODO handle backspace 
#ifdef KEY_DEBUG
std::cout << "Character: " << (int)key << "\n";
#endif

  if (down)
  {
    static const std::string wavFile = Engine::Instance()->GetConfigValue(
      "gui_element_click_wav");
    Engine::Instance()->PlayWav(wavFile);

    m_errorText = "";
  }
  EngineStateEnterText::OnKey(key, down);
}

void EngineStatePoolGetPlayerName::Red(bool down)
{
  // Allow spaces
  OnKey(' ', down);
}

bool EngineStatePoolGetPlayerName::Load()
{
  if (!EngineStateEnterText::Load())
  {
    return false;
  }

  // Get the text entry TQ
  if (!m_textboxTq.Load("power-guage2.png", "power-guage2-a.png")) 
    // TODO CONFIG
  {
    return false;
  }

  // Create OK and Back buttons.
  m_pOkButton = new GuiButton;
  m_pBackButton = new GuiButton; 

  static const float x1 = Engine::Instance()->GetConfigFloat(
    "pool_get_button_x1");
  static const float x2 = Engine::Instance()->GetConfigFloat(
    "pool_get_button_x2");

  std::string backFile = "golf-reg-cancel-button.txt"; // TODO CONFIG
  if (!m_pBackButton->Load(backFile))
  {
    return false;
  }
  m_pBackButton->SetSize(2.0f, 2.0f);
  m_pBackButton->SetRelPos(12.0f, x1);
  m_pBackButton->SetCommand(&OnGetPlayerCancel);

  std::string okFile = "pool-ok-button.txt"; // TODO CONFIG
  if (!m_pOkButton->Load(okFile))
  {
    return false;
  }
  m_pOkButton->SetSize(4.0f, 2.0f);
  m_pOkButton->SetRelPos(12.0f, x2);
  m_pOkButton->SetCommand(&OnGetPlayerOk);

  return true;
}

void EngineStatePoolGetPlayerName::SetPlayerToGet(int n)
{
  m_currentPlayer = n;
}

void EngineStatePoolGetPlayerName::SetNumberOfPlayers(int n)
{
  m_numPlayers = n;
}

void EngineStatePoolGetPlayerName::SetActive(bool active)
{
  EngineStateEnterText::SetActive(active);

  if (active)
  {
    // No TimerExpired() - don't time out.
    m_maxTime = 12000.0f;
    m_time = 0;
    // Get last player name TODO
    m_text = ""; 
    m_pos = m_text.size();
    MakeText();

    m_errorText = "";
    m_currentPlayer = 0; 
  }
}

void EngineStatePoolGetPlayerName::MouseButton(bool down, bool ctrl, bool shift)
{ 
  m_pOkButton->MouseButton(down, ctrl, shift);
  m_pBackButton->MouseButton(down, ctrl, shift);
    
  if (down)
  {
    m_mouseDown = true;
  }

/*    
  if (m_mouseDown && !down)
  {
////    std::cout << "Selecting menu item.\n";
    int c = m_pMenu->Choose();
 
    if (c >= 0)
    {
      Assert(c < m_names.size());
      // The selected text is written to the editable text.
      m_text = m_names[c]; 
      m_pos = m_text.size(); // TODO off by one or ok ?
      MakeText();
    }
  }
*/

  if (!down)
  {
    m_mouseDown = false;
  }
}
}


