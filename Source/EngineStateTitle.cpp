/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStateTitle.cpp,v $
Revision 1.5.2.1.2.17  2007/12/10 22:55:25  jay
Remove dead code

Revision 1.5.2.1.2.16  2007/06/05 08:17:22  jay
#ifdef for thread which checks version; remove old XML includes

Revision 1.5.2.1.2.15  2006/08/14 17:38:03  jay
Rename "Pool"

Revision 1.5.2.1.2.14  2006/07/26 21:25:17  jay
Use Pool/, not Pool/

Revision 1.5.2.1.2.13  2006/03/24 22:57:42  jay
No hardcoded trial period here

Revision 1.5.2.1.2.12  2006/03/23 08:28:03  jay
Amju trial period is one hour

Revision 1.5.2.1.2.11  2006/03/10 19:04:52  jay
Demo may not be unlockable

Revision 1.5.2.1.2.10  2005/09/23 20:03:21  jay
Use AmjuExit so we can nag if unregistered

Revision 1.5.2.1.2.9  2005/08/31 17:28:22  jay
Changing colour background

Revision 1.5.2.1.2.8  2005/08/26 21:06:21  jay
Add Pool background

Revision 1.5.2.1.2.7  2005/07/05 09:35:07  jay
Disable buy/reg buttons if registered

Revision 1.5.2.1.2.6  2005/06/22 21:48:30  jay
Minor improvements to title

Revision 1.5.2.1.2.5  2005/06/13 22:10:44  jay
Set max time to 10s

Revision 1.5.2.1.2.4  2005/06/04 22:44:49  jay
Ongoing improvements to title

Revision 1.5.2.1.2.3  2005/05/24 20:34:07  jay
Ongoing improvements for pool

Revision 1.5.2.1.2.2  2005/04/03 16:29:00  jay
We need to draw something here to fix the lighting in all subsequent drawing
in other States! Obviously needs work...

Revision 1.5.2.1.2.1  2005/03/31 22:28:24  jay
Pool: "Title" state goes immediately into "Demo" state.

Revision 1.5.2.1  2004/11/09 16:01:31  jay
Merged full screen and exit features from XING 2.1 to this branch.

Revision 1.5.4.2  2004/11/08 13:58:15  jay
Added mac exit function

Revision 1.5.4.1  2004/11/08 10:16:56  Administrator
Added exit button to title screen.

Revision 1.5  2004/09/17 13:55:45  jay
Had to remove and re-add because of bad case when comitting file from dos.

Revision 1.3  2004/09/17 13:52:25  jay
Watch out for case issues when committing from dos :-(

Revision 1.2  2004/09/15 09:15:37  Administrator
Minor fixes

Revision 1.1  2004/09/08 15:42:37  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <iostream>
#include "EngineStateTitle.h"
#include "Level.h"
#include "Engine.h"
#include "FunctionFactory.h"
#include "TextFactory.h"
#include "TextWriter.h"
#include "CharacterManager.h"
#include "Text3dLeaf.h"
#include "EngineStatePoolGameSelect.h"
#include "EngineStatePoolCourseSelect.h"
#include "PoolMisc.h"
#include "GuiButton.h"
#include "StringUtils.h"
#include "EngineStateWorldDemoHiScores.h"
#include "EngineStatePoolSetUpShotDemo.h"
#include "PoolMisc.h" // StartDemoMode
#include "Launcher.h"
#include "TextColourizerColourList.h"
#include "Thread.h"
#include "HttpClient.h"
#include "TimePeriod.h"
#include "PoolBg.h"

using namespace std;

void OnStartClicked()
{
  Amju::Engine::Instance()->ChangeState(Amju::EngineStatePoolGameSelect::Name, 
    Amju::Engine::IMMEDIATE);
}

void OnRegisterClicked()
{
//  Amju::Engine::Instance()->ChangeState(Amju::EngineStateRegister::Name, 
//    Amju::Engine::IMMEDIATE);
}

void OnExitClicked();

void OnBuyButton() {}
void OnCancelButton() {}

namespace Amju
{
bool IsRegistered();

const char* EngineStateTitle::Name = "title"; 
// Register this State with the Engine.
static const bool registeredtitle = Engine::Instance()->
  RegisterEngineState(EngineStateTitle::Name, new EngineStateTitle);

#ifdef VERSION_THREAD

float EngineStateTitle::s_latestVersion;
Mutex EngineStateTitle::s_mutex;

class VersionThread : public Thread
{
public:
  virtual void Work();
};

void VersionThread::Work()
{
  static const char* PADFILE_URL = "http://www.amju.com/padfiles/amju-super-golf-pad-file.xml";

  HttpClient hc;
  HttpResult r = hc.Get(PADFILE_URL, HttpClient::GET);

//std::cout << r.c_str() << "\n";

  // Result should be an XML file. Parse it and extract the version info.
  XmlMemParser xmp(r.GetString().c_str());
  SharedPtr<XmlNodeInterface> pNode = xmp.getRootNode();
  XmlOutput myout(std::cout);
  myout << pNode.GetPtr();

  float latestVer = 1.0; // TODO

  // NB The static variable set in this function must be locked when set/read.
  EngineStateTitle::SetLatestVersion(latestVer);

  // Now send some general info for marketing.
  std::string url = "http://www.amju.com/cgi-bin/playerstats.pl?a=b";
  // TODO add info we are interested in

  hc.Get(url, HttpClient::GET);
}

void EngineStateTitle::SetLatestVersion(float v)
{
  MutexLocker ml(s_mutex);
  s_latestVersion = v;
}

float EngineStateTitle::GetLatestVersion()
{
  MutexLocker ml(s_mutex);
  return s_latestVersion;
}

#endif // VERSION_THREAD

EngineStateTitle::EngineStateTitle()
{
  m_time = 0;
  // Set in Load, title_max_time
  m_maxTime = 10.0f;
  m_verticalPos = 0; 

  explosionTimer = 3.0f;
  m_currentExplosion = 0;

  // Begin a thread to get the latest version no. of the software from the
  // web site.
  //VersionThread* pVersionThread = new VersionThread;
  //pVersionThread->Start();
}

bool EngineStateTitle::Load()
{
  if (!ThePoolBg::Instance()->Load())
  {
    GetEngine()->ReportError("Failed to load Pool BG.");
    return false;
  }

  // First title comp - these currently move with scaling
  string comp = GetEngine()->GetConfigValue("title_comp");
  m_pComp = SolidComponent::LoadSolid(comp);
  if (!m_pComp.GetPtr())
  {
    GetEngine()->ReportError("Failed to load title letters.");
    return false;
  }

  // Second title comp - no scaling
  std::string comp2 = Engine::Instance()->GetConfigValue("title_comp2");
  m_pGreetText = SolidComponent::LoadSolid(comp2);
  if (!m_pGreetText.GetPtr())
  {
    Engine::Instance()->ReportError("Failed to load second title comp.");
    return false;
  }

//  string bg = GetEngine()->GetConfigValue("title_bg");
  if (!m_bg.Load("wh8.png", ""))
  {
    return false;
  }

/*
  // Characters are: amju, cat, bird, fox, cow, hippo (?)
  if (Engine::Instance()->GetConfigValue("title_character_amju") == "y")
  { 
    LoadCharacter("amju");
    LoadCharacter("marin");
  }
*/

  // Set up GUI buttons
  // "Start" button
  m_pStartButton = new GuiButton;
  std::string goButtonFile = GetEngine()->GetConfigValue("pool_startbutton");
  if (!m_pStartButton->Load(goButtonFile))
  {
    return false;
  }
  m_pStartButton->SetSize(7.75f, 2.0f);
  m_pStartButton->SetRelPos(12.0f, 7.25f);
  m_pStartButton->SetCommand(&OnStartClicked);

  // "Register" button
  m_pRegButton = new GuiButton;
  std::string regButtonFile = GetEngine()->GetConfigValue("golf_reg_button");
  if (!m_pRegButton->Load(regButtonFile))
  {
    return false;
  }
  m_pRegButton->SetSize(7.75f, 2.0f);
  m_pRegButton->SetRelPos(14.0f, 5.0f);
  m_pRegButton->SetCommand(&OnRegisterClicked);

  // Buy now button
  m_pBuyButton = new GuiButton;
  std::string buyButtonFile = GetEngine()->GetConfigValue("golf_buy_button");
  if (!m_pBuyButton->Load(buyButtonFile))
  {
    return false;
  }
  m_pBuyButton->SetSize(8.0f, 2.0f);
  m_pBuyButton->SetRelPos(14.0f, 12.5f);
  m_pBuyButton->SetCommand(&OnBuyButton);
  
  // Exit button
  m_pExitButton = new GuiButton;
  std::string exitButtonFile = GetEngine()->GetConfigValue("pool_exit_button");
  if (!m_pExitButton->Load(exitButtonFile))
  {
    return false;
  }
  m_pExitButton->SetSize(2.0f, 2.0f);
  m_pExitButton->SetRelPos(12.0f, 15.25f);  // top, left
  m_pExitButton->SetCommand(&OnExitClicked);

  if (!LoadExplosion())
  {
    return false;
  }

  // Load all letters, so none are marked as "unused".
  PSolidComponent pLetters = TextFactory::Instance()->Create(
   "abcdefghijklmnopqrstuvwxyz0123456789!");

  m_maxTime = Engine::Instance()->GetConfigFloat("title_max_time");

  return true;
}

bool EngineStateTitle::LoadCharacter(const std::string& charname)
{
  SharedPtr<CharacterGameObject> pChar = new CharacterGameObject;
  pChar->AddMesh(CharacterManager::Instance()->GetCharacter(charname));
  m_characters.push_back(pChar);

  return true;
}

bool EngineStateTitle::LoadExplosion()
{
  string solidName = Engine::Instance()->GetConfigValue("title_particle");

  // Load the explosion effect
  PSolidComponent pBb = SolidComponent::LoadSolid(solidName);
  if (!pBb.GetPtr())
  {
    Engine::Instance()->ReportError("Bad particle name for title.");
    return false;
  }

  // TODO CONFIG num particles, max vel/time 
  int numParticles = (int)Engine::Instance()->GetConfigFloat("egg_num_particles");
  if (numParticles == 0)
  {
    return false;
  }

  for (int i = 0; i < NUM_EXPLOSIONS; i++)
  {
    m_explosion[i].Init(numParticles, pBb, 10, 10);
  }

  return true;
}

void EngineStateTitle::SetActive(bool active)
{
////  EngineStateText::SetActive(active);
  m_time = 0;
  m_lighting.Init();

  if (active)
  {
    std::string titlesong = GetEngine()->GetConfigValue("title_song");
    GetEngine()->PlaySong(titlesong);
  }
}

void EngineStateTitle::Red(bool down)
{
}

void EngineStateTitle::TimerExpired()
{
  StartDemoMode();
  GetEngine()->ChangeState(EngineStatePoolSetUpShotDemo::Name, Engine::IMMEDIATE);
}

void EngineStateTitle::Draw()
{
  AmjuGL::PushMatrix();
  // Paint background - cycle through colours
  static float r = 0;
  static float g = 1.0f;
  static float b = 2.0f;
  float dt = Engine::Instance()->GetDeltaTime();
  r += dt * 1.0f;
  g += dt * 1.5f;
  b += dt * 2.0f;
  Engine::Instance()->PushColour(
    (sin(r) + 1.0f) * 0.5f, 
    (sin(g) + 1.0f) * 0.5f, 
    (sin(b) + 1.0f) * 0.5f, 
    1.0f);
  DrawBg();
  Engine::Instance()->PopColour();

  // Do camera work
  m_pCam->Update();
  m_pCam->Draw();

  m_lighting.DrawLight();

  m_or.Draw();

  // Draw Pool Background
  ThePoolBg::Instance()->Draw(1.0f);

  for (int i = 0; i < NUM_EXPLOSIONS; i++)
  {
    m_explosion[i].Draw();
  }

  // Wave effect using scaling - POOL TEST
  static float f = 0;
  f += Engine::Instance()->GetDeltaTime();
    AmjuGL::PushMatrix();
      static const float s = 0.5f;

      AmjuGL::Scale(s + 0.2f * sin(f), 
               s + 0.2f * sin(f + 1.0f), 
               s + 0.1f * sin(f + 2.0f));

      AmjuGL::Translate(0, m_verticalPos, 0);
      // Rotate from flat to upright. (All 3D letters are currently orientated
      // to be viewed from above.)
      AmjuGL::RotateX(90); //, 1, 0, 0);

      m_pComp->Update();
      m_pComp->Draw();
    AmjuGL::PopMatrix();

    m_pGreetText->Update();
    m_pGreetText->Draw();

  AmjuGL::PopMatrix();

  m_time += GetEngine()->GetDeltaTime();
  if (m_time > m_maxTime)
  {
    TimerExpired();
  }
/*  
  GetEngine()->GetTextWriter()->PrintCentre(
    GetEngine()->GetConfigFloat("website_y"), 
    GetEngine()->GetConfigValue("website").c_str());
*/
}

void EngineStateTitle::DrawOverlays()
{
  bool unreg = !IsRegistered();
  int secs = 0; 
  bool expired = (secs == 0); 
  if (unreg && expired)
  {
    m_pStartButton->SetEnabled(false);
  }
  else
  {
    m_pStartButton->SetEnabled(true);
  }

  m_pStartButton->Draw();
  m_pExitButton->Draw();

  if (unreg)
  {
#ifndef AMJU_NOT_UNLOCKABLE
    m_pRegButton->Draw();
    m_pBuyButton->Draw();
#endif
  }
  else
  {
    m_pRegButton->SetEnabled(false);
  }

  if (unreg && expired)
  {
    std::string nag = "Thanks for playing Amju Super Cool Pool!";
    GetEngine()->GetTextWriter()->Print(0, 0, nag.c_str());

#ifdef AMJU_NOT_UNLOCKABLE
    nag = "Your trial period has finished.";
#else
    nag = "Please register to keep playing!";
#endif
    GetEngine()->GetTextWriter()->Print(0, 0.8f, nag.c_str());
  }
  else if (unreg)
  {
    std::string nag = "Thanks for playing Amju Super Cool Pool!";
    GetEngine()->GetTextWriter()->Print(0, 0, nag.c_str());
    
    nag = "Your trial period expires in ";
    nag += TimePeriod(secs).ToString();
    GetEngine()->GetTextWriter()->Print(0, 0.8f, nag.c_str());
  }
}

void EngineStateTitle::MouseButton(bool down, bool ctrl, bool shift)
{
  m_pStartButton->MouseButton(down, ctrl, shift);
  m_pExitButton->MouseButton(down, ctrl, shift);

  bool unreg = !IsRegistered();
  if (unreg)
  {
    m_pRegButton->MouseButton(down, ctrl, shift);
    m_pBuyButton->MouseButton(down, ctrl, shift);
  }
}

void EngineStateTitle::MousePos(int x, int y)
{
  m_pStartButton->MousePos(x, y);
  m_pRegButton->MousePos(x, y);
  m_pBuyButton->MousePos(x, y);
  m_pExitButton->MousePos(x, y);
}

}

