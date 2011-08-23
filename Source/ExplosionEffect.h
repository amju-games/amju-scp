/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ExplosionEffect.h,v $
Revision 1.1.10.1  2006/04/11 17:00:20  jay
Add Kill() to immediately kill effect

Revision 1.1  2004/09/08 15:42:38  jay
Added to repository
  
*/

#if !defined(EXPLOSION_EFFECT_H_INCLUDED)
#define EXPLOSION_EFFECT_H_INCLUDED

#include "Effect.h"
//#include "Body.h"
#include "Vertex.h"
#include "SolidComponent.h"
#include "VisibleGameObject.h"

namespace Amju
{
// One particle in the effect, which should be like a shower of sparks.
// Each spark fades to invisible - at a different rate ?
// The sparks fall to the floor, and ideally will bounce when they hit
// the ground.
class ExplosionParticle //: public Body
{
public:
  ExplosionParticle(SolidComponent*);
  void Draw();

  // Restart the effect
  void Reset(const VertexBase& centre);

  // Kill the effect - make the particle dead
  void Kill();

protected:
  float m_velx, m_vely, m_velz;
  VertexBase m_centre;
  // Each particle looks the same - so we only want one SolidComponent object
  // per explosion effect (other explosion objects may use different images).
  PSolidComponent m_pBb; 
  float m_maxTime; // particle remains alive for this long
  float m_time; // current time value
  bool m_isDead;
};

// ExplosionEffect - triggered when player hits something.
// Not sure if we should inherit from Vis.G.O. - but having a HeightServer
// means the particles can correctly bounce off the floor, etc.
class ExplosionEffect : public Effect
{
public:
  ExplosionEffect();

  virtual bool Load(const std::string& filename);

  // Create the effect: number of particles, the BillBoard/Solid used for all particles 
  // in the effect, the max speed for each particle, max fadeout time for each
  // particle. 
  virtual void Init(int numParticles, PSolidComponent pBb, float maxSpeed, float maxTime);

  virtual void Draw();

  // Reset the particles, giving the new explosion centre.
  // Each particle is assigned a random velocity, with max speed as set in ctor.
  virtual void Reset(const VertexBase& centre);

  // Kill the effect - make all the particles dead
  void Kill();

protected:
  typedef std::vector<ExplosionParticle> ParticleList;
  ParticleList m_particles;
};

// A special kind of explosion effect, where each particle gets set to a
// different colour in Init().
class ExplosionEffectMulticolour : public ExplosionEffect
{
public:
  virtual void Init(int numParticles, PSolidComponent pBb, float maxSpeed, float maxTime);
};
}
#endif
