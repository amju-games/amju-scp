/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: PowerUpHealth.h,v $
Revision 1.1  2004/09/08 15:42:41  jay
Added to repository
  
*/

#if !defined(POWER_UP_HEALTH_H_INCLUDED)
#define POWER_UP_HEALTH_H_INCLUDED

#include "SpecialRecipient.h"

namespace Amju
{
// This power-up restores health (up to current player health max)
class PowerUpHealth : public SpecialRecipient
{
public:
  virtual void ReceiveMessage(const Message& m);
};
}

#endif

