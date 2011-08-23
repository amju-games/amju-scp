/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: GameObjectSoundSource.cpp,v $
Revision 1.1.8.1  2005/06/05 00:22:54  Administrator
Compile fix for const Orientation

Revision 1.1  2004/09/08 15:42:50  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "GameObjectSoundSource.h"
#include "File.h"
#include "Engine.h"
#include "Player.h"

namespace Amju
{
GameObjectSoundSource::GameObjectSoundSource() : 
  m_time(0)
{
#if defined(SCENE_EDITOR)
  // default constructed Game Object must be saveable for Editor.
  m_wavFile = "tada.wav"; // TODO CONFIG PREFERENCES
  m_period = 1.0f;
#endif
}

void GameObjectSoundSource::Update()
{
  if (GetState() == OUT_OF_PLAY)
  {
    // Sound has been turned off.
    return;
  }
  
  // Wait until next period
  m_time += Engine::Instance()->GetDeltaTime();
  if (m_time < m_period)
  {
    return;
  }

  m_time = 0;
  // When next period, play wav with level proportional to player
  // distance from sound source centre.
  // Get player pos
  const Orientation* pO = 
    Engine::Instance()->GetPlayer()->FreeMovingGameObject::GetOrientation();
  const float dx = pO->GetX() - m_soundSphere.GetCentre().x;
  const float dy = pO->GetY() - m_soundSphere.GetCentre().y;
  const float dz = pO->GetZ() - m_soundSphere.GetCentre().z;
  const float dist2 = dx * dx  +  dy * dy  +  dz * dz;
  const float radius = m_soundSphere.GetRadius();
  const float radius2 = radius * radius;
  if (dist2 < radius2)
  {
    float dist = sqrt(dist2);
    // Player is within sound sphere. Work out the volume.
    float volume = 1.0f - dist / radius;
    // TODO increase vol (but clamp at 1.0) ?
    Engine::Instance()->PlayWav(m_wavFile, volume);
  }
}

bool GameObjectSoundSource::Load(File* pf)
{
  if (!m_soundSphere.Load(pf))
  {
    pf->ReportError("Failed to load sound source sphere.");
    return false;
  }

  if (!pf->GetDataLine(&m_wavFile))
  {
    pf->ReportError("Expected wav file for sound source.");
    return false;
  }

  if (!pf->GetFloat(&m_period))
  {
    pf->ReportError("Expected sound source period.");
    return false;
  }
  return true;
}

#if defined(SCENE_EDITOR)
bool GameObjectSoundSource::Save(File* pf)
{
  pf->WriteComment("// Sound sphere");
  m_soundSphere.Save(pf);

  pf->WriteComment("// Wav file");
  pf->Write(m_wavFile);

  pf->WriteComment("// Period");
  pf->WriteFloat(m_period);

  return true; 
}

static const std::string RADIUS = "radius";
static const std::string WAV_FILE = "wav file";
static const std::string PERIOD = "period";

GameObject::PropertyMap GameObjectSoundSource::GetProperties() const
{
  PropertyMap pm;
  pm[RADIUS] = GameObject::Property(m_soundSphere.GetRadius());
  pm[WAV_FILE] = GameObject::Property(m_wavFile);
  pm[PERIOD] = GameObject::Property(m_period);

  return pm;
}

void GameObjectSoundSource::SetProperties(const GameObject::PropertyMap& pm)
{
  m_soundSphere.SetRadius(const_cast<GameObject::PropertyMap&>(pm)[RADIUS].GetFloat());
  m_wavFile = const_cast<GameObject::PropertyMap&>(pm)[WAV_FILE].GetString();
  m_period = const_cast<GameObject::PropertyMap&>(pm)[PERIOD].GetFloat();
}
#endif

const char* GameObjectSoundSource::GetTypeName() const
{
  return "sound";
}
}

