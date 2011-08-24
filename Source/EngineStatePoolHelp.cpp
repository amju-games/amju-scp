/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: EngineStatePoolHelp.cpp,v $
Revision 1.1.2.5  2007/05/12 21:46:14  Administrator
GUI improvements for POOL_ONLINE

Revision 1.1.2.4  2006/08/30 21:12:14  Administrator
Get Pool to build in MSVC

Revision 1.1.2.3  2006/08/17 18:15:40  jay
Ongoing work for Online Pool

Revision 1.1.2.2  2006/08/14 17:50:14  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:32  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.4  2005/09/30 13:00:10  jay
Kill off any earthquake

Revision 1.1.2.3  2005/09/29 19:05:42  jay
Make sure message queue is frozen during this state - ok here but
Freeze is not working it seems.

Revision 1.1.2.2  2005/08/26 21:41:40  jay
Remove scroll buttons

Revision 1.1.2.1  2005/07/05 09:42:34  jay
Added help state

*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "EngineStatePoolHelp.h"
#include "PoolChangeState.h"
#include "Engine.h"
#include "TextFactory.h"
#include "PoolPlayerStats.h"
#include "StringUtils.h"
#include "TextWriter.h"
#include "GuiButton.h"
#include "Rules.h"
#include "PoolMisc.h"
#include "LevelServer.h"
#include "PoolBg.h"

namespace Amju
{
extern void ReportError(const std::string&);

void OnHelpOkButton()
{
  ChangeStateToSetUpShot();

//  Amju::Engine::Instance()->ChangeState(
//    Amju::EngineStatePoolSetUpShot::Name,
//    Amju::Engine::IMMEDIATE);
}

/*
void OnScrollUpButton()
{
  static const float scrollvel = Engine::Instance()->GetConfigFloat(
    "help_scroll_vel");

  ((EngineStatePoolHelp*)
    Engine::Instance()->GetEngineState(EngineStatePoolHelp::Name).GetPtr())->
    SetScrollVel(scrollvel); 
}

void OnScrollDownButton()
{
  static const float scrollvel = Engine::Instance()->GetConfigFloat(
    "help_scroll_vel");

  ((EngineStatePoolHelp*)
    Engine::Instance()->GetEngineState(EngineStatePoolHelp::Name).GetPtr())->
    SetScrollVel(-scrollvel); 
}
*/

const char* EngineStatePoolHelp::Name = "pool-help";
// Register this State with the Engine.
static const bool registered = Engine::Instance()->
  RegisterEngineState(EngineStatePoolHelp::Name, new EngineStatePoolHelp);

void EngineStatePoolHelp::SetScrollVel(float v)
{
  m_yVel = v;
}

void EngineStatePoolHelp::SetActive(bool active)
{
  // Kill any active earthquake
  Engine::Instance()->GetEarthquake().Reset();

  Engine::Instance()->Freeze(active);

  m_y = 0;
  m_yVel = 0;
}

bool EngineStatePoolHelp::Load()
{
  if (!m_bg.Load(GetEngine()->GetConfigValue("file_bg"), ""))
  {
    ReportError("Help state: failed to load bg.");
    return false;
  }
  
  m_pOkButton = new GuiButton;
  if (!m_pOkButton->Load("pool-ok-button.txt"))
  {
    ReportError("Help state: failed to load ok button.");
    return false;
  }
  m_pOkButton->SetSize(4.0f, 2.0f);
  m_pOkButton->SetRelPos(13.0f, 10.5f); // TODO TEMP TEST
  m_pOkButton->SetCommand(&OnHelpOkButton);
  
  /*
  m_pScrollUpButton = new GuiButton;
  if (!m_pScrollUpButton->Load("pool-up-button.txt"))
  {
    ReportError("Help state: failed to load scroll up button.");
    return false;
  }
  m_pScrollUpButton->SetSize(2.0f, 2.0f);
  m_pScrollUpButton->SetRelPos(0, 20.0f); // TODO TEMP TEST
  m_pScrollUpButton->SetCommand(&OnScrollUpButton);
  
  m_pScrollDownButton = new GuiButton;
  if (!m_pScrollDownButton->Load("pool-down-button.txt"))
  {
    ReportError("Help state: failed to load scroll down button.");
    return false;
  }
  m_pScrollDownButton->SetSize(2.0f, 2.0f);
  m_pScrollDownButton->SetRelPos(13.0f, 20.0f); // TODO TEMP TEST
  m_pScrollDownButton->SetCommand(&OnScrollDownButton);
  */

  return true; 
}

void EngineStatePoolHelp::Red(bool down)
{
}

void EngineStatePoolHelp::Draw()
{
  AmjuGL::PushMatrix();
  DrawBg();
  m_pCam->Update();
  m_pCam->Draw();
  m_lighting.DrawLight();
  m_or.Draw();
  ThePoolBg::Instance()->Draw();
  AmjuGL::PopMatrix();

  // decelerate scrolling
  static const float accel = Engine::Instance()->GetConfigFloat(
    "help_scroll_accel");
  float dt = Engine::Instance()->GetDeltaTime();
  // Don't allow scrolling at limits
  bool allowScroll = true;
  if (m_y < 0)
  {
    allowScroll = false;
  }
#ifdef SCROLL_HELP_DEBUG
std::cout << "m_y: " << m_y << "  vel: " << m_yVel << " allow scroll: " << allowScroll << "\n";
#endif

  if (allowScroll)
  {
    m_y += dt * m_yVel;
  }

  // Decelerate
  if (m_yVel > 0)
  {
    m_yVel -= dt * accel;
    if (m_yVel < 0)
    {
      m_yVel = 0;
    }
  }
  else if (m_yVel < 0)
  {
    m_yVel += dt * accel;
    if (m_yVel > 0)
    {
      m_yVel = 0;
    }
  }
}

void EngineStatePoolHelp::DrawOverlays()
{
  // Rules for current level/room has help text.
  Level* pLevel = LevelServer::Instance()->GetCurrentLevel().GetPtr();
  GetRules(pLevel)->GetHelpText()->Draw(m_y);

  m_pOkButton->Draw();
  //m_pScrollUpButton->Draw();
  //m_pScrollDownButton->Draw();
}

void EngineStatePoolHelp::MouseButton(bool down, bool, bool)
{
  m_pOkButton->MouseButton(down, false, false);
  //m_pScrollUpButton->MouseButton(down, false, false);
  //m_pScrollDownButton->MouseButton(down, false, false);
}
}


