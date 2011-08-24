/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: EngineStatePlayerEdit.cpp,v $
Revision 1.1.2.3  2006/08/30 21:12:13  Administrator
Get Pool to build in MSVC

Revision 1.1.2.2  2006/08/14 17:50:12  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:31  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.1  2005/10/21 17:43:08  jay
This state lets user edit a player name

*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "EngineStatePlayerEdit.h"
#include "Engine.h"
#include "GuiButton.h"
#include "PoolBg.h"
#include "TextWriter.h"
#include "StringUtils.h"
#include "PlayerNames.h"
#include "EngineStatePoolPlayerMaint.h"

namespace Amju
{
const char* EngineStatePlayerEdit::Name = "player-edit";
static const bool registered = Engine::Instance()->
  RegisterEngineState(EngineStatePlayerEdit::Name, 
  new EngineStatePlayerEdit);

void OnEditPlayerOk()
{
  ((EngineStatePlayerEdit*)Engine::Instance()->GetEngineState(EngineStatePlayerEdit::Name).GetPtr())->Finished();
}

void OnEditPlayerCancel()
{
  Engine::Instance()->ChangeState(EngineStatePoolPlayerMaint::Name,
    Engine::IMMEDIATE);
}

EngineStatePlayerEdit::EngineStatePlayerEdit()
{
}

void EngineStatePlayerEdit::SetActive(bool active)
{
  EngineStateEnterText::SetActive(active); // NB resets 3D text
  m_errorText = "";
}

void EngineStatePlayerEdit::SetName(const std::string& name)
{
  m_oldName = name;
  m_text = name;
  m_pos = m_text.size();
  MakeText();
}

void EngineStatePlayerEdit::Draw()
{
  AmjuGL::PushMatrix();
  DrawBg();
  m_pCam->Update();
  m_pCam->Draw();

  m_lighting.DrawLight();

//  AmjuGL::PushMatrix();
  m_or.Draw();
  ThePoolBg::Instance()->Draw();
//  AmjuGL::PopMatrix();

  static const float nameY = Engine::Instance()->GetConfigFloat("pool_name_y");
  AmjuGL::Translate(0, nameY, 0); // above menu of old names
  static const float z = Engine::Instance()->GetConfigFloat("hi_score_text_z2");
  AmjuGL::Translate(0, 0, z);
  AmjuGL::Translate(0, -3.0f, 0);
  AmjuGL::PushMatrix();
  // Draw the text being edited:
  AmjuGL::Translate(-m_totalSize/2.0f, 0, -4.0f); 
  // Draw the text to the left of the cursor
  m_pComp->Draw();
  // Draw the text to the right of the cursor
  AmjuGL::Translate(m_leftSize + 0.5f, 0, 0);
  DrawCursor();
  m_pComp2->Draw();
  AmjuGL::PopMatrix();

  AmjuGL::PopMatrix();

  std::string s = "Please edit your name then hit return";
  Engine::Instance()->GetTextWriter()->PrintCentre(3.0f, s.c_str());
  Engine::Instance()->GetTextWriter()->PrintCentre(10.0f, m_errorText.c_str());
}

void EngineStatePlayerEdit::DrawOverlays()
{
  m_pOkButton->Draw();
  m_pBackButton->Draw();
}

void EngineStatePlayerEdit::OnKey(char key, bool down)
{
  // TODO handle backspace
#ifdef KEY_DEBUG
std::cout << "Character: " << (int)key << "\n";
#endif

  if (down)
  {
    static const std::string wavFile = Engine::Instance()->GetConfigValue(
      "gui_element_click_wav");
    Engine::Instance()->PlayWav(wavFile);

    m_errorText = "";
  }
  EngineStateEnterText::OnKey(key, down);

}

bool EngineStatePlayerEdit::Load()
{
  if (!EngineStateEnterText::Load())
  {
    return false;
  }

  // Create OK and Back buttons.
  m_pOkButton = new GuiButton;
  m_pBackButton = new GuiButton;

  static const float x1 = Engine::Instance()->GetConfigFloat(
    "pool_get_button_x1");
  static const float x2 = Engine::Instance()->GetConfigFloat(
    "pool_get_button_x2");

  std::string backFile = "golf-reg-cancel-button.txt"; // TODO CONFIG
  if (!m_pBackButton->Load(backFile))
  {
    return false;
  }
  m_pBackButton->SetSize(2.0f, 2.0f);
  m_pBackButton->SetRelPos(12.0f, x1);
  m_pBackButton->SetCommand(&OnEditPlayerCancel);

  std::string okFile = "pool-ok-button.txt"; // TODO CONFIG
  if (!m_pOkButton->Load(okFile))
  {
    return false;
  }
  m_pOkButton->SetSize(4.0f, 2.0f);
  m_pOkButton->SetRelPos(12.0f, x2);
  m_pOkButton->SetCommand(&OnEditPlayerOk);

  return true;
}

void EngineStatePlayerEdit::Red(bool down)
{
  // Allow spaces
  OnKey(' ', down);
}

void EngineStatePlayerEdit::MouseButton(bool down, bool ctrl, bool shift)
{
  m_pOkButton->MouseButton(down, ctrl, shift);
  m_pBackButton->MouseButton(down, ctrl, shift);
}

void EngineStatePlayerEdit::MousePos(int x, int y)
{
}

void EngineStatePlayerEdit::Finished()
{
  m_errorText = "";

  if (m_text.empty())
  {
    m_errorText = "Please enter your name!";
    return;
  }

  m_text = ToLower(m_text);

  // Check if the name hasn't changed
  if (m_text == m_oldName)
  {
#ifdef _DEBUG
std::cout << "Name has not changed, so no edit required.\n";
#endif
  }
  else
  {
    // Check this name does not already exist
    if (ThePlayerNames::Instance()->NameExists(m_text))
    {
      m_errorText = "That name already exists! Please type a new name.";
      return;
    }

#ifdef _DEBUG
std::cout << "Replacing old name: " << m_oldName.c_str() << " with new name: " << m_text.c_str() << "\n";
#endif
    // Replace the name, rename stats file
    ThePlayerNames::Instance()->EditName(m_oldName, m_text);
  }

  // Back to the parent Engine State
  Engine::Instance()->ChangeState(EngineStatePoolPlayerMaint::Name,
    Engine::IMMEDIATE);
}

void EngineStatePlayerEdit::Back()
{
  Engine::Instance()->ChangeState(EngineStatePoolPlayerMaint::Name,
    Engine::IMMEDIATE);
}
}


