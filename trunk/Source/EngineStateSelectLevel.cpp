/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStateSelectLevel.cpp,v $
Revision 1.1  2004/09/08 15:42:37  jay
Added to repository
 
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "EngineStateSelectLevel.h"
#include "EngineStateShowLevel.h"
#include "Engine.h"
#include "EngineRunning.h"
#include "TextFactory.h"
#include "TextMenu.h"
#include "Text3dLeaf.h"
#include "StringUtils.h"
#include "GameState.h"
#include "SchAssert.h"

namespace Amju
{
const char* EngineStateSelectLevel::Name = "selectlevel"; 
// Register this State with the Engine.
static const bool registeredshowlev = Engine::Instance()->
  RegisterEngineState(EngineStateSelectLevel::Name, new EngineStateSelectLevel);

EngineStateSelectLevel::EngineStateSelectLevel() 
{
}

void EngineStateSelectLevel::Draw()
{

  // Draw Background and menu
  EngineMenuState::Draw();

  // Draw state-specific text
  m_pCam->Update();
  m_pCam->Draw();

  // Draw "level <n>"
  AmjuGL::PushMatrix();
    AmjuGL::Scale(0.5f, 0.5f, 0.5f);

    // position
    AmjuGL::Translate(0, 8.0f, 0);

    AmjuGL::PushMatrix();
      m_text->Update();
      m_text->Draw();
    AmjuGL::PopMatrix();

  AmjuGL::PopMatrix();
}

void EngineStateSelectLevel::SetActive(bool active)
{
  GetEngine()->GetGameState()->SetName("gamedata.cfg");

  if (!GetEngine()->GetEngineState()->Load())
  {
#if defined(_DEBUG)
    GetEngine()->ReportError("Failed to load game.");
#endif
    // Reset to new game state
    GetEngine()->GetGameState()->Set("real_level", 0); 

  }

  // Call when this state is activated (TODO make sure)
  TextMenu* pMenu = new TextMenu;

  // TODO key must be same when file saved
  int maxLevel = (int)GetEngine()->GetGameState()->GetFloat("real_level"); 

  for (int i = 0; i <= maxLevel; i++)
  {
    std::string s = "level " + ToString(i + 1);
    PSolidComponent p = new Text3dLeaf(s.c_str());
    pMenu->AddItem(p.GetPtr());
  }
  // Make maxLevel the default selected item
  pMenu->SetSelected(maxLevel);
  m_pMenu = pMenu;
}

bool EngineStateSelectLevel::Load()
{
  // TODO CONFIG! Luckily this code is not actually used anywhere
  m_text = new Text3dLeaf(".please select start level.");

  return EngineMenuState::Load();
}

void EngineStateSelectLevel::Red(bool down)
{
  if (down)
  {
//    string startWav = GetEngine()->GetConfigValue("start_wav");
//    GetEngine()->PlayWav(startWav);
    int startLev = m_pMenu->Choose();
    GetEngine()->GetGameState()->Set("real_level", startLev);

    GetEngine()->ChangeState(EngineStateShowLevel::Name, Engine::IMMEDIATE);
  }
}
}

