/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStatePoolSetUpShotDemo.cpp,v $
Revision 1.1.2.2  2007/07/18 22:15:55  jay
Set Online flag false

Revision 1.1.2.1  2006/08/14 17:50:16  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:28  jay
Moved Pool code from Pool/ to Pool/

Revision 1.5.6.9  2006/03/24 22:58:37  jay
No harcoded trial period here

Revision 1.5.6.8  2006/03/10 19:05:57  jay
Trial may not be unlockable

Revision 1.5.6.7  2005/09/23 19:38:47  jay
Remove unused code

Revision 1.5.6.6  2005/08/26 21:28:41  jay
Reposition buttons, add nag text

Revision 1.5.6.5  2005/07/05 09:36:40  jay
Check for registered/expired

Revision 1.5.6.4  2005/06/22 21:38:47  jay
Disable start button if expired

Revision 1.5.6.3  2005/05/24 20:40:54  jay
Register/buy now buttons

Revision 1.5.6.2  2005/04/17 22:04:06  jay
Added english and draw/roll parameters to shot

Revision 1.5.6.1  2005/04/01 06:00:01  jay
Pool Demo mode

Revision 1.5  2004/09/27 12:33:04  Administrator
Improvements to Demo mode - shot strength; character shot differences.

Revision 1.4  2004/09/25 21:14:33  Administrator
Code to select room and set up player stats

Revision 1.3  2004/09/17 13:59:10  jay
Added demo mode

Revision 1.2  2004/09/15 09:23:56  Administrator
Unfinished changes for "attract" mode

Revision 1.1  2004/09/08 15:43:05  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "EngineStatePoolSetUpShotDemo.h"
#include "EngineStatePoolShotInPlayDemo.h"
#include "EngineStateTitle.h"
#include "Engine.h"
#include "LevelServer.h"
#include "PowerGuage.h"
#include "Behaviour.h"
#include "GameState.h"
#include "PoolCourseManager.h"
#include "PoolMisc.h"
#include "GuiButton.h"
#include "Registration.h"
#include "TextWriter.h"
#include "TimePeriod.h"

extern void OnStartClicked();
extern void OnRegisterClicked();
extern void OnExitClicked();
extern void OnBuyButton();

namespace Amju
{
const char* EngineStatePoolSetUpShotDemo::Name = "golf_setup_demo";

// Register this State with the Engine.
static const bool registeredgolfinplay = Engine::Instance()->
  RegisterEngineState(EngineStatePoolSetUpShotDemo::Name, 
  new EngineStatePoolSetUpShotDemo);

PGuiElement EngineStatePoolSetUpShotDemo::s_startButton;
PGuiElement EngineStatePoolSetUpShotDemo::s_exitButton;
PGuiElement EngineStatePoolSetUpShotDemo::s_registerButton;
PGuiElement EngineStatePoolSetUpShotDemo::s_buynowButton;

void EngineStatePoolSetUpShotDemo::SDrawOverlays()
{
  // TODO disable start if trial period has expired.
  // Hide buy/register buttons if registered.
  bool unreg = !Registration::IsRegistered();
  int secs = Registration::GetRemainingEvalPeriod();
  bool expired = (secs == 0);

  if (unreg && expired)
  {
    s_startButton->SetEnabled(false);
  }
  else
  { 
    s_startButton->SetEnabled(true);
  }

  s_startButton->Draw();
  //s_exitButton->Draw();

  if (unreg)
  {
#ifndef AMJU_NOT_UNLOCKABLE
    s_registerButton->Draw();
    s_buynowButton->Draw();
#endif
  }

  static const float LINE1 = 14.5f;
  static const float LINE2 = 15.3f;
  if (unreg && expired)
  {
    std::string nag = "Thanks for playing Amju Super Cool Pool!";
    GetEngine()->GetTextWriter()->Print(0, LINE1, nag.c_str());

#ifdef AMJU_NOT_UNLOCKABLE
    nag = "Your trial period has finished.";
#else
    nag = "Please register to keep playing!";
#endif
    GetEngine()->GetTextWriter()->Print(0, LINE2, nag.c_str());
  }
  else if (unreg)
  {
    std::string nag = "Thanks for playing Amju Super Cool Pool!";
    GetEngine()->GetTextWriter()->Print(0, LINE1, nag.c_str());
   
    nag = "Your trial period expires in ";
    nag += TimePeriod(secs).ToString();
    //nag += ".";
    GetEngine()->GetTextWriter()->Print(0, LINE2, nag.c_str());
  }
}

void EngineStatePoolSetUpShotDemo::SMouseButton(bool down, bool ctrl, bool shift)
{
  s_startButton->MouseButton(down, ctrl, shift);
  s_exitButton->MouseButton(down, ctrl, shift);
 
  bool unreg = !Registration::IsRegistered();
  if (unreg)
  {
    s_registerButton->MouseButton(down, ctrl, shift);
    s_buynowButton->MouseButton(down, ctrl, shift);
  }
}

void EngineStatePoolSetUpShotDemo::SMousePos(int x, int y)
{
  s_startButton->MousePos(x, y);
  s_exitButton->MousePos(x, y);
  s_registerButton->MousePos(x, y);
  s_buynowButton->MousePos(x, y);
}

bool EngineStatePoolSetUpShotDemo::Load()
{
  if (!EngineStatePoolSetUpShot::Load())
  {
    return false;
  }
  // Can't use max time to end demo, as this state is entered multiple times
  // during one demo sequence.
  m_maxTime = 100; // unreachable

  // Load buttons
  s_startButton = new GuiButton;
  if (!s_startButton->Load(GetEngine()->GetConfigValue("pool_startbutton")))
  {
    return false;
  }
  s_startButton->SetSize(7.0f, 2.0f);
  s_startButton->SetRelPos(0.0f, 0.0f);
  s_startButton->SetCommand(&OnStartClicked);

  s_exitButton = new GuiButton;
  if (!s_exitButton->Load(GetEngine()->GetConfigValue("pool_exitbutton")))
  {
    return false;
  }
  s_exitButton->SetSize(2.0f, 2.0f);
  s_exitButton->SetRelPos(0.0f, 7.25f); // bah, wrong way round!
  s_exitButton->SetCommand(&OnExitClicked);

  s_registerButton = new GuiButton;
  std::string regButtonFile = GetEngine()->GetConfigValue("golf_reg_button");
  if (!s_registerButton->Load(regButtonFile))
  {
    return false;
  }
  s_registerButton->SetSize(7.75f, 2.0f);
  s_registerButton->SetRelPos(0, 8.125f);
  s_registerButton->SetCommand(&OnRegisterClicked);

  s_buynowButton = new GuiButton;
  std::string buyButtonFile = GetEngine()->GetConfigValue("golf_buy_button");
  if (!s_buynowButton->Load(buyButtonFile))
  {
    return false;
  }
  s_buynowButton->SetSize(8.0f, 2.0f);
  s_buynowButton->SetRelPos(0, 17.0f);
  s_buynowButton->SetCommand(&OnBuyButton);

  return true;
}

void EngineStatePoolSetUpShotDemo::Red(bool down)
{
  if (down)
  {
    GetEngine()->ChangeState(EngineStateTitle::Name, Engine::IMMEDIATE);
  }
}

void EngineStatePoolSetUpShotDemo::MousePos(int x, int y)
{
  SMousePos(x, y);
}

void EngineStatePoolSetUpShotDemo::MouseButton(bool down, bool ctrl, bool shift)
{
  SMouseButton(down, ctrl, shift); 
/*
  // OLD - go back to title state
  if (down)
  {
    GetEngine()->ChangeState(EngineStateTitle::Name, Engine::IMMEDIATE);
  }
*/
}

void EngineStatePoolSetUpShotDemo::SetActive(bool active)
{
  EngineStatePoolSetUpShot::SetActive(active);

  // Demo mode: NEVER online!
  SetIsOnlineGame(false);

  if (active)
  {
    GetEngine()->SetClearColour(1.0f, 0, 1.0f);
    GetEngine()->SetLetterbox(true);
  }
  else
  {
    GetEngine()->SetClearColour(0, 0, 0);
    GetEngine()->SetLetterbox(false);  
  }
}

void EngineStatePoolSetUpShotDemo::DrawOverlays()
{
  // TODO Draw buttons for
  // start game (1/2 players), exit, buy, register.

  SDrawOverlays();
}
}

