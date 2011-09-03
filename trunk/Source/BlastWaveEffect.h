/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: BlastWaveEffect.h,v $
Revision 1.1  2004/09/08 15:42:33  jay
Added to repository
  
*/

#if !defined(BLAST_WAVE_EFFECT_H_INCLUDED)
#define BLAST_WAVE_EFFECT_H_INCLUDED

#include "Effect.h"
#include "Vertex.h"

namespace Amju
{
class PoolTexture;

// Expanding textured quad, which should look like a blast wave 
// radiating from a point.
// TODO set angle - horizontal, vertical, etc.
class BlastWaveEffect : public Effect
{
public:
  BlastWaveEffect();

  void Init(PoolTexture* pBb, float speed, float maxTime, float xtilt, float ztilt);

  virtual bool Load(const std::string& filename);
  virtual void Reset(const VertexBase& centre);
  virtual void Draw();

  bool IsDead() const;

protected:
  PoolTexture* m_pTexture;
  float m_speed;
  float m_maxTime;
  float m_time;
  float m_size;
  bool m_dead;
  VertexBase m_centre;
  float m_xtilt, m_ztilt; // rotation about x and z axes
};
}
#endif
