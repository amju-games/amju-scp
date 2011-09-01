/*
Amju Games source code (c) Copyright Jason Colman 2007
*/

#ifdef ONLINE_PLAYER_PROFILE

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <iostream>
#include "OnlinePlayerProfile.h"
#include "HttpClient.h"
#include "StringUtils.h"
#include "GameObject.h"
#include "Orientation.h"
#include "Engine.h"
#include "OnlineReqTestable.h"
#include "AmjuTime.h"
#include "StringUtils.h"
#include "GameState.h"
#include "PoolOnlineReqManager.h"
#include "PlayerNames.h"
#include "PoolOnline.h"

#define POOL_ONLINE_DEBUG

// Define this to use test data, not really connect to server
// Not needed any more because we can run the server script locally
// But good to test threading issues!
//#define OFFLINE_TEST

namespace Amju
{
// TODO increment this if the server must treat two client versions 
// differently.
static const int CLIENT_VERSION = 1;

const std::string SCRIPT = "ascp-player.pl";

// Define base class for all requests:
// For offline debugging, use Testable base class.
#ifdef OFFLINE_TEST
class OnlinePlayerProfileBase : public OnlineReqTestable
{
public:
  OnlinePlayerProfileBase(const std::string& url, const char* debugName) :
    OnlineReqTestable(debugName) /* discard url */
  {
  }
};
#else
class OnlinePlayerProfileBase : public OnlineReq
{
public:
  OnlinePlayerProfileBase(const std::string& url, const char* debugName) :
    OnlineReq(url, HttpClient::GET, debugName)
  {
  }
};
#endif

// Class for registering player ID and getting a unique ID from server
class ProfileSendIdReq : public OnlinePlayerProfileBase
{
public:
  ProfileSendIdReq(
    const std::string& url) :
    OnlinePlayerProfileBase(url, "player-PROFILE-send-id")
  {
#ifdef _DEBUG
   std::cout << "********* SENDING ONLINE PROFILE!!! **********\n";
#endif
  }

  virtual void HandleResult()
  {
  }
};

// Class for getting ID from server then sending profile using the ID
class ProfileSendNameReq : public OnlinePlayerProfileBase
{
public:
  ProfileSendNameReq(
    const std::string& url, const std::string& playerName, int won, int lost, int hearts) :
    OnlinePlayerProfileBase(url, "player-PROFILE-send-name"),
    m_playerName(playerName),
    m_won(won),
    m_lost(lost),
    m_hearts(hearts)
  {
#ifdef _DEBUG
   std::cout << "********* SENDING ONLINE PROFILE!!! **********\n";
#endif
  }

  virtual void HandleResult()
  {
    // Extract ID from result. Then send profile
    if (!GetResult().GetSuccess())
    {
      return; 
    }
    PXml xml = ParseXml(GetResult().GetString().c_str());

    // Set name: this is so when we call SetPlayerIdFromXml we will save name/ID in player names
    // NB We don't want to request ID again in SetPlayerName
    ThePoolOnlineManager::Instance()->SetPlayerName(m_playerName, -1, false /* don't request ID */);

    // Parse the XML, setting ID. Sets player name/ID in PlayerNames.
    if (!ThePoolOnlineManager::Instance()->SetPlayerIdFromXml(xml))
    {
      return;
    }

    int id = ThePoolOnlineManager::Instance()->GetPlayerId();
    Assert(id > -1);

    // No need to save ID in player names - done in SetPlayerIdFromXml

    // Send online score
    OnlinePlayerProfile opp;
    opp.SendPlayerInfoWithId(id, m_won, m_lost, m_hearts);
  }

private:
  const std::string m_playerName;
  int m_won;
  int m_lost;
  int m_hearts;
};


OnlinePlayerProfile::OnlinePlayerProfile()
{
  std::string server = Engine::Instance()->GetEngineState()->Get("server");
  if (server == "")
  {
    server = "www.amju.com/cgi-bin/";
  }
  m_serverUrl = server;
}

std::string OnlinePlayerProfile::GetBaseUrl(Task task)
{
  std::string url = m_serverUrl + SCRIPT;

  switch (task)
  {
  case SEND_SCORE_WITH_ID:
    url += "?task=16";   // TODO server side
    break;
  case SEND_SCORE_WITH_NAME:
    url += "?task=15";
    break;
  default:
    Assert(0);
  }

  // Add client version. This lets the server send back data which the client
  // expects, if the format changes.
  url += "&client_ver=";
  url += ToString(CLIENT_VERSION);

  return url;
}

void OnlinePlayerProfile::SetServerUrl(const std::string& url)
{
  m_serverUrl = url;
}

void OnlinePlayerProfile::SendPlayerInfo(
  const std::string& playerName, int won, int lost, int hearts)
{
  // Look up ID for this name - do we have it yet ?
  int id = ThePlayerNames::Instance()->GetId(playerName);
  if (id == -1)
  {
    SendPlayerInfoWithName(playerName, won, lost, hearts);
  }
  else
  {
    SendPlayerInfoWithId(id, won, lost, hearts);
  }
}

void OnlinePlayerProfile::SendPlayerInfoWithId(
  int id, int won, int lost, int hearts)
{
  std::string url = GetBaseUrl(SEND_SCORE_WITH_ID);
  url += "&playerid=";
  url += ToString(id); 
  url += "&won=";
  url += ToString(won);
  url += "&lost=";
  url += ToString(lost);
  url += "&hearts=";
  url += ToString(hearts);

  ProfileSendIdReq* pReq = new ProfileSendIdReq(url);

  // 4 players max, so up to 4 concurrent requests.
  static const int NUM_CONCURRENT_REQS = 4;
  ThePlayerProfileManager::Instance()->AddReq(pReq, NUM_CONCURRENT_REQS);
}

void OnlinePlayerProfile::SendPlayerInfoWithName(
  const std::string& playerName, int won, int lost, int hearts)
{
  // Get ID using PoolOnline URL, but using our own class to handle the result.
  std::string url = ThePoolOnlineManager::Instance()->GetBaseUrl(PoolOnline::REGISTER_PLAYER);
  url += "&playername="; 
  url += playerName; // dodgy characters are handled in HttpClient
  url += "&won=0&lost=0&hearts=0";

  ProfileSendNameReq* pReq = new ProfileSendNameReq(url, playerName, won, lost, hearts);

  // 4 players max, so up to 4 concurrent requests.
  static const int NUM_CONCURRENT_REQS = 4;
  ThePlayerProfileManager::Instance()->AddReq(pReq, NUM_CONCURRENT_REQS);
}

}

#endif // POOL_ONLINE
