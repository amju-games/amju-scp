/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Effect.h,v $
Revision 1.1  2004/09/08 15:42:48  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_EFFECT_H_INCLUDED)
#define SCHMICKEN_EFFECT_H_INCLUDED

#include "SharedPtr.h"
#include <Vec3.h>
#include <string>
#include <vector>

namespace Amju
{
class Effect : public Shareable
{
public:
  virtual ~Effect() {}

  virtual bool Load(const std::string& filename) = 0;

  virtual void Draw() = 0;

  // Reset the effect, giving the centre coord of the effect.
  virtual void Reset(const Vec3f& centre) = 0;

public:
  std::string GetName() const { return m_name; }
  void SetName(const std::string& name) { m_name = name; }

protected:
  std::string m_name;
};

typedef SharedPtr<Effect> PEffect;

typedef std::vector<PEffect> EffectVector;
}

#endif

