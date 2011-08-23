/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Fire.h,v $
Revision 1.1  2004/09/08 15:42:49  jay
Added to repository
  
*/

#if !defined(FIRE_H_INCLUDED)
#define FIRE_H_INCLUDED

#include "BillBoard.h"
#include "TextureModFire.h"

namespace Amju
{
class Texture;
// A specialised billboard, which uses the Fire texture modifier.
// This is in code so we have control over the texture modifiers.
// In a room full of fires, we don't want to be generating each
// fire separately.
class Fire : public BillBoard
{
public:
  Fire();
  virtual void Draw();
  virtual bool Load(const std::string& filename);
  virtual void Update();

protected:
  // This object changes the texture at run time to get a fire effect.
  TextureModFire m_fireTextureModifier; 
};
}

#endif

