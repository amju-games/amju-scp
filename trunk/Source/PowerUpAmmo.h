/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: PowerUpAmmo.h,v $
Revision 1.1  2004/09/08 15:42:40  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_POWER_UP_AMMO_H_INCLUDED)
#define SCHMICKEN_POWER_UP_AMMO_H_INCLUDED

#include "SpecialRecipient.h"

namespace Amju
{
// PowerUpAmmo: grants player ammo until time limit expires.
// (Rather than a certain number of bullets.)
class PowerUpAmmo : public SpecialRecipient
{
public:
  virtual void ReceiveMessage(const Message& m);
};
}

#endif

