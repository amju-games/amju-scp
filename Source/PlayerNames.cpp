/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: PlayerNames.cpp,v $
Revision 1.1.2.5  2007/07/21 23:42:39  Administrator
Store Online ID in player names: only request ID from server when uploading
stats for playing an online game for the first time.

Revision 1.1.2.4  2007/07/21 10:25:45  Administrator
Fix rename player: stats were trashed! Load the stats before saving the
new file.
 + MSVC warning

Revision 1.1.2.3  2007/07/21 10:06:06  jay
Add ID to player names

Revision 1.1.2.2  2006/08/14 17:50:21  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:36  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.3  2005/10/21 17:10:13  jay
Filled in code for Edit and Erase functions

Revision 1.1.2.2  2005/08/26 21:51:33  jay
Add EraseAll()

Revision 1.1.2.1  2005/06/04 22:41:54  jay
Store all player names

*/

#if defined(MSVC)
#pragma warning(disable: 4786)
#endif

#include <algorithm>
#include <iostream>
#include "PlayerNames.h"
#include "PoolPlayerStats.h"
#include "File.h"
#include "Directory.h"
#include "SchAssert.h"

namespace Amju
{
extern const char* APPLICATION_NAME;

PlayerNames::PlayerNames()
{
}

bool PlayerNames::Load()
{
  m_names.clear();

  // Get previously entered player names.
  File f(true, File::STD); // yes has version info, no to glue file.
  std::string root = File::GetRoot();
  File::SetRoot(GetSaveDir(APPLICATION_NAME), "/");
  if (f.OpenRead("playernames.cfg"))
  {
    // Get number of names
    int numNames = 0;
    if (f.GetInteger(&numNames))
    {
      std::string s;
      int i;
      for (i = 0; i < numNames; i++)
      {
        if (f.GetDataLine(&s))
        {
#ifdef _DEBUG
std::cout << "PLAYER NAMES: Loading name " << s.c_str() << "\n";
#endif
          m_names.push_back(s);
        }
      }
      // Load IDs - may fail for backward compatibility
      for (i = 0; i < numNames; i++)
      {
        int id = -1;
        f.GetInteger(&id);
        m_names[i].m_id = id;
      }
    }
  }
  File::SetRoot(root, "/");
  return true;
}

bool PlayerNames::Save()
{
#ifdef _DEBUG
std::cout << "*** Save player names\n";
#endif

  File f(true, File::STD); // yes has version info, no to glue file.
  std::string root = File::GetRoot();
  File::SetRoot(GetSaveDir(APPLICATION_NAME), "/");
  if (f.OpenWrite("playernames.cfg"))
  {
    int size = m_names.size();
    f.WriteInteger(size);
    int i;
    for (i = 0; i < size; i++)
    {
      f.Write(m_names[i].m_name);
    }
    // Write IDs
    for (i = 0; i < size; i++)
    {
      f.WriteInteger(m_names[i].m_id);
    }
  }
  File::SetRoot(root, "/");
  return true;
}

void PlayerNames::AddName(const std::string& name)
{
  // TODO Ensure name unique ?
  m_names.push_back(name);
}

bool PlayerNames::NameExists(const std::string& name) const
{
  return (std::find(m_names.begin(), m_names.end(), name) != m_names.end());
}

void PlayerNames::EraseName(const std::string& name)
{
  // Erase name from vector; save the vector.
#ifdef _DEBUG
std::cout << "PlayerNames: deleting name: " << name.c_str() << "\n";
#endif

  if (!NameExists(name))
  {
#ifdef _DEBUG
    std::cout << "PlayerNames: name " << name.c_str() << " does not exist!\n";
#endif
    Assert(0);
    return;
  }

  m_names.erase(std::remove(m_names.begin(), m_names.end(), name), m_names.end()); 
  Assert(!NameExists(name));

  Save();
  // TODO Also erase the player stats for the name ? 
}

void PlayerNames::EraseAll()
{
  Load();

  // Erase all stats
  for (unsigned int i = 0; i < m_names.size(); i++)
  {
    PoolPlayerStats ps(m_names[i].m_name);
    ps.Erase();
  }
  m_names.clear();
  Save();
}

void PlayerNames::EditName(
  const std::string& oldName, const std::string& newName)
{
#ifdef _DEBUG
std::cout << "PlayerNames: replacing old name: " << oldName.c_str() << " with new name: " << newName.c_str() << "\n";
#endif

  if (!NameExists(oldName))
  {
#ifdef _DEBUG
    std::cout << "PlayerNames: name " << oldName.c_str() << " does not exist!\n";
#endif
    Assert(0);
    return;
  }

  Names::iterator it = std::find(m_names.begin(), m_names.end(), oldName);
  *it = newName;
  Save();

  Assert(!NameExists(oldName));

  // Rename player stats
  PoolPlayerStats ps(oldName);
  ps.Load();
  ps.Rename(newName); 
}

int PlayerNames::GetNumNames() const
{
  return m_names.size();
}

std::string PlayerNames::GetName(int i) const
{
  Assert(i < (int)m_names.size());
  return m_names[i].m_name;
}

int PlayerNames::GetId(int i) const
{
  Assert(i < (int)m_names.size());
  return m_names[i].m_id;
}

int PlayerNames::GetId(const std::string& playerName) const
{
  Names::const_iterator it = std::find(m_names.begin(), m_names.end(), playerName);
  Assert(it != m_names.end());
  return it->m_id;
}

void PlayerNames::SetId(const std::string& playerName, int id)
{
  Names::iterator it = std::find(m_names.begin(), m_names.end(), playerName);
  Assert(it != m_names.end());
  it->m_id = id;
  Save();
}
}

