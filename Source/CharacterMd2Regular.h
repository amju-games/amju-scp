/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: CharacterMd2Regular.h,v $
Revision 1.1  2004/09/08 15:42:46  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_CHARACTER_MD2_REGULAR_H_INCLUDED)
#define SCHMICKEN_CHARACTER_MD2_REGULAR_H_INCLUDED

#include "CharacterMd2MultiSkin.h"

namespace Amju
{
// This class provides a basic one-skin MD2 character.
// This is so we can test things using off-the-internet MD2 models.
class CharacterMd2Regular : public CharacterMd2MultiSkin
{
public:
  virtual Character* Clone() const;

protected:
  virtual void BindSkin();
  virtual void CreateSkinBinding();
};
}

#endif
