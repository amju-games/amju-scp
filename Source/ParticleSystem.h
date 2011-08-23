/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ParticleSystem.h,v $
Revision 1.1  2004/09/08 15:42:54  jay
Added to repository
  
*/

#if !defined(PARTICLE_SYSTEM_H_INCLUDED)
#define PARTICLE_SYSTEM_H_INCLUDED

// ParticleSystem.h

// A ParticleSystem is a collection of moving Composites.
// Patterns: Decorator
// A ParticleSystem is itself a Composite.

#include "SolidComponent.h"
#include "Particle.h"
#include "Function.h"
#include <vector>

namespace Amju
{
class ParticleSystem : public SolidComponent
{
  typedef SolidComponent Particle;

public:
  ParticleSystem();
  virtual ~ParticleSystem();

  virtual void Draw();
  virtual int CullDraw(const Frustum& f);
  virtual bool Load(const std::string& filename);
  virtual int GetNumberOfPolygons() const;

protected:
  // Owns a collection of composites. 
  CompVector m_particles;
  
  // Points to function which gets the next orientation for every particle.
  PFunction m_pFunc;
};
}
#endif

