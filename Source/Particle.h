/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Particle.h,v $
Revision 1.1  2004/09/08 15:42:54  jay
Added to repository
  
*/

#if !defined(PARTICLE_H_INCLUDED)
#define PARTICLE_H_INCLUDED

#include "SolidComponent.h"

namespace Amju
{
class Particle 
{
public:
  Particle() {}
  virtual ~Particle() {}

  virtual void Draw();
  virtual bool Load(const std::string& filename);

  virtual Particle* Clone();

  virtual void SetTime(float t);
};
}
#endif

