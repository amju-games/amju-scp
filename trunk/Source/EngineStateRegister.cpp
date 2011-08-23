/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStateRegister.cpp,v $
Revision 1.1.10.2  2005/07/09 06:43:57  jay
Remove dependency on Registration::REG_VERSION

Revision 1.1.10.1  2005/06/29 20:17:28  jay
Changes for POOL look and feel

Revision 1.1  2004/09/08 15:42:37  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <time.h>
#include "EngineStateRegister.h"
#include "EngineStateTitle.h"
#include "Engine.h"
#include "Registration.h"
#include "EngineStateThanks.h"
#include "EngineStateRegError.h"
#include "GuiButton.h"
#include "TextWriter.h"
#include "Launcher.h"
#include "TextColourizerColourList.h"
#include "TextFactory.h"
#include "LogFile.h"
#include "StringUtils.h"

void OnOkButton()
{
  ((Amju::EngineStateRegister*)Amju::Engine::Instance()->GetEngineState(
   Amju::EngineStateRegister::Name).GetPtr())->Finished(); 
}

void OnCancelButton()
{
  Amju::Engine::Instance()->ChangeState(
    Amju::EngineStateTitle::Name, Amju::Engine::IMMEDIATE);
}

void OnBuyButton()
{
  // Go to Store web page.
  std::string storeWebPage = Amju::Engine::Instance()->GetConfigValue("web_store");
  Amju::LaunchBrowser(storeWebPage.c_str());  
}

namespace Amju
{
const char* EngineStateRegister::Name = "register"; 
// Register this State with the Engine.
static const bool registered = Engine::Instance()->
  RegisterEngineState(EngineStateRegister::Name, new EngineStateRegister);

EngineStateRegister::EngineStateRegister()
{
}

void EngineStateRegister::Finished()
{
  // Check if the reg. code is valid. If so, the game is now registered!
  // Otherwise, show a message telling the user to get in touch for a 
  // new code.
  bool isValid = Registration::IsRegKeyValid(m_text);

  if (!isValid)
  {
    LogFile log;
    log.SetName("reg.txt");
    std::string s = "Key entered: ";
    s += m_text;
    log.Log(s);
    log.Log("Not a valid key.");
    time_t now;
    time(&now);
    s = "Current timestamp: ";
    s += ToString((unsigned int)now);
    log.Log(s);
  }

#ifdef _DEBUG
  if (m_text == "1234")
  {
    isValid = true;
  }
#endif

  if (isValid)
  {
    Registration::SetRegistered(true);
    // Go to Thanks page

    GetEngine()->ChangeState(
      EngineStateThanks::Name, Engine::IMMEDIATE);
  }
  else
  {
    // Key not valid - go to try again page 
    ((EngineStateRegError*)Engine::Instance()->GetEngineState(
      EngineStateRegError::Name).GetPtr())->SetRegCode(m_text);

    GetEngine()->ChangeState(
      EngineStateRegError::Name, Engine::IMMEDIATE); 
  }
}

void EngineStateRegister::Back()
{
  GetEngine()->ChangeState(
    EngineStateTitle::Name, Engine::IMMEDIATE);
}

bool EngineStateRegister::Load()
{
  if (!EngineStateEnterText::Load())
  {
    return false;
  }
/*
  if (!m_greet.Load("register.text"))
  {
    return false;
  }
*/
  // Create GUI buttons
  m_pOkButton = new GuiButton;
  m_pCancelButton = new GuiButton;
  m_pBuyButton = new GuiButton;

  std::string okButtonFile = GetEngine()->GetConfigValue("golf_reg_ok_button");
  if (!m_pOkButton->Load(okButtonFile))
  {
    return false;
  }
  m_pOkButton->SetSize(4.0f, 2.0f);
  m_pOkButton->SetRelPos(11.0f, 8.0f); // top, left
  m_pOkButton->SetCommand(&OnOkButton);

  std::string cancelButtonFile = 
    GetEngine()->GetConfigValue("golf_reg_cancel_button");
  if (!m_pCancelButton->Load(cancelButtonFile))
  {
    return false;
  }
  m_pCancelButton->SetSize(2.0f, 2.0f);
  m_pCancelButton->SetRelPos(11.0f, 6.0f);
  m_pCancelButton->SetCommand(&OnCancelButton);

  std::string buyButtonFile = 
    GetEngine()->GetConfigValue("golf_reg_buy_button");
  if (!m_pBuyButton->Load(buyButtonFile))
  {
    return false;
  }
  m_pBuyButton->SetSize(8.0f, 2.0f);
  m_pBuyButton->SetRelPos(11.0f, 12.0f);
  m_pBuyButton->SetCommand(&OnBuyButton);

  // Text to go next to the buttons
  // Start text TODO intl
  TextColourizerColourList tc1;
  tc1.AddColour(Colour(0.5f, 0.5f, 1.0f, 1.0f));
  m_pOkText = TextFactory::Instance()->Create("ok!", &tc1);
  m_pCancelText = TextFactory::Instance()->Create("maybe later!", &tc1);
  m_pBuyText = TextFactory::Instance()->Create("buy now!", &tc1);

  return true;
}

void EngineStateRegister::Draw()
{
  // copied EngineStateEnterText::Draw() but with no menu.
  AmjuGL::PushMatrix();
  DrawBg();

  m_pCam->Update();
  m_pCam->Draw();

  m_lighting.DrawLight();

  m_or.Draw();
  
  AmjuGL::PushMatrix();
  // Draw the text being edited:
  AmjuGL::Translate(-m_totalSize/2.0f, 0, -m_totalSize); 
  // Draw the text to the left of the cursor
  m_pComp->Draw();
  // Draw the text to the right of the cursor
  AmjuGL::Translate(m_leftSize + 0.5f, 0, 0);
  m_pCursor->Draw();
  m_pComp2->Draw();
  AmjuGL::PopMatrix();
  AmjuGL::PopMatrix();
}

void EngineStateRegister::DrawOverlays()
{
  m_pOkButton->Draw();
  m_pBuyButton->Draw();
  m_pCancelButton->Draw();

  GetEngine()->GetTextWriter()->PrintCentre(0,
    "Please type your registration code, then click the OK button.");

  GetEngine()->GetTextWriter()->PrintCentre(1,
    "If you don't have a registration code, you can visit the");

  GetEngine()->GetTextWriter()->PrintCentre(2,
    "online Amju Store and buy one by clicking Buy Now!");

  static float left = atof(Engine::Instance()->GetConfigValue("name_left").c_str());
  static float top = atof(Engine::Instance()->GetConfigValue("name_top").c_str());
  static float z = atof(Engine::Instance()->GetConfigValue("name_z").c_str());
/*
  AmjuGL::PushMatrix();
  AmjuGL::Translate(left, top, z);
  AmjuGL::Translate(14.0f, -12.0f, -1.0f);
  AmjuGL::Scale(0.8f, 0.8f, 0.8f);
  m_pOkText->Draw();

  AmjuGL::Translate(0, -2.2f, 0);
  m_pBuyText->Draw();
 
  AmjuGL::Translate(0, -2.2f, 0);
  m_pCancelText->Draw();

  AmjuGL::PopMatrix();
*/
}

void EngineStateRegister::MousePos(int x, int y)
{
  m_pOkButton->MousePos(x, y);
  m_pBuyButton->MousePos(x, y);
  m_pCancelButton->MousePos(x, y);
}

void EngineStateRegister::MouseButton(bool down, bool ctrl, bool shift)
{
  m_pOkButton->MouseButton(down, ctrl, shift);
  m_pBuyButton->MouseButton(down, ctrl, shift);
  m_pCancelButton->MouseButton(down, ctrl, shift);
}
}

