/*
Amju Games source code (c) Copyright Jason Colman 2000-2006
$Log: PoolBehaviourOnline.cpp,v $
Revision 1.1.2.14  2007/08/02 08:48:47  jay
Replace hard-coded times with PoolOnline::Constants.
These can be updated from the server in future.

Revision 1.1.2.13  2007/04/28 20:43:34  Administrator
Previous attempt did not like incremented shot ID

Revision 1.1.2.12  2007/04/28 20:29:04  Administrator
Fix: IsNewShot() is repeatedly called and so we cannot update shot ID there

Revision 1.1.2.11  2007/04/28 11:39:48  Administrator
We now get object states for new shot in PoolOnline::IsNewShot

Revision 1.1.2.10  2007/03/16 08:50:40  jay
Online changes: each ball state change/collision is sent to the server, and
used in the replay to keep consistency between clients.

Revision 1.1.2.9  2007/03/09 21:50:41  jay
Online game - ongoing fixes

Revision 1.1.2.8  2006/08/30 21:12:18  Administrator
Get Pool to build in MSVC

Revision 1.1.2.7  2006/08/30 14:50:58  jay
Point player in direction of shot

Revision 1.1.2.6  2006/08/28 22:50:32  jay
Fix cue ball position, broken when factoring out Round() calls

Revision 1.1.2.5  2006/08/28 22:33:18  jay
Round shot info in an attempt to remove inconsistencies between clients

Revision 1.1.2.4  2006/08/26 21:28:37  jay
Ongoing changes for Online game; all Online features are off unless
POOL_ONLINE is defined

Revision 1.1.2.3  2006/08/14 17:50:21  jay
Rename "Pool"

Revision 1.1.2.2  2006/08/12 07:43:08  jay
Ongoing fixes for Online Pool: currently trying to get both clients to always
be consistent.

Revision 1.1.2.1  2006/08/07 20:42:50  jay
Online Pool - not working yet

*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <iostream>
#include "PoolBehaviourOnline.h"
#include "Engine.h"
#include "PoolOnline.h"
#include "GameState.h"
#include "PoolChangeState.h"
#include "PoolMisc.h"
#include "Round.h"
#include "AngleCompare.h"
#include "PoolCharacter.h"

namespace Amju
{
PoolBehaviourOnline::PoolBehaviourOnline() : 
  m_pollTime(9999.9f), m_lastShotId(-1)
{
}

void PoolBehaviourOnline::Update()
{
#ifdef POOL_ONLINE

  Assert(IsOnlineGame());

  // Get opponent's name for messages
  std::string opName = Engine::Instance()->GetGameState()->GetCurrentPlayerInfo()->m_name;

  // Check server for new shot periodically.
  m_pollTime += Engine::Instance()->GetDeltaTime();
  if (m_pollTime > ThePoolOnlineManager::Instance()->
        GetConstant(PoolOnline::NEW_SHOT_POLL_PERIOD))
  {
#ifdef _DEBUG
std::cout << "POLLING FOR ONLINE OPPONENT'S SHOT!\n";
#endif
    m_pollTime = 0;
    ThePoolOnlineManager::Instance()->RequestShotInfo();

    std::string s = "Waiting for " + opName + " to play...";
    ThePoolOnlineManager::Instance()->SetLastStatusMsg(s);
    return;
  } 

  if (ThePoolOnlineManager::Instance()->IsNewShot(m_lastShotId) &&
      !m_waitingForObjects)
  {
    // Update to the new shot ID so we download the object changes for the
    // correct shot!

    // (Make sure we have all the object states for the shot.
    //  Don't take the shot until we have this info.)
    // We must now download the object state changes which happened
    // when the shot was played on the other client.
    ThePoolOnlineManager::Instance()->GetOnlineObjectStates();

    // A flag is now set. When the above request returns the flag will
    //  be reset and we can proceed with showing the shot.
    m_waitingForObjects = true;
    return;
  }
  else if (m_waitingForObjects)
  {
    if (ThePoolOnlineManager::Instance()->IsWaitingForObjectStates())
    {
      // Still waiting for response from server.
      return;
    }

    // Server has responded. We can now take the shot!

std::cout << "OPPONENT HAS TAKEN SHOT!\n";
    std::string str = opName + " has taken a shot!";
    ThePoolOnlineManager::Instance()->SetLastStatusMsg(str);

    // There is a new shot  
    PoolOnline::ShotInfo s = ThePoolOnlineManager::Instance()->
      GetLastShotInfo();

    m_lastShotId = s.m_shotId;

//std::cout << " ..now, m_lastShotId: " << m_lastShotId << "\n";

    // Take the shot
    // Set the shot data, then change to state which shows player 
    // taking the shot.
    GameState::PlayerInfo* pInfo = Engine::Instance()->GetGameState()->GetCurrentPlayerInfo();
    pInfo->m_golfStroke =
      GameState::PlayerInfo::PoolStroke(
      s.m_yRot, s.m_verticalVel, s.m_horVel, 
      s.m_accel, s.m_english, s.m_drawRoll);

    // Make sure data is rounded, removing any conversion errors.
    // This also sets the cue ball pos to the rounded position.
    VertexBase v(s.m_cueX, s.m_cueY, s.m_cueZ);
    pInfo->m_golfStroke.SetCueBallPos(v);

    Round(pInfo);

    // Set the cue ball position
/*
    VertexBase v(s.m_cueX, s.m_cueY, s.m_cueZ);
    Orientation o = *(GetBall()->GetOrientation());
    o.SetVertex(v);
    GetBall()->SetOrientation(o);
*/

/*
    VertexBase v(s.m_cueX, s.m_cueY, s.m_cueZ);
std::cout << "PoolBehaviourOnline: Setting cue ball pos to " << ToString(v).c_str() << "\n";
std::cout << "??? cueX: " << s.m_cueX 
  << " cueY: " << s.m_cueY 
  << " cueZ: " << s.m_cueZ << "\n";
*/

    // Point the player in the direction of the shot
    Orientation oPlayer(*(GetBall()->GetOrientation()));
    VertexBase v1 = oPlayer.GetVertex();
    oPlayer.SetXRot(0);
    oPlayer.SetYRot(0);
    oPlayer.SetZRot(0);
    float yRadians = DegToRad(s.m_yRot); 
    float dx = sin(yRadians);
    float dz = cos(yRadians);
    static const float DIST_TO_BALL = 
      Engine::Instance()->GetConfigFloat("golf_dist_to_ball");
    oPlayer.SetX(oPlayer.GetX() + dx * DIST_TO_BALL);
    oPlayer.SetZ(oPlayer.GetZ() + dz * DIST_TO_BALL);
    oPlayer.SetYRot(s.m_yRot);
    GetActivePlayer()->SetOrientation(oPlayer);

    ChangeStateToShowShot();
  }
#endif // POOL_ONLINE

}

void PoolBehaviourOnline::OnEsActivated()
{
  // TODO
  // There's no point in polling immediately, because the other client
  //  has to receive the last shot info and play the shot through ??

  m_pollTime = 9999.9f; // so we poll immediately

  // This is set to true when we get a new shot, and then must wait until
  // we get the object changes for the shot.
  m_waitingForObjects = false;
}

void PoolBehaviourOnline::OnNewRoom()
{
  m_lastShotId = -1;
}
}

