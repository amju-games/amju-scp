/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Trajectory.cpp,v $
Revision 1.1.2.3  2006/08/30 21:12:22  Administrator
Get Pool to build in MSVC

Revision 1.1.2.2  2006/08/14 17:50:26  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:41  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.8.3  2005/09/05 20:09:04  jay
Turn off debug output

Revision 1.1.8.2  2005/07/30 12:28:04  jay
Overhauled trajectory to show directions of multiple balls.

Revision 1.1.8.1  2005/07/17 23:05:09  jay
Unfinished changes for POOL

Revision 1.1  2004/09/08 15:43:07  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <iostream>
#include "Common.h"
#include "AngleCompare.h"
#include "Trajectory.h"
#include "FreeMovingGameObject.h"
#include "Engine.h"
#include "LevelServer.h"
#include "PoolMisc.h"
#include "HeightServer.h"
#include "Controller.h"
#include "Capsule.h"

namespace Amju
{
SharedPtr<SolidComponent> Trajectory::s_pBillBoard;

Trajectory::Trajectory()
{
  m_maxPoints = 0;
}

bool Trajectory::Init()
{
  std::string filename = Engine::Instance()->GetConfigValue("trajectory_bb");

  s_pBillBoard = new BillBoard;
  if (!s_pBillBoard->Load(filename))
  {
    return false;
  }
  return true;
}

void Trajectory::Clear()
{
  m_trs.clear();
}

void Trajectory::BallTr::Draw()
{
//  Engine::Instance()->PushColour(m_colour);
  for (unsigned int i = 0; i <  m_linesegs.size(); i++)
  {
    const std::pair<VertexBase, VertexBase> seg = m_linesegs[i];
#define TR_DOTS
#ifdef TR_DOTS
    const VertexBase& v1 = seg.first;
    const VertexBase& v2 = seg.second;
    VertexBase vDiff = v2 - v1;
    float len = vDiff.Length();
    vDiff.Normalize();
    float t = 0;
    while (t < len)
    {
      VertexBase v(v1);
      v.x += vDiff.x * t;
      v.z += vDiff.z * t;
    
      AmjuGL::PushMatrix();
      AmjuGL::Translate(v.x, v.y, v.z);
      s_pBillBoard->Draw();
      AmjuGL::PopMatrix();

      static const float DOTS_SPACING = Engine::Instance()->
        GetConfigFloat("pool_dots_spacing");
      t += DOTS_SPACING;
    }
#else  
    glBegin(GL_LINES);
    glVertex3f(seg.first.x, seg.first.y, seg.first.z);
    glVertex3f(seg.second.x, seg.second.y, seg.second.z);
    glEnd();
#endif
  }
//  Engine::Instance()->PopColour();
}

void Trajectory::Draw()
{
//  AmjuGL::Disable(AmjuGL::AMJU_DEPTH_READ);
  AmjuGL::Disable(AmjuGL::AMJU_DEPTH_WRITE);

  // Draw each ball trajectory.
  for (BallTrs::iterator it = m_trs.begin(); it != m_trs.end(); ++it) 
  {
    it->second.Draw();
  }
/*
  for (Verts::iterator it = m_vertices.begin(); it != m_vertices.end(); ++it)
  {
    VertexBase& v = *it;
    // Draw a billboard centred on v.
    //Orientation o(v.x, v.y, v.z, 0, 0, 0);
    //s_pBillBoard->SetOrientation(o);
    AmjuGL::PushMatrix();
    AmjuGL::Translate(v.x, v.y, v.z);
    s_pBillBoard->Draw();
    AmjuGL::PopMatrix();
  }
*/

  AmjuGL::Enable(AmjuGL::AMJU_DEPTH_WRITE);
//  AmjuGL::Enable(AmjuGL::AMJU_DEPTH_READ);
}

void Trajectory::SetDeltaTime(float dt)
{
  m_deltaTime = dt;
}

void Trajectory::Recalc(const VertexBase& v0, float yRot, float vertVel, float horVel)
{
  m_v0 = v0;
  m_yRot = yRot;
  m_vertVel = vertVel;
  m_horVel = horVel;
  Update();
}

void Trajectory::SetBallPos(const VertexBase& v)
{
  m_v0 = v;
}

void Trajectory::Update()
{
  if (m_maxPoints == 0)
  {
    m_maxPoints = (int)Engine::Instance()->GetConfigFloat("trajectory_segs");
  }

  m_trs.clear();

#ifdef TR_DEBUG
std::cout << "TRAJECTORY: orig dir: " << m_yRot << "\n";
#endif

  GetLineSegsForBall(GetBall()->GetId(), m_v0, m_yRot);
}

// Get the direction of the object ball.
// The object ball is initially at rest. The Ghost Ball has collided
// with it. Assume ghost ball is at the exact position of first contact.
float GetObjectBallDir(const VertexBase& vObj, const VertexBase& vGhost)
{
  // Get centre line
  VertexBase vDiff = vObj - vGhost;
  vDiff.Normalize();
  // Get angle
  float yRads = atan2(vDiff.x, vDiff.z);
  float yDegs = RadToDeg(yRads);
  return yDegs;
}

void Trajectory::GetLineSegsForBall(
  int ballId, 
  VertexBase v1, 
  float yRot)
{
#ifdef TR_DEBUG
std::cout << "TRAJECTORY: creating line segs for ball " << ballId << "\n";
#endif

  // Work out x/z vector
  float yRads = DegToRad(yRot);
  float dx = sin(yRads);
  float dz = cos(yRads);

  // Number of times we have bounced off a rail.
  int hitrail = 0;

  // Add segments until we reach the max
  for (int i = 0; i < m_maxPoints; i++)
  {
    // Test the capsule from v1 to some distance ahead, for collisions
    // with a ball.
    VertexBase v3 = v1;
    static const float CAPSULE_LENGTH = 50.0f; 
    v3.x += dx * CAPSULE_LENGTH;
    v3.z += dz * CAPSULE_LENGTH;
    int newBallId;
    VertexBase vGhost;
    if (FindClosestBallCollision(ballId, v1, v3, &newBallId, &vGhost))
    {
      // Found collision with ball.
      // Does trajectory already exist for the new ball ?
      // Create one if not, and we haven't reached a limit.
      if (m_trs.find(newBallId) != m_trs.end())
      {
#ifdef TR_DEBUG
std::cout << "Ball " << newBallId << " already in map m_trs..??\n";
#endif
        // Ball exists in map already: Do nothing
      }
      else
      {
#ifdef TR_DEBUG
std::cout << "Find trajectory for new ball " << newBallId << "\n";
#endif
        PPoolGameObject pGo = Engine::Instance()->GetGameObject(newBallId);
        VertexBase vNewBallCentre = pGo->GetBoundingSphere()->GetCentre();
        // Get the new direction, from the new ball pos and the ghost
        //  ball pos. As the current vel is zero, the direction is along
        //  the centre line.
        float newBallDir = GetObjectBallDir(vNewBallCentre, vGhost); 
        // Create a trajectory for this new ball
        // We have to limit the number of trajectories for speed.
        static const int BALL_RECURSE = (int)Engine::Instance()->GetConfigFloat(
          "trajectory_ball_recurse");
        if ((int)m_trs.size() < BALL_RECURSE)
        {
          GetLineSegsForBall(newBallId, vNewBallCentre, newBallDir);
        }
      }
      break; // no more segments for this ball's trajectory.
    }
    else
    {
#ifdef TR_DEBUG
std::cout << "No ball collision, looking for wall collision...\n";
#endif
      VertexBase v2;
      // Get point where we hit wall, v2; and new direction.
      if (FindWallCollision(ballId, v1, yRot, &v2, &yRot))
      {
#ifdef TR_DEBUG
std::cout << "FOUND wall collision!\n";
#endif
        v1 = v2; // new start pos for next segment
        hitrail++;

        // DON'T show bounce angle for now
        // NB This is only accurate if the exact distance from the wall
        // is correct.
        break;
      }
      else
      {
#ifdef TR_DEBUG
std::cout << "NO wall collision!\n";
#endif

        v1 = v2;
      }
    }
  }
}

void GetClosestWall(
  const std::vector<WallPoly>& walls, 
  const VertexBase& v1,
  const VertexBase& v2,
  VertexBase* pHitPoint,
  float* pReflectDegs,
  float yRot)
{
  // TODO 
  // This doesn't work - just here for later.
    float newYRot = 0;
    WallPoly* pClosest = 0;
    float bestDist = 999999.0f;
    VertexBase bestIntersectPoint;
    unsigned int s = walls.size();
    for (unsigned int i = 0; i < s; i++)
    {
      WallPoly* pWp = const_cast<WallPoly*>(&(walls[i]));
      // Get dist
      float dsq = pWp->SqDist(v1);
      float d = sqrt(dsq);

      VertexBase intersectPoint;

#ifdef TR_DEBUG
      bool gotIntPoint = pWp->IntersectGetPoint(v1, v2, &intersectPoint);

std::cout << "  ..checking wall " 
  << pWp << ": dist: " 
  << d << " gotintpoint: " << gotIntPoint 
  << "\n";
#endif
      if (d < bestDist)// && gotIntPoint)
      {
        bestDist = d;
        pClosest = pWp;
        bestIntersectPoint = intersectPoint;
      }
    }
    Assert(pClosest);
    float reflectDegs = Controller::GetReflectionDegs(yRot, pClosest);
    newYRot += reflectDegs;
    *pHitPoint = bestIntersectPoint;
     
    // No longer getting avg
    //newYRot /= (float)s; // i.e. get average angle
    yRot = newYRot; 
}

bool Trajectory::FindWallCollision(
  int ballId,
  VertexBase v1,
  float yRot,
  VertexBase* pEndPoint,
  float* pReflectionDir)
{
  VertexBase v2(v1); // end of current line seg
  // Only look ahead a small distance. 
  // TODO make this more efficient by looking ahead a long way.
  float t = 2.0f; 
  float yRads = DegToRad(yRot);
  float dx = sin(yRads);
  float dz = cos(yRads);
  v2.x += dx * t;  
  v2.z += dz * t;  

  Level* pLevel = LevelServer::Instance()->GetCurrentLevel().GetPtr();

  // Check this position - is it outside the table ?
  // TODO Show bounces off rails
  static const float RADIUS = GetPoolBallRadius();

  if (pLevel->GetScene()->LineIntersects(v1, v2, RADIUS))
  {
    // Fill a HeightServer with wall polys including the one we just hit.
    HeightServer hs;
    BoundingSphere bs(v2, (v1 - v2).Length());
    pLevel->GetScene()->AddHeights(&hs, bs);

    BoundingSphere bsBefore(v1, RADIUS);
    BoundingSphere bsAfter(v2,RADIUS);
    std::vector<WallPoly> walls;
    bool collided = hs.Intersects(bsBefore, bsAfter, &walls);
    if (!collided)
    {
      // That's strange, this heightserver must not be picking up all
      // the wall polys.
      Assert(0);
    }
    // Get unique walls
    std::sort(walls.begin(), walls.end(), PlaneLessThan());
    walls.erase(std::unique(walls.begin(), walls.end()), walls.end());

    // Find the closest wall.
    unsigned int s = walls.size();
#ifdef TR_DEBUG
std::cout << " Trajectory: hitting " << s << " walls, getting refleact angle\n";
#endif

    float newYRot = 0;
    for (unsigned int i = 0; i < s; i++)
    {
      WallPoly* pWp = const_cast<WallPoly*>(&(walls[i]));

      float reflectDegs = Controller::GetReflectionDegs(yRot, pWp);
      newYRot += reflectDegs;
    }
    // TODO Adjust for ball radius
    *pEndPoint = v1;
     
    newYRot /= (float)s; // i.e. get average angle

    *pReflectionDir = newYRot;

    // TODO Also set colour for this ball's trajectory          
//    m_trs[ballId].m_linesegs.push_back(std::make_pair(v1, v2));
    return true;
  }

  // TODO Also set colour for this ball's trajectory          
  m_trs[ballId].m_linesegs.push_back(std::make_pair(v1, v2));
  *pEndPoint = v2;

  return false; // no wall collision
}

// Get the exact ghost ball position for ball pBall moving in direction
// (dx, dz). The ball hits pObjBall.
void GetExactCollisionCoords(
  PoolBall* pBall, 
  float dx, 
  float dz, 
  PoolBall* pObjBall, 
  VertexBase* pVGhost)
{
  // Err... do we need to search for this position, or can we just 
  // work it out ?!??!
  // TODO
 
  // Binary chop position until the distance between the 2 balls is
  // 2 * the balls' radius.
  // No need to worry about vel/accel etc.
  VertexBase v1 = pBall->GetBoundingSphere()->GetCentre();
  VertexBase vObj = pObjBall->GetBoundingSphere()->GetCentre();
  VertexBase vDiff = v1 - vObj;

  // We will chop this max length until we get to 2R.
  float len = vDiff.Length();

  static double TWO_R = 0;
  if (TWO_R == 0)
  {
    // Get the radius of a ball - they are all the same, so use the
    // cue ball.
    double r = GetBall()->GetBoundingSphere()->GetRadius();
    TWO_R = 2.0 * r;
  }

  // Start half-way between the balls.
  float dt = len * 0.5f;

  bool broke = false;
  int count = 0;
  float t = 0;
  VertexBase vDir(dx, 0, dz);
  while (count++ < 10) // TODO TEMP TEST
  {
    // Get new position at distance t
    VertexBase v2 = v1;
    v2.x += dx * t;
    v2.z += dz * t;
    float dist = (v2 - vObj).Length();

#ifdef TR_DEBUG
std::cout << "Trajectory chop: count: " << count << " dist: " << dist << "\n";
#endif

    // Is dist < or > 2R ?
    // Do the OPPOSITE of the code in PoolBall. There we are trying to
    // pull two balls apart until they just touch. Here we are bringing
    // them together until they touch.
    if (dist > TWO_R)
    {
#ifdef TR_DEBUG
std::cout << " ..distance too big\n";
#endif
      t -= dt;
    }
    else if (dist < TWO_R)
    {
#ifdef TR_DEBUG
std::cout << " ..distance too small\n";
#endif
//      Assert(t >= dt && "Balls were intersecting already ?!");
      // At this t, the balls are intersecting. We want to find an earlier t.
      t += dt;
    }
    else
    {
      broke = true;
#ifdef TR_DEBUG
std::cout << " just right! Breaking after " << count << " loops, dist=" << dist << "\n";
#endif
      break;
    }
    dt *= 0.5f;
  }
  VertexBase v2 = v1;
  v2.x += dx * t;
  v2.z += dz * t;
  *pVGhost = v2;
#ifdef TR_DEBUG
std::cout << " Final ghost ball pos: " << ToString(*pVGhost) << "\n";
#endif
}

bool Trajectory::FindClosestBallCollision(
  int ballId,
  const VertexBase& v1,
  const VertexBase& v2,
  int* pNewBallId,
  VertexBase* pVGhost)
{
  static const float RADIUS = GetPoolBallRadius();

  PoolBall* pBall = dynamic_cast<PoolBall*>(Engine::Instance()->
    GetGameObject(ballId).GetPtr());
  Assert(pBall);

  Level* pLevel = LevelServer::Instance()->GetCurrentLevel().GetPtr();

  // Test if line v1-v2 hits a ball. If so find the first ball
  // and point of first contact. Get the new direction for the current
  // trajectory, and start a new trajectory for the ball just hit.
  // Get all balls in current room
  Assert(pLevel);

  // Current best distance (we want the closest ball)
  float bestLen = 999999.0f;
  // Closest ball
  PoolBall* pBestBall = 0;

  // Get the game objects which are in the same room as the player.
  int levelId = pLevel->GetId();
  int roomId = pLevel->GetRoomId();
  GameObjectMap& objs = Engine::Instance()->GetGameObjects(levelId, roomId);
  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    PPoolGameObject pGo = it->second;
    PoolBall* p = dynamic_cast<PoolBall*>(pGo.GetPtr());
    // TODO Each ball may have its own trajectory. Ignore balls which
    // already have one.
    if (p && p != pBall && p->GetState() == UNKNOWN && !IsCueBall(p))
    {
      // Capsule v1-v2 intersects ball p ?
      BoundingSphere bsBall = *(p->GetBoundingSphere());
      Capsule c2(v1, v2, RADIUS);
      bool b = c2.Intersects(bsBall);
#ifdef TR_DEBUG
      Capsule c1(bsBall.GetCentre(), bsBall.GetCentre(), RADIUS);
      bool b2 = c2.Intersects(c1);
      bool b3 = c1.Intersects(c2); 
      if (b != b2)
      {
        std::cout << "CAPSULE BUG: b != b2\n";
      }
      if (b != b3)
      {
        std::cout << "CAPSULE BUG: b != b3\n";
      }
      if (b2 != b3)
      {
        std::cout << "CAPSULE BUG: b2 != b3\n";
      }
      // TODO output debug data
#endif

      if (b)
      {
#ifdef TR_DEBUG
std::cout << "TRAJECTORY: intersects ball " << p->GetNumber() << "\n";
#endif
        // Get distance
        VertexBase vDiff = v1 - bsBall.GetCentre();
        float len = vDiff.Length();
        if (len < bestLen)
        {
          bestLen = len;
          pBestBall = p;
        }
      }
    }
  }

  if (pBestBall)
  {
#ifdef TR_DEBUG
std::cout << "TRAJECTORY: COLLISION FOUND: ball " 
  << pBall->GetNumber() << " hits " << pBestBall->GetNumber() << "\n";
#endif

    *pNewBallId = pBestBall->GetId(); 

    // Get ghost ball centre. I.e. find first position along line
    //  v1-v2 where ball is intersected. 
    //float collisionTime = 0;
    VertexBase resultV1, resultV2;

    VertexBase vDiff = v1 - v2;
    vDiff.Normalize();
    float dx = vDiff.x;
    float dz = vDiff.z; 
    GetExactCollisionCoords(pBall, dx, dz, pBestBall, pVGhost);

    // TODO depends on ball colour etc
    m_trs[ballId].m_linesegs.push_back(std::make_pair(v1, *pVGhost));
    return true;
  }
  return false;
}
}

