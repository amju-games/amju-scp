/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: PoolPlayerStats.h,v $
Revision 1.1.2.2  2006/08/14 17:50:24  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:39  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.6  2005/10/21 17:10:47  jay
Added Rename function, for when a player name is edited.

Revision 1.1.2.5  2005/09/29 19:12:01  jay
Add reminder that Save is virtual in PlayerStats

Revision 1.1.2.4  2005/09/20 13:17:49  jay
Allow adding any number to a bonus

Revision 1.1.2.3  2005/08/26 21:58:19  jay
Add bonus counts; added Erase()

Revision 1.1.2.2  2005/06/04 22:38:21  jay
Add convenience function to add stats to a player

Revision 1.1.2.1  2005/05/24 20:47:33  jay
New class to store data on all played games for a player.

*/

#ifndef POOL_PLAYER_STATS_H_INCLUDED
#define POOL_PLAYER_STATS_H_INCLUDED

#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include "PlayerStats.h"

namespace Amju
{
class File;

// Store info about all games played by one user.
class PoolPlayerStats : public PlayerStats
{
public:
  // Set name and stats for one player.
  static void SetPlayerStats(const std::string& name, int playerIndex);

public:
  PoolPlayerStats(const std::string& playerName);

  bool Load();
  virtual bool Save();

  enum Result { POOL_RESULT_WIN, POOL_RESULT_LOSE, POOL_RESULT_DRAW };

  void Update(Result r, const std::string& opName, int level, int room);

  void GetStats(int* pWin, int* pLose, int* pDraw);

  // Increment the count for the given bonus type.
  void AddBonus(int type, int num = 1);
  // Get the count for the given bonus type.
  int GetNumBonuses(int type);

  // Erase stats file - when a player name is deleted
  void Erase();

  // Set flag indicating that this room has been solved.
  void SetRoomSolved(int levelId, int roomId, bool solved);
  // Get solved flag for room
  bool GetRoomSolved(int levelId, int roomId);

  // Rename file if player name is edited.
  void Rename(const std::string& newName);

protected:
  // Initialise stats 
  void MakeDefault();

protected:
  std::string m_playerName;
  struct OneGame
  {
    bool Load(File* pf);
    bool Save(File* pf);

    Result r;
    std::string opname;
    int level;
    int room;
  };
  typedef std::vector<OneGame> Games;
  Games m_games;
  // Bonuses collected by player
  // Map of <bonus type, count>
  std::map<int, int> m_numBonuses;

  // Level/room for solved rooms
  typedef std::set<std::pair<int, int> > SolvedRooms;
  SolvedRooms m_solvedRooms;
};
}

#endif


