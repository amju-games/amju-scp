/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: FunctionTimedBlocker.cpp,v $
Revision 1.1.10.1  2007/07/15 21:55:16  Administrator
MSVC warnings

Revision 1.1  2004/09/08 15:42:50  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "FunctionTimedBlocker.h"
#include "File.h"
#include "Engine.h"
#include "Earthquake.h" // TOTALLY the wring place for this
#include "SchAssert.h"

namespace Amju
{
const char* FunctionTimedBlocker::Name = "timed_blocker";

void FunctionTimedBlocker::Recalc(Orientation* p)
{
  const float dt = Engine::Instance()->GetDeltaTime();

  if (m_time <= m_duration)
  {
    FunctionBlocker::Recalc(p);
    return;
  }
  if (m_time <= (m_duration + m_timeOpen))
  {
    // In open state; do nothing until m_timeOpen secs have passed.
    m_time += dt;
    return;
  }
  if (m_time <= (2 * m_duration + m_timeOpen))
  {
    // Returning to initial orientation

    m_time += dt;

    float between = (m_time - m_duration - m_timeOpen) / m_duration;
    Assert(between >= 0);
    Assert(between <= 1.0f);
    // Use handy interpolation function to get position between before
    // and after. (See Orientation)

    // NB before and after are swapped so we move back
    *p = Interpolate(m_after, m_before, between);

    // Repeating sound
    static const float maxSoundTime = Engine::Instance()->
      GetConfigFloat("blocker_sound_period");
    static const std::string wav = Engine::Instance()->
      GetConfigValue("wav_blocker_repeating");

    m_soundTime += dt;
    if (m_soundTime > maxSoundTime)
    {
      // Play the sound
      Engine::Instance()->PlayWav(wav);
      m_soundTime = 0;
    }
    return;
  }
  // Has returned to initital state; do nothing.
  // Except for final sound effect
  if (!m_quake)
  {
    m_quake = true;
    // TODO CONFIG
    Engine::Instance()->GetEarthquake().SetActive(0.5f, 0.1f);

    static const std::string wav = Engine::Instance()->
      GetConfigValue("wav_blocker_final");
    Engine::Instance()->PlayWav(wav);
  }

}

bool FunctionTimedBlocker::Load(File* pf)
{
  if (!FunctionBlocker::Load(pf))
  {
    return false;
  }
  if (!pf->GetFloat(&m_timeOpen))
  {
    pf->ReportError("Expected open time.");
    return false;
  }
  return true;
}

#if defined(SCENE_EDITOR)
const char* FunctionTimedBlocker::GetTypeName() const
{
  return FunctionTimedBlocker::Name;
}

bool FunctionTimedBlocker::Save(File* pf)
{
  if (!FunctionBlocker::Save(pf))
  {
    return false;
  }
  pf->WriteComment("// Open time");
  pf->WriteFloat(m_timeOpen);
  return true;
}
#endif
}

