/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: PoolSnapshot.cpp,v $
Revision 1.1.2.3  2006/08/30 21:12:21  Administrator
Get Pool to build in MSVC

Revision 1.1.2.2  2006/08/14 17:50:24  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:39  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.3  2005/09/06 20:01:02  jay
Turn off debug output

Revision 1.1.2.2  2005/08/02 18:31:06  jay
Undo fixes

Revision 1.1.2.1  2005/07/30 12:29:32  jay
Added Undo manager

*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "PoolSnapshot.h"
#include "LevelServer.h"
#include "GameObject.h"
#include "Engine.h"
#include "PoolMisc.h"

namespace Amju
{
void SnapshotManager::GetSnapshot()
{
#ifdef UNDO_DEBUG
std::cout << "**UNDO** Getting snapshot: no of snapshots before: " 
  << m_snapshots.size() << "\n";
#endif

  // Erase any snapshots after the current index

  // Create new snapshot and add it to list.
  PoolSnapshot s;
  s.CreateFromCurrentState();
  m_snapshots.push_back(s);

//  m_index++;
//  Assert(m_index == m_snapshots.size());
}

bool SnapshotManager::CanUndo()
{
  return m_snapshots.size() > 1;
}

void SnapshotManager::Undo()
{
  Assert(CanUndo());

  if (m_snapshots.size() > 1)
  {
    m_snapshots.pop_back();
  }

  SetStateToSnapshot(*(m_snapshots.rbegin()));
}

void SnapshotManager::Reset()
{
  m_snapshots.clear();
  m_index = 0;
}

void SnapshotManager::SetStateToSnapshot(const PoolSnapshot& s)
{
  s.SetStateToSnapshot();
}

void PoolSnapshot::CreateFromCurrentState()
{
  // Get the state of all game objects in the current room.
  Level* pLevel = LevelServer::Instance()->GetCurrentLevel().GetPtr();
  int levelId = pLevel->GetId(); 
  int roomId = pLevel->GetRoomId();

  GameObjectMap& objs = Engine::Instance()->GetGameObjects(levelId, roomId);
  // Iterate through map of Game Objects.  
  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    PPoolGameObject pGo = it->second; 
    Assert(pGo.GetPtr());

#ifdef UNDO_DEBUG
if (gameObjId == 10)
  std::cout << "** UNDO: Storing cue ball: pos is " 
    << ToString(*(pGo->GetOrientation()))
    << "\n";
#endif

    ObjInfo info;
    info.m_pGo = pGo;
    if (pGo->GetOrientation())
    {
      info.m_or = *(pGo->GetOrientation());
    }
    info.m_state = pGo->GetState();

    m_objInfo.push_back(info);
  }

  // No of ball spotted by each player
  int numPlayers = Engine::Instance()->GetGameState()->GetNumberOfPlayers();
  for (int i = 0; i < numPlayers; i++)
  {
    m_playerInfo.push_back(
      *(Engine::Instance()->GetGameState()->GetPlayerInfo(i)));
  }  

  // Store the state of the Rules - like is this a free ball, can the
  // ball be placed anywhere, etc.
  m_pRules = GetRules(pLevel)->Clone();
}

void PoolSnapshot::SetStateToSnapshot() const
{
  // Set the state of all game objects we have stored.
  for (unsigned int i = 0; i < m_objInfo.size(); i++)
  {
    const ObjInfo& info = m_objInfo[i];

#ifdef UNDO_DEBUG
if (info.m_pGo->GetId() == 10)
{
  std::cout << "** UNDO: Restoring cue ball: pos is: "
    << ToString(info.m_or)
    << "\n";
}
else  // NB NOT #else
{
  std::cout << "** UNDO: Restoring obj: " << info.m_pGo->GetId() 
    << "  (" << info.m_pGo->GetTypeName() 
    << ") or: "
    << ToString(info.m_or) << "\n";
}
#endif 

    info.m_pGo->SetOrientation(info.m_or);
    info.m_pGo->SetState(info.m_state);
  }

  for (unsigned int i = 0; i < m_playerInfo.size(); i++)
  {
    Engine::Instance()->GetGameState()->SetPlayerInfo(i, m_playerInfo[i]);
  }

  // Set Rules state
  Level* pLevel = LevelServer::Instance()->GetCurrentLevel().GetPtr();
  //Rules* pRules = GetRules(pLevel);
  //int id = pRules->GetId();
  
  // This was an attempt to assign the object into the one held by the Engine.
  // This won't work - it just assigns to the local variable!
  //*pRules = *(m_pRules.GetPtr());

  // Replace the Rules object held by the Engine with this new object.
  Engine::Instance()->HoldGameObject(pLevel->GetId(), pLevel->GetRoomId(), 
    m_pRules.GetPtr());
}
}


