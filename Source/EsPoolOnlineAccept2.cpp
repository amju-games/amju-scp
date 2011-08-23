/*
Amju Games source code (c) Copyright Jason Colman 2007
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "EsPoolOnlineAccept2.h"
#include "Engine.h"
#include "PoolOnline.h"
#include "EngineStatePoolSetUpShot.h"
#include "EngineStatePoolOnlineTable.h"
#include "PoolMisc.h"

namespace Amju
{
const char* EsPoolOnlineAccept2::Name = "pool-online-accept-2";
static const bool registered = Engine::Instance()->
  RegisterEngineState(EsPoolOnlineAccept2::Name,
  new EsPoolOnlineAccept2);

void EsPoolOnlineAccept2::OnYes()
{
  // Accept join request

  // TODO Clean up current one-player game

  ThePoolOnlineManager::Instance()->SendGameJoinAccepted();
  SetIsOnlineGame(true);

// TODO Do we need to do this somewhere ?
  StartGame(2, false // not friendly
   );

  EngineStatePoolOnlineTable::StartOnlineGame(m_playerId, m_opponentId, true /* we choose game */);
}

void EsPoolOnlineAccept2::OnNo()
{
  // Send request to server to update the game between us and opponent.
  // The opponent should be removed from the game row. This will tell the opponent 
  //  that his join request was declined (could also be that another player joined)
  ThePoolOnlineManager::Instance()->SendGameJoinDeclined();

  // Return to one-player game  
  Engine::Instance()->ChangeState(EngineStatePoolSetUpShot::Name,
    Engine::IMMEDIATE);
}
}


