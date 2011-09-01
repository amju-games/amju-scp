/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: PoolBehaviour.cpp,v $
Revision 1.1.2.4  2007/06/10 21:33:20  Administrator
Check for extra states BALL_IN_POCKET and BALL_OUT_OF_BOUNDS

Revision 1.1.2.3  2006/08/30 21:12:18  Administrator
Get Pool to build in MSVC

Revision 1.1.2.2  2006/08/14 17:50:21  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:37  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.22  2005/09/29 19:09:36  jay
Use new func IsPoolBall to distinguish between real balls and subclassed
objects.

Revision 1.1.2.21  2005/09/23 19:33:51  jay
Use PoolChangeState

Revision 1.1.2.20  2005/09/13 23:08:06  jay
Count balls as valid as long as they are not OUT_OF_PLAY - i.e. they may be
hidden in "CRAZY" levels.

Revision 1.1.2.19  2005/09/11 19:02:50  jay
Add CalcPlaceCueBall() - work out where AI should place cue ball after a foul.

Revision 1.1.2.18  2005/09/05 20:07:11  jay
Turn off debug output

Revision 1.1.2.17  2005/09/02 18:17:40  jay
Increase angle for LOS to pockets

Revision 1.1.2.16  2005/08/26 21:56:45  jay
Penalise bounce shots, especially on the final shot

Revision 1.1.2.15  2005/08/06 18:41:09  Administrator
Turn off free ball for AI, it's broken.

Revision 1.1.2.14  2005/08/06 17:56:41  Administrator
Allow AI to take free shots

Revision 1.1.2.13  2005/08/02 18:30:13  jay
Remove m_pRules. This breaks Undo and isn't even necessary.

Revision 1.1.2.12  2005/07/30 12:25:22  jay
Added Rail::LineIntersects, was to be used by trajectory but is now spare.

Revision 1.1.2.11  2005/07/17 23:04:13  jay
Just comments; commented out some useless code (ranking)

Revision 1.1.2.10  2005/07/05 09:39:01  jay
Set jump shot H and V Vel

Revision 1.1.2.9  2005/06/29 20:23:53  jay
Store all shots, then sort and pick best one. Allows us to show all
shots considered, for debugging.

Revision 1.1.2.8  2005/06/22 21:43:25  jay
Added US 8 Ball

Revision 1.1.2.7  2005/06/19 21:35:25  jay
Construct with Rules object

Revision 1.1.2.6  2005/06/13 22:14:35  jay
Bounce off rails; draw trajectories for debugging

Revision 1.1.2.5  2005/06/04 22:37:18  jay
Recognise when unable to pot a ball and when totally snookered -
not finished. Also need to consider rail shots to hit a valid ball.

Revision 1.1.2.4  2005/05/15 17:20:25  jay
Ongoing improvements to AI; aim at correct ball on break.

Revision 1.1.2.3  2005/05/08 09:55:48  jay
Ongoing improvements to AI

Revision 1.1.2.2  2005/04/11 21:23:53  jay
Ongoing fixes

Revision 1.1.2.1  2005/03/31 22:31:44  jay
Pool AI: initial revision is totally stupid.

*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "Common.h"
#include "PoolBehaviour.h"
#include "Engine.h"
#include "GameState.h"
#include "EngineStatePoolShowShot.h"
#include "EngineStatePoolShowShotDemo.h"
#include "PoolMisc.h"
#include "LevelServer.h"
#include "AngleCompare.h"
#include "StringUtils.h"
#include "StateStrings.h"
#include "RulesUk.h"
#include "RulesUs8Ball.h"
#include "TextWriter.h"
#include "Mgc/MgcDist3DLinLin.h"
#include "LevelServer.h"
#include "PoolChangeState.h"

#ifdef _DEBUG
//#define AI_DEBUG
//#define RAIL_DEBUG
#endif

namespace Amju
{
Rules* PoolBehaviour::GetRules()
{
  Level* pLevel = LevelServer::Instance()->GetCurrentLevel().GetPtr();
  return Amju::GetRules(pLevel);
}

PoolBehaviour::PoolBehaviour()
{
  m_pHudBall = 0;
}

int PoolBehaviour::GetNumValidShots() const
{
  return m_validShots.size();
}

void PoolBehaviour::DrawValidShot(int i)
{
  m_validShots[i].Draw();
}

std::string PoolBehaviour::ValidShotString(int i)
{
  return m_validShots[i].ToString();
}

static const float DRAW_Y = 2.0f; 

PoolBehaviour::ValidShot::ValidShot(
  ShotType shotType, 
  GameObject* pTargetObject, 
  PoolHole* pPocket, 
  Geom2d::Point2d* pRailPos, 
  BoundingSphere* pGhostBall,
  float rank)
{
  m_shotType = shotType;
  m_pTargetObject = pTargetObject;
  m_pPocket = pPocket;
  if (pRailPos)
  {
    m_railPos = *pRailPos;
  }
  if (pGhostBall)
  {
    m_ghostBall = *pGhostBall;
  }
  m_rank = rank;
  m_cutAngle = 0;
  m_dist = 0;
  m_hVel = 0;
  m_vVel = 0;
}

bool PoolBehaviour::ValidShot::operator<(const PoolBehaviour::ValidShot& rhs) const
{
  // Reverse so we get top rank first
  return m_rank > rhs.m_rank;
}

std::string PoolBehaviour::ValidShot::ToString()
{
  std::string s;
  switch (m_shotType)
  {
  case BREAK: 
    s = "break"; 
    break;
  case POT: 
    s = "pot"; 
    break;
  case RAIL_POT:
    s = "rail pot";
    break;
  case HIT:
    s = "hit";
    break;
  case RAIL_HIT:
    s = "rail hit";
    break;
  case JUMP:
    s = "jump";
    break;
  default:
    s = "unknown";
  }
  s += " ball: ";
  s += Amju::ToString(((PoolBall*)(m_pTargetObject))->GetNumber());
  s += " rank: ";
  s += Amju::ToString(m_rank);
  s += " cut: ";
  s += Amju::ToString(m_cutAngle);
  return s;
}

void PoolBehaviour::ValidShot::Draw()
{
  VertexBase vCue = GetBall()->GetBoundingSphere()->GetCentre();
  // Rail/bounce shot ? If so draw line from cue ball to rail
  if (m_shotType == RAIL_POT || m_shotType == RAIL_HIT)
  {
    VertexBase vObj = m_pTargetObject->GetBoundingSphere()->GetCentre();
/*
    glBegin(GL_LINES);
      glVertex3f(vCue.x, DRAW_Y, vCue.z);
      glVertex3f(m_railPos.x, DRAW_Y, m_railPos.y);
      glVertex3f(m_railPos.x, DRAW_Y, m_railPos.y);
      glVertex3f(vObj.x, DRAW_Y, vObj.z);
    glEnd();
*/
  }
  else
  {
    // Draw line from cue ball to ghost ball
/*
    glBegin(GL_LINES);
      glVertex3f(vCue.x, DRAW_Y, vCue.z);
      glVertex3f(m_ghostBall.GetCentre().x, DRAW_Y, m_ghostBall.GetCentre().z);
    glEnd();
*/
  } 
  m_ghostBall.Draw();
  // Draw line from object ball to pocket
  if (m_pPocket)
  {
    VertexBase vPoc = m_pPocket->GetBoundingSphere()->GetCentre();
    VertexBase vObj = m_pTargetObject->GetBoundingSphere()->GetCentre();
/*
    glBegin(GL_LINES);
      glVertex3f(vPoc.x, DRAW_Y, vPoc.z);
      glVertex3f(vObj.x, DRAW_Y, vObj.z);
    glEnd();
*/
  }

  // Draw lines from object ball to all visible pockets
  for (unsigned int i = 0; i < m_pockets.size(); i++)
  {
    VertexBase vObj = m_pTargetObject->GetBoundingSphere()->GetCentre();
    VertexBase vPoc = m_pockets[i]->GetBoundingSphere()->GetCentre();
/*
    glLineWidth(1);
    glBegin(GL_LINES);
      glVertex3f(vPoc.x, DRAW_Y, vPoc.z);
      glVertex3f(vObj.x, DRAW_Y, vObj.z);
    glEnd();
    glLineWidth(2);
*/
  }
}

void PoolBehaviour::Draw()
{
#ifdef AI_DEBUG
  for (int i = 0; i < m_rails.size(); i++)
  {
    m_rails[i].Draw();
  }
  VertexBase vCue = GetBall()->GetBoundingSphere()->GetCentre();
  // Rail/bounce shot ? If so draw line from cue ball to rail
  if (m_isBounceShot)
  {
    VertexBase vObj = m_pTargetObject->GetBoundingSphere()->GetCentre();
    glBegin(GL_LINES);
      glVertex3f(vCue.x, DRAW_Y, vCue.z);
      glVertex3f(m_railPos.x, DRAW_Y, m_railPos.y);
      glVertex3f(m_railPos.x, DRAW_Y, m_railPos.y);
      glVertex3f(vObj.x, DRAW_Y, vObj.z);
    glEnd();
    // Draw perpendiculars
    glLineWidth(1);
    glBegin(GL_LINES);
      glVertex3f(vCue.x, DRAW_Y, vCue.z);
      glVertex3f(m_perpPt1.x, DRAW_Y, m_perpPt1.y);
      glVertex3f(vObj.x, DRAW_Y, vObj.z);
      glVertex3f(m_perpPt2.x, DRAW_Y, m_perpPt2.y);
    glEnd();
    glLineWidth(2);
  }
  else
  {
    // Draw line from cue ball to ghost ball
    glBegin(GL_LINES);
      glVertex3f(vCue.x, DRAW_Y, vCue.z);
      glVertex3f(m_ghostBall.GetCentre().x, DRAW_Y, m_ghostBall.GetCentre().z);
    glEnd();
  } 
  m_ghostBall.Draw();
  // Draw line from object ball to pocket
  if (m_pPocket)
  {
    VertexBase vPoc = m_pPocket->GetBoundingSphere()->GetCentre();
    VertexBase vObj = m_pTargetObject->GetBoundingSphere()->GetCentre();
    glBegin(GL_LINES);
      glVertex3f(vPoc.x, DRAW_Y, vPoc.z);
      glVertex3f(vObj.x, DRAW_Y, vObj.z);
    glEnd();
  }

#endif
}

const PoolBehaviour::Rails& PoolBehaviour::GetRails()
{
  return m_rails;
}

void PoolBehaviour::CreateRails()
{
  m_rails.clear();
  // This works by creating a rail between each adjacent pair of Pockets
  // (PoolHole objects). The PoolHoles need to be in order around the table.
  int num = m_pockets.size();
  if (num < 4)
  {
#ifdef AI_DEBUG
std::cout << "RAILS: none created, not enough pockets: " << num << "\n";
#endif

    return;
  }
  for (int i = 0; i < num - 1; i++)
  {
    VertexBase v1 = m_pockets[i]->GetBoundingSphere()->GetCentre();
    VertexBase v2 = m_pockets[i + 1]->GetBoundingSphere()->GetCentre();
    Geom2d::Point2d pt1(v1.x, v1.z);
    Geom2d::Point2d pt2(v2.x, v2.z);
    Geom2d::Line2d lineseg(pt1, pt2);
    m_rails.push_back(Rail(lineseg, i));
  }  
  VertexBase v1 = m_pockets[num - 1]->GetBoundingSphere()->GetCentre();
  VertexBase v2 = m_pockets[0]->GetBoundingSphere()->GetCentre();
  Geom2d::Point2d pt1(v1.x, v1.z);
  Geom2d::Point2d pt2(v2.x, v2.z);
  Geom2d::Line2d lineseg(pt1, pt2);
  m_rails.push_back(Rail(lineseg, num - 1));
}

PoolBehaviour::Rail::Rail(const Geom2d::Line2d& lineseg, int id) : 
  m_lineseg(lineseg), m_id(id)
{
#ifdef RAIL_DEBUG_2
std::cout << "RAIL: Creating a rail: " << ToString(lineseg.pt1).c_str() << " - " 
  << ToString(lineseg.pt2).c_str() << "\n";
#endif
}

#ifdef AI_DEBUG
void PoolBehaviour::Rail::Draw()
{
  glLineWidth(3);
  glBegin(GL_LINES);
    glVertex3f(m_lineseg.pt1.x, DRAW_Y, m_lineseg.pt1.y);
    glVertex3f(m_lineseg.pt2.x, DRAW_Y, m_lineseg.pt2.y);
  glEnd();
}
#endif

bool PoolBehaviour::Rail::Bounce(
  const Geom2d::Point2d& cue,
  const Geom2d::Point2d& obj,
  Geom2d::Point2d* pGhost,
  Geom2d::Point2d* pPerpPt1,
  Geom2d::Point2d* pPerpPt2)
{
  // To find the ghost ball position, we must be able to draw a
  // perpendicular from m_lineseg to the cue and object balls.
  float y1, y2; // lengths of perps
  Geom2d::Point2d pt1, pt2; // points on m_lineseg where perps cross
  if (!GetPerpendicular(cue, &pt1, &y1) ||
      !GetPerpendicular(obj, &pt2, &y2))
  {
#ifdef RAIL_DEBUG_NO_BOUNCE
std::cout << "RAIL " 
  << m_id 
  << ": NO BOUNCE from " 
  << ToString(cue).c_str() 
  << " to " 
  << ToString(obj).c_str() << "\n";
#endif
    return false;
  }
  Assert(y1 >= 0);
  Assert(y2 >= 0);

  // Reject the bounce if the perp from the object ball to the rail is more
  // than some multiple of the perp from the cue ball to the rail.
  // The reason is that this shot is not likely to work.
  float multiple = y2 / y1;
  static const float MAX_BOUNCE_MULTIPLE = Engine::Instance()->GetConfigFloat(
    "max_bounce_multiple");
  if (multiple > MAX_BOUNCE_MULTIPLE)
  {
#ifdef AI_DEBUG
std::cout << "Bounce rejected, multiple " << multiple << " too great.\n";
#endif 
    return false;
  }

#ifdef DP_DEBUG
  // Found 2 perpendiculars to the same line: they should be parallel.
  Geom2d::Point2d test1 = cue - pt1;
  Geom2d::Point2d test2 = obj - pt2;
  test1.Normalize();
  test2.Normalize();
  float dp = fabs(Geom2d::DotProduct(test1, test2));
  if (dp != 1.0f)
  {
    std::cout << "Dot product of 2 perpendics. to the same line: " << dp << "\n";
  }
  Assert(dp >= 0.9999f);
#endif

  // Reject the shot if one or both of the points on the rail are too close
  // to a pocket - the ball will bounce off the mouth in the wrong direction.
  // Get the distance to the ends of the rail for each point, and reject if
  // any distance is less than some limit.
  // TODO Limit depends on table size ?
  static const float RAIL_END = Engine::Instance()->
    GetConfigFloat("pool_rail_end");
  if ((pt1 - m_lineseg.pt1).Magnitude() < RAIL_END  ||
      (pt1 - m_lineseg.pt2).Magnitude() < RAIL_END  ||
      (pt2 - m_lineseg.pt1).Magnitude() < RAIL_END  ||
      (pt2 - m_lineseg.pt2).Magnitude() < RAIL_END )
  {
#ifdef AI_DEBUG
std::cout << "Bounce shot rejected, too close to pocket.\n";
// TODO details
#endif
    return false;
  }

  // This Rail spans the centres of two adjacent Pool Holes, and because 
  // of this is 'behind' the real cushion on the table solid.
  // Subtract some magic number from the perpendicular lengths to get
  // a more realistic ghost pos.
  // Also EVEN IF this rail exactly matched the table cushion, we need to 
  // subtract the ball radius from the perp. length, because the ball centre
  // bounces back at distance R from the cushion.
  static const float PERP_DIST = Engine::Instance()->
    GetConfigFloat("pool_perp_dist");
  y1 -= PERP_DIST; // includes R
  y2 -= PERP_DIST;
  if (y1 < 0 || y2 < 0)
  {
    return false;
  }
  // Shorten the distance from pt1 to cue and pt2 to obj, by the same amount,
  // i.e. PERP_DIST.
  // Get line from pt1 to cue, Normalise, mult by PERP_DIST, add to pt1.
  Geom2d::Point2d fixpt1 = cue - pt1;
  fixpt1.Normalize();
  fixpt1 *= PERP_DIST;
  pt1 = pt1 + fixpt1;

  Geom2d::Point2d fixpt2 = obj - pt2;
  fixpt2.Normalize();
  fixpt2 *= PERP_DIST;
  pt2 = pt2 + fixpt2;

  *pPerpPt1 = pt1;
  *pPerpPt2 = pt2;

  // Similar triangles: we split line (pt1, pt2) with ratio x1:x2.
  // x1/x2 == y1/y2.
  // => So x1/(x1+x2) == y1/(y1+y2) (yes, multiplied out to prove)
  Geom2d::Point2d ghostPt = pt1 - pt2;
  float y = y2 / (y1 + y2);
  Assert(y >= 0);
  Assert(y <= 1.0f);
  ghostPt *= y;
  ghostPt = pt2 + ghostPt;
  *pGhost = ghostPt;

#ifdef RAIL_DEBUG_BOUNCE
std::cout << "RAIL " 
  << m_id 
  << ": FOUND bounce from " 
  << ToString(cue).c_str() 
  << " to " 
  << ToString(obj).c_str() 
  << " Ghost pos: " 
  << ToString(*pGhost).c_str()
  << "\n";
#endif
  return true;
}

/*

// Distance from point to line sample code, from 
// http://astronomy.swin.edu.au/~pbourke/geometry/pointline/source.c
//
struct tagXYZ
{
    float X, Y, Z;
}
XYZ;

float Magnitude( XYZ *Point1, XYZ *Point2 )
{
    XYZ Vector;

    Vector.X = Point2->X - Point1->X;
    Vector.Y = Point2->Y - Point1->Y;
    Vector.Z = Point2->Z - Point1->Z;

    return (float)sqrt( Vector.X * Vector.X + Vector.Y * Vector.Y + Vector.Z * Vector.Z );
}

int DistancePointLine( XYZ *Point, XYZ *LineStart, XYZ *LineEnd, float *Distance )
{
    float LineMag;
    float U;
    XYZ Intersection;
 
    LineMag = Magnitude( LineEnd, LineStart );
 
    U = ( ( ( Point->X - LineStart->X ) * ( LineEnd->X - LineStart->X ) ) +
        ( ( Point->Y - LineStart->Y ) * ( LineEnd->Y - LineStart->Y ) ) +
        ( ( Point->Z - LineStart->Z ) * ( LineEnd->Z - LineStart->Z ) ) ) /
        ( LineMag * LineMag );
 
    if( U < 0.0f || U > 1.0f )
        return 0;   // closest point does not fall within the line segment
 
    Intersection.X = LineStart->X + U * ( LineEnd->X - LineStart->X );
    Intersection.Y = LineStart->Y + U * ( LineEnd->Y - LineStart->Y );
    Intersection.Z = LineStart->Z + U * ( LineEnd->Z - LineStart->Z );
 
    *Distance = Magnitude( Point, &Intersection );
 
    return 1;
}

*/

bool PoolBehaviour::Rail::GetPerpendicular(
  const Geom2d::Point2d& pt,
  Geom2d::Point2d* pPointOnLine,
  float* pLineLen)
{
  // Find perpendicular line from pt to m_lineseg, and get the point
  // on m_lineseg.
  float lineMag = m_lineseg.Length();

  float u = (((pt.x - m_lineseg.pt1.x) * (m_lineseg.pt2.x - m_lineseg.pt1.x)) + 
             ((pt.y - m_lineseg.pt1.y) * (m_lineseg.pt2.y - m_lineseg.pt1.y))) /
             (lineMag * lineMag);
  if (u < 0 || u > 1.0f)
  {
    return false;
  }
  pPointOnLine->x = m_lineseg.pt1.x + u * (m_lineseg.pt2.x - m_lineseg.pt1.x);
  pPointOnLine->y = m_lineseg.pt1.y + u * (m_lineseg.pt2.y - m_lineseg.pt1.y);
  *pLineLen = (pt - *pPointOnLine).Magnitude();

#ifdef RAIL_DEBUG_GET_PERP
std::cout << "RAIL " << m_id 
  << " found perpendicular from point " 
  << ToString(pt).c_str() 
  << " intersect: " << ToString(*pPointOnLine).c_str() 
  << " len: " << *pLineLen 
  << "\n";
#endif

  return true;
}

bool PoolBehaviour::Rail::LineIntersects(
  const Geom2d::Line2d& line,
  Geom2d::Point2d* pIntersectPoint) const
{
  // TODO Also could use Geom2d::Intersects

  Mgc::Segment3 mLine1;
  mLine1.Origin() = Mgc::Vector3(line.pt1.x, 0, line.pt1.y);
  mLine1.Direction() = Mgc::Vector3(
    line.pt2.x - line.pt1.x,
    0,
    line.pt2.y - line.pt1.y);

  Mgc::Segment3 mLine2;
  mLine2.Origin() = Mgc::Vector3(m_lineseg.pt1.x, 0, m_lineseg.pt1.y);
  mLine2.Direction() = Mgc::Vector3(
    m_lineseg.pt2.x - m_lineseg.pt1.x,
    0,
    m_lineseg.pt2.y - m_lineseg.pt1.y);

  // TODO Intersection point
  float x = 0, z = 0;
  float dsq = Mgc::SqrDistance(mLine1, mLine2, &x, &z); 
  *pIntersectPoint = Geom2d::Point2d(x, z);
  return (dsq < 0.01f);
}

Geom2d::Line2d PoolBehaviour::Rail::GetLineseg() const
{
  return m_lineseg;
}

void PoolBehaviour::Update()
{
  // TODO TEMP TEST pause for debugging
  static const float waitTime = Engine::Instance()->GetConfigFloat("golf_wait");
  static const float accel = Engine::Instance()->GetConfigFloat("golf_ball_accel");

  float dt = Engine::Instance()->GetDeltaTime();
  m_time += dt;

  // TODO Move around the ball to face the direction of the shot.
  
  if (m_time < waitTime)
  {
    return;
  }

  // Set the shot data, then change to state which shows player swinging the club.
  Engine::Instance()->GetGameState()->GetCurrentPlayerInfo()->m_golfStroke =
        PoolGameState::PlayerInfo::PoolStroke(m_yRot, m_vVel, m_hVel, accel,
        0, 0);  // TODO english, draw/roll

  ChangeStateToShowShot();
}

void PoolBehaviour::OnActivated()
{
  m_time = 0;
  CalcShot();
}

bool PoolBehaviour::IsLos(
  const VertexBase& v1,
  const VertexBase& v2, 
  float r,
  GameObject* p1, 
  GameObject* p2,
  bool showDebug) 
{
  // Check LOS between two points.
  // Ignore objects p1 and p2.

  PLevel pLevel = LevelServer::Instance()->GetCurrentLevel();


  // TODO Table test causes obvious pots to be missed ???

  // Does scene (i.e. the pool table) intersect ?
  bool sceneInt = pLevel->GetScene()->LineIntersects(v1, v2, r);
  if (sceneInt)
  {
    return false;
  }

  // Find out if the capsule intersects any GameObjects' collision vol.
  int levelId = pLevel->GetId();
  int roomId = pLevel->GetRoomId();
  
  // Get the game objects which are in the same room as the player. 
  GameObjectMap& objs = Engine::Instance()->GetGameObjects(levelId, roomId);
  
  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  { 
    GameObject* pGo = it->second.GetPtr();
    if (pGo == p1 || pGo == p2)
    {
      continue;
    }

    // Only check for Pool Balls - we especially don't want to hit the
    // non headstring area.
    PoolBall* pVis = dynamic_cast<PoolBall*>(pGo);

#ifdef LOS_DEBUG
if (showDebug)
{ 
  if (pVis)
  {
    std::cout << " LOS: ball " << pVis->GetNumber() << " " 
      << StateStrings[(int)pVis->GetState()] << " ";
    if (pVis->GetState() == UNKNOWN &&
        pVis->GetBoundingSphere()->Intersects(v1, v2, r))
    {
      std::cout << "OBSTRUCTS!\n";
    }
    else
    {
      std::cout << "does not obstruct\n";
    }
  }
}
#endif

    if (pVis &&  
        pVis->GetState() == UNKNOWN &&
        pVis->GetBoundingSphere()->Intersects(v1, v2, r))
    {
      return false;
    }
  }
  return true;
}

bool PoolBehaviour::IsLos(GameObject* p1, GameObject* p2) 
{
  // Find LOS between the two objects. Need to account for
  // - static scene
  // - other balls blocking LOS.
  // - radius of object spheres.

  static const float radius = GetPoolBallRadius(); 

  bool b =  IsLos(p1->GetBoundingSphere()->GetCentre(),
    p2->GetBoundingSphere()->GetCentre(), radius, p1, p2);

#ifdef AI_DEBUG
if (b)
{
  std::cout << "There IS LOS between " << p1->GetId() << " (" 
            << p1->GetTypeName() << ") (" 
            << ToString(p1->GetBoundingSphere()->GetCentre()).c_str() << ")\n" 
            << "  and " << p2->GetId() << " (" << p2->GetTypeName() << ") (" 
            << ToString(p2->GetBoundingSphere()->GetCentre()).c_str() << ")\n";
}
else
{
//  std::cout << "There is no LOS between " << p1->GetId() << " and " << p2->GetId() << "\n";
}
#endif
  
  return b;
}

PoolBehaviour::Pockets PoolBehaviour::GetLosPockets(PoolBall* pb)
{
  Pockets pockets; // pockets in current level/room

  // Reduce the radius, so we see the pocket from a wider range of positions.
  // This is because the object centres are quite a way back and so the 
  // mouth of the pocket can hide the centre.
  static const float radius = GetPoolBallRadius() * 0.2f; 

  // For each object in m_pockets, is there LOS to ball pb ? If so 
  // add the obj. to pockets.
  for (unsigned int i = 0; i < m_pockets.size(); i++)
  {
    PoolHole* pPocket = m_pockets[i];
    VertexBase v1 = pb->GetBoundingSphere()->GetCentre();
    VertexBase v2 = pPocket->GetBoundingSphere()->GetCentre();
    v2.y = v1.y; // pockets are lower than balls
    if (IsLos(v1, v2, radius, pb, 0))
    {
      pockets.push_back(pPocket);
    }
  }

  return pockets;
}

void PoolBehaviour::FindPockets()
{
  m_pockets.clear();
  Level* pLevel = LevelServer::Instance()->GetCurrentLevel().GetPtr();
  Assert(pLevel);

  // Get the game objects which are in the same room as the player.
  int levelId = pLevel->GetId();
  int roomId = pLevel->GetRoomId();
  GameObjectMap& objs = Engine::Instance()->GetGameObjects(levelId, roomId);
  // Iterate through map of Game Objects. 
  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  { 
    GameObject* pGo = it->second.GetPtr();
    PoolHole* pHole = dynamic_cast<PoolHole*>(pGo);
    if (pHole)
    {
      m_pockets.push_back(pHole);
    }
  }
}
        
float GetCutAngle(const VertexBase& vCue, const VertexBase& vObj, const VertexBase& vPocket)
{
  VertexBase line1 = vCue - vObj;
  VertexBase line2 = vObj - vPocket;
  Geom2d::Vec2d v21(line1.x, line1.z);
  v21.Normalize(); // necessary 
  Geom2d::Vec2d v22(line2.x, line2.z);
  v22.Normalize();
  float dp = Geom2d::DotProduct(v21, v22);

  float rads = acos(dp);
  float degs = RadToDeg(rads);
  return degs;
}

VertexBase PoolBehaviour::GetPlaceCueBallPos() const
{
  return m_placeCueBallPos;
}

bool PoolBehaviour::CalcPlaceCueBallPos()
{
#ifdef CALC_PLACE_DEBUG
std::cout << "CALC PLACE CUE BALL:\n";
#endif

  // Find valid balls with LOS to a pocket
  // For each (ball, pocket) pair 
  //   Pick a position behind the object ball. 
  //   Get the ghost ball - is there LOS ?
  //   If so this position is ok.

  static const float PLACE_MAX_DIST = Engine::Instance()->GetConfigFloat(
    "calc_place_max_dist");
  static const float PLACE_MIN_DIST = Engine::Instance()->GetConfigFloat(
    "calc_place_min_dist");
  static const float radius = GetPoolBallRadius();

  FreeMovingGameObject* pCueBall = GetBall();

  // Rank each position. If the current rank is higher than the best,
  // it becomes the new best.
  VertexBase vBestPos;
  float bestPosRank = 0; 

  ValidBalls vb = GetValidBalls();
  Assert(!vb.empty());

  for (unsigned int i = 0; i < vb.size(); i++)
  {
    // Get pockets to which this object ball has LOS.
    Pockets pockets = GetLosPockets(vb[i]);

    for (unsigned int j = 0; j < pockets.size(); j++)
    {
      VertexBase vPocket = pockets[j]->GetBoundingSphere()->GetCentre();
      VertexBase vObj = vb[i]->GetBoundingSphere()->GetCentre();
      vPocket.y = vObj.y; // Just get Y-values the same for LOS test.

#ifdef CALC_PLACE_DEBUG
std::cout << "..considering ball " << vb[i]->GetNumber() 
    << " and pocket " << pockets[j]->GetId() << "\n";
#endif
 
      // Get a candidate cue ball position - i.e. on the line made from
      // the pocket to the object ball.
      // Find out if this candidate pos has LOS, and if so, rank this
      // shot.
      VertexBase vCue; // candidate position
      // Distance: random, between 2 extremes.
      float r = 
        (float)rand() / (float)RAND_MAX * (PLACE_MAX_DIST - PLACE_MIN_DIST) +
        PLACE_MIN_DIST;

      VertexBase vDiff = vObj - vPocket;
      vDiff.Normalize();
      vDiff *= r;
      vCue = vObj + vDiff;

      float rank = 0;
      BoundingSphere ghost; // pos of ghost ball - we don't use it here.
      float hvel = 0, cut = 0; // more unused shot data.
      if (CalcPotShot(vCue, vObj, vPocket, radius, &rank, &ghost, &hvel, &cut))
      {
        if (rank > bestPosRank)
        {
          vBestPos = vCue;
          bestPosRank = rank;
#ifdef CALC_PLACE_DEBUG
std::cout << "..best POT SHOT so far, rank: " << rank << "\n";
#endif
        }
      }      
    }
  }
  if (bestPosRank > 0)
  {
#ifdef CALC_PLACE_DEBUG
std::cout << "Best place cue ball pos found - can pot a ball!\n";
#endif

    m_placeCueBallPos = vBestPos;
    return true;
  } 

  // No pos found where a ball can be potted.
  // Try to find a pos where we can at least hit a valid ball.
  for (unsigned int i = 0; i < vb.size(); i++)
  {
    VertexBase vObj = vb[i]->GetBoundingSphere()->GetCentre();

#ifdef CALC_PLACE_DEBUG
std::cout << "..considering ball " << vb[i]->GetNumber() 
    << " for no-pot shot.\n";
#endif

    // Try a range of angles/distances around this object ball.
    // Then rank each candidate cue ball position.
    for (float a = 0; a < 360.0f; a += 30.0f) // TODO CONFIG
    {
      float rad = DegToRad(a);
      float r = // 10.0f; // TODO TEMP TEST
        (float)rand() / (float)RAND_MAX * (PLACE_MAX_DIST - PLACE_MIN_DIST) +
        PLACE_MIN_DIST;

      VertexBase vDiff(sin(rad), 0, cos(rad));
      vDiff *= r;
      VertexBase vCue = vObj;
      vCue += vDiff;      

#ifdef CALC_PLACE_DEBUG
std::cout << "..angle: " << a << " dist: " << r << " pos: " 
    << ToString(vCue).c_str()  << "\n";
#endif

      bool isLos = IsLos(vObj, vCue, radius, pCueBall, vb[i],  false);
      if (isLos)
      {
        float distCueToObj = (vCue - vObj).Length();
        float rank = 1000.0f - distCueToObj;
#ifdef CALC_PLACE_DEBUG
std::cout << " ..LOS ok! Rank is: " << rank << "\n";
#endif
        if (rank > bestPosRank)
        {
          vBestPos = vCue;
          bestPosRank = rank;
#ifdef CALC_PLACE_DEBUG
std::cout << "..best so far, (no pot) rank: " << rank << "\n";
#endif
        }
      }      
    }
  }
  if (bestPosRank > 0)
  {
#ifdef CALC_PLACE_DEBUG
std::cout << "Best place cue ball pos found - NO POT\n";
#endif

    m_placeCueBallPos = vBestPos;
    return true;
  } 

#ifdef CALC_PLACE_DEBUG
std::cout << "NO Best place cue ball pos found!\n";
#endif
  return false;
}

void PoolBehaviour::CalcShot()
{
#ifdef AI_DEBUG
std::cout << "*** AI CALC SHOT *** \n";
#endif

  static const float accel = Engine::Instance()->GetConfigFloat("golf_ball_accel");

  // We don't need the pockets and rails every time, so shouldn't really do this
  // here, but it's helpful for debugging.
  // Get the pockets for this room.
  FindPockets();
  // Now is a good time to create Rail objects, as we rely on the pockets.
  CreateRails();
  m_pPocket = 0;
  m_isBounceShot = false;

  m_validShots.clear();

  // If this is a Break, just aim straight ahead.
  // TODO Will this work for "crazy" levels ?
  bool isBreak = GetRules()->ShotIsBreak();
  if (isBreak)
  {
    float r = (float)rand() / (float)RAND_MAX * 8.0f - 4.0f;

#ifdef AI_DEBUG
std::cout << "AI Break: random angle offset: " << r << "\n";
#endif

    m_yRot = 180.0f + r;
    m_vVel = 0;
    m_hVel = 200.0f; // TODO TEMP TEST
    Engine::Instance()->GetGameState()->GetCurrentPlayerInfo()->m_golfStroke =
        PoolGameState::PlayerInfo::PoolStroke(m_yRot, m_vVel, m_hVel, accel,
        0, 0);  // TODO english, draw/roll

#ifdef AI_DEBUG
std::cout << "AI: Break shot\n";
#endif
  
    // Even though this is a break, we need to get a valid ball, so the
    // cue/camera/player character is pointing in the right direction, 
    // and the HUD shows the target ball.
    ValidBalls vb = GetValidBalls();
    Assert(!vb.empty()); // No ball valid on break ?!

    m_pTargetObject = vb[0];
    m_ghostBall = *(vb[0]->GetBoundingSphere());
    // Set aim target too
    m_target.SetSphere(m_ghostBall);
   
    m_validShots.push_back(ValidShot(
      ValidShot::BREAK, m_pTargetObject.GetPtr(), 0, 0, &m_ghostBall, -1));
 
    return;
  }

#ifdef AI_DEBUG
std::cout << "AI: Not a break shot.\n";
#endif

  ValidBalls vb = GetValidBalls();
  Assert(!vb.empty());
  if (vb.empty())
  {
#ifdef AI_DEBUG
std::cout << "AI: No valid balls, so nothing to aim at!!\n";
#endif

    // Unexpected - just shoot in some random dir.
    m_yRot = (float)rand() / (float)RAND_MAX * 360.0f;
    m_vVel = 0;
    m_hVel = Engine::Instance()->GetConfigFloat("pool_demovel"); 
    return;
  }

  // vCue is the cue ball position.
  FreeMovingGameObject* pCueBall = GetBall();
  VertexBase vCue = pCueBall->GetBoundingSphere()->GetCentre();

  m_placeCueBallPos = vCue;
  if (GetRules()->PlayerMayPlaceCueBall() &&
      !GetRules()->PlayerMayPlaceCueBallBehindHeadstring() &&
      !GetRules()->ShotIsBreak())
  {
    // Try to find a sensible place to put the cue ball.
    if (CalcPlaceCueBallPos())
    {
#ifdef _DEBUG
std::cout << "CALC PLACE BALL: Found this pos: " 
  << ToString(m_placeCueBallPos).c_str() << "\n";
#endif
      // TODO Only if AI is taking the shot ?
      vCue = m_placeCueBallPos;  
    }
  }

  // Store temp info for current shot.
  float rank = 0;
  BoundingSphere ghostBs; // ghost ball for curent ball/pocket
  float hVel;

  // Get radius of all balls
  float radius = vb[0]->GetBoundingSphere()->GetRadius();

  for (unsigned int i = 0; i < vb.size(); i++)
  {
    // Get pockets to which this object ball has LOS.
    Pockets pockets = GetLosPockets(vb[i]);

    for (unsigned int j = 0; j < pockets.size(); j++)
    {
      VertexBase vPocket = pockets[j]->GetBoundingSphere()->GetCentre();
      VertexBase vObj = vb[i]->GetBoundingSphere()->GetCentre();
      vPocket.y = vCue.y;
      vObj.y = vCue.y;
      bool bounce = false; // shot bounces off a rail ?
      BoundingSphere ghostRailBs; // for bounce shot, the ghost ball on rail

      rank = 0; 
      float cutAngle = 0;
      if (CalcPotShot(vCue, vObj, vPocket, radius, &rank, &ghostBs, &hVel, &cutAngle))
      {
        ValidShot validshot(ValidShot::POT, vb[i], pockets[j], 0, &ghostBs, rank);
        validshot.m_pockets = pockets; 
        validshot.m_cutAngle = cutAngle;
        validshot.m_hVel = hVel;
        m_validShots.push_back(validshot);
      } 
      
      // Also look for a bounce shot into the same pocket (?)  
      rank = 0; // so we don't reject any shot in next line
      if (CalcBouncePotShot(vCue, vObj, vPocket, radius, &rank, &ghostBs, &hVel))
      {
        Geom2d::Point2d railPos;
        railPos.x = ghostBs.GetCentre().x;
        railPos.y = ghostBs.GetCentre().z;
        ValidShot validshot(
          ValidShot::RAIL_POT, vb[i], pockets[j], &railPos, &ghostBs, rank);
        validshot.m_pockets = pockets; 
        validshot.m_hVel = hVel;
        m_validShots.push_back(validshot);

        bounce = true;
      }
    }
  }

  // Don't reject any shot because its rank is too low
  rank = 0;
  // Look for shots where we simply HIT a valid ball.
  if (CalcAnyValidShot(vCue, radius, &rank, &ghostBs))
  {
  }
  // Can't directly hit a valid ball. Try bouncing off rails to hit a ball.
  else if (CalcValidBounceShot(vCue, radius, &rank, &ghostBs))
  {
  }
  else 
  {
    float hVel, vVel;
    CalcJumpShot(&ghostBs, &hVel, &vVel); // NB must set m_pTargetObject

    ValidShot validshot(
      ValidShot::JUMP, m_pTargetObject.GetPtr(), 0, 0, &ghostBs, -2);
    Pockets pockets = GetLosPockets((PoolBall*)(m_pTargetObject.GetPtr()));
    validshot.m_pockets = pockets; 
    validshot.m_vVel = vVel; 
    validshot.m_hVel = hVel;
    m_validShots.push_back(validshot);
  }

  // Sort the shots and pick the best one.
  std::sort(m_validShots.begin(), m_validShots.end());
  ValidShot& best = m_validShots[0];
#ifdef AI_DEBUG
  std::cout << "Best shot: " << best.ToString().c_str() << "\n";
#endif
  // Get best shot params into member vars.
  m_isBounceShot = best.m_shotType == ValidShot::RAIL_POT || 
    best.m_shotType == ValidShot::RAIL_HIT;
  m_target.SetSphere(best.m_ghostBall);
  m_railPos = best.m_railPos;
  m_ghostBall = best.m_ghostBall;
  m_pPocket = best.m_pPocket;
  m_pTargetObject = best.m_pTargetObject;
  m_vVel = best.m_vVel;
  m_hVel = best.m_hVel;

  // Get direction to face
  VertexBase v3 = m_ghostBall.GetCentre();
  v3 -= vCue;
  v3.Normalize();
  m_yRot = RadToDeg(atan2(v3.x, v3.z));

  Engine::Instance()->GetGameState()->GetCurrentPlayerInfo()->m_golfStroke =
        PoolGameState::PlayerInfo::PoolStroke(m_yRot, m_vVel, m_hVel, accel,
        0, 0);  // TODO english, draw/roll
}

bool PoolBehaviour::CalcPotShot(
  const VertexBase& vCue,
  const VertexBase& vObj,
  const VertexBase& vPocket,
  float radius,
  float* pRank,
  BoundingSphere* pGhostBs,
  float* pHVel,
  float* pCutAngle)
{
  FreeMovingGameObject* pCueBall = GetBall();

  VertexBase vDiff = vObj - vPocket;
  vDiff.Normalize();
  vDiff *= (2.0f * radius);
  BoundingSphere ghostBs(vObj + vDiff, radius); 

  // Is ghost ball visible from cue ball ?
  // Reduce radius slightly because we don't want the object ball to count
  // as an obstruction unless it's really in the way.
  bool ghostLos = IsLos(ghostBs.GetCentre(), vCue, radius - 0.1f, pCueBall, 0);
  if (!ghostLos)
  {
    return false;
  }

  // RANK The shot:
  // Evaluator: distance from cue ball to obj ball
  // Evaluator: distance from obj ball to pocket
  float distCueToObj = (vCue - ghostBs.GetCentre()).Length();
  float distObjToPocket = (ghostBs.GetCentre() - vPocket).Length();

  static const float POT_RANK_REWARD = Engine::Instance()->
    GetConfigFloat("pool_pot_rank_reward");

  *pRank = 1000.0f - (distCueToObj + distObjToPocket) + POT_RANK_REWARD;
  *pGhostBs = ghostBs;

  // TODO work out ball speed
  *pHVel = Engine::Instance()->GetConfigFloat("pool_demovel"); 

  *pCutAngle = GetCutAngle(vCue, vObj, vPocket);

  // Thin cut shots get penalised
  *pRank -= fabs(*pCutAngle);

  return true;
}

bool PoolBehaviour::CalcAnyValidShot(
  const VertexBase& vCue,
  float radius,
  float* pRank,
  BoundingSphere* pGhostBs)
{
  // Can't pot a ball, just try to hit any valid ball with a direct
  // shot (i.e. don't try to bounce off a rail here).

  // For each valid ball, check LOS to a circle around the ball. The circle
  // radius is 2R.
  *pRank = 0;
  bool found = false;
  ValidBalls vb = GetValidBalls();
  // vCue is the cue ball position.
  FreeMovingGameObject* pCueBall = GetBall();
      
  for (unsigned int i = 0; i < vb.size(); i++)
  {
    VertexBase vObj = vb[i]->GetBoundingSphere()->GetCentre();

    // Direct LOS (i.e. no rail bounce) from cue ball to object ball 
    // bounding circle ?
    // TODO Search for LOS to circle around object ball
    bool showDebugOut = false;
#ifdef AI_DEBUG
showDebugOut = true;
#endif
    bool isLos = IsLos(vObj, vCue, radius, pCueBall, vb[i],  showDebugOut);

    if (isLos)
    {
      float distCueToObj = (vCue - vObj).Length();
      float rank = 1000.0f - distCueToObj;

      // v.1.1: Move the target (ghost) ball around randomly ??
      // If the ball can be hit with lots of room to spare, this will
      // improve the demo look and could open up the table more.
      // TODO

      // Store this valid shot
      ValidShot validshot(
        ValidShot::HIT, vb[i], 0, 0, 
        (BoundingSphere*)vb[i]->GetBoundingSphere(), rank);
      Pockets pockets = GetLosPockets(vb[i]);
      validshot.m_pockets = pockets; 
   
      // TODO work out speed based on distance
      static const float hVel = Engine::Instance()->GetConfigFloat(
        "pool_demovel"); 
      validshot.m_hVel = hVel; 
      m_validShots.push_back(validshot);
    }
  }
#ifdef AI_DEBUG
if (!found)
{
  std::cout << "VALID SHOT: NO direct shot to any valid ball.\n";
}
#endif
  return found;
}

bool PoolBehaviour::CalcValidBounceShot(
  const VertexBase& vCue,
  float radius,
  float* pRank,
  BoundingSphere* pGhostBs)
{
#ifdef AI_DEBUG
std::cout << "Try to find valid bounce shot...\n";
#endif

  // NB Must set m_pTargetObject
  FreeMovingGameObject* pCueBall = GetBall();

  bool found = false;

  // Try each valid ball
  ValidBalls vb = GetValidBalls();
// CALLER Sets *pRank, so we can reject shots here.
/////  *pRank = 0; // best so far
  int numRails = m_rails.size();
  Geom2d::Point2d cuePt(vCue.x, vCue.z);
 
  for (unsigned int j = 0; j < vb.size(); j++)
  {
    // Aim directly at the object ball.
    // TODO We really want to search for valid shots to the circle
    // surrounding this ball.

    VertexBase vObj = vb[j]->GetBoundingSphere()->GetCentre();
    Geom2d::Point2d objPt(vObj.x, vObj.z);
    Geom2d::Point2d perpPt1, perpPt2;
 
    for (int i = 0; i < numRails; i++)
    {
      // position on rail to aim for to hit object ball
      Geom2d::Point2d ghostRailPos;
      if (m_rails[i].Bounce(cuePt, objPt, &ghostRailPos, &perpPt1, &perpPt2))
      {
        VertexBase v1(ghostRailPos.x, vCue.y, ghostRailPos.y);
        // Reduce radius slightly because we don't want the object ball to count
        // as an obstruction unless it's really in the way.
        bool isLos = IsLos(v1, vCue, radius - 0.1f, pCueBall, 0);
        if (!isLos)
        {
          // Try next rail
          continue;
        }
        isLos = IsLos(vObj, v1, radius - 0.1f, pCueBall, 0);
        if (!isLos)
        {
          // Try next rail
          continue;
        }
        // Found a rail shot with LOS from cue ball to rail pos, and from there
        // to the object ball.
        // Rank the shot; if best rank store the ghost position (on the rail)
        float dist1 = (cuePt - ghostRailPos).Magnitude();
        float dist2 = (ghostRailPos - objPt).Magnitude();

        // Skew the ranking for bounce shots, so we prefer a straight shot if
        // there is one.
        static const float BOUNCE_PREFERENCE = Engine::Instance()->
          GetConfigFloat("pool_bounce_preference");
  
        float rank = 1000.0f - dist1 - dist2 - BOUNCE_PREFERENCE;
        // NB  Slice evaluator in calling code, as it handles other shot types too.

        // Store this valid shot
        Pockets pockets = GetLosPockets(vb[j]);
        ValidShot validshot(ValidShot::RAIL_HIT, 
          vb[j], 0, &ghostRailPos, 
          (BoundingSphere*)vb[j]->GetBoundingSphere(), rank);
        validshot.m_pockets = pockets; 

        // TODO speed based on distance      
        static const float hVel = Engine::Instance()->GetConfigFloat("pool_demovel"); 
        validshot.m_hVel = hVel;

        m_validShots.push_back(validshot);

        if (rank > *pRank)
        {
          *pRank = rank;
          found = true;
#ifdef AI_DEBUG
std::cout << "RAIL VALID SHOT: Found valid shot off rail - can't pot it though.\n";
std::cout << " Ball: " << vb[j]->GetId() << " number: " << vb[j]->GetNumber() << "\n";
#endif
          pGhostBs->SetCentre(v1);    
          m_perpPt1 = perpPt1;
          m_perpPt2 = perpPt2;
        }
      }
    }
  }

  return found;
}

bool PoolBehaviour::CalcBouncePotShot(
  const VertexBase& vCue,
  const VertexBase& vObj,
  const VertexBase& vPocket,
  float radius,
  float* pRank,
  BoundingSphere* pGhostBs,
  float* pHVel)
{
  FreeMovingGameObject* pCueBall = GetBall();

  bool found = false;
  Geom2d::Point2d pocketPt(vPocket.x, vPocket.z);

  // Get the real ghost ball position given the pocket and object ball.
  VertexBase vDiff = vObj - vPocket;
  vDiff.Normalize();
  vDiff *= (2.0f * radius);
  BoundingSphere ghostBs(vObj + vDiff, radius); 

  // For each rail, work out the position of the ghost ball
  // on the rail. 
  // Rail shot is possible if there is LOS from cue ball to rail,
  // from rail to object ball, (and object ball to pocket, but we have
  // already worked this out).

// CALLER Sets *pRank
////  *pRank = 0; // best so far

  Geom2d::Point2d perpPt1, perpPt2;

  int numRails = m_rails.size();
  for (int i = 0; i < numRails; i++)
  {
    // position on rail to aim for to hit object ball
    Geom2d::Point2d ghostRailPos;
    Geom2d::Point2d cuePt(vCue.x, vCue.z);
    Geom2d::Point2d ghostPt(ghostBs.GetCentre().x, ghostBs.GetCentre().z);
    if (m_rails[i].Bounce(cuePt, ghostPt, &ghostRailPos, &perpPt1, &perpPt2))
    {
      // You can hit the ghost ball with the cue ball if you aim at
      // ghostRailPos. But we must check LOS from cue ball to ghostRailPos,
      // and from ghostRailPos to the ghost Ball.
      VertexBase v1(ghostRailPos.x, vCue.y, ghostRailPos.y);
      bool isLos = IsLos(v1, vCue, radius, pCueBall, 0);
      if (!isLos)
      {
        // Try next rail
        continue;
      }
      isLos = IsLos(ghostBs.GetCentre(), v1, radius, pCueBall, 0);
      if (!isLos)
      {
        // Try next rail
        continue;
      }
      // Found a rail shot with LOS from cue ball to rail pos, and from there
      // to the ghost ball (touching the object ball).
      // Rank the shot; if best rank store the ghost position (on the rail)
      float dist1 = (cuePt - ghostRailPos).Magnitude();
      float dist2 = (ghostRailPos - ghostPt).Magnitude();
      float dist3 = (ghostPt - pocketPt).Magnitude();

      // Reduce the ranking for bounce shots, so we prefer a straight shot if
      // there is one.
      static const float BOUNCE_PREFERENCE = Engine::Instance()->
        GetConfigFloat("pool_bounce_preference");
      // PLUS, reduce the rank for a bounce shot even more if we are on the
      //  final ball. The reasoning is that a bouce shot is more risky, and 
      //  on the final ball we may lose if we miss.
      float finalBall = 1.0f;
      if (GetRules()->GetNumBallsRemainingForPlayer() == 1)
      {
        finalBall = 2.0f; // TODO CONFIG
      }
 
      static const float POT_RANK_REWARD = Engine::Instance()->
        GetConfigFloat("pool_pot_rank_reward");

      float rank = 1000.0f - dist1 - dist2 - dist3 - 
        BOUNCE_PREFERENCE * finalBall +
        POT_RANK_REWARD;

      // NB  Slice evaluator in calling code, as it handles other shot types too.
      if (rank > *pRank)
      {
        *pRank = rank;
        found = true;
#ifdef AI_DEBUG
std::cout << "RAIL POT SHOT: Found pot shot off rail.\n";
#endif
        pGhostBs->SetCentre(v1);    
        m_perpPt1 = perpPt1;
        m_perpPt2 = perpPt2;
      }
      else
      {
#ifdef AI_DEBUG
std::cout << "RAIL POT SHOT: Found pot shot off rail, REJECTED, rank: " << rank << " best: " << *pRank << "\n";
#endif
      }
    }
  }
  *pHVel = Engine::Instance()->GetConfigFloat("pool_demovel"); 
  return found; 
}

void PoolBehaviour::CalcJumpShot(
  BoundingSphere* pGhostBall,
  float* pHVel,
  float* pVVel)
{
  ValidBalls vb = GetValidBalls();
  // vCue is the cue ball position.
  FreeMovingGameObject* pCueBall = GetBall();
  VertexBase vCue = pCueBall->GetBoundingSphere()->GetCentre();
      
  // Find the closest object ball
  int closest = -1;
  float bestDist = 99999.0f;
  for (unsigned int i = 0; i < vb.size(); i++)
  {
    VertexBase vObj = vb[i]->GetBoundingSphere()->GetCentre();
    float distCueToObj = (vCue - vObj).Length();
    if (distCueToObj < bestDist)
    {
      bestDist = distCueToObj;
      closest = i;
    }
  }
  Assert(closest >= 0);
  Assert(closest < (int)vb.size());

  m_pTargetObject = vb[closest];
  *pGhostBall = *(vb[closest]->GetBoundingSphere());
  VertexBase v3 = vb[closest]->GetBoundingSphere()->GetCentre();
  v3 -= vCue;
  float dist = v3.Length();
  v3.Normalize();
  // Get direction to face
  m_yRot = RadToDeg(atan2(v3.x, v3.z));
  static const float JUMP_SHOT_H_MULT = Engine::Instance()->
    GetConfigFloat("jump_shot_h_mult");
  static const float JUMP_SHOT_V_MULT = Engine::Instance()->
    GetConfigFloat("jump_shot_v_mult");

  // Need some randomness here, or demo can end up in an infinite loop.
  if (rand() % 2 == 0)
  {
    *pVVel = 0; // no jump
    *pHVel = Engine::Instance()->GetConfigFloat("pool_demovel"); 
  }
  else
  {
    *pHVel = dist * JUMP_SHOT_H_MULT; 
    *pVVel = -dist * JUMP_SHOT_V_MULT; 
  }
}

GameObject* PoolBehaviour::GetAimTarget()
{
  return &m_target; 
}

GameObject* PoolBehaviour::GetTargetObject() 
{
  return m_pTargetObject.GetPtr();
}

TexturedQuad* PoolBehaviour::GetHudBall()
{
  if (m_pHudBall)
  {
    return m_pHudBall->GetTq();
  }
  return 0;
}

PoolBehaviourNBall::PoolBehaviourNBall(int numBalls) :
  m_numBalls(numBalls)
{
}

PoolBehaviourUk::PoolBehaviourUk() 
{
}

PoolBehaviourUs8Ball::PoolBehaviourUs8Ball() 
{
  // TODO CONFIG
  m_spots.Load("spots.bmp", "moon_a.bmp");
  m_stripes.Load("stripes.bmp", "moon_a.bmp");
}

TexturedQuad* PoolBehaviourUs8Ball::GetHudBall()
{
  if (m_pHudBall && m_pHudBall->GetNumber() == 8)
  {
    return m_pHudBall->GetTq();
  }

  RulesUs8Ball* pRulesUs8Ball = dynamic_cast<RulesUs8Ball*>(GetRules());
  Assert(pRulesUs8Ball);

  if (!pRulesUs8Ball->ColoursDecided())
  {
    return 0;
  }
  
  int id = Engine::Instance()->GetGameState()->GetCurrentPlayer();
  RulesUs8Ball::SpotsOrStripes playerColour = pRulesUs8Ball->GetPlayerColour(id);
  if (playerColour == RulesUs8Ball::SPOTS)
  {
    return &m_spots;
  } 
  else
  {
    return &m_stripes;
  }
}

PoolBehaviour::ValidBalls PoolBehaviourUs8Ball::GetValidBalls()
{
  ValidBalls vb;

  // We need US 8 Ball  rules to work out if we have decided colours yet, etc.
  RulesUs8Ball* pRulesUs8Ball = dynamic_cast<RulesUs8Ball*>(GetRules());
  Assert(pRulesUs8Ball);

  bool coloursDecided = pRulesUs8Ball->ColoursDecided();
  int id = Engine::Instance()->GetGameState()->GetCurrentPlayer();
  int numBallsRem = pRulesUs8Ball->GetNumBallsRemainingForPlayer();
 
  // If colours have been decided, get the balls of that colour.
  // If there are no balls left of the player's colour, add the black ball.
  // If colours have not been decided, add all colours except black.

  // Loop through all balls in this room
  PLevel pLevel = LevelServer::Instance()->GetCurrentLevel();
  int levelId = pLevel->GetId();
  int roomId = pLevel->GetRoomId();
  
  // Get the game objects which are in the same room as the player. 
  GameObjectMap& objs = Engine::Instance()->GetGameObjects(levelId, roomId);
  
  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    GameObject* pGo = it->second.GetPtr(); 
    if (IsPoolBall(pGo))
    {
      PoolBall* pBall = dynamic_cast<PoolBall*>(pGo);
      Assert(pBall);
      if (InPlay(pBall) && !IsCueBall(pBall))
      {
        int num = pBall->GetNumber();
        RulesUs8Ball::SpotsOrStripes sos = (num < 8) ? 
          RulesUs8Ball::SPOTS : RulesUs8Ball::STRIPES;

        if (coloursDecided)
        {
          RulesUs8Ball::SpotsOrStripes playerColour = 
            pRulesUs8Ball->GetPlayerColour(id);
          // All colours the same as the player colour are valid.
          // If the current player has potted all colours, the black is valid.
          if (num == 8)
          {
            // TODO check this - it depends on GetNumBallsRem... behaviour
            // 8 Ball is valid if no other balls are available.
            if (numBallsRem < 2)
            {
              vb.push_back(pBall);
            }
          }
          else if (sos == playerColour)
          {
            vb.push_back(pBall);
          }
        }
        else
        {
          // All balls except the black ball are valid.
          if (num != 8) 
          {
            vb.push_back(pBall);
          }
        }
      }
    }
  }
  
  m_pHudBall = 0;
  if (coloursDecided && !vb.empty())
  {
    if (vb.size() == 1)
    {
      m_pHudBall = vb[0];
    }
    else
    {
      // Set TQ to a spot or stripe ball icon.
      // TODO
    }
  }
  return vb;
}

PoolBehaviour::ValidBalls PoolBehaviourUk::GetValidBalls()
{
  ValidBalls vb;

  // We need UK rules to work out if we have decided colours yet, etc.
  RulesUk* pRulesUk = dynamic_cast<RulesUk*>(GetRules());
  Assert(pRulesUk);

  bool coloursDecided = pRulesUk->ColoursDecided();
  int id = Engine::Instance()->GetGameState()->GetCurrentPlayer();
  int playerColour = pRulesUk->GetPlayerColour(id);
  int numBallsRem = pRulesUk->GetNumBallsRemainingForPlayer();
 
  // If colours have been decided, get the balls of that colour.
  // If there are no balls left of the player's colour, add the black ball.
  // If colours have not been decided, add all colours except black.

  // Loop through all balls in this room
  PLevel pLevel = LevelServer::Instance()->GetCurrentLevel();
  int levelId = pLevel->GetId();
  int roomId = pLevel->GetRoomId();
  
  // Get the game objects which are in the same room as the player. 
  GameObjectMap& objs = Engine::Instance()->GetGameObjects(levelId, roomId);
  
  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    GameObject* pGo = it->second.GetPtr(); 
    if (IsPoolBall(pGo))
    {
      PoolBall* pBall = dynamic_cast<PoolBall*>(pGo);
      Assert(pBall);
      if (InPlay(pBall) && !IsCueBall(pBall))
      {
        int num = pBall->GetNumber();

        if (coloursDecided)
        {
          // 8 Ball is valid if no other balls are available.
          if (numBallsRem < 2) 
          {
            // Only one ball left, i.e. the black. 
            if (num == 3) // TODO CONFIG
            {
              vb.push_back(pBall);
            }
          }
          // All colours the same as the player colour are valid.
          // If free ball, all balls except black are valid. TODO
          else if (num == playerColour) 
                   // (pRulesUk->IsFreeShot() && num != 3) )
          {
            vb.push_back(pBall);
          }
        }
        else
        {
          // All balls except the black ball are valid.
          if (num != 3)  // TODO CONFIG
          {
            vb.push_back(pBall);
          }
        }
      }
    }
  }

  m_pHudBall = 0;
  if (coloursDecided && !vb.empty())
  {
    m_pHudBall = vb[0];
  }
  
  return vb;
}

PoolBehaviour::ValidBalls PoolBehaviourNBall::GetValidBalls()
{
  ValidBalls vb;
  vb.push_back(GetLowestBall(LevelServer::Instance()->GetCurrentLevel().GetPtr()));

  m_pHudBall = 0;
  if (!vb.empty())
  {
    m_pHudBall = vb[0];
  }

  return vb;
}
}


