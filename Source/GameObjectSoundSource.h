/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: GameObjectSoundSource.h,v $
Revision 1.1  2004/09/08 15:42:50  jay
Added to repository
  
*/

#if !defined(GAME_OBJECT_SOUND_SOURCE_H_INCLUDED)
#define GAME_OBJECT_SOUND_SOURCE_H_INCLUDED

#include "GameObject.h"
#include "BoundingSphere.h"

namespace Amju
{
// A sound generated when the player approaches the source.
// A sound is played periodically. The volume each time
// depends on the player's position relative to the sound source sphere.
class GameObjectSoundSource : public GameObject
{
public:
  GameObjectSoundSource();

  // Sound sources aren't visible
  virtual void Draw() {}
  virtual const BoundingSphere* GetBoundingSphere() const { return 0; }
  virtual void CreateBoundingSphere(const Matrix& cm) {}
  virtual bool HasLocation() const { return false; }

  virtual void Update();
  virtual bool Load(File*);
  virtual const char* GetTypeName() const;

protected:
  // Radius of sound. The volume of the sound ranges from 0 (at radius
  // and beyond) to 1.0 at centre.
  BoundingSphere m_soundSphere;
  std::string m_wavFile;
  // Time between wavs
  float m_period;
  // Current time between wavs.
  float m_time;

#if defined(SCENE_EDITOR)
public:
  virtual bool Save(File*);
  virtual PropertyMap GetProperties() const;
  virtual void SetProperties(const PropertyMap&);

#endif
};
}

#endif

