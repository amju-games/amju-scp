#include "ExplosionEffect.h"
#include "Engine.h"
#include "File.h"
#include "TextColourizerColourList.h"

namespace Amju
{
static const float s_g = 0.01f;

ExplosionEffect::ExplosionEffect()
{
}

bool ExplosionEffect::Load(const std::string& fileName)
{
  File f;
  if (!f.OpenRead(fileName))
  {
    f.ReportError("Failed to open explosion effect file.");
    return false;
  }

#if defined(SCENE_EDITOR)
  m_name = fileName;
#endif

  // Get the solid (billboard)
  std::string solidName;
  if (!f.GetDataLine(&solidName))
  {
    f.ReportError("Expected Solid name.");
    return false;
  }
  PSolidComponent pSolid = SolidComponent::LoadSolid(solidName);
  if (!pSolid.GetPtr())
  {
    f.ReportError("Failed to load Solid for explosion.");
    return false;
  }

  // Get number of particles
  int numParticles = 0;
  if (!f.GetInteger(&numParticles))
  {
    f.ReportError("Expected number of particles.");
    return false;
  }

  // Get max particle velocity
  float maxVel = 0;
  if (!f.GetFloat(&maxVel))
  {
    f.ReportError("Expected max particle velocity.");
    return false; 
  }

  // Get max particle time
  float maxTime = 0;
  if (!f.GetFloat(&maxTime))
  {
    f.ReportError("Expected max particle time.");
    return false;
  }

  Init(numParticles, pSolid, maxVel, maxTime);
  return true;
}

void ExplosionEffect::Init(
  int numParticles, 
  PSolidComponent pBb, 
  float maxSpeed, 
  float maxTime)
{
  for (int i = 0; i < numParticles; i++)
  {
    m_particles.push_back(ExplosionParticle(pBb.GetPtr()));
  }
}

void ExplosionEffect::Draw()
{
return; // TODO TEMP TEST

  // Disable lighting for the particles.
  AmjuGL::PushAttrib(AmjuGL::AMJU_LIGHTING);
  AmjuGL::Disable(AmjuGL::AMJU_LIGHTING);
  // Disable writing to the depth buffer.
  AmjuGL::Disable(AmjuGL::AMJU_DEPTH_WRITE);
  for (ParticleList::iterator it = m_particles.begin(); it != m_particles.end(); ++it)
  {
    ExplosionParticle& e = *it;
    e.Draw();
  }
  AmjuGL::Enable(AmjuGL::AMJU_DEPTH_WRITE);
  AmjuGL::PopAttrib();
}

void ExplosionEffect::Kill()
{
  for (ParticleList::iterator it = m_particles.begin(); it != m_particles.end(); ++it)
  {
    ExplosionParticle& e = *it;
    e.Kill();
  }
}

void ExplosionEffect::Reset(const Vec3f& centre)
{
  for (ParticleList::iterator it = m_particles.begin(); it != m_particles.end(); ++it)
  {
    ExplosionParticle& e = *it;
    e.Reset(centre);
  }
}

ExplosionParticle::ExplosionParticle(SolidComponent* pBb)
{
  m_pBb = pBb;
  m_isDead = true;
}

void ExplosionParticle::Draw()
{
  if (m_isDead)
  {
    return;
  }

  const float delta = Engine::Instance()->GetDeltaTime();
  m_time += delta;
  if (m_time > m_maxTime)
  {
    // Particle is dead.
    m_isDead = true;
  }

  // Update the position of the particle, then draw it.
  m_vely += s_g * delta;
  m_centre = Vec3f(m_centre.x + m_velx, m_centre.y - m_vely, m_centre.z + m_velz);
  // NB Positive Y vel => downwards

  AmjuGL::PushMatrix();
  AmjuGL::Translate(m_centre.x, m_centre.y, m_centre.z);

  float f = 1.0f - m_time / m_maxTime;
  Engine::Instance()->PushColour(f, f, 1, f); // more blue + transparent

  m_pBb->Draw();

  Engine::Instance()->PopColour();

  AmjuGL::PopMatrix();
}

void ExplosionParticle::Kill()
{
  m_isDead = true;
}

void ExplosionParticle::Reset(const Vec3f& centre)
{
  m_centre = centre;
  m_time = 0;
  m_isDead = false;
  // Get random max time and velocities
  m_maxTime = 6;//(float)rand() / (float)RAND_MAX * 25.0f; // TODO CONFIG
  m_velx = (float)rand() / (float)RAND_MAX * 0.5f - 0.25f;
  m_vely = (float)rand() / (float)RAND_MAX * 0.5f - 0.25f;
  m_velz = (float)rand() / (float)RAND_MAX * 0.5f - 0.25f;
}

// ----------------------------------------------------------------------------

void ExplosionEffectMulticolour::Init(
  int numParticles, 
  PSolidComponent pBb, 
  float maxSpeed, 
  float maxTime)
{
  // Use a "Colourizer", usually used to colour letters different colours.
  static TextColourizerColourList colourizer;
  static bool first = true;
  if (first)
  {
    colourizer.AddColour(Colour(1.0f, 0.5f, 0.5f, 1.0f));
    colourizer.AddColour(Colour(0.5f, 1.0f, 0.5f, 1.0f));
    colourizer.AddColour(Colour(1.0f, 1.0f, 0.5f, 1.0f));
    colourizer.AddColour(Colour(0.5f, 1.0f, 1.0f, 1.0f));
    // TODO more colours ?
    first = false;
  }

  for (int i = 0; i < numParticles; i++)
  {
    SolidComponent* p = colourizer.ColourizeCharacter(pBb.GetPtr());
    m_particles.push_back(ExplosionParticle(p));
  }
}

}

