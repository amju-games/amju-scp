/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: PowerUpServer.h,v $
Revision 1.1  2004/09/08 15:42:55  jay
Added to repository
  
*/

#if !defined(POWER_UP_SERVER_H_INCLUDED)
#define POWER_UP_SERVER_H_INCLUDED

#include "SpecialRecipient.h"
#include "PowerGuage.h"

namespace Amju
{
// Gets messages when player earns a power up.
// The behaviour for the message is registered by the power-up ?
class PowerUpServer : public SpecialRecipient
{
public:
  virtual void ReceiveMessage(const Message& m);
  virtual void DrawOverlays();
  virtual bool Load();

private:
  PowerGuage m_guage;
};
}

#endif

