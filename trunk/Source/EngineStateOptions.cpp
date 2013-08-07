/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: EngineStateOptions.cpp,v $
Revision 1.1.2.5  2007/06/01 17:48:41  jay
Tidy up

Revision 1.1.2.4  2006/08/30 21:12:13  Administrator
Get Pool to build in MSVC

Revision 1.1.2.3  2006/08/14 17:50:11  jay
Rename "Pool"

Revision 1.1.2.2  2006/07/26 21:25:31  jay
Use Pool/, not Pool/

Revision 1.1.2.1  2006/07/24 18:20:31  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.1  2005/10/21 17:42:06  jay
Add new options menu, accessed from main menu

*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <string>
#include "Common.h" 
#include "EngineStateOptions.h"
#include "Engine.h"
#include "TextWriter.h"
#include "GuiButton.h"
#include "EngineStatePoolGameSelect.h"
#include "PoolGameState.h"
#include "PoolBg.h"
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

static void OnBack()
{
  // Go back to Main Menu state
  Engine::Instance()->ChangeState(EngineStatePoolGameSelect::Name,
    Engine::IMMEDIATE);

}

bool IsFullScreenFlagSet();

void OnFullScreen();

static void OnMusic()
{
  EngineStateOptions::SetShowConfirmButtons(false);
  float f = Engine::Instance()->GetSoundPlayer()->GetSongMaxVolume();
  float r = 0;
  if (f == 0)
  {
    r = 1.0f;
  }
  Engine::Instance()->GetSoundPlayer()->SetSongMaxVolume(r);
}

static void OnSoundEffects()
{
  EngineStateOptions::SetShowConfirmButtons(false);
  float f = Engine::Instance()->GetSoundPlayer()->GetWavMaxVolume();
  float r = 0;
  if (f == 0)
  {
    r = 1.0f;
  }
  Engine::Instance()->GetSoundPlayer()->SetWavMaxVolume(r);
}

const std::string EngineStateOptions::Name = "options";
// Register this State with the Engine.
static const bool registered = Engine::Instance()->
  RegisterEngineState(EngineStateOptions::Name, new EngineStateOptions);

bool EngineStateOptions::s_showConfirmButtons;

EngineStateOptions::EngineStateOptions()
{ 
}

void EngineStateOptions::SetShowConfirmButtons(bool b)
{
  s_showConfirmButtons = b;
}

void EngineStateOptions::SetActive(bool active)
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


bool EngineStateOptions::Load()
{
  std::string bg = Engine::Instance()->GetConfigValue("paused_bg");
  m_bg.Load(bg, "");

  std::string menuButtonFile = GetEngine()->GetConfigValue("golf_menu_button");

  // Get buttons
  m_pButtonResume = new PoolGuiButton;
  m_pButtonResume->Load("pool-options-back-button.txt");
  m_pButtonResume->SetSize(WIDTH, HEIGHT);
  m_pButtonResume->SetCommand(&OnBack);
  m_pButtonResume->SetRelPos(TOP + 1.0f * HSPACE, LEFT);

//  m_pButtonQuit = new PoolGuiButton;
//  m_pButtonQuit->Load("pool-quit-game.txt");
//  m_pButtonQuit->SetSize(WIDTH, HEIGHT);
//  m_pButtonQuit->SetCommand(&OnQuitThisCourse);
//  m_pButtonQuit->SetRelPos(TOP + 1.0f * HSPACE, LEFT);

  m_pButtonFullScreen = new PoolGuiButton;
  m_pButtonFullScreen->Load("pool-fullscreen.txt");
  m_pButtonFullScreen->SetSize(WIDTH, HEIGHT);
  m_pButtonFullScreen->SetCommand(&OnFullScreen);
  m_pButtonFullScreen->SetRelPos(TOP + 4.0f * HSPACE, LEFT);

  m_pButtonSoundVol = new PoolGuiButton;
  m_pButtonSoundVol->Load("pool-sound.txt");
  m_pButtonSoundVol->SetSize(WIDTH, HEIGHT);
  m_pButtonSoundVol->SetCommand(&OnSoundEffects);
  m_pButtonSoundVol->SetRelPos(TOP + 2.0f * HSPACE, LEFT);

  m_pButtonMusicVol = new PoolGuiButton;
  m_pButtonMusicVol->Load("pool-music.txt");
  m_pButtonMusicVol->SetSize(WIDTH, HEIGHT);
  m_pButtonMusicVol->SetCommand(&OnMusic);
  m_pButtonMusicVol->SetRelPos(TOP + 3.0f * HSPACE, LEFT);

  m_pButtonQuitGame = new PoolGuiButton;
//  m_pButtonQuitGame->Load(menuButtonFile);
//  m_pButtonQuitGame->SetSize(w, h);
//  m_pButtonQuitGame->SetCommand(OnQuitGame);
//  m_pButtonQuitGame->SetRelPos(y + 10.0f, x);  

  m_pButtonQuitYes = new PoolGuiButton;
//  m_pButtonQuitYes->Load(menuButtonFile);
//  m_pButtonQuitYes->SetSize(WIDTH, HEIGHT);
//  m_pButtonQuitYes->SetCommand(&OnQuitYes);
//  m_pButtonQuitYes->SetRelPos(TOP + 2.0f * HSPACE, LEFT);

  m_pButtonQuitNo = new PoolGuiButton;
//  m_pButtonQuitNo->Load(menuButtonFile);
//  m_pButtonQuitNo->SetSize(WIDTH, HEIGHT);
//  m_pButtonQuitNo->SetCommand(&OnQuitNo);
//  m_pButtonQuitNo->SetRelPos(TOP + 4.0f * HSPACE, LEFT);

  if (!m_tqSound.Load("sound.png", "pool-exit-a.png"))
  {
    return false;
  }
  if (!m_tqNoSound.Load("no-sound.png", "pool-exit-a.png"))
  {
    return false;
  }

  return true;
}

void EngineStateOptions::Clear()
{
}

void EngineStateOptions::Draw()
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

void EngineStateOptions::DrawOverlays()
{
  // Get full screen setting before any change is made
  static bool initialFullScreen = (Engine::Instance()->GetGameState()->
    Get("fullscreen") == "y");

  if (s_showConfirmButtons)
  {
    //m_pButtonQuitYes->Draw();
    //m_pButtonQuitNo->Draw();
  }
  else
  {
    m_pButtonResume->Draw();
    //m_pButtonQuit->Draw();
    m_pButtonFullScreen->Draw();
    m_pButtonSoundVol->Draw();
    m_pButtonMusicVol->Draw();
    //m_pButtonQuitGame->Draw();

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

void EngineStateOptions::MouseButton(bool down, bool, bool)
{
  m_pButtonResume->MouseButton(down, false, false);
  m_pButtonFullScreen->MouseButton(down, false, false);
  m_pButtonSoundVol->MouseButton(down, false, false);
  m_pButtonMusicVol->MouseButton(down, false, false);
//  m_pButtonQuit->MouseButton(down, false, false);
//  m_pButtonQuitGame->MouseButton(down, false, false);

  if (s_showConfirmButtons)
  {
    //m_pButtonQuitYes->MouseButton(down, false, false);
    //m_pButtonQuitNo->MouseButton(down, false, false);
  }
}

void EngineStateOptions::Red(bool down)
{
}


}
