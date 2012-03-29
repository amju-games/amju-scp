/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ParticleSystem.cpp,v $
Revision 1.1  2004/09/08 15:42:54  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "ParticleSystem.h"
#include "CompositeFactory.h"
#include "Function.h"
#include "FunctionFactory.h"
#include "File.h"
#include "Engine.h"
#include <stdlib.h>

using namespace std;

namespace Amju
{
ParticleSystem::ParticleSystem() 
{
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::Draw()
{
  for (unsigned int i = 0; i< m_particles.size(); i++)
  {
    Particle* pParticle = m_particles[i].GetPtr();

    if (pParticle)
    {
      // Set the time for the particle. This is because we allow particles of
      // arbitrary complexity - it could be a moving platform, or another
      // particle system, etc. 
      //pParticle->SetTime(m_time); 
      
      // Recalc orientation for the particle. We use a function, with the particle
      // ID modifying the time parameter, so the particles will not all have the 
      // same orientation.
      float t = (float)Engine::Instance()->GetElapsedTime();

      m_pFunc->SetTime(t + i * 0.02f);
      m_pFunc->Recalc(pParticle->GetOrientation());

      pParticle->Orientate();
      pParticle->Draw();
    }
  }
}

int ParticleSystem::CullDraw(const Frustum& f)
{
  // TODO 
  Draw();
  return GetNumberOfPolygons();
}

int ParticleSystem::GetNumberOfPolygons() const
{
  // Get the number of polys in each particle. As we know all the particles are
  // the same, we can just get number of polys for the first particle, and
  // mult by the number of particles.
  if (m_particles.empty())
  {
    return 0;
  }
  Particle* pParticle = m_particles[0].GetPtr();
  int total = m_particles.size() * pParticle->GetNumberOfPolygons();
  return total;
}

bool ParticleSystem::Load(const string& filename)
{
  // Open file.
  File f;
  if (!f.OpenRead(filename))
  {
    f.ReportError("Couldn't open particle file.");
    return false;
  }

  // Get number of particles.
  int numParticles = 0;
  if (!f.GetInteger(&numParticles))
  {
    f.ReportError("Expected number of particles.");
    return false;
  }

  // Get composite name of particle - this is the same for all.
  // Each particle is a composite, allowing arbitrary complexity.
  string compName;
  if (!f.GetDataLine(&compName))
  {
    f.ReportError("Expected particle (composite) filename.");
    return false;
  }
  
  // Load the particles. 
  for (int i = 0; i < numParticles; i++)
  {
    PSolidComponent pParticle = SolidComponent::LoadSolid(compName);
    if (!pParticle.GetPtr())
    {
      f.ReportError("Failed to load particle.");
      return false;
    }

    m_particles.push_back(pParticle);
  }

  // Load the movement function.
  // Get function name
  string fnName;
  if (!f.GetDataLine(&fnName))
  {
    f.ReportError("Expected particle system function name.");
    return false;
  }

  if (fnName.empty())
  {
    f.ReportError("Empty particle function name.");
    return false;
  }
  m_pFunc = FunctionFactory::Instance()->Create(fnName);
  if (!m_pFunc.GetPtr())
  {
    f.ReportError("Bad particle function name.");
    return false;
  }
  if (!m_pFunc->Load(&f))
  {
    return false;
  }

  return true;
}
}
