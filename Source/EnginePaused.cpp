/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EnginePaused.cpp,v $
Revision 1.1.4.3.2.13  2007/07/25 07:43:24  jay
Quit online game -> OnlineTableState, not main menu

Revision 1.1.4.3.2.12  2007/06/24 12:42:28  jay
Online quit frame -> Main menu

Revision 1.1.4.3.2.11  2007/05/15 12:30:35  jay
If online, go to special game select

Revision 1.1.4.3.2.10  2007/03/20 23:30:31  jay
Confirm Quit buttons

Revision 1.1.4.3.2.9  2007/03/17 23:57:46  jay
Fix for non-online game

Revision 1.1.4.3.2.8  2007/03/10 12:57:44  jay
Quit confirm

Revision 1.1.4.3.2.7  2006/08/14 17:38:03  jay
Rename "Pool"

Revision 1.1.4.3.2.6  2006/07/26 21:25:16  jay
Use Pool/, not Pool/

Revision 1.1.4.3.2.5  2005/10/21 17:03:29  jay
Added fullscreen option

Revision 1.1.4.3.2.4  2005/09/30 12:56:22  jay
Kill any active earthquake

Revision 1.1.4.3.2.3  2005/08/26 21:06:21  jay
Add Pool background

Revision 1.1.4.3.2.2  2005/08/03 19:09:26  jay
Fixed button sizes

Revision 1.1.4.3.2.1  2005/06/29 20:17:28  jay
Changes for POOL look and feel

Revision 1.1.4.3  2004/11/11 11:03:06  Administrator
Fix for full-screen flag

Revision 1.1.4.2  2004/11/10 15:45:07  jay
Changed fullscreen command so it only sets flag, on Windows as well as Mac.
The reason is that on Windows changing from full screen to windowed mode won't
work if the colour depth of the desktop is not the same as in fullscreen mode.

Revision 1.1.4.1  2004/11/09 16:01:30  jay
Merged full screen and exit features from XING 2.1 to this branch.

Revision 1.1.6.4  2004/11/08 12:03:26  Administrator
Fixed badly placed #endif which worked on mac but not windows.

Revision 1.1.6.3  2004/11/08 13:57:49  jay
Added full screen features for mac.

Revision 1.1.6.2  2004/11/08 09:48:48  Administrator
Added Exit and confirm buttons

Revision 1.1.6.1  2004/11/08 08:27:43  Administrator
Added fullscreen button. (Currently only works for windows)

Revision 1.1  2004/09/08 15:42:36  jay
Added to repository
 
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <string>
#include "Common.h" // include os-specific call to quit application
#include "EnginePaused.h"
#include "EngineRunning.h"
#include "EngineStateTitle.h"
#include "EngineStateShowLevel.h"
#include "SolidComponent.h"
#include "CompositeFactory.h"
#include "Engine.h"
#include "LeafData.h"
#include "TextMenu.h"
#include "ExplosionEffect.h"
#include "TextWriter.h"
#include "GuiButton.h"
#include "EngineStatePoolSetUpShot.h"
#include "EngineStatePoolCourseSelect.h"
#include "EngineStatePoolGameSelect.h"
#include "PoolGameState.h"
#include "PoolBg.h"
#include "PoolMisc.h"
#include "EngineStatePoolOnlineTable.h"
#include "SchAssert.h"

using namespace std;

//extern bool IsFullScreen();
extern void GoWindowed();
extern void GoFullScreen();

namespace Amju
{
static float TOP = 2.3f;
static float LEFT = 8.0f;
static float WIDTH = 8.0f;
static float HEIGHT = 2.0f;
static float HSPACE = 2.25f; 

bool IsFullScreenFlagSet()
{
  bool b = (Engine::Instance()->GetGameState()->Get("fullscreen") == "y");
  return b;
}

void OnResume()
{
  Amju::Engine::Instance()->ChangeState(
    Amju::EngineStatePoolSetUpShot::Name,
    Amju::Engine::IMMEDIATE);
}

void OnQuitThisCourse()
{
  EnginePaused::SetShowConfirmButtons(true);

/*
  Amju::Engine::Instance()->ChangeState(
    Amju::EngineStatePoolCourseSelect::Name,
    Amju::Engine::IMMEDIATE);
*/
}

void OnQuitYes()
{
  // Inc. number of games Lost by this player
  // (Applies to Online and offline)
  // But not if this is a practice game
#ifdef POOL_ONLINE
  if (IsOnlineGame())
  {
    // Set game state on server to over
    ThePoolOnlineManager::Instance()->SendGameFinished();

    // Back to Online Table state
    Amju::Engine::Instance()->ChangeState(
      Amju::EngineStatePoolOnlineTable::Name, // NOT main menu
      Amju::Engine::IMMEDIATE);
  }
  else
#endif
  {
    Amju::Engine::Instance()->ChangeState(
      Amju::EngineStatePoolCourseSelect::Name,
      Amju::Engine::IMMEDIATE);
  }
}

void OnQuitNo()
{
  EnginePaused::SetShowConfirmButtons(false);
}

void OnFullScreen()
{
  EnginePaused::SetShowConfirmButtons(false);
  if (IsFullScreenFlagSet())
  {
    // Don't do it. Instead set a flag and tell the user to restart the
    // application. This is the only thing we can do on Mac OS X, and for windows
    // it avoids some problems.
    Engine::Instance()->GetGameState()->Set("fullscreen", "n");
  }
  else
  {
    Engine::Instance()->GetGameState()->Set("fullscreen", "y");
  }
}

void OnMusic()
{
  EnginePaused::SetShowConfirmButtons(false);
  float f = Engine::Instance()->GetSoundPlayer()->GetSongMaxVolume();
  float r = 0;
  if (f == 0)
  {
    r = 1.0f;
  }
  Engine::Instance()->GetSoundPlayer()->SetSongMaxVolume(r);
}

void OnSoundEffects()
{
  EnginePaused::SetShowConfirmButtons(false);
  float f = Engine::Instance()->GetSoundPlayer()->GetWavMaxVolume();
  float r = 0;
  if (f == 0)
  {
    r = 1.0f;
  }
  Engine::Instance()->GetSoundPlayer()->SetWavMaxVolume(r);
}

void OnInternet()
{
}


const std::string EnginePaused::Name = "paused";
// Register this State with the Engine.
static const bool registered = Engine::Instance()->
  RegisterEngineState(EnginePaused::Name, new EnginePaused);

bool EnginePaused::s_showConfirmButtons;

EnginePaused::EnginePaused()
{ 
  m_elapsedTime = 0;
}

void EnginePaused::SetShowConfirmButtons(bool b)
{
  s_showConfirmButtons = b;
}

void EnginePaused::SetActive(bool active)
{
  // Kill any active earthquake
  Engine::Instance()->GetEarthquake().Reset();

  // Freeze the message queue 
  GetEngine()->Freeze(active);
  if (!active)
  {
    return;
  }
  SetShowConfirmButtons(false);
}


bool EnginePaused::Load()
{
  std::string bg = Engine::Instance()->GetConfigValue("paused_bg");
  m_bg.Load(bg, "");

  std::string menuButtonFile = GetEngine()->GetConfigValue("golf_menu_button");

  // Get buttons
  m_pButtonResume = new GuiButton;
  m_pButtonResume->Load("pool-resume-button.txt");
  m_pButtonResume->SetSize(WIDTH, HEIGHT);
  m_pButtonResume->SetCommand(&OnResume);
  m_pButtonResume->SetRelPos(TOP, LEFT);

  m_pButtonQuit = new GuiButton;
  m_pButtonQuit->Load("pool-quit-game.txt");
  m_pButtonQuit->SetSize(WIDTH, HEIGHT);
  m_pButtonQuit->SetCommand(&OnQuitThisCourse);
  m_pButtonQuit->SetRelPos(TOP + 1.0f * HSPACE, LEFT);

  m_pButtonFullScreen = new GuiButton;
  m_pButtonFullScreen->Load("pool-fullscreen.txt");
  m_pButtonFullScreen->SetSize(WIDTH, HEIGHT);
  m_pButtonFullScreen->SetCommand(&OnFullScreen);
  m_pButtonFullScreen->SetRelPos(TOP + 4.0f * HSPACE, LEFT);

  m_pButtonSoundVol = new GuiButton;
  m_pButtonSoundVol->Load("pool-sound.txt");
  m_pButtonSoundVol->SetSize(WIDTH, HEIGHT);
  m_pButtonSoundVol->SetCommand(&OnSoundEffects);
  m_pButtonSoundVol->SetRelPos(TOP + 2.0f * HSPACE, LEFT);

  m_pButtonMusicVol = new GuiButton;
  m_pButtonMusicVol->Load("pool-music.txt");
  m_pButtonMusicVol->SetSize(WIDTH, HEIGHT);
  m_pButtonMusicVol->SetCommand(&OnMusic);
  m_pButtonMusicVol->SetRelPos(TOP + 3.0f * HSPACE, LEFT);

  m_pButtonQuitGame = new GuiButton;
//  m_pButtonQuitGame->Load(menuButtonFile);
//  m_pButtonQuitGame->SetSize(w, h);
//  m_pButtonQuitGame->SetCommand(OnQuitGame);
//  m_pButtonQuitGame->SetRelPos(y + 10.0f, x);  

  m_pButtonQuitYes = new GuiButton;
  m_pButtonQuitYes->Load("quit-yes-button.txt");
  m_pButtonQuitYes->SetSize(WIDTH, HEIGHT);
  m_pButtonQuitYes->SetCommand(&OnQuitYes);
  m_pButtonQuitYes->SetRelPos(TOP + 2.0f * HSPACE, LEFT);

  m_pButtonQuitNo = new GuiButton;
  m_pButtonQuitNo->Load("quit-no-button.txt");
  m_pButtonQuitNo->SetSize(WIDTH, HEIGHT);
  m_pButtonQuitNo->SetCommand(&OnQuitNo);
  m_pButtonQuitNo->SetRelPos(TOP + 3.0f * HSPACE, LEFT);

  if (!m_tqSound.Load("sound.bmp", "pool-exit-a.bmp"))
  {
    return false;
  }
  if (!m_tqNoSound.Load("no-sound.bmp", "pool-exit-a.bmp"))
  {
    return false;
  }

  return true;
}

void EnginePaused::Clear()
{
}

void EnginePaused::Draw()
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

void EnginePaused::DrawOverlays()
{
  // Get full screen setting before any change is made
  static bool initialFullScreen = (Engine::Instance()->GetGameState()->
    Get("fullscreen") == "y");

  if (s_showConfirmButtons)
  {
    Engine::Instance()->GetTextWriter()->PrintCentre(4.0f,
      "Are you sure you want to quit this frame ?");

    m_pButtonQuitYes->Draw();
    m_pButtonQuitNo->Draw();
  }
  else
  {
    m_pButtonResume->Draw();
    m_pButtonQuit->Draw();
    m_pButtonFullScreen->Draw();
    m_pButtonSoundVol->Draw();
    m_pButtonMusicVol->Draw();
    m_pButtonQuitGame->Draw();

    // Positions for on/of icons
    float left = LEFT + 10.0f;
    float right = left + HEIGHT; // square, i.e. HEIGHT * HEIGHT
    float top = TOP + 2.0f * HSPACE;
    float bot = top + HEIGHT;
 
    // Music
    bool music = (Engine::Instance()->GetSoundPlayer()->GetSongMaxVolume() > 0);
    bool sound = (Engine::Instance()->GetSoundPlayer()->GetWavMaxVolume() > 0);
    if (music)
    {
      m_tqSound.Draw(top + HSPACE, left, bot + HSPACE, right); // t, l, b, r
    }
    else
    {
      m_tqNoSound.Draw(top + HSPACE, left, bot + HSPACE, right);
    }
    // Sound FX
    if (sound)
    {
      m_tqSound.Draw(top, left, bot, right);
    }
    else
    {
      m_tqNoSound.Draw(top, left, bot, right);
    }
  
    // If full screen setting has changed, ask user to restart game.
    bool fullscreen = (Engine::Instance()->GetGameState()->
      Get("fullscreen") == "y");

    if (initialFullScreen != fullscreen)
    {
      Engine::Instance()->GetTextWriter()->PrintCentre(15.0f, 
       "Please restart the game to change screen setting.");
    }
  }
}

void EnginePaused::MouseButton(bool down, bool, bool)
{
  if (s_showConfirmButtons)
  {
    m_pButtonQuitYes->MouseButton(down, false, false);
    m_pButtonQuitNo->MouseButton(down, false, false);
  }
  else
  {
    m_pButtonResume->MouseButton(down, false, false);
    m_pButtonFullScreen->MouseButton(down, false, false);
    m_pButtonSoundVol->MouseButton(down, false, false);
    m_pButtonMusicVol->MouseButton(down, false, false);
    m_pButtonQuit->MouseButton(down, false, false);
  }
}

void EnginePaused::Red(bool down)
{
}
}
