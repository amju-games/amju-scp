/*
Amju Games source code (c) Copyright Jason Colman 2005
*/

#ifndef ONLINE_PLAYER_PROFILE_H_INCLUDED
#define ONLINE_PLAYER_PROFILE_H_INCLUDED

#ifdef ONLINE_PLAYER_PROFILE

#include <string>

namespace Amju
{
class OnlinePlayerProfile
{
public:
  OnlinePlayerProfile();

  // Only call if necessary
  void SetServerUrl(const std::string& url);

  void SendPlayerInfo(const std::string& playerName, int won, int lost, int hearts);

protected:
  enum Task { SEND_SCORE_WITH_NAME, SEND_SCORE_WITH_ID };
  std::string GetBaseUrl(Task task);

  void SendPlayerInfoWithName(
    const std::string& playerName, int won, int lost, int hearts);

public:
  void SendPlayerInfoWithId(int id, int won, int lost, int hearts);

protected:
  std::string m_serverUrl;
};
}

#endif // ONLINE_PLAYER_PROFILE

#endif // include guard

