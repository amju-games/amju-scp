/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStateCongrats.cpp,v $
Revision 1.1.2.2  2006/08/14 17:50:08  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:25  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1  2004/09/08 15:43:04  jay
Added to repository
  
*/

#include "EngineStateCongrats.h"
#include "EngineStateTitle.h"
#include "Engine.h"
#include "TextWriter.h"
#include "GuiButton.h"
#include "StringUtils.h"
#include "PoolGameState.h"
#include "PoolMisc.h"

void OnCongratsOkButton()
{
  Amju::CheckForHiScores();
  // TODO Open web page, giving stats as arguments..?
}

namespace Amju
{
const char* EngineStateCongrats::Name = "congrats";

static const bool registered = Engine::Instance()->
  RegisterEngineState(EngineStateCongrats::Name, new EngineStateCongrats);

bool EngineStateCongrats::Load()
{
  m_bg.Load("oct_t.png", ""); // TODO CONFIG
  SetText("congratulations!");

  m_pOkButton = new GuiButton;
  std::string menuButtonFile = GetEngine()->GetConfigValue("golf_menu_button");
  if (!m_pOkButton->Load(menuButtonFile))
  {
    return false;
  }
  m_pOkButton->SetSize(1.0f, 1.0f);
  m_pOkButton->SetRelPos(15.0f, 23.0f);
  m_pOkButton->SetCommand(&OnCongratsOkButton);

  return true;
}

void EngineStateCongrats::Draw()
{
  EngineStateText::Draw();
  float i = 8.0f;
  Engine::Instance()->GetTextWriter()->PrintCentre(i,
    "Well done! You have completed all the holes!");

  float x1 = 4.0f;
  float x2 = 18.0f;

  Engine::Instance()->GetTextWriter()->Print(x1, i+1.0f,
    "Your score");

  Engine::Instance()->GetTextWriter()->Print(x1, i+2.0f,
    "Total number of strokes");

  Engine::Instance()->GetTextWriter()->Print(x1, i+3.0f, 
    "Total bonuses collected");

  Engine::Instance()->GetTextWriter()->Print(x1, i+4.0f,
    "Total animals bopped");

//  Engine::Instance()->GetTextWriter()->Print(x1, i+5.0f,
//    "Areas explored");
  

  // Display numbers
  std::string score = ToString(
    Engine::Instance()->GetGameState()->GetCurrentPlayerInfo()->m_score);

  std::string strokes = ToString(
    Engine::Instance()->GetGameState()->GetCurrentPlayerInfo()->m_totalStrokes);

  std::string bonuses = ToString(
    Engine::Instance()->GetGameState()->GetCurrentPlayerInfo()->m_totalBonuses);

  std::string animals = ToString(
    Engine::Instance()->GetGameState()->GetCurrentPlayerInfo()->m_animalsHit);

  std::string blockers = ToString(
    Engine::Instance()->GetGameState()->GetCurrentPlayerInfo()->m_blockersMoved);

  Engine::Instance()->GetTextWriter()->Print(x2, i+1.0f, score.c_str());

  Engine::Instance()->GetTextWriter()->Print(x2, i+2.0f, strokes.c_str());

  Engine::Instance()->GetTextWriter()->Print(x2, i+3.0f, bonuses.c_str());

  Engine::Instance()->GetTextWriter()->Print(x2, i+4.0f, animals.c_str());

//  Engine::Instance()->GetTextWriter()->Print(x2, i+5.0f, blockers.c_str());


  // Display BEST or Best is:


  m_pOkButton->Draw();
}

void EngineStateCongrats::MouseButton(bool down, bool, bool)
{
  m_pOkButton->MouseButton(down, false, false);
}
}

