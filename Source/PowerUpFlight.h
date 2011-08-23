/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: PowerUpFlight.h,v $
Revision 1.1  2004/09/08 15:42:40  jay
Added to repository
  
*/

#if !defined(POWER_UP_FLIGHT_H_INCLUDED)
#define POWER_UP_FLIGHT_H_INCLUDED

#include "SpecialRecipient.h"

namespace Amju
{
class PowerUpFlight : public SpecialRecipient
{
public:
  virtual void ReceiveMessage(const Message& m);
};
}

#endif

