/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStateThanksForPlaying.cpp,v $
Revision 1.1.8.2  2006/08/14 17:38:03  jay
Rename "Pool"

Revision 1.1.8.1  2006/07/26 21:25:16  jay
Use Pool/, not Pool/

Revision 1.1  2004/09/08 15:42:37  jay
Added to repository
  
*/

#include "EngineStateThanksForPlaying.h"
#include "EngineStateTitle.h"
#include "Engine.h"
#include "TextWriter.h"
#include "GuiButton.h"
#include "EngineStatePoolLoser.h"
#include "PoolMisc.h"

extern void OnBuyButton();

void OnThanksOkButton()
{
  // Either go to title state or enter hi score state for
  // each player with a high score.
  Amju::CheckForHiScores(); // In PoolMisc
}

namespace Amju
{
const char* EngineStateThanksForPlaying::Name = "thanks_for_playing"; 
// Register this State with the Engine.
static const bool registered = Engine::Instance()->
  RegisterEngineState(EngineStateThanksForPlaying::Name, new EngineStateThanksForPlaying);

bool EngineStateThanksForPlaying::Load()
{
  std::string thanksbg = Engine::Instance()->GetConfigValue("thanks_bg");
  m_bg.Load(thanksbg, "");
  SetText("thanks for playing!");

  m_pOkButton = new GuiButton;
  std::string menuButtonFile = GetEngine()->GetConfigValue("golf_thanks_button");
  if (!m_pOkButton->Load(menuButtonFile))
  {
    return false;
  }
  m_pOkButton->SetSize(1.0f, 1.0f);
  m_pOkButton->SetRelPos(14.0f, 17.5f);
  m_pOkButton->SetCommand(&OnThanksOkButton);
  
  m_pBuyButton = new GuiButton;
  if (!m_pBuyButton->Load(menuButtonFile))
  {
    return false;
  }
  m_pBuyButton->SetSize(1.0f, 1.0f);
  m_pBuyButton->SetRelPos(14.0f, 11.0f);
  m_pBuyButton->SetCommand(&OnBuyButton);


  return true;
}

void EngineStateThanksForPlaying::Red(bool down)
{
/*
  if (down)
  {
    // TODO If any player has got a high score, go to the high score entry state.

    GetEngine()->ChangeState(EngineStateTitle::Name, Engine::IMMEDIATE);
  }
*/
}

void EngineStateThanksForPlaying::Draw()
{
  EngineStateText::Draw();
  GetEngine()->GetTextWriter()->PrintCentre(8, 
    "Thanks for playing Amju Super Pool!");

  GetEngine()->GetTextWriter()->PrintCentre(9, 
    "If you like this game, why not purchase the full version!");

  GetEngine()->GetTextWriter()->PrintCentre(10, 
    "Please go to the online Amju Store to buy a registration key");

  GetEngine()->GetTextWriter()->PrintCentre(11, 
    "which will let you play the whole game");

  GetEngine()->GetTextWriter()->PrintCentre(12, 
    "for as long as you like! ");

  GetEngine()->GetTextWriter()->PrintCentre(14, 
    "   Buy Now!          Maybe later!");

  m_pBuyButton->Draw();
  m_pOkButton->Draw();

  
}

void EngineStateThanksForPlaying::MouseButton(bool down, bool, bool)
{
  m_pBuyButton->MouseButton(down, false, false);
  m_pOkButton->MouseButton(down, false, false);
}
}



