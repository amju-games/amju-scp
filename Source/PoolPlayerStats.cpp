/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: PoolPlayerStats.cpp,v $
Revision 1.1.2.4  2007/07/08 19:50:01  Administrator
Only send player info when we save player stats - so we don't send info
for all players (bumping their last connection time)

Revision 1.1.2.3  2006/08/30 21:12:20  Administrator
Get Pool to build in MSVC

Revision 1.1.2.2  2006/08/14 17:50:23  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:39  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.7  2005/10/21 17:10:47  jay
Added Rename function, for when a player name is edited.

Revision 1.1.2.6  2005/09/20 13:17:49  jay
Allow adding any number to a bonus

Revision 1.1.2.5  2005/09/13 23:08:46  jay
Added simple checksum to hinder cheating

Revision 1.1.2.4  2005/09/05 20:07:52  jay
Turn off debug output

Revision 1.1.2.3  2005/08/26 21:58:18  jay
Add bonus counts; added Erase()

Revision 1.1.2.2  2005/06/04 22:38:21  jay
Add convenience function to add stats to a player

Revision 1.1.2.1  2005/05/24 20:47:31  jay
New class to store data on all played games for a player.

*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "PoolPlayerStats.h"
#include "File.h"
#include "Directory.h"
#include "Engine.h"
#include "GameState.h"
#include "OnlinePlayerProfile.h"

namespace Amju
{
void PoolPlayerStats::SetPlayerStats(const std::string& name, int playerIndex)
{
  std::string capitalised = name;
  capitalised[0] = toupper(capitalised[0]);

  Engine::Instance()->GetGameState()->GetPlayerInfo(playerIndex)->m_name = capitalised;
  // Now we have the player names we can get the player stats.
  PoolPlayerStats* pStats = new PoolPlayerStats(name);
  if (pStats->Load())
  {
    Engine::Instance()->GetGameState()->GetPlayerInfo(playerIndex)->SetPlayerStats(pStats);
  }
  else
  {
#ifdef STATS_DEBUG
std::cout << "POOL PLAYER STATS: failed to load for " << name.c_str() << "\n";
#endif
    // TODO ..show error message to user ?
    // Initialise with new stats anyway
    Engine::Instance()->GetGameState()->GetPlayerInfo(playerIndex)->SetPlayerStats(pStats);
  }
}

PoolPlayerStats::PoolPlayerStats(const std::string& playerName) :
  m_playerName(playerName)
{
}

void PoolPlayerStats::MakeDefault()
{
  m_games.clear(); 
  m_numBonuses.clear();
}

bool PoolPlayerStats::Load()
{
  File f(true, File::STD); // yes has version info, no to glue file.

  std::string root = File::GetRoot();
  File::SetRoot(GetSaveDir(), "/");
  // TODO check for bad characters in name ?
  std::string s = m_playerName;
  s += ".cfg";
 
  if (!f.OpenRead(s))
  {
    // Make default hi score table and return.
    MakeDefault();
    File::SetRoot(root, "/");
    return true;
  }
  File::SetRoot(root, "/");

  int c = 0; // checksum
 
  // TODO Load data 
  // Get number of saved games
  int numGames = 0;
  if (!f.GetInteger(&numGames))
  {
    MakeDefault();
    return true;
  }
  c ^= numGames;

  for (int i = 0; i < numGames; i++)
  {
    OneGame g;
    if (!g.Load(&f))
    {
      MakeDefault();
      return true;
    }
    m_games.push_back(g);
  }
  // Get number of each type of bonus collected
  int numBonusTypes = 0;
  if (!f.GetInteger(&numBonusTypes))
  {
    MakeDefault();
    return true;
  }
  for (int j = 0; j < numBonusTypes; j++)
  {
    int k, v;
    if (!f.GetInteger(&k) || !f.GetInteger(&v))
    {
      MakeDefault();
      return true;
    }
    m_numBonuses[k] = v;
    c ^= v;
  }

  int numSolvedRooms = 0;
  if (f.GetInteger(&numSolvedRooms))
  {
    for (int i = 0; i < numSolvedRooms; i++)
    {
      int a, b;
      f.GetInteger(&a);
      f.GetInteger(&b);
      m_solvedRooms.insert(std::make_pair(a, b));
    }
  }

  // End of loading data
 
  int check = 0;
  if (!f.GetInteger(&check))
  {
#ifdef _DEBUG
std::cout << "PLAYER STATS CHECKSUM MISSING\n";
#endif

    MakeDefault();
    return true;
  }
  if (check != c)
  {
#ifdef _DEBUG
std::cout << "PLAYER STATS CHECKSUM FAILED! check: " 
    << check << " expected: " << c << "\n";
#endif

    MakeDefault();
    return true;
  }

  return true;
}

void PoolPlayerStats::Rename(const std::string& newName)
{
  m_playerName = newName;
  Save();
}

bool PoolPlayerStats::Save()
{
#ifdef STATS_DEBUG
std::cout << "POOL PLAYER STATS: saving stats for " << m_playerName.c_str() << "\n";
#endif

  int check = 0;
  File f;
  std::string root = File::GetRoot();
  File::SetRoot(GetSaveDir(), "/");
  std::string s = m_playerName;
  s += ".cfg";
  if (!f.OpenWrite(s))
  {
    File::SetRoot(root, "/");
    return false;
  }
  File::SetRoot(root, "/");

  // TODO Write data
  int numGames = (int)m_games.size();
  check ^= numGames;
  f.WriteInteger(numGames);
  for (unsigned int i = 0; i < m_games.size(); i++)
  {
    OneGame g = m_games[i];
    g.Save(&f);
  }

  f.WriteInteger(m_numBonuses.size());
  for (std::map<int, int>::iterator it = m_numBonuses.begin(); 
       it != m_numBonuses.end(); ++it)
  {
    f.WriteInteger(it->first);
    int v = it->second;
    check ^= v;
    f.WriteInteger(v);
  }

  f.WriteInteger(m_solvedRooms.size());
  for (SolvedRooms::iterator jt = m_solvedRooms.begin(); 
       jt != m_solvedRooms.end(); ++jt)
  {
    f.WriteInteger(jt->first);
    f.WriteInteger(jt->second);
  }

  f.WriteInteger(check);

  File::SetRoot(root, "/");

#ifdef ONLINE_PLAYER_PROFILE
  // Send hi scores to web site.
  OnlinePlayerProfile playerProfile;
  int won, lost, d;
  GetStats(&won, &lost, &d);
  int hearts = GetNumBonuses(1);
  playerProfile.SendPlayerInfo(m_playerName, won, lost, hearts);
#endif

  return true;
}

void PoolPlayerStats::Erase()
{
/*
  std::string f = GetSaveDir();
  f += m_playerName;
  f += ".cfg";
#ifdef STATS_DEBUG
std::cout << "Deleting file: " << f.c_str() << "\n";
#endif

  unlink(f.c_str());
*/

#ifdef STATS_DEBUG
std::cout << "ERASING ALL DATA FOR " << m_playerName.c_str() << "\n";
#endif

  // Trash data, but keep file
  m_games.clear();
  m_numBonuses.clear();
  Save();
}

void PoolPlayerStats::Update(PoolPlayerStats::Result r, const std::string& opname, int level, int room)
{
  OneGame g;
  g.r = r;
  g.opname = opname;
  g.level = level;
  g.room = room;
  m_games.push_back(g);
}

void PoolPlayerStats::GetStats(int* pWin, int* pLose, int* pDraw)
{
  int win = 0;
  int lose = 0;
  int draw = 0;
  int played = 0;
  for (unsigned int i = 0; i < m_games.size(); i++)
  {
    played++;
    const OneGame& g = m_games[i];
    switch (g.r)
    {
    case POOL_RESULT_WIN:
      ++win;
      break;
    case POOL_RESULT_LOSE:
      ++lose;
      break;
    default:
      ++draw;
    }
  }
  *pWin = win;
  *pLose = lose;
  *pDraw = draw;
}

bool PoolPlayerStats::OneGame::Load(File* pf)
{
  if (!pf->GetInteger((int*)&r))
  {
    return false;
  }
  if (!pf->GetDataLine(&opname))
  {
    return false;
  }
  if (!pf->GetInteger(&level))
  { 
    return false;
  }
  if (!pf->GetInteger(&room))
  {
    return false;
  }
  return true;
}

bool PoolPlayerStats::OneGame::Save(File* pf)
{
  pf->WriteInteger(r);
  pf->Write(opname);
  pf->WriteInteger(level);
  pf->WriteInteger(room);
  return true;
}

void PoolPlayerStats::AddBonus(int type, int num)
{
  m_numBonuses[type] += num;
  // Don't save - wait till the end of the game.
}

int PoolPlayerStats::GetNumBonuses(int type)
{
  return m_numBonuses[type];
}

void PoolPlayerStats::SetRoomSolved(int levelId, int roomId, bool solved)
{
  if (solved)
  {
    m_solvedRooms.insert(std::make_pair(levelId, roomId));
  }
  else
  {
    m_solvedRooms.erase(std::make_pair(levelId, roomId));
  }
}

bool PoolPlayerStats::GetRoomSolved(int levelId, int roomId)
{
  bool b = (m_solvedRooms.find(std::make_pair(levelId, roomId)) != m_solvedRooms.end());
  return b;
}
}


