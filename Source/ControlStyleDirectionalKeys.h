/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ControlStyleDirectionalKeys.h,v $
Revision 1.1  2004/09/08 15:42:35  jay
Added to repository
  
*/

#if !defined(CONTROL_STYLE_DIRECTIONAL_KEYS_H_INCLUDED)
#define CONTROL_STYLE_DIRECTIONAL_KEYS_H_INCLUDED

//#include "ControlStyleMovePlayerBase.h"
#include "ControlStyleRotateKeys.h"

namespace Amju
{
// The idea of this control style is that when you proess 'left' the
// player character immediately moves left, rather than rotating.
// Don'y know if it will be usable... especially as the camera moves
// freely, so 'left' means the player character's left.

// This control style should be paired with a 'fixed' camera, which
// doesn't rotate around the player.

class ControlStyleDirectionalKeys: public ControlStyleRotateKeys  
//ControlStyleMovePlayerBase
{
public:
  ControlStyleDirectionalKeys();

  //virtual void Blue(bool);
  virtual void PlusUp(bool);
  virtual void PlusDown(bool);
  virtual void PlusLeft(bool);
  virtual void PlusRight(bool);

  //virtual void Update();
  //virtual void Reset();

protected:
  void CommonKey(bool);
};
}


#endif

