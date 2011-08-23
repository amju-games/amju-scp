/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Egg.h,v $
Revision 1.1  2004/09/08 15:42:35  jay
Added to repository
  
*/

#if !defined(EGG_H_INCLUDED)
#define EGG_H_INCLUDED

#include "SolidGameObject.h"
#include "ExplosionEffect.h"
#include "AttribDecorator.h"

namespace Amju
{
class Egg : public SolidGameObject
{
public:
  Egg();

  virtual bool Load(File* pf);
  virtual const char* GetTypeName() const;

  virtual void HandlePlayerCollision(CharacterGameObject* pPlayer);
  virtual void Draw();
  virtual void CullDraw(Frustum*); // for explosion effect
  virtual void Update();
  virtual void DrawShadow() const;
  virtual void SetState(State newState);

  // Returns true if Egg is allowed to regenerate once its chick is OUT_OF_PLAY.
  // This could cause problems if used with an Egg containing more than one chick.
  bool CanRegenerate() const { return m_regenerate; }
  
protected:
  void Hatch();

  // Each egg shows a number when 'hatched'.
  //Number m_number;
 
  // Texture for egg number - numbers don't have a texture built-in, to
  // make them more reuseable.
  AttribDecorator m_numTexture;

  // Particle effect when egg hatches
  ExplosionEffect m_explosion;

  // Numbers spin around y-axis.
  float m_rotate; // current angle
  float m_speed; // current speed of rotation

  static float s_maxRotate; // maximum (initial) rotate vel.
  static float s_rotateDecel; // rotate deceleration - the spinning slows down.

  // IDs of chicks which hatch from this egg.
  // NB more than one chick may hatch from an egg.
  std::vector<int> m_chickIds;

  // If true, egg can regenerate when chick goes OUT_OF_PLAY.
  // Otherwise egg is not reset until Level is reset.
  bool m_regenerate;

#if defined(SCENE_EDITOR)
public:
  virtual bool Save(File* pf);
  virtual PropertyMap GetProperties() const;
  virtual void SetProperties(const PropertyMap&);

#endif
};
}
#endif

