/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: PoolGameState.cpp,v $
Revision 1.1.4.1.2.7  2006/08/27 19:18:24  jay
Added Server URL; don't check checksum, it just makes life inconvenient

Revision 1.1.4.1.2.6  2006/05/12 07:53:39  jay
Save after setting float value

Revision 1.1.4.1.2.5  2006/03/18 23:23:52  Administrator
Initially windowed

Revision 1.1.4.1.2.4  2005/09/02 18:15:50  jay
Remove hashing code which used srand() - it made rand() totally predictable

Revision 1.1.4.1.2.3  2005/07/30 12:13:45  jay
Added SetPlayerInfo()

Revision 1.1.4.1.2.2  2005/05/24 20:35:28  jay
Add Player Stats for pool, and friendly games.

Revision 1.1.4.1.2.1  2005/03/31 22:22:18  jay
Added more player state for Pool game.

Revision 1.1.4.1  2004/11/09 16:03:52  jay
Merged fullscreen code from XING branch.

Revision 1.1.6.1  2004/11/08 13:58:59  jay
Added full screen flag (on by default)

Revision 1.1  2004/09/08 15:42:51  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "PoolGameState.h"
#include "StringUtils.h"
#include "Directory.h"
#include "File.h"
#include "SchAssert.h"

using namespace std;

namespace Amju
{
void ReportError(const std::string&);

static const std::string CHECKSUM = "check";

extern const char* APPLICATION_NAME;

PoolGameState::PoolGameState()
{
  m_currentPlayerId = 0;
  m_isFriendly = true;
}

PoolGameState::~PoolGameState()
{
}

void PoolGameState::Init()
{ 
  Set("control_style", "rkeys");
  Set("camera_style", "follow");
  Set("fullscreen", "n");

  // Online game server: normally www.amju.com/cgi-bin/
  // Can be localhost/ascp/ for testing on localhost.
  Set("server", "www.amju.com/cgi-bin/");
}

void PoolGameState::Set(const std::string& key, const std::string& value)
{
  m_cf.Set(key, value);
  Save();
}

void PoolGameState::Set(const std::string& key, float f)
{
  m_cf.Set(key, ToString(f));
  Save(); // we want to save after setting a float -- probably not
    // done before because of infinite recursion ?
}

std::string PoolGameState::Get(const std::string& key)
{
  return m_cf.GetValue(key);
}

float PoolGameState::GetFloat(const std::string& key)
{
  return (float)atof(m_cf.GetValue(key).c_str());
}

bool PoolGameState::Load()
{ 
  // GetSaveDir() (in Directory) gets a user-preferences
  // directory where it's ok to save stuff to.
  std::string root = File::GetRoot();
  File::SetRoot("", "/");
  std::string filename = GetSaveDir(APPLICATION_NAME) + m_filename;
  bool b = m_cf.Load(filename);
  File::SetRoot(root, "/");

  if (!b)
  {
    Init();
    return false;
  }
  //float cs = GetFloat(CHECKSUM);
  m_cf.Erase(CHECKSUM);
/*
  // Don't check 
  if (cs != Checksum())
  {
    // Check failed.
    std::string err = "Checksum failed for config file ";
    err += m_filename;
    ReportError(err);
    Init();
    m_cf.Clear();
    return false;
  }
*/

  return true;
}

bool PoolGameState::Save()
{
  m_cf.Erase(CHECKSUM);
  float cs = (float)Checksum();
  // Don't use Set(), it will call Save()...
  m_cf.Set(CHECKSUM, ToString(cs));

  // GetSaveDir() (in Directory) gets a user-preferences
  // directory where it's ok to save stuff to.
  std::string filename = GetSaveDir(APPLICATION_NAME) + m_filename;
  std::string root = File::GetRoot();
  File::SetRoot("", "/");
  bool b = m_cf.Save(filename);
  File::SetRoot(root, "/");
  return b;
}

int PoolGameState::Checksum()
{
  int c = m_cf.GetChecksum(); // get all the characters added up.
  return c;
}

PoolGameState::PlayerInfo::PlayerInfo(
  int id, const std::string& name, const Orientation& o, bool isAi) :
  m_golfStroke(0, 0, 0, 0, 0, 0)
{
  m_id = id;
  m_name = name;
  m_or = o;
  m_isAi = isAi;
  m_reset = true;
  m_isPlaying = true;
  m_score = 0;
  m_numBalls = 0;
  m_isHighPower = false;
  m_totalStrokes = 0;
  m_totalBonuses = 0;
  m_animalsHit = 0;
  m_blockersMoved = 0;
  m_isOnline = false;
}

PoolGameState::PlayerInfo* PoolGameState::GetCurrentPlayerInfo()
{
  Assert(!m_playersInfo.empty());
  Assert(m_currentPlayerId >= 0);
  Assert(m_currentPlayerId < (int)m_playersInfo.size());
  return &(m_playersInfo[m_currentPlayerId]);
}

PoolGameState::PlayerInfo* PoolGameState::GetPlayerInfo(int index)
{
  Assert(!m_playersInfo.empty());
  Assert(index >= 0);
  Assert(index < (int)m_playersInfo.size());
  return &(m_playersInfo[index]);
}

void PoolGameState::SetPlayerInfo(int index, const PoolGameState::PlayerInfo& info)
{
  Assert(!m_playersInfo.empty());
  Assert(index >= 0);
  Assert(index < (int)m_playersInfo.size());
  m_playersInfo[index] = info;
}

int PoolGameState::RotateToNextPlayer()
{
  m_currentPlayerId++;
  if (m_currentPlayerId == (int)m_playersInfo.size())
  {
    m_currentPlayerId = 0;
  }
  return m_currentPlayerId;
}

void PoolGameState::AddPlayerInfo(const PlayerInfo& pi)
{
  m_playersInfo.push_back(pi);
}

void PoolGameState::ClearPlayers()
{
  m_playersInfo.clear();
}

void PoolGameState::SetCurrentPlayer(int index)
{
  m_currentPlayerId = index;
}

int PoolGameState::GetCurrentPlayer() const
{
  return m_currentPlayerId;
}

int PoolGameState::GetNumberOfPlayers() const
{
  return m_playersInfo.size();
}

void PoolGameState::SetIsFriendly(bool f)
{
  m_isFriendly = f;
}

bool PoolGameState::IsFriendly() const
{
  return m_isFriendly;
}
}

