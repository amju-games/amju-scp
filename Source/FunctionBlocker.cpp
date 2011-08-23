/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: FunctionBlocker.cpp,v $
Revision 1.1  2004/09/08 15:42:50  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "FunctionBlocker.h"
#include "FunctionFactory.h"
#include "File.h"
#include "Engine.h"
#include "Earthquake.h" // TOTALLY the wring place for this
#include "SchAssert.h"

namespace Amju
{
/*
// Register class with Factory.
Function* CreateFnBlocker() { return new FunctionBlocker; }
static const bool isRegistered = FunctionFactory::Instance()->
  RegisterFactoryFunction(FunctionBlocker::Name, CreateFnBlocker);
*/

const char* FunctionBlocker::Name = "blocker";

#if defined(SCENE_EDITOR)
const char* FunctionBlocker::GetTypeName() const
{
  return FunctionBlocker::Name;
}

bool FunctionBlocker::Save(File* pf)
{
  pf->WriteComment("// Orientation before");
  m_before.Save(pf);

  pf->WriteComment("// Orientation after");
  m_after.Save(pf);

  pf->WriteComment("// Period");
  pf->WriteFloat(m_duration);

  return true;
}
#endif

bool FunctionBlocker::Load(File* pf)
{
  if (!m_before.Load(pf))
  {
    pf->ReportError("Failed to load Before orientation in function.");
    return false;
  }

  if (!m_after.Load(pf))
  {
    pf->ReportError("Failed to load After orientation in function.");
    return false;
  }
 
  if (!pf->GetFloat(&m_duration))
  {
    pf->ReportError("Failed to load duration in function.");
  }
 
  // Work out the velocities
  m_vels.SetX((m_after.GetX() - m_before.GetX()) / m_duration);
  m_vels.SetY((m_after.GetY() - m_before.GetY()) / m_duration);
  m_vels.SetZ((m_after.GetZ() - m_before.GetZ()) / m_duration);
  // TODO no rotation for now

  m_soundTime = 0;
  m_quake = false;
  return true;
}

void FunctionBlocker::Recalc(Orientation* p)
{
  if (m_time == 0)
  {
    m_quake = false;
  }

  const float dt = Engine::Instance()->GetDeltaTime();
  m_time += dt;
  if (m_time > m_duration)
  {
    if (!m_quake)
    {
      m_quake = true;
      // TODO CONFIG
      Engine::Instance()->GetEarthquake().SetActive(0.5f, 0.1f);

      static const std::string wav = Engine::Instance()->
        GetConfigValue("wav_blocker_final");
      Engine::Instance()->PlayWav(wav);
    }

    m_time = m_duration;
  }
  else
  {
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
  }

  float between = m_time / m_duration;
  Assert(between >= 0);
  Assert(between <= 1.0f);
  // Use handy interpolation function to get position between before
  // and after. (See Orientation)
  *p = Interpolate(m_before, m_after, between);

}
}

