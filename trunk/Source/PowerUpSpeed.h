/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: PowerUpSpeed.h,v $
Revision 1.1  2004/09/08 15:42:41  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_POWER_UP_SPEED_H_INCLUDED)
#define SCHMICKEN_POWER_UP_SPEED_H_INCLUDED

#include "SpecialRecipient.h"

namespace Amju
{
// This power-up speeds up player.
class PowerUpSpeed : public SpecialRecipient
{
public:
  virtual void ReceiveMessage(const Message& m);
};
}

#endif

