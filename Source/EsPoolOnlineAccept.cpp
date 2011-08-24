/*
Amju Games source code (c) Copyright Jason Colman 2007
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "EsPoolOnlineAccept.h"
#include "Engine.h"
#include "PoolOnline.h"
#include "EngineStatePoolOnlineTable.h"
#include "StringUtils.h"

namespace Amju
{
const char* EsPoolOnlineAccept::Name = "pool-online-accept";
static const bool registered = Engine::Instance()->
  RegisterEngineState(EsPoolOnlineAccept::Name, 
  new EsPoolOnlineAccept);

EsPoolOnlineAccept::EsPoolOnlineAccept()
{
  m_yesFilename = "pool-accept-button.txt";
  m_noFilename = "pool-decline-button.txt";
}

void EsPoolOnlineAccept::Set(int playerId, int opponentId, const std::string& opponentName, int gameId)
{
  m_question.clear();

  m_playerId = playerId;
  m_opponentId = opponentId;
  m_opponentName = opponentName;
  Assert(m_opponentName.size() > 0);
  m_opponentName[0] = toupper(m_opponentName[0]);

  std::string s = m_opponentName + " would like to play a game with you!";
#ifdef POOL_ONLINE_BETA_TEST
  s += " Game ID: " + ToString(gameId);
#endif

  AddQ(s);
  AddQ("Would you like to play ?");
}

void EsPoolOnlineAccept::OnYes()
{
#ifdef POOL_ONLINE
  // Accept join request
  ThePoolOnlineManager::Instance()->SendGameJoinAccepted();
  EngineStatePoolOnlineTable::StartOnlineGame(m_playerId, m_opponentId, true /* we choose game */);
#endif
}

void EsPoolOnlineAccept::OnNo()
{
#ifdef POOL_ONLINE
  // Go back to EsOnlineTable - but send request to server to update 
  //  the game between us and opponent.
  // The opponent should be removed from the game row. This will tell the opponent 
  //  that his join request was declined (could also be that another player joined)
  ThePoolOnlineManager::Instance()->SendGameJoinDeclined();

  Engine::Instance()->ChangeState(EngineStatePoolOnlineTable::Name,
    Engine::IMMEDIATE);
#endif
}
}
