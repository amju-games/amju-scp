/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: PlayerNames.h,v $
Revision 1.1.2.4  2007/07/21 23:42:39  Administrator
Store Online ID in player names: only request ID from server when uploading
stats for playing an online game for the first time.

Revision 1.1.2.3  2007/07/21 10:06:06  jay
Add ID to player names

Revision 1.1.2.2  2006/08/14 17:50:21  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:36  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.2  2005/08/26 21:51:33  jay
Add EraseAll()

Revision 1.1.2.1  2005/06/04 22:41:54  jay
Store all player names

*/

#ifndef PLAYER_NAMES_H_INCLUDED
#define PLAYER_NAMES_H_INCLUDED

#include <string>
#include <vector>
#include "Singleton.h"

namespace Amju
{
class PlayerNames
{
public:
  PlayerNames();

  bool Load();
  bool Save();

  // Add a new name - if it exists, add a number to the end to make
  // the name unique ...?
  void AddName(const std::string& name);

  bool NameExists(const std::string& name) const;

  void EraseName(const std::string& name);
  void EraseAll();

  void EditName(const std::string& oldName, const std::string& newName);

  int GetNumNames() const;

  // Get name for index 0..num names
  std::string GetName(int i) const;

  // Get online ID for index
  int GetId(int i) const;

  // Look up name to get online ID
  int GetId(const std::string& playerName) const;

  // Set ID for given player and save file
  void SetId(const std::string& playerName, int id);

protected:
  struct NameIdPair
  {
    NameIdPair() : m_id(-1) {}
    NameIdPair(const NameIdPair& rhs) { m_name = rhs.m_name; m_id = rhs.m_id; }
    NameIdPair(const std::string& name) : m_name(name), m_id(-1) {}
    bool operator==(const std::string& n) const { return m_name == n; }
    std::string m_name;
    int m_id;
  };
  typedef std::vector<NameIdPair> Names;
  Names m_names; 
};
typedef Singleton<PlayerNames> ThePlayerNames;
}

#endif

