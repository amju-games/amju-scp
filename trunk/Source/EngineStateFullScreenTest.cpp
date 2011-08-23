/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: EngineStateFullScreenTest.cpp,v $
Revision 1.1.2.4  2006/08/14 17:37:29  jay
Rename "Pool"

Revision 1.1.2.3  2006/07/26 21:25:23  jay
Use Pool/, not Pool/

Revision 1.1.2.2  2005/10/29 14:04:37  Administrator
Turn off full screen flag on failure - explicit call required for Win.

Revision 1.1.2.1  2005/10/21 17:04:39  jay
Added test screen for when full screen mode is first entered.

*/

#include "EngineStateFullScreenTest.h"
#include "Engine.h"
#include "TextWriter.h"
#include "GuiButton.h"
#include "StringUtils.h"
#include "FileCheckState.h"
#include "AmjuExit.h"
#include "GameState.h"

extern void GoWindowed();

namespace Amju
{
const char* EngineStateFullScreenTest::Name = "fullscreentest";

static const bool registered = Engine::Instance()->
  RegisterEngineState(EngineStateFullScreenTest::Name, 
    new EngineStateFullScreenTest);

static const float MAX_TIME = 10.0f;


static void OnOk()
{
  // Full screen ok, carry on.
  // The idea here is that this state is the very first one displayed.
  // Once this state ends, we go on to the Load state, which is usually
  // the first state.
  // NB must match values elsewhere - TODO use constants
  Engine::Instance()->GetGameState()->Set("fsconfirmed", "y");
  // TODO This state should be a parameter
  Engine::Instance()->ChangeState(FileCheckState::Name, Engine::IMMEDIATE);
}

void EngineStateFullScreenTest::TimerExpired()
{
  // Reset full screen flag.
  // Now show error message and quit.
  // Make sure full screen flag is reset so we open in window next time
  GoWindowed(); 
  // For windows, we need to reset the flag explicitly here
  Engine::Instance()->GetGameState()->Set("fullscreen", "n");
  ReallyExit();  
}

EngineStateFullScreenTest::EngineStateFullScreenTest()
{
  m_maxTime = MAX_TIME;
}

bool EngineStateFullScreenTest::Load()
{
  // Can't load button here. 
  // If this is the first state shown, Load() won't be called before Draw().
  return true;
}

void EngineStateFullScreenTest::SetActive(bool active)
{
  EngineState::SetActive(active);
  if (active)
  {
    Engine::Instance()->SetClearColour(0, 0, 0);
  }
  else
  {
    Engine::Instance()->SetClearColour(1.0f, 1.0f, 1.0f);
  }
}

void EngineStateFullScreenTest::Draw()
{
  static bool first = true;
  if (first)
  {
    first = false;

    m_pOkButton = new GuiButton;
    std::string menuButtonFile = GetEngine()->GetConfigValue("golf_thx_reg_button");
    if (!m_pOkButton->Load(menuButtonFile))
    {
      Engine::Instance()->ReportError("Failed to load ok button");
      return;
    }
    m_pOkButton->SetSize(4.0f, 2.0f);
    m_pOkButton->SetRelPos(10.0f, 10.0f); // top, left
    m_pOkButton->SetCommand(&OnOk);
  }
}

void EngineStateFullScreenTest::DrawOverlays()
{
  Font* pFont = Engine::Instance()->GetTextWriter()->GetDefaultFont();
  float size = pFont->GetSize();

  // Draw message, countdown time and buttons.
  pFont->SetSize(2.0f);
  Engine::Instance()->GetTextWriter()->PrintCentre(5.0f, 
    "Full screen ok ?", pFont);
  
  pFont->SetSize(1.0f);
  Engine::Instance()->GetTextWriter()->PrintCentre(5.8f, 
    "Click the OK button if full screen mode works for you!", pFont);

  pFont->SetSize(2.0f);
  std::string s = ToString((int)(MAX_TIME - m_time) + 1);
  Engine::Instance()->GetTextWriter()->PrintCentre(7.5f, s.c_str());
 
#ifdef MACOSX 
  pFont->SetSize(1.0f);
  Engine::Instance()->GetTextWriter()->PrintCentre(8.0f, 
    "Please note, you can't switch to other apps in full screen mode.", pFont);
#endif
  
  pFont->SetSize(size); 
  Assert(m_pOkButton.GetPtr()); 
  m_pOkButton->Draw();
}

void EngineStateFullScreenTest::MouseButton(bool down, bool, bool)
{
  m_pOkButton->MouseButton(down, false, false);
}

void EngineStateFullScreenTest::Update()
{
  EngineState::Update();
}
}


