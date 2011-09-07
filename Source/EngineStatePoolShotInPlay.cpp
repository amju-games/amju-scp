/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStatePoolShotInPlay.cpp,v $
Revision 1.1.2.21  2007/07/22 12:43:25  jay
Turn off debug output in release mode

Revision 1.1.2.20  2007/07/16 08:26:56  jay
Tidy up unused include

Revision 1.1.2.19  2007/06/28 15:48:47  Administrator
Sometimes final ball positions not set ? -- So hopefully this fixes it ?!

Revision 1.1.2.18  2007/06/23 11:34:10  jay
Reset Rules FOUL flag each frame.
Don't finish shot until we have played all the frames sent from server.
Removed unnecessary code (waiting to finish flag).
Check for no ball hit before sending shot info and telling Rules shot has
  finished.

Revision 1.1.2.17  2007/06/17 20:46:17  jay
Send object states: all types, not just PoolBalls; try to fix online camera,
 but still broken

Revision 1.1.2.16  2007/06/11 21:56:15  jay
Turn off debug output in non-debug build

Revision 1.1.2.15  2007/06/10 21:33:17  Administrator
Check for extra states BALL_IN_POCKET and BALL_OUT_OF_BOUNDS

Revision 1.1.2.14  2007/06/08 08:14:46  jay
Code tidy up

Revision 1.1.2.13  2007/05/23 18:45:42  jay
Send all object changes in one Http request (POST), not individually.
 + Some tidying up.

Revision 1.1.2.12  2007/05/16 22:05:30  Administrator
Don't run Online code in a non-online game

Revision 1.1.2.11  2007/04/28 20:43:34  Administrator
Previous attempt did not like incremented shot ID

Revision 1.1.2.10  2007/04/28 11:39:11  Administrator
Don't increment the shot number unless we are taking the shot now.
The shot is also incremented in PoolOnline::IsNewShot()

Revision 1.1.2.9  2007/03/28 21:24:54  jay
Output ball positions in debug mode only

Revision 1.1.2.8  2007/03/17 23:58:03  jay
Fix non-online game

Revision 1.1.2.7  2007/03/16 08:50:40  jay
Online changes: each ball state change/collision is sent to the server, and
used in the replay to keep consistency between clients.

Revision 1.1.2.6  2007/03/11 20:55:08  Administrator
Pool online debug

Revision 1.1.2.5  2007/03/10 21:51:52  jay
ShotLog

Revision 1.1.2.4  2006/08/30 14:51:45  jay
Fixed comment

Revision 1.1.2.3  2006/08/26 21:28:37  jay
Ongoing changes for Online game; all Online features are off unless
POOL_ONLINE is defined

Revision 1.1.2.2  2006/08/17 18:15:41  jay
Ongoing work for Online Pool

Revision 1.1.2.1  2006/08/14 17:50:16  jay
Rename "Pool"

Revision 1.1.2.3  2006/08/12 07:43:07  jay
Ongoing fixes for Online Pool: currently trying to get both clients to always
be consistent.

Revision 1.1.2.2  2006/08/07 20:42:49  jay
Online Pool - not working yet

Revision 1.1.2.1  2006/07/24 18:20:29  jay
Moved Pool code from Pool/ to Pool/

Revision 1.2.6.18  2005/09/23 19:39:35  jay
Remove unused code

Revision 1.2.6.17  2005/09/12 17:22:09  jay
Fix NonBallCollision so we don't call Intersect on two balls

Revision 1.2.6.16  2005/09/11 19:02:01  jay
Turn off debug output

Revision 1.2.6.15  2005/09/06 20:07:11  jay
Remove call to play bad wav.
Debug output for ball-ball collision test.

Revision 1.2.6.14  2005/09/05 20:06:21  jay
New COLLISION code: collide with the closest object!

Revision 1.2.6.13  2005/08/26 21:29:45  jay
Allow mouse camera control; wait in case cue ball rolls back

Revision 1.2.6.12  2005/07/30 12:18:38  jay
Add call to store new position in Undo stack

Revision 1.2.6.11  2005/06/29 20:21:20  jay
Experimented with new ball-ball collision algo, not very good :-(

Revision 1.2.6.10  2005/06/22 21:39:36  jay
Get HUD to draw consistently

Revision 1.2.6.9  2005/06/19 21:30:31  jay
Changes to ball collision code, to try to eliminate intersecting balls.

Revision 1.2.6.8  2005/06/13 22:18:22  jay
Draw AI for debugging

Revision 1.2.6.7  2005/05/15 17:16:25  jay
Set/print Big Text

Revision 1.2.6.6  2005/05/08 17:27:49  jay
Minor fix, result from GetOrientation is now const.

Revision 1.2.6.5  2005/04/21 17:35:29  jay
Show big text

Revision 1.2.6.4  2005/04/11 21:21:24  jay
Try to extract any intersecting balls before ending this state.

Revision 1.2.6.3  2005/04/01 06:00:40  jay
Code tidy-up

Revision 1.2.6.2  2005/03/20 20:13:27  jay
Ongoing work for Pool game

Revision 1.2.6.1  2005/03/17 20:58:12  jay
Initial changes from Pool to Pool

Revision 1.2  2004/09/17 14:00:38  jay
Added virtual BallInHole() so we can override in Demo mode

Revision 1.1  2004/09/08 15:43:05  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "EngineStatePoolShotInPlay.h"
//#include "EngineStatePoolOutOfBounds.h"
#include "Engine.h"
#include "LevelServer.h"
#include "PoolGameState.h"
#include "ThirdPersonCamera2.h"
#include "FreeMovingSolidGameObject.h" 
#include "EngineStatePoolSetUpShot.h"
#include "EngineStateShowLevel.h"
#include "EngineStateTimerText.h"
#include "PoolMisc.h"
#include "Trajectory.h"
#include "PoolCharacter.h"
#include "TextWriter.h"
#include "AngleCompare.h"
#include "PoolBall.h"
#include "TextFactory.h"
#include "PoolSnapshot.h"
#include "PoolOnline.h"
#include "ShotLog.h"

#ifdef _DEBUG
//#define COLLISION_DEBUG
#endif

namespace Amju
{
const char* EngineStatePoolShotInPlay::Name = "golfinplay";

// Register this State with the Engine.
static const bool registeredgolfinplay = Engine::Instance()->
  RegisterEngineState(EngineStatePoolShotInPlay::Name, new EngineStatePoolShotInPlay);

void EngineStatePoolShotInPlay::Draw()
{
  EngineStatePoolBase::Draw();

#ifdef SHOT_IN_PLAY_DEBUG
  GetRules(m_pLevel.GetPtr())->Draw();
#endif

  m_explosion.Draw();
}

void EngineStatePoolShotInPlay::DrawOverlays()
{
  EngineStatePoolBase::DrawOverlays();
  DrawHudBall();
}

void EngineStatePoolShotInPlay::SetActive(bool active)
{
  m_pLevel = LevelServer::Instance()->GetCurrentLevel();

  if (active)
  {
#ifdef POOL_ONLINE
    // TODO SOrt this out: only the PoolOnlineMgr should inc the shot ID
    //if (ThePoolOnlineManager::Instance()->IsTakingShot())
    // Always
    {
      // Increment the shot number 
      int shot = ThePoolOnlineManager::Instance()->GetShotId();
      ++shot;
      ThePoolOnlineManager::Instance()->SetShotId(shot);
    }

    // Reset the frame number
    ThePoolOnlineManager::Instance()->SetFrameNumber(0);

    // TODO Does this help prevent sending unwanted Foul message ?
    ResetSendStateFlags();

#endif

    // TODO TEMP TEST
    // Log position of each ball - are they exactly the same in both clients ?
    int levelId = m_pLevel->GetId();
    int roomId = m_pLevel->GetRoomId();
    GameObjectMap& objs = Engine::Instance()->GetGameObjects(levelId, roomId);
    GameObjectMap::iterator it;
    for (it = objs.begin(); it != objs.end(); ++it)
    {
      PPoolGameObject pGo1 = it->second;
      PoolBall* p = dynamic_cast<PoolBall*>(pGo1.GetPtr());
      if (p)
      {
#ifdef _DEBUG
        std::cout << "Ball: " << p->GetNumber() << ": " << ToString(p->GetOrientation()->GetVertex()).c_str() << "\n";
#endif
      }
    }

    // Log shot info - is it _exactly_ the same for all clients
    PoolGameState::PlayerInfo* pInfo =
      GetEngine()->GetGameState()->GetCurrentPlayerInfo();
  
    std::string s = "Shot taken: ";  
    s += " Y rot: " + ToString(pInfo->m_golfStroke.m_yRot);
    s += " v vel: " + ToString(pInfo->m_golfStroke.m_vertVel);
    s += " h vel: " + ToString(pInfo->m_golfStroke.m_horVel);
    s += " accel: " + ToString(pInfo->m_golfStroke.m_accel);
    s += " engli: " + ToString(pInfo->m_golfStroke.m_english);
    s += " drawr: " + ToString(pInfo->m_golfStroke.m_drawRoll);
    ShotLog(s); 

    // Log position of all balls - TODO turn off eventually

    ShotLog("Initial positions of all balls follow:");
    for (it = objs.begin(); it != objs.end(); ++it)
    {
      PPoolGameObject pGo = it->second;
      PoolBall* pBall = dynamic_cast<PoolBall*>(pGo.GetPtr());
      if (pBall)
      {
        std::string s = "  Ball ID: ";
        s += ToString(pBall->GetId());
        s += " Number: ";
        s += ToString(pBall->GetNumber());
        s += " pos: x: ";
        s += ToString(pBall->GetPosition().x);
        s += " y: ";
        s += ToString(pBall->GetPosition().y);
        s += " z: ";
        s += ToString(pBall->GetPosition().z);
        ShotLog(s);
      }
    }


    m_time = 0; // for logging collisions

    GetEngine()->SetLetterbox(true);
    // Set target for camera
    GetCamera()->SetPlayer(GetBall()); 

    /*
    if (IsOnlineGame())
    {
      Orientation o = *(GetBall()->GetOrientation());
      o.SetYRot(o.GetYRot() + 180.0f); // TODO TEMP TEST fix online cam ?
      GetCamera()->SetOrientation(o);
      GetCamera()->Reset();
    }
    */

    m_isFading = false;

    // Remember player position
    SetPlayerOrientation(*(GetActivePlayer()->GetOrientation()));

    // Done already
    //GetActivePlayer()->SetActionGo();

    ////GetBall()->SetActivePlatform(0);

    VertexBase v = GetBall()->GetOrientation()->GetVertex();
    v.y += 0.1f;
    m_explosion.Reset(v);

    // Make putting or driving sound
    bool hp = Engine::Instance()->GetGameState()->
      GetCurrentPlayerInfo()->m_isHighPower;
    if (hp)
    {
      Engine::Instance()->PlayWav(
        Engine::Instance()->GetConfigValue("golf_wav_15"));
    }

    // POOL: Reset the rules flags
    GetRules(m_pLevel.GetPtr())->Reset();

    m_onlineReadyToFinish = false;
  }
  else
  { 
    GetEngine()->SetLetterbox(false);
  }
}

bool EngineStatePoolShotInPlay::Load()
{
  EngineStatePoolBase::Load();

  // Particle effect
  std::string effect = Engine::Instance()->GetConfigValue("golf_club_explode");

  if (!m_explosion.Load(effect))
  {
    return false;
  }

  return true;
}

void EngineStatePoolShotInPlay::Clear()
{
}

bool EngineStatePoolShotInPlay::CollisionTest()
{
  // Check for ball-pocket intersections, then handle collisions
  // between pairs of balls.
  NonBallPairTest();
  bool b = BallCollisions();

  return b; 
}

void EngineStatePoolShotInPlay::NonBallPairTest()
{
  // Iterate over the game objects which are in the current level/room.
  int levelId = m_pLevel->GetId();
  int roomId = m_pLevel->GetRoomId();

  // Get the game objects which are in the same room as the player.
  GameObjectMap& objs = Engine::Instance()->GetGameObjects(levelId, roomId);
   
  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    PPoolGameObject pGo1 = it->second;
    PoolBall* pBall1 = dynamic_cast<PoolBall*>(pGo1.GetPtr());

    if (InPlay(pGo1))
    {
      GameObjectMap::iterator jt = it;
      ++jt;
      for (; jt != objs.end(); ++jt)
      {
        PPoolGameObject pGo2 = jt->second;
        PoolBall* pBall2 = dynamic_cast<PoolBall*>(pGo2.GetPtr());
        if (pBall1 && pBall2)
        {
          // Both objects are balls - don't handle this here.
          continue;
        }

        Assert(pGo1.GetPtr() != pGo2.GetPtr());

        if (InPlay(pGo2) && 
            pGo1->Intersects(*(pGo2.GetPtr())))
        {
          // Handle collision if one or both objects are not balls.
          // (Pairs of balls are handled specially to avoid leaving intersecting
          //  pairs)
          pGo1->HandleObjectCollision(pGo2.GetPtr());
        }
      }
    }
  }
}

bool EngineStatePoolShotInPlay::BallCollisionTest(
  EngineStatePoolShotInPlay::BallPairs* pBallPairs)
{
  bool ret = false;

  // Iterate over the game objects which are in the current level/room.
  int levelId = m_pLevel->GetId();
  int roomId = m_pLevel->GetRoomId();

  // Get the game objects which are in the same room as the player.
  GameObjectMap& objs = Engine::Instance()->GetGameObjects(levelId, roomId);
   
  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    PPoolGameObject pGo1 = it->second;
    PoolBall* pBall1 = dynamic_cast<PoolBall*>(pGo1.GetPtr());

    if (pBall1 && InPlay(pBall1))
    {
      GameObjectMap::iterator jt = it;
      ++jt;
      for (; jt != objs.end(); ++jt)
      {
        PPoolGameObject pGo2 = jt->second;

        Assert(pGo1.GetPtr() != pGo2.GetPtr());

        if (InPlay(pGo2) && pGo1->Intersects(*(pGo2.GetPtr())))
        {
          // Store the pair of balls if they collide.
          PoolBall* pBall2 = dynamic_cast<PoolBall*>(pGo2.GetPtr());
          if (pBall2)
          {
            pBallPairs->push_back(std::make_pair(pBall1, pBall2));
            ret = true;
          }
        }
      }
    }
  } 
  return ret;
}

bool EngineStatePoolShotInPlay::BallCollisions()
{
// NEW COLLISION CODE

  // For each ball, find all colliding balls, then sort them by distance.
  // Handle the closest ball first, then check for any remaning collisions.
  bool ret = false;

  // Iterate over the game objects which are in the current level/room.
  int levelId = m_pLevel->GetId();
  int roomId = m_pLevel->GetRoomId();

  // Get the game objects which are in the same room as the player.
  GameObjectMap& objs = Engine::Instance()->GetGameObjects(levelId, roomId);
   
  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    PPoolGameObject pGo1 = it->second;
    PoolBall* pBall1 = dynamic_cast<PoolBall*>(pGo1.GetPtr());

    if (pBall1 && InPlay(pBall1))
    {
#ifdef CUE_BALL_SPHERE_DEBUG
      // Test for bad Capsule
      if (IsCueBall(pBall1))
      {
        if (pBall1->GetPrevOrientation().GetVertex() ==
            pBall1->GetOrientation()->GetVertex())
        {
          std::cout << "CUE BALL CAPSULE is a SPHERE!\n";
        }
      }
      // End of bad capsule test
#endif

      BallPairs pairs;

      GameObjectMap::iterator jt = it;
      ++jt;
      for (; jt != objs.end(); ++jt)
      {
        PPoolGameObject pGo2 = jt->second;
        Assert(pGo1.GetPtr() != pGo2.GetPtr());
        PoolBall* pBall2 = dynamic_cast<PoolBall*>(pGo2.GetPtr());

        if (pBall2 && 
            InPlay(pBall2) && 
            (pBall1->Intersects(*pBall2)))
        {
          // Store the pair of balls if they collide.
          if (pBall2)
          {
#ifdef COLLISION_DEBUG
std::cout << "COLLISION: time: " << m_time << ": balls " << pBall1->GetNumber() 
  << ", " << pBall2->GetNumber() << "\n";
#endif
            pairs.push_back(std::make_pair(pBall1, pBall2));
            ret = true;
          }
        }
      }

#ifdef COLLISION_DEBUG
if (pairs.size() > 1)
{
  std::cout << "COLLISION: Ball " << pBall1->GetNumber() 
    << " collides with " << pairs.size() << " balls!\n";
}
#endif

      // Find the closest ball to pBall1.
      float bestDist = 99999.9f;
      PoolBall* pBest = 0;
      for (unsigned int i = 0; i < pairs.size(); i++)
      {
        VertexBase v1 = pairs[i].first->GetPrevOrientation().GetVertex();
        VertexBase v2 = pairs[i].second->GetPrevOrientation().GetVertex();
        float dist = (v1 - v2).Length();
        if (dist < bestDist)
        {
          bestDist = dist;
          pBest = pairs[i].second;
        } 
      } 

      // Handle the closest ball
      if (pBest)
      {
#ifdef COLLISION_DEBUG
if (pairs.size() > 1)
{
  std::cout << "COLLISION: ...Ball " << pBest->GetNumber() 
    << " is closest\n"; 
}
#endif
        pBall1->HandleObjectCollision(pBest);
      }
     
      // Check again for collisions, handle remaining balls ? 
    }
  } 
  return ret;
// NEW COLLISION CODE

/*
  // This way should work fine :-( But it doesn't.

  bool ballsCollide = false;
  
  // Get Pairs of colliding balls - if no pairs we are finished!
  BallPairs pairs;
  if (!BallCollisionTest(&pairs))
  {
    // Done - no pairs of colliding balls THIS ITERATION - return flag
    // as we may have set it in a previous iteration.
    return ballsCollide;
  }
  ballsCollide = true;

#ifdef _DEBUG
std::cout << "Ball collision: found " << pairs.size() << " colliding pairs\n";
#endif

  Assert(!pairs.empty());
  // Find the collision time for each pair of balls
  float earliest = 100.0f; 
  int earliestPair = -1;
  for (int i = 0; i < pairs.size(); i++)
  {
    PoolBall* p1 = pairs[i].first;
    PoolBall* p2 = pairs[i].second;

    float t = 0; // time of exact collision
    VertexBase v1, v2; // exact positions at collision time
    PoolBall::GetExactCollisionCoords(p1, p2, &t, &v1,&v2);
    if (t < earliest)
    {
        earliest = t;
        earliestPair = i;
    }
  }
  if (earliestPair == -1)
  {
    return true;
  }

#ifdef _DEBUG
std::cout << " Earliest t: " << earliest << "\n";
#endif

  // Get the earliest collision
  // Set the positions at collision time and do collision response
  PoolBall* p1 = pairs[earliestPair].first;
  PoolBall* p2 = pairs[earliestPair].second;
  p1->HandleObjectCollision(p2);

  // Set the position of every other ball to the position at the earliest
  // collision time.
  // Loop over game objects in the current room.
  Level* pLevel = LevelServer::Instance()->GetCurrentLevel().GetPtr();
  int levelId = pLevel->GetId(); 
  int roomId = pLevel->GetRoomId();
  GameObjectMap& objs = Engine::Instance()->GetGameObjects(levelId, roomId);
  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    PPoolGameObject pGo = it->second; 
    PoolBall* pBall = dynamic_cast<PoolBall*>(pGo.GetPtr());
    if (pBall && pBall != p1 && pBall != p2)
    {
      pBall->SetPositionAtTime(earliest);
    }
  }

  return true; 
*/

  // TODO Bad easy version
  BallPairs pairs;
  if (!BallCollisionTest(&pairs))
  {
    return false;
  }
  for (unsigned int i = 0; i < pairs.size(); i++)
  {
    PoolBall* p1 = pairs[i].first;
    PoolBall* p2 = pairs[i].second;
    p1->HandleObjectCollision(p2);
  }
  return true; 

}

bool EngineStatePoolShotInPlay::IsUnderwater()
{
  return false;
}

bool EngineStatePoolShotInPlay::IsOutOfBounds()
{
  const Orientation* pO = GetBall()->GetOrientation();

  if (pO->GetY() < -2.0f) // TODO CONFIG
  {
    return true;
  }

  return false;
}

// Return true if all balls have stopped moving.
bool EngineStatePoolShotInPlay::BallsHaveStopped()
{
#ifdef POOL_ONLINE
  // If we are the receiving client of an online game, make sure we don't
  // finish the shot too early, and so not set the final positions!
  if (IsOnlineGame() && !IsUserControlled() &&
      ThePoolOnlineManager::Instance()->GetFrameNumber() <=   
       ThePoolOnlineManager::Instance()->GetFinalFrameNumberSent())
  {
#ifdef _DEBUG
std::cout << "Have not yet reached final frame sent (current: " 
  << ThePoolOnlineManager::Instance()->GetFrameNumber() 
  << " final: " << ThePoolOnlineManager::Instance()->GetFinalFrameNumberSent()
  << "\n";
#endif

     return false;
  }
#endif

  int levelId = m_pLevel->GetId();
  int roomId = m_pLevel->GetRoomId();

  // Get the game objects which are in the same room as the player.
  GameObjectMap& objs = Engine::Instance()->GetGameObjects(levelId, roomId);
   
  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    PPoolGameObject pGo1 = it->second;
    PoolBall* pBall = dynamic_cast<PoolBall*>(pGo1.GetPtr());
    if (pBall && InPlay(pBall))
    {
      if (pBall->GetForwardVel() > 0)
      {
//std::cout << " ball " << pBall->GetId() << " has fwd vel " << pBall->GetForwardVel() << "\n";
        return false;
      }
    }
  }
  return true;
}

void EngineStatePoolShotInPlay::ResetSendStateFlags()
{
#ifdef POOL_ONLINE
  int levelId = m_pLevel->GetId();
  int roomId = m_pLevel->GetRoomId();

  // Get the game objects which are in the same room as the player.
  GameObjectMap& objs = Engine::Instance()->GetGameObjects(levelId, roomId);
   
  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    PPoolGameObject pGo = it->second;
    pGo->SetSendState(false);
    // NB if true, does send all ball states every frame. 

    Rules* pRules = dynamic_cast<Rules*>(pGo.GetPtr());
    if (pRules)
    {
      pRules->SetState(UNKNOWN);
    }
  }
#endif
}

void EngineStatePoolShotInPlay::Update()
{
#ifdef POOL_ONLINE
  if (IsOnlineGame())
  {
    ThePoolOnlineManager::Instance()->Update();

    ShotLog("Update: time: " + ToString(m_time));

    // Reset state change flag for all balls. This is set for balls that
    // collide etc during the frame.
    ResetSendStateFlags();
  }

#endif // POOL_ONLINE

  EngineStatePoolBase::Update();
 
  // Test for ball-object collisions
  bool isCollision = CollisionTest();

  if (!Engine::Instance()->GetGameState()->GetCurrentPlayerInfo()->m_isPlaying)
  {
    return;
  }
  
  if (m_isFading)
  {
    return;
  }

  // Notify Rules if all balls have stopped moving
  // Rules may switch player or not.
  // TODO TEMP TEST don't stop if there's a collision ? -> causes hang :-(
  // Now balls get extra vel if there's an intersection, so this should be ok.
  static int frameCount = 0;
  static float drawRollWaitTime = 0;

  bool ballsHaveStopped = BallsHaveStopped();
  if (ballsHaveStopped)
  {
    frameCount++;
    // Draw/roll takes effect after a time delay, so we must wait this long 
    // to make sure the cue ball won't start moving again.
    drawRollWaitTime += Engine::Instance()->GetDeltaTime();
  }
  else
  {
    frameCount = 0;
    drawRollWaitTime = 0;
  }
  static const int FRAMES_MAX = 200; // TODO CONFIG
  static const float MAX_DRAW_ROLL_WAIT = Engine::Instance()->GetConfigFloat(
    "pool_max_roll_pause");
  // If we should wait in case draw/roll takes effect, reset ballsHaveStopped.
  if (drawRollWaitTime <= MAX_DRAW_ROLL_WAIT)
  {
    ballsHaveStopped = false;
  }

  // Check how many frames the balls have stopped. 
  // If we have waited too long, we tell the Rules that the balls have stopped,
  // even though there are still balls intersecting.
  // (PoolBalls try to extract themselves from each other.)
  if (ballsHaveStopped && (!isCollision || frameCount > FRAMES_MAX))
  {
    // This stores a separate position for each player.
    // No good - we want the ball to stay where it is.
    Orientation o = *(GetBall()->GetOrientation());
    GetEngine()->GetGameState()->GetCurrentPlayerInfo()->m_or = o;
    // Put the character at the new position
    GetActivePlayer()->SetOrientation(o);
   
    // We need to set Rules Foul if no ball hit - but if we call 
    //  Rules::BallsHaveStopped we swap players before sending shot info....?

    GetRules(m_pLevel.GetPtr())->CheckForNoBallHit();
 
    // Handle Online game stuff
    OnlineBallsStopped();

    GetRules(m_pLevel.GetPtr())->BallsHaveStopped();

    // Store this state in the Undo stack.
    TheSnapshotManager::Instance()->GetSnapshot();

#ifdef _DEBUG
std::cout << "** Balls have stopped --- this is the end of the shot **\n";
#endif
  }

  // Check for new Text in Rules.
  std::string text = GetRules(m_pLevel.GetPtr())->GetBigText();
  // Create 3D text
  SetBigText(text.c_str());

#ifdef POOL_ONLINE
  if (IsOnlineGame())
  {
    // Increment the frame count
    int frame = ThePoolOnlineManager::Instance()->GetFrameNumber();
    ++frame;
    ThePoolOnlineManager::Instance()->SetFrameNumber(frame);

    // Log position of all balls 
  #ifdef WRITE_SHOT_LOG
    int levelId = m_pLevel->GetId();
    int roomId = m_pLevel->GetRoomId();
    GameObjectMap& objs = Engine::Instance()->GetGameObjects(levelId, roomId);
    for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
    {
      PPoolGameObject pGo = it->second;
      PoolBall* pBall = dynamic_cast<PoolBall*>(pGo.GetPtr());
      if (pBall)
      {
        std::string s = "  Ball ID: ";
        s += ToString(pBall->GetId());
        s += " Number: ";
        s += ToString(pBall->GetNumber());
        s += " pos: x: ";
        s += ToString(pBall->GetPosition().x);
        s += " y: ";
        s += ToString(pBall->GetPosition().y);
        s += " z: ";
        s += ToString(pBall->GetPosition().z);
        ShotLog(s);
      }
    }
#endif
    // UPDATE OBJECT STATES:
    // Either send states to the server, or update local objects.
    // We want this to be the final update to the objects this frame. 
    // If it is not, then the object could be wrongly
    //  updated and consistency with other client will be lost :-(
    ThePoolOnlineManager::Instance()->UpdateOrSendObjectStatesThisFrame();
  }
#endif
}

void EngineStatePoolShotInPlay::OnlineBallsStopped()
{
#ifdef POOL_ONLINE
    // ONLINE:
    // If we just took the shot: send the state of every ball to the server.
    // This is so the other clients will be in exactly the same state as
    // this client.
    // If another client took the shot, get the state of every ball from
    // the server.
    if (IsOnlineGame())
    {
      Assert(m_pLevel.GetPtr());
      // TODO Make sure 'current' player hasn't just been changed by the Rules.
      if (Engine::Instance()->GetEngineState()->GetCurrentPlayerInfo()->m_isOnline)
      {
        // We want to GET the state of all balls.

        // NB Get this list of object states before we start the shot.
        // Then update objects in the list as the current frame matches
        // frames in the list.

        // This is now called in PoolOnline::RequestShotInfo(),
        // so when we request the shot, we also get the object state changes.
        //ThePoolOnlineManager::Instance()->GetOnlineObjectStates();

        // Now don't quit this state until we have updated all objects:
        // Poll PoolOnline::IsWaitingForObjectStates().
      }
      else
      {
        // Send the shot info, then send the state of every ball at the end
        // of the shot.
        GameState::PlayerInfo* pInfo =
          GetEngine()->GetEngineState()->GetCurrentPlayerInfo();

        // Send the final state of every object - this is in addition
        //  to sending state changes during the shot.
        ThePoolOnlineManager::Instance()->SetOnlineObjectStates(m_pLevel.GetPtr());
    
        // NB Shot values are rounded when shot taken by this client.
        ThePoolOnlineManager::Instance()->SendShotInfo(PoolOnline::ShotInfo(
          pInfo->m_golfStroke.m_yRot,
          pInfo->m_golfStroke.m_vertVel,
          pInfo->m_golfStroke.m_horVel,
          pInfo->m_golfStroke.m_accel, 
          pInfo->m_golfStroke.m_english,
          pInfo->m_golfStroke.m_drawRoll,
          pInfo->m_golfStroke.GetCueBallPos().x,
          pInfo->m_golfStroke.GetCueBallPos().y,
          pInfo->m_golfStroke.GetCueBallPos().z
        ));
      }
    }
#endif // POOL_ONLINE
}

void EngineStatePoolShotInPlay::OutOfBoundsReset()
{
}

void EngineStatePoolShotInPlay::DoCameraWork()
{
  EngineStatePoolBase::DoCameraWork(); // this calls Update - shouldn't.

/*
  // POOL: no good, we want birds eye view if user has selected it.
  // Stay at the same height as the ball.
  float y = GetBall()->GetHeight() + 1.0f; // TODO CONFIG
  GetCamera()->GetOrientation()->SetY(y);
*/
}

void EngineStatePoolShotInPlay::SetPlayerOrientation(const Orientation& o)
{
  m_playerOr = o;
}

void EngineStatePoolShotInPlay::DrawPlayer()
{
  Orientation o = *(GetActivePlayer()->GetOrientation());
  GetActivePlayer()->SetOrientation(m_playerOr);
  GetActivePlayer()->Draw();
  GetActivePlayer()->SetOrientation(o);
}

void EngineStatePoolShotInPlay::MousePos(int x, int y)
{
  GetCamera()->MousePos(x, y);
}

void EngineStatePoolShotInPlay::MouseButton(bool down, bool ctrl, bool shift)
{
  GetCamera()->MouseButton(down, ctrl, shift);
}
}


