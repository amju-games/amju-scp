/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: PoolSetUpShotBehaviour.cpp,v $
Revision 1.1.2.1  2006/08/14 17:50:24  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:35  jay
Moved Pool code from Pool/ to Pool/

Revision 1.6.6.2  2005/04/01 06:07:18  jay
Added extra params for shot

Revision 1.6.6.1  2005/03/20 20:13:27  jay
Ongoing work for Pool game

Revision 1.6  2004/09/27 12:33:04  Administrator
Improvements to Demo mode - shot strength; character shot differences.

Revision 1.5  2004/09/25 21:15:45  Administrator
Aim at a bonus if hole and no waypoints cannot be seen.

Revision 1.4  2004/09/20 13:35:53  Administrator
Ongoing improvements to AI for demo mode. Shot speed is now varied, but
is not accurate (distance travelled is not linear with shot speed ?).
Should aim at each waypoint in turn, or check for floor where the ball
will land.

Revision 1.3  2004/09/18 14:36:52  Administrator
Check distance to hole/waypoint

Revision 1.2  2004/09/17 14:02:45  jay
Improved player AI for demo mode, mainly by adding Waypoints code.

Revision 1.1  2004/09/08 15:43:06  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <math.h>
#include "PoolSetUpShotBehaviour.h"
#include "EngineStatePoolSetUpShot.h"
#include "EngineStatePoolShowShotDemo.h"
#include "Engine.h"
#include "PoolMisc.h"
#include "PoolHole.h"
#include "CharacterGameObject.h"
#include "AngleCompare.h"
#include "PoolCharacter.h"
#include "GameState.h"
#include "Waypoint.h"
#include "LevelServer.h"
#include "Bonus.h"

namespace Amju
{
bool LosExists(const VertexBase& v1, const VertexBase& v2)
{
  PLevel pLevel = LevelServer::Instance()->GetCurrentLevel();
  bool los = !pLevel->GetScene()->LineIntersects(v1, v2, 0.5f);
  return los;
}

bool DistanceIsOk(const VertexBase& v1, const VertexBase& v2)
{
  // Returns true if the distance between the verts is not too
  // large.
  static float maxDist = Engine::Instance()->GetConfigFloat("demo_max_dist");
  static float maxSq = maxDist * maxDist;

  float dx = v1.x - v2.x;
  float dy = v1.y - v2.y;
  float dz = v1.z - v2.z;
  float dsq = dx * dx + dy * dy + dz * dz;
  bool b = (dsq <= maxSq);

  if (!b)
  {
    return false;
  }

  // Check that there is some floor at the point where the ball will
  // hit the ground.

  return b;
}

void PoolSetUpShotBehaviour::Update()
{
  static float waitTime = Engine::Instance()->GetConfigFloat("golf_wait");

  float dt = Engine::Instance()->GetDeltaTime();
  m_time += dt;

  // TODO Move around the ball to face the direction of the shot.

  if (m_time > waitTime)  
  {
    static float accel = Engine::Instance()->GetConfigFloat("golf_ball_accel");

    // Make adjustments to the shot params based on the players skill
    // stats. 
    float accuracy = Engine::Instance()->GetGameState()->GetCurrentPlayerInfo()->
      m_stats[1];
    float bias = Engine::Instance()->GetGameState()->GetCurrentPlayerInfo()->
      m_stats[2];
    // If putting, get different stats.
    bool isPutting = 
      !Engine::Instance()->GetGameState()->GetCurrentPlayerInfo()->m_isHighPower;
    if (isPutting)
    {
      // Putting
      accuracy = Engine::Instance()->GetGameState()->GetCurrentPlayerInfo()->
        m_stats[3];
      bias = Engine::Instance()->GetGameState()->GetCurrentPlayerInfo()->
        m_stats[4];
    }
    // Constant by which to multiply stats' effect
    static const float multAcc = Engine::Instance()->GetConfigFloat("golf_stat_mult_acc");
    static const float multBias = Engine::Instance()->GetConfigFloat("golf_stat_mult_bias");

    // Make the adjustments to the shot
    // Accuracy of 1.0 means perfectly accurate.
    // Accuracy affects horizontal speed
    m_hVel += multAcc * (1.0f - accuracy);

    // Bias of 0.5 means exactly right - the extremes are 0 and 1.0.
    // NB that - is there so the direction matches the stats page.
    m_yRot += multBias * -(bias - 0.5f);

    // Set the shot data, then change to state which shows player swinging the club.
    Engine::Instance()->GetGameState()->GetCurrentPlayerInfo()->m_golfStroke =
        GameState::PlayerInfo::PoolStroke(m_yRot, m_vVel, m_hVel, accel,
        0, 0 );  // TODO english, roll/draw

    // TODO This is no good - it only works for demo mode.
    //Engine::Instance()->ChangeState(EngineStatePoolShowShotDemo::Name, Engine::IMMEDIATE);
    // POOL
    Engine::Instance()->ChangeState(EngineStatePoolShowShot::Name, Engine::IMMEDIATE);
  }
}

void PoolSetUpShotBehaviour::CalcShot()
{
  PLevel pLevel = LevelServer::Instance()->GetCurrentLevel();

  // Get all objects in this Room.
  int levelId = pLevel->GetId();
  int roomId = pLevel->GetRoomId();
  GameObjectMap& objs = Engine::Instance()->GetGameObjects(levelId, roomId);

  // Aim for the hole, or the best Waypoint if the hole is unreachable 
  // from the current position.
  PoolHole* pHole = GetPoolHole(pLevel.GetPtr());
  // The hole or waypoint at which we decide to aim. Zero means
  // we couldn't see any target.
  m_pTargetObject = 0;

  // Work out the angle we should be pointing in.
  VertexBase vPlayer = GetActivePlayer()->GetOrientation()->GetVertex();
  // Get up off the floor so it doesn't interfere with LOS tests.
  vPlayer.y += 2.0f;

  VertexBase vHole = pHole->GetTargetVertex();
  vHole.y += 2.0f;
  VertexBase vAimAt = vHole;

  bool found = false;

  // Work out if we should aim for the hole, or aim for a waypoint.
  if (LosExists(vPlayer, vHole) && DistanceIsOk(vPlayer, vHole))
  {
    // We can see the hole, so aim for it.
    found = true;
    m_pTargetObject = pHole;
  }
  else
  {
    // Find the Waypoint with the lowest ID which we can see.
    // This works because Waypoints start at the hole and increase ID as they
    // get nearer to the Tee. Waypoints must be able to see their neighbours.
    // Iterate through map of Game Objects.
    for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
    {
      GameObjectId id = it->first;  
      PGameObject pGo = it->second;
  
      Waypoint* pW = dynamic_cast<Waypoint*>(pGo.GetPtr());
      if (pW)
      {
        Orientation o = *(pW->GetOrientation());
        VertexBase v = o.GetVertex();
        if (LosExists(vPlayer, v) && DistanceIsOk(vPlayer, v))
        {
          // We can see this Waypoint, so aim for it.
          vAimAt = v;
          found = true;
          m_pTargetObject = pW;
          break;
        }
      }
    }
  }

  if (!found)
  {
    // Can't see any waypoints - aim for a bonus instead.
    // Distance to closest bonus so far.
    float bestDist = 99999.9f;

    for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
    {
      GameObjectId id = it->first;  
      PGameObject pGo = it->second;
  
      Bonus* pB = dynamic_cast<Bonus*>(pGo.GetPtr());
      if (pB && pB->GetState() == UNKNOWN)
      {
        // Is the distance from player to this bonus better than
        // the current best distance ?
        VertexBase vBonus = pB->GetOrientation()->GetVertex();
        float dist = (vBonus - vPlayer).Length();
        if (dist < bestDist)
        {
          bool los = LosExists(vPlayer, vBonus);
          if (los)
          {
            bestDist = dist;
            m_pTargetObject = pB;
            vAimAt = vBonus;
            found = true;

            // Keep looking, there may be a closer bonus.
          }
        }
      }
    }
  }

  if (!found)
  {
    // Still can't find anything to aim at!
    // Just aim at some random point ???
    vAimAt = vPlayer; 
    vAimAt += VertexBase(1.0f, 1.0f, 1.0f); 
  }

#ifdef DEBUG_SHOW_DEMO_CALC
  if (m_pTargetObject.GetPtr())
  {
    std::cout << "Aiming for " << m_pTargetObject->GetTypeName() << " " 
      << m_pTargetObject->GetId() << "\n";
  }
  else
  {
    std::cout << "Can't see ANYTHING to aim at!\n";
  }

  std::cout << "Player: (" << vPlayer.x << ", " << vPlayer.y << ", " << vPlayer.z
    << ") Aim at: (" << vAimAt.x << ", " << vAimAt.y << ", " << vAimAt.z << ")\n";
#endif

  float yRot = AngleCompare::TurnToFace(vAimAt.x, vAimAt.z, vPlayer.x, vPlayer.z);
  m_yRot = RadToDeg(yRot);

  float dx = vPlayer.x - vAimAt.x;
  float dz = vPlayer.z - vAimAt.z;
  float distsq = dx * dx + dz * dz;
  float dist = (vPlayer - vAimAt).Length();
 
  static const float velMult = 
    Engine::Instance()->GetConfigFloat("golf_ball_vel_test");
  // NB Relationship is not linear
  m_hVel = velMult * log10(dist);

  // Same as in EngineStatePoolSetUpShot.
  m_vVel = -m_hVel * 1.0f; 
}

void PoolSetUpShotBehaviour::OnActivated()
{
  m_time = 0;

  CalcShot();
  // Now we have decided in which direction to hit the ball. Make the
  // player face in that direction.
   
}

}
