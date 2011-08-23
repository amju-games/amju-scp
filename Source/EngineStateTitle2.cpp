/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStateTitle2.cpp,v $
Revision 1.1.8.1  2007/07/15 22:07:16  Administrator
MSVC warning

Revision 1.1  2004/09/15 09:13:47  Administrator
Extra title state
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "EngineStateTitle2.h"
#include "EngineStateTitle.h"
#include "Engine.h"

namespace Amju
{
extern void ReportError(const std::string&);

const char* EngineStateTitle2::Name = "title_2";
// Register this State with the Engine.
static const bool registered = Engine::Instance()->
  RegisterEngineState(EngineStateTitle2::Name, new EngineStateTitle2);

EngineStateTitle2::EngineStateTitle2()
{
  m_hScroll = 0;
}

bool EngineStateTitle2::Load()
{
  // Get max time
  m_maxTime = GetEngine()->GetConfigFloat("title2_max_time");

  // Get title composite
  std::string titlecomp = GetEngine()->GetConfigValue("title2_comp");
  m_pComp = SolidComponent::LoadSolid(titlecomp);
  if (!m_pComp.GetPtr())
  {
    ReportError("Failed to load title 2 comp.");
    return false;
  }

  std::string bg = GetEngine()->GetConfigValue("title2_bg");
  if (!m_bg.Load(bg, ""))
  {
    return false;
  }
 
  return true;
}

void EngineStateTitle2::Red(bool down)
{
  // Go back to main title.
  Engine::Instance()->ChangeState(EngineStateTitle::Name, 
    Engine::IMMEDIATE);
}

void EngineStateTitle2::TimerExpired()
{
  Red(true);
}

void EngineStateTitle2::Draw()
{
  AmjuGL::PushMatrix();
  // Paint background
  DrawBg();

  // Do camera work
  m_pCam->Update();
  m_pCam->Draw();

  // Lighting
  m_lighting.DrawLight();

  // Position the text
  m_or.Draw();

  // Draw the text
  if (m_pComp.GetPtr())
  {
    AmjuGL::PushMatrix();
    // position
    static const float y = Engine::Instance()->GetConfigFloat("title2_y");
    static const float z = Engine::Instance()->GetConfigFloat("title2_z");
    AmjuGL::Translate(m_hScroll, y, z);

    // horizontal scroll
    static const float vel = 
      Engine::Instance()->GetConfigFloat("title2_scroll_vel");
    m_hScroll -= Engine::Instance()->GetDeltaTime() * vel;

    // Rotate from flat to upright. (All 3D letters are currently orientated
    // to be viewed from above.)
    AmjuGL::RotateX(90); //, 1, 0, 0);

    m_pComp->Update();
    m_pComp->Draw();

    AmjuGL::PopMatrix();
  }
  AmjuGL::PopMatrix();
}

void EngineStateTitle2::DrawOverlays()
{
}

void EngineStateTitle2::SetActive(bool active)
{
  EngineStateText::SetActive(active);
  m_hScroll = Engine::Instance()->GetConfigFloat("title2_scroll_pos");

}

void EngineStateTitle2::MouseButton(bool down, bool ctrl, bool shift)
{
  if (down)
  {
    Red(true);
  }
}

void EngineStateTitle2::MousePos(int x, int y)
{
}

}


