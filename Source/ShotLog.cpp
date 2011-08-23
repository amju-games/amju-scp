/*
Amju Games source code (c) Copyright Jason Colman 2007
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "ShotLog.h"
#include "LogFile.h"
#include "GameState.h"
#include "Engine.h"
#include "PoolOnline.h"

namespace Amju
{
#if defined(POOL_ONLINE) && defined(WRITE_SHOT_LOG)
void ShotLog(const std::string& s)
{
  LogFile f;

  std::string filename = "shotlog-";
  filename += ThePoolOnlineManager::Instance()->GetPlayerName();
    //Engine::Instance()->GetGameState()->GetPlayerInfo(0)->m_name;
  filename += ".txt";

  f.SetName(filename);
  f.Log(s, false /* no time, for diffing */);
}
#endif
}


