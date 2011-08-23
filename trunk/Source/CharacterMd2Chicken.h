/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: CharacterMd2Chicken.h,v $
Revision 1.1  2004/09/08 15:42:34  jay
Added to repository
  
*/

#include "CharacterMd2MultiSkin.h"

namespace Amju
{
// Specialises MD2 character class for Chicken character
class CharacterMd2Chicken : public CharacterMd2MultiSkin
{
public:
  virtual Character* Clone() const;

protected:
  // Override to do blinking by swapping skins.
  virtual void BindSkin();

  // Bind the texture - called during loading. Override to create binding for both
  // textures.
  virtual void CreateSkinBinding();
};
}

