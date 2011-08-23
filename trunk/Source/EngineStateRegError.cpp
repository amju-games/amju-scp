/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStateRegError.cpp,v $
Revision 1.1.10.2  2007/03/25 17:50:16  jay
No letterbox for Sorry state

Revision 1.1.10.1  2005/06/29 20:17:28  jay
Changes for POOL look and feel

Revision 1.1  2004/09/08 15:42:36  jay
Added to repository
  
*/

#include "EngineStateRegError.h"
#include "EngineStateTitle.h"
#include "EngineStateRegister.h"
#include "Engine.h"
#include "TextWriter.h"
#include "GuiButton.h"

void OnBackButton()
{
  Amju::Engine::Instance()->ChangeState(
    Amju::EngineStateRegister::Name, Amju::Engine::IMMEDIATE);
}

namespace Amju
{
const char* EngineStateRegError::Name = "reg_error";
// Register this State with the Engine.
static const bool registered = Engine::Instance()->
  RegisterEngineState(EngineStateRegError::Name, new EngineStateRegError);

void EngineStateRegError::SetActive(bool active)
{
  m_time = 0;
  if (active)
  {
    m_lighting.Init();
  }
}

bool EngineStateRegError::Load()
{
  SetText("sorry!");
  std::string bg = Engine::Instance()->GetConfigValue("sorry-bg");
  if (!m_bg.Load(bg, ""))
  {
    return false;
  }

  m_pBackButton = new GuiButton;
  std::string backButtonFile = GetEngine()->GetConfigValue("golf_reg_ok_button");
  if (!m_pBackButton->Load(backButtonFile))
  {
    return false;
  }
  m_pBackButton->SetSize(4.0f, 2.0f);
  m_pBackButton->SetRelPos(14.5f, 10.0f);
  m_pBackButton->SetCommand(&OnBackButton);

  return true;
}

void EngineStateRegError::Draw()
{
  EngineStateText::Draw();
  GetEngine()->GetTextWriter()->PrintCentre(6, m_regCode.c_str());
  GetEngine()->GetTextWriter()->PrintCentre(8, 
    "The registration code you entered is not recognised.");
/*
  GetEngine()->GetTextWriter()->PrintCentre(9, 
    "If you entered your code incorrectly, please use the");
  
  GetEngine()->GetTextWriter()->PrintCentre(10, 
    "back button to try again!");
*/
  GetEngine()->GetTextWriter()->PrintCentre(11, 
    "If your code is correct, it has probably gone stale.");

  GetEngine()->GetTextWriter()->PrintCentre(12, 
    "This is nothing to worry about! Simply email");

  GetEngine()->GetTextWriter()->PrintCentre(13, 
    "support@amju.com to get a new code!");
  
  bool e = (m_time > 3.0f);
  m_pBackButton->SetEnabled(e);
  m_pBackButton->Draw();
}

void EngineStateRegError::MousPos(int x, int y)
{
  m_pBackButton->MousePos(x, y);
}

void EngineStateRegError::MouseButton(bool down, bool, bool)
{
  m_pBackButton->MouseButton(down, false, false);
}

void EngineStateRegError::DrawOverlays()
{
}

void EngineStateRegError::Red(bool down)
{
}
}



