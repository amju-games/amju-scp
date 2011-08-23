/*
Amju Games source code (c) Copyright Jason Colman 2006
$Log: EsLogo.cpp,v $
Revision 1.1.2.6  2007/03/19 20:53:07  jay
Fix for final frame of this state

Revision 1.1.2.5  2006/08/30 21:08:46  Administrator
Get Pool to build in MSVC

Revision 1.1.2.4  2006/03/19 22:55:56  Administrator
Possible startup sound

Revision 1.1.2.3  2006/03/08 19:21:10  jay
Add #ifdef BOONTY

Revision 1.1.2.2  2006/03/08 19:06:15  jay
Added Logo state - shows Amju logo and BOONTY if defined

Revision 1.1  2006/02/16 13:29:07  jay
State to show Amju Games logo on startup

*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <iostream>
#include "EsLogo.h"
#include "Engine.h"
#include "Font.h"
#include "TextureServer.h"
#include "FileCheckState.h"

namespace Amju
{
const char* EsLogo::Name = "logo";
// Register this State with the Engine.
static const bool registered = Engine::Instance()->
  RegisterEngineState(EsLogo::Name, new EsLogo);

// Time per logo
static const float LOGO_TIME = 3.0f;

EsLogo::EsLogo()
{
  m_maxTime = LOGO_TIME;
}

void EsLogo::SetActive(bool active)
{
  m_time = 0;
  Engine::Instance()->SetClearColour(0, 0, 0);
  Engine::Instance()->SetLetterbox(active);

#ifdef STARTUP_SOUND
  if (active)
  {
    Engine::Instance()->PlayWav("rubber_ducky.wav");
  }
#endif
}

void EsLogo::TimerExpired()
{
  Engine::Instance()->ChangeState(FileCheckState::Name, Engine::IMMEDIATE);
}

void EsLogo::DrawOverlays()
{
}

void EsLogo::Draw()
{
  AmjuGL::Disable(AmjuGL::AMJU_LIGHTING);

  static bool first = true;
  if (first)
  {
    first = false;
    // Can't do this in ctor
    TexturedQuad tq;
    tq.Load("logo4.bmp", "");
    m_logos.push_back(tq);

#ifdef BOONTY
    TexturedQuad tq2;
    tq2.Load("boonty.bmp", "");
    m_logos.push_back(tq2);
#endif

    m_bg.Load("wh8.bmp", "");

    m_maxTime = LOGO_TIME * m_logos.size();
  }

  float rem = m_time - LOGO_TIME * (int)(m_time / LOGO_TIME);
  float c = sin(rem / LOGO_TIME * pi);
  Engine::Instance()->PushColour(c, c, c, 1.0f);

  m_bg.Draw(-1.0f, -5.0f, 20.0f, 30.0f); 
  static const float LOGO_WIDTH = 10.0f;
  static const float LOGO_HEIGHT = 10.0f;

  int i = (int)(m_time / LOGO_TIME);

  // Fix last frame error 
  if (i < m_logos.size())
  {
    m_logos[i].Draw(3.0f, 7.0f, 3.0f + LOGO_HEIGHT, 8.0f + LOGO_WIDTH); 
  }

  Engine::Instance()->PopColour();
}

void EsLogo::Update()
{
  EngineState::Update(); // update timer
}
}

