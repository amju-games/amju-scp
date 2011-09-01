/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: PoolSnapshot.h,v $
Revision 1.1.2.2  2006/08/14 17:50:24  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:39  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.1  2005/07/30 12:29:32  jay
Added Undo manager

*/

#ifndef POOL_SNAPSHOT_H_INCLUDED
#define POOL_SNAPSHOT_H_INCLUDED

#include <vector>
#include "GameObject.h"
#include "Orientation.h"
#include "Singleton.h"
#include "GameState.h"
#include "Rules.h"

namespace Amju
{
// Snapshot of Pool frame.
// This lets us rewind games by going back to previous snapshots.
// Also could be useful for 2-player network games where we must
// synchronise two processes.
class PoolSnapshot
{
public:
  // Store the current game state in this snapshot.
  void CreateFromCurrentState();

  // Set the current game state to the contents of this snapshot.
  // This is used to Undo a move, or to synch. with another game state.
  void SetStateToSnapshot() const;

protected:
  // Current score/stats of each player

  // List of balls potted for each player


  // State of each object in current room
  // For Pool, all we need is the position and object state
  // Things like ball vel etc are not needed as snapsnots are always
  // taken when balls are stationary.
  struct ObjInfo
  {
    PGameObject m_pGo;
    Orientation m_or;
    State m_state;
  };
  std::vector<ObjInfo> m_objInfo;

  std::vector<PoolGameState::PlayerInfo> m_playerInfo;

  // Store a copy of the Rules state for this room. 
  // NB Must be on heap, we will copy some subclass of Rules.
  // We will use virtual Rules::Clone() to get a copy of the subclassed
  // object.
  SharedPtr<Rules> m_pRules;
};

// Manage snapshots;
// Rewind or fast-forward by one snapshot; collect a new snapshot 
class SnapshotManager
{
public:
  // Get a snapshot of the current game state.
  // This new snapshot gets put in the current position. If we have not
  // rewound, the new snapshot goes at the end of the existing ones. If
  // we have gone back, it's possible that some snapshots will be lost.
  void GetSnapshot();

  bool CanUndo();

  //bool CanRedo();

  // Undo the game state to the previous snapshot.
  void Undo();

  //void Redo();

  // Call on new game
  void Reset();

protected:
  void SetStateToSnapshot(const PoolSnapshot& s);

protected:
  std::vector<PoolSnapshot> m_snapshots;
  int m_index;
};

typedef Singleton<SnapshotManager> TheSnapshotManager;
}

#endif

