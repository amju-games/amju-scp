/*
Amju Games source code (c) Copyright Jason Colman 2000-2007
$Log: EsPoolDisconnect.h,v $
Revision 1.1.2.2  2007/07/12 20:29:40  jay
Keep polling for opponent's connect time

Revision 1.1.2.1  2007/06/23 11:27:44  jay
Add Error message page for when online opponent goes away

*/

#ifndef ES_POOL_DISCONNECT_H_INCLUDED
#define ES_POOL_DISCONNECT_H_INCLUDED

#include "EsPoolYesNo.h"

namespace Amju
{
class EsPoolDisconnect : public EsPoolYesNo
{
public:
  static const char* Name;

  EsPoolDisconnect();

  virtual void Update();
  virtual void OnYes();
  virtual void OnNo();
};
}

#endif

