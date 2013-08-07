/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: EngineStatePlayerDelete.cpp,v $
Revision 1.1.2.3  2006/08/30 21:12:13  Administrator
Get Pool to build in MSVC

Revision 1.1.2.2  2006/08/14 17:50:12  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:31  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.1  2005/10/21 17:42:47  jay
This state is for the user to confirm player delete

*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "EngineStatePlayerDelete.h"
#include "Engine.h"
#include "EngineStatePoolPlayerMaint.h"
#include "PoolBg.h"
#include "PlayerNames.h"
#include "GuiButton.h"
#include "TextWriter.h"

namespace Amju
{
const char* EngineStatePlayerDelete::Name = "player-delete";
static const bool registered = Engine::Instance()->
  RegisterEngineState(EngineStatePlayerDelete::Name,
  new EngineStatePlayerDelete);

void OnDeleteOk()
{
#ifdef _DEBUG
std::cout << "Delete OK pressed..\n";
#endif
  ((EngineStatePlayerDelete*)Engine::Instance()->GetEngineState(
    EngineStatePlayerDelete::Name).GetPtr())->DeletePlayer();
}

void OnDeleteCancel()
{
  Engine::Instance()->ChangeState(EngineStatePoolPlayerMaint::Name,
    Engine::IMMEDIATE);
}

void EngineStatePlayerDelete::SetActive(bool active)
{
}

bool EngineStatePlayerDelete::Load()
{
  std::string bg = Engine::Instance()->GetConfigValue("paused_bg");
  m_bg.Load(bg, "");

  static const float x1 = Engine::Instance()->GetConfigFloat(
    "pool_choose_button_x1");
  static const float SIZE_W = Engine::Instance()->GetConfigFloat(
    "pool_choose_button_w");
  static const float SIZE_H = Engine::Instance()->GetConfigFloat(
    "pool_choose_button_h");
  static const float SPACE_H = Engine::Instance()->GetConfigFloat(
    "pool_choose_button_h_space");

  static const float TOP = 5.0f; // TODO CONFIG

  m_pCancelButton = new PoolGuiButton;
  std::string backFile = "choose-player-cancel.txt"; 
  if (!m_pCancelButton->Load(backFile))
  {
    return false;
  }
  m_pCancelButton->SetSize(SIZE_W, SIZE_H);
  m_pCancelButton->SetRelPos(TOP + SPACE_H, x1); 
  m_pCancelButton->SetCommand(&OnDeleteCancel);

  m_pOkButton = new PoolGuiButton;
  std::string okFile = "pool-player-delete-button.txt"; 
  if (!m_pOkButton->Load(okFile))
  {
    return false;
  }
  m_pOkButton->SetSize(SIZE_W, SIZE_H);
  m_pOkButton->SetRelPos(TOP, x1); // TODO CONFIG
  m_pOkButton->SetCommand(&OnDeleteOk);

  return true;
}

void EngineStatePlayerDelete::Draw()
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

void EngineStatePlayerDelete::DrawOverlays()
{
  std::string s = "Are you sure you want to delete player ";
  s += m_name;
  s += " ?";
  Engine::Instance()->GetTextWriter()->PrintCentre(3.0f, s.c_str());
  m_pOkButton->Draw();
  m_pCancelButton->Draw();
}

void EngineStatePlayerDelete::MouseButton(bool down, bool, bool)
{
  m_pOkButton->MouseButton(down, false, false);
  m_pCancelButton->MouseButton(down, false, false);
}

void EngineStatePlayerDelete::SetName(const std::string& name)
{
  m_name = name;
}

void EngineStatePlayerDelete::DeletePlayer()
{
#ifdef _DEBUG
std::cout << "DeletePlayer().. really deleting " << m_name.c_str() << "\n";
#endif
  // Really delete the player from the list of player names.
  ThePlayerNames::Instance()->EraseName(m_name);

  Engine::Instance()->ChangeState(EngineStatePoolPlayerMaint::Name,
    Engine::IMMEDIATE);
}
}

