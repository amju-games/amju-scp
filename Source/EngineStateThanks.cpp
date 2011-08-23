/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStateThanks.cpp,v $
Revision 1.1.10.2  2005/06/29 20:17:28  jay
Changes for POOL look and feel

Revision 1.1.10.1  2005/06/22 21:48:50  jay
Fix text to refer to this game

Revision 1.1  2004/09/08 15:42:37  jay
Added to repository
  
*/

#include "EngineStateThanks.h"
#include "EngineStateTitle.h"
#include "Engine.h"
#include "TextWriter.h"
#include "TextColourizerColourList.h"
#include "TextFactory.h"
#include "GuiButton.h"

extern void OnCancelButton();

namespace Amju
{
extern void ReportError(const std::string&);

const char* EngineStateThanks::Name = "thanks"; 
// Register this State with the Engine.
static const bool registered = Engine::Instance()->
  RegisterEngineState(EngineStateThanks::Name, new EngineStateThanks);

void EngineStateThanks::SetActive(bool active)
{
}

bool EngineStateThanks::Load()
{
  m_pHearts = SolidComponent::LoadSolid("thanks_hearts.comp");
  if (!m_pHearts.GetPtr())
  {
    ReportError("Failed to load thanks hearts.");
    return false;
  }
  TextColourizerColourList tc;
  tc.AddColour(Colour(1.0f, 0.5f, 0.5f, 1.0f));
  tc.AddColour(Colour(1.0f, 0.5f, 1.0f, 1.0f));
  tc.AddColour(Colour(1.0f, 0.2f, 0.2f, 1.0f));
  tc.AddColour(Colour(1.0f, 0.1f, 0.5f, 1.0f));

  m_pComp = TextFactory::Instance()->Create("thanks!", &tc); // TODO!!!

  m_bg.Load("thanks-bg.bmp", "");

  m_pOkButton = new GuiButton;
  std::string menuButtonFile = GetEngine()->GetConfigValue("golf_thx_reg_button");
  if (!m_pOkButton->Load(menuButtonFile))
  {
    return false;
  }
  m_pOkButton->SetSize(4.0f, 2.0f);
  m_pOkButton->SetRelPos(14.5, 10.0f); // top, left
  m_pOkButton->SetCommand(&OnCancelButton);

  return true;
}

void EngineStateThanks::Red(bool down)
{
}

void EngineStateThanks::Draw()
{
  EngineStateText::Draw();

  AmjuGL::PushMatrix();
  m_pCam->Draw();
  m_pHearts->Update(); // TODO
  m_pHearts->Draw();
  AmjuGL::PopMatrix();

  GetEngine()->GetTextWriter()->PrintCentre(9, 
    "Thanks for purchasing Amju Super Cool Pool!");
  GetEngine()->GetTextWriter()->PrintCentre(10, 
    "You can now play the full game for as long as you like!");
  GetEngine()->GetTextWriter()->PrintCentre(11, 
    "Also, your registration is good for");
  GetEngine()->GetTextWriter()->PrintCentre(12, 
    "all future versions of Amju Super Cool Pool!"); 
  GetEngine()->GetTextWriter()->PrintCentre(13, 
    "Enjoy your game!");

  m_pOkButton->Draw();
}

void EngineStateThanks::MouseButton(bool down, bool, bool)
{
  m_pOkButton->MouseButton(down, false, false);
}
}



