/*
Amju Games source code (c) Copyright Jason Colman 2007
*/

#ifndef ES_POOL_ONLINE_ACCEPT_H_INCLUDED
#define ES_POOL_ONLINE_ACCEPT_H_INCLUDED

#include "EsPoolYesNo.h"
#include "GuiButton.h"

namespace Amju
{
// Accept/decline game with another player.
class EsPoolOnlineAccept : public EsPoolYesNo
{
public:
  static const char* Name;

  EsPoolOnlineAccept();
  // Set this player and opponent when known
  void Set(int playerId, int opponentId, const std::string& opponentName, int gameId);

  virtual void OnYes();
  virtual void OnNo();

// We subclass this for Accepting a game in the middle of a one-player game
protected:
  int m_playerId;
  int m_opponentId;
  std::string m_opponentName;
};
}

#endif


