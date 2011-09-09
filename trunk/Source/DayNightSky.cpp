/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: DayNightSky.cpp,v $
Revision 1.1.8.1  2005/08/26 21:08:01  jay
Turn off day/night change

Revision 1.1  2004/09/08 15:42:47  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "DayNightSky.h"
#include "Engine.h"
#include "Common.h"
#include "SchAssert.h"
#include <iostream>
#include <string>

using namespace std;

namespace Amju
{
DayNightSky::DayNightSky()
{
  m_timeOfDay = 11.0f;
  m_gameHoursPerSec = 1.0f;
  m_isShootingStar = false;
  m_shootStarTimer = 0;
  m_shootstarAngle = 0;
}

bool DayNightSky::Init()
{
  m_lighting.Load("normal.lite");


  string sun = Engine::Instance()->GetConfigValue("sun");
  m_sun = SolidComponent::LoadSolid(sun);
  if (!m_sun.GetPtr())
  {
    return false;
  }

  string moon = Engine::Instance()->GetConfigValue("moon");
  m_moon = SolidComponent::LoadSolid(moon);
  if (!m_moon.GetPtr())
  {
    return false;
  }

  // Get billboards
  if (!m_sunsparkle.Load(Engine::Instance()->GetConfigValue("sun_sparkle")))
  {
    return false;
  }

  if (!m_shootingStar.Load(Engine::Instance()->GetConfigValue("shooting_star")))
  {
    return false;
  }

  // Get skybox composites
  string daybox = Engine::Instance()->GetConfigValue("day_box");
  m_pDayBox = SolidComponent::LoadSolid(daybox);
  if (!m_pDayBox.GetPtr())
  {
    return false;
  }

  string nightbox = Engine::Instance()->GetConfigValue("night_box");
  m_pNightBox = SolidComponent::LoadSolid(nightbox);
  if (!m_pNightBox.GetPtr())
  {
    return false;
  }

  m_gameHoursPerSec = atof(Engine::Instance()->GetConfigValue("game_hours_per_sec").c_str());

  m_dawnBegin = atof(Engine::Instance()->GetConfigValue("dawn_begin").c_str());
  m_dawnEnd = atof(Engine::Instance()->GetConfigValue("dawn_end").c_str());
  m_duskBegin = atof(Engine::Instance()->GetConfigValue("dusk_begin").c_str());
  m_duskEnd = atof(Engine::Instance()->GetConfigValue("dusk_end").c_str());
  m_sparkleBegin = atof(Engine::Instance()->GetConfigValue("sparkle_begin").c_str());
  m_sparkleEnd = atof(Engine::Instance()->GetConfigValue("sparkle_end").c_str());

  m_dayLevel = atof(Engine::Instance()->GetConfigValue("day_lightlevel").c_str());
  m_nightLevel = atof(Engine::Instance()->GetConfigValue("night_lightlevel").c_str());

  return true;
}

void DayNightSky::Update()
{
  // Update time.
  float d = Engine::Instance()->GetDeltaTime();
  m_timeOfDay += d * m_gameHoursPerSec;

  if (m_timeOfDay >= 24.0f)
  {
    m_timeOfDay -= 24.0f;
  }

  // Update all positions, etc: only do this every so often.
  // Get daylight value. This is 0 at night, 1 in the day, and somewhere in between during
  // dusk and dawn.
    m_daylight = 1.0f;
    if (m_timeOfDay >= m_dawnBegin && m_timeOfDay <= m_dawnEnd)
    {
      m_daylight = (m_timeOfDay - m_dawnBegin) / (m_dawnEnd - m_dawnBegin);
    }
    else if (m_timeOfDay >= m_duskBegin && m_timeOfDay <= m_duskEnd)
    {
      m_daylight = (m_duskEnd - m_timeOfDay) / (m_duskEnd - m_duskBegin);
    }
    else if (m_timeOfDay > m_duskEnd || m_timeOfDay < m_dawnBegin)
    {
      m_daylight = 0;
    }
    Assert(m_daylight >= 0);
    Assert(m_daylight <= 1.0f);

    m_s = -sin(m_timeOfDay / 24.0f * pi*2);
    m_c = -cos(m_timeOfDay / 24.0f * pi*2);

    m_sunpos = Vec3f(m_s * 400, m_c * 400 - 100, 0);
    m_moonpos = Vec3f(0, -m_c * 400 - 100, m_s * 400);
    m_sunsparklepos = Vec3f(m_s * 350, m_c * 350 - 100, 0);

/*
    // Shooting star
    // If currently no shooting star, count down time until we create one.
    if (!m_isShootingStar)
    {
      m_shootStarTimer -= d;
      if (m_shootStarTimer <= 0)
      {
        // It's time to create a new shooting star.
        m_isShootingStar = true;
        // Reset timer
        m_shootStarTimer = 2.0f; //(float)rand() / (float)RAND_MAX * 10.0f + 3.0f;
        // Get start pos and velocity for the shooting star.
        m_shootstarAngle = (float)rand() / (float)RAND_MAX * 360.0f;
        m_shootstarpos = Vec3f(-200, 200, 200);
      }
    }
    else
    {
      float x = m_shootstarpos.x;
      x += d * 500.0f; // TODO CONFIG
      float y = m_shootstarpos.y;
      y -= d * 40.0f;
      float z = m_shootstarpos.z;
      m_shootstarpos = Vec3f(x, y, z);

      if (x > 200.0f)
      {
        m_isShootingStar = false;
      }
    }
*/
}

void DayNightSky::DrawSkybox()
{
  Assert(m_pDayBox.GetPtr());
  Assert(m_pNightBox.GetPtr());

  // No lighting for skybox - we don't want it to actually look like the 
  // inside of a box.
  // Also disable any fog.
  AmjuGL::PushAttrib(AmjuGL::AMJU_LIGHTING | AmjuGL::AMJU_DEPTH_READ);
  AmjuGL::Disable(AmjuGL::AMJU_LIGHTING);
  AmjuGL::Disable(AmjuGL::AMJU_DEPTH_READ); // Don't need to depth test

  m_pDayBox->Draw();

  // POOL - don't bother, the sky is barely visible.
/*
  // Blend in night box, which assumes it is slightly smaller than the day box.
  if (m_daylight < 1.0f)
  {
    Engine::Instance()->PushColour(1.0f, 1.0f, 1.0f, 1.0f - m_daylight);
    m_pNightBox->Draw();
    Engine::Instance()->PopColour();
  }
*/
  // Turn the things we disabled back on.
  AmjuGL::PopAttrib();
}

void DayNightSky::DrawLight()
{
  return; // Uh ?

  // Set light 0 position.
  // This is independent of the dusk/dawn times, to make the changes in the light pos.
  // as smooth as possible.
  float c2 = m_c;
  if (c2 < 0)
  {
    c2 = -c2;
  }

  m_lighting.SetPosition(m_s + 1, c2 + 1, 0);

  bool sparkle = (m_timeOfDay >= m_sparkleBegin && m_timeOfDay <= m_sparkleEnd);

  // Draw sun if daylight
  if (m_timeOfDay >= m_dawnBegin && m_timeOfDay <= m_duskEnd)
  {
    // Set sun colour to be more red at dusk/dawn
    Engine::Instance()->PushColour(1.0f, m_daylight, m_daylight, 1.0f);

    // Position and draw sun
    AmjuGL::PushMatrix();
    AmjuGL::Translate(m_sunpos.x, m_sunpos.y, m_sunpos.z);
    m_sun->Draw();
    AmjuGL::PopMatrix();

    if (sparkle)
    {
      AmjuGL::PushMatrix();
      AmjuGL::Translate(m_sunsparklepos.x, m_sunsparklepos.y, m_sunsparklepos.z);
      AmjuGL::RotateZ(m_timeOfDay); //, 0, 0, 1);
      m_sunsparkle.Draw();
      AmjuGL::PopMatrix();
    }

    Engine::Instance()->PopColour();

    // Set ambient light. This fades from white to blue at dusk and dawn.
    float w = m_daylight * (m_dayLevel - m_nightLevel) + m_nightLevel;
    m_lighting.SetGlobalAmbient(w, w, m_dayLevel, 1.0f);
  }
  else
  {
    // Night time. 
    // Set ambient light to Blue.
    m_lighting.SetGlobalAmbient(m_nightLevel, m_nightLevel, m_dayLevel, 1.0f); 
  }

  // Position and draw moon 
  if (m_timeOfDay <= (m_dawnBegin + 2.0f) && 
      m_timeOfDay <= (m_duskEnd - 2.0f) ) 
  {
    AmjuGL::PushMatrix();
    AmjuGL::Translate(m_moonpos.x, m_moonpos.y, m_moonpos.z);
    m_moon->Draw();
    AmjuGL::PopMatrix();
  }

/*
    if (m_isShootingStar)
    {
      // Position and draw shooting star.
      AmjuGL::PushMatrix();
      // Rotate about y-axis
      AmjuGL::Translate(m_shootstarpos.x, m_shootstarpos.y, m_shootstarpos.z);
      AmjuGL::Rotate(m_shootstarAngle,  0, 1, 0);
      m_shootingStar.Draw();
      AmjuGL::PopMatrix();
    }
*/

  m_lighting.DrawLight(); 
}

bool DayNightSky::IsSparkleVisible() const
{
  return (m_timeOfDay >= m_sparkleBegin && m_timeOfDay <= m_sparkleEnd);
}
}
