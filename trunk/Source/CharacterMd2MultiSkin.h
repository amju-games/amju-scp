/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: CharacterMd2MultiSkin.h,v $
Revision 1.1  2004/09/08 15:42:46  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_CHARACTER_MD2_MULTI_SKIN_H_INCLUDED)
#define SCHMICKEN_CHARACTER_MD2_MULTI_SKIN_H_INCLUDED

#include <vector>
#include "CharacterMd2.h"

namespace Amju
{
// This is the base class for all MD2 Character types with more than
// one skin.
class CharacterMd2MultiSkin : public CharacterMd2
{
public:
  virtual Character* Clone() const = 0;
  virtual bool Load(File*);

protected:
  // Override BindSkin() in subclasses to bind the appropriate skin each frame.
  virtual void BindSkin() = 0;

  // Override CreateSkinBinding() to create the bindings - this is called once
  // the skins have been loaded.
  virtual void CreateSkinBinding() = 0;

protected:
  typedef std::vector<PoolTexture*> Skins;
  Skins m_skins;
};
}

#endif
