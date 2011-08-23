/*
Amju Games source code (c) Copyright Jason Colman 2007
*/

#ifndef ES_POOL_ONLINE_ACCEPT_2_H_INCLUDED
#define ES_POOL_ONLINE_ACCEPT_2_H_INCLUDED

#include "EsPoolOnlineAccept.h"

namespace Amju
{
// Show accept/decline screen when we get an invite when we are playing
// a one-player game.
// This is a different class to when we get an invite when in Table mode
// because we must clean up the current one-player game, or return to it.
class EsPoolOnlineAccept2 : public EsPoolOnlineAccept
{
public:
  static const char* Name;

  virtual void OnYes();
  virtual void OnNo();
};
}

#endif

