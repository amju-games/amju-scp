/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Controller.cpp,v $
Revision 1.1.10.11  2007/03/10 22:25:33  jay
Add calls to ShotLog

Revision 1.1.10.10  2006/08/14 17:37:29  jay
Rename "Pool"

Revision 1.1.10.9  2005/09/26 21:05:33  jay
Check for zero-length vectors, to avoid NAN errors

Revision 1.1.10.8  2005/09/08 20:00:11  jay
Try to limmit POOL hacks to BallController

Revision 1.1.10.7  2005/06/29 20:18:18  jay
Fixed ExtractCharacterFromWall(), it apparently never worked before.

Revision 1.1.10.6  2005/06/04 22:24:14  jay
Just changing debug output

Revision 1.1.10.5  2005/05/08 08:17:47  jay
Fixed calls to GetOrientation() which used non-const return value.

Revision 1.1.10.4  2005/04/12 20:05:12  jay
Attempt to fix bad reflection angle when ball hits multiple walls.

Revision 1.1.10.3  2005/04/11 21:15:53  jay
Improvements to pool physics.

Revision 1.1.10.2  2005/03/31 22:26:41  jay
Add a call to Slide in BounceOffWall, so balls never get stuck in walls.

Revision 1.1.10.1  2005/03/17 20:48:27  jay
Pool hack: add ball radius to height

Revision 1.1  2004/09/08 15:42:47  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <algorithm>
#include <iostream>
#include <math.h>
#include "Controller.h"
#include "HeightServer.h"
#include "FreeMovingGameObject.h"
#include "Common.h"
#include "Geometry.h"
#include "Geom2d.h"
#include "Engine.h"
#include "AngleCompare.h"
#include "StringUtils.h"
#include "ShotLog.h"
#include "SchAssert.h"

namespace Amju
{
float Controller::s_stepHeight;
float Controller::s_smallestReverseDistance;
float Controller::s_smallestDotProduct;
float Controller::s_nearlyFallingTimeLimit;
float Controller::s_smallestFallingReverseDistance;

void Controller::Init()
{
  s_stepHeight = Engine::Instance()->GetConfigFloat("max_step_height");
  s_smallestReverseDistance = atof(Engine::Instance()->
    GetConfigValue(Cfg::SMALLEST_REVERSE_DIST).c_str());

  s_smallestFallingReverseDistance = atof(Engine::Instance()->
    GetConfigValue("smallest_reverse_dist_falling").c_str());

  s_smallestDotProduct = atof(Engine::Instance()->
    GetConfigValue(Cfg::SMALLEST_DOT_PRODUCT).c_str());

  s_nearlyFallingTimeLimit = 2.0f; // TODO CONFIG

}

void Controller::ExtractCharacterFromWall(FreeMovingGameObject* pCharacter)
{
  // TODO 
  // By extracting the character from one wall, this code can embed him in 
  // another! Try to fix this!

  const HeightServer* pCharacterHeightServer = pCharacter->GetHeightServer();
  typedef std::vector<WallPoly> WallVector;
  WallVector walls;

  // bsAfter is the current player b.sphere.
  BoundingSphere bsAfter = *(pCharacter->GetBoundingSphere());
  BoundingSphere bsBefore(bsAfter);

  bool collided = pCharacterHeightServer->Intersects(bsBefore, bsAfter, &walls);

  if (!collided)
  {
    // We are not intersecting a wall.
    return;
  }
 
#ifdef EXTRACT_DEBUG
std::cout << "CONTROLLER: EXTRACTING obj " << pCharacter->GetId() << " from wall.\n";
#endif

#ifdef POOL_ONLINE
  std::string s = "Extract from wall: object ID: ";
  s += ToString(pCharacter->GetId());
  ShotLog(s);
#endif

  // Move away from the wall by the amount we are stuck in the wall.
  // Get distance from sphere centre to wall. 
  // (Just pick the first wall in vector.)

  // TODO What distance should we use if there are multiple walls ??
  float dsq = walls[0].SqDist(bsBefore.GetCentre());
  float d = sqrt(dsq);

  // Do this in 2D, i.e. in the x-z plane.
  Vec3f n;
  // Get avg normal for all walls, not just the 0th wall
  for (unsigned int i = 0; i < walls.size(); i++)
  {
    n += walls[i].GetNormal();
  }
  n.Normalise(); 
  float r = bsBefore.GetRadius();
  if (r <= d)
  {
#ifdef _DEBUG
std::cout << "EXTRACT FROM WALL: unexpected, r: " << r << " d: " << d << "\n";
#endif

#ifdef POOL_ONLINE
    ShotLog("..unexpected, r < d.. any wierdness after this point ?");
#endif
    return;
  }

  Assert(r > d); // not colliding with this wall ??
  n *= (r - d);

  Orientation o = *(pCharacter->GetOrientation());
  float x = o.GetX();
  float z = o.GetZ();
  x += n.x;
  z += n.z;
  o.SetX(x);
  o.SetZ(z);
  pCharacter->SetOrientation(o);
}

void Controller::OnHitWall(  
  FreeMovingGameObject* pCharacter,
  WallPoly* pWp, 
  float oldx, float oldz, 
  float newx, float newz, 
  float* resultx, float* resultz)
{
  Slide(pCharacter, pWp, oldx, oldz, newx, newz, resultx, resultz);
}

void Controller::OnHitWalls(  
  FreeMovingGameObject* pCharacter,
  const WallVector& walls, 
  float oldx, float oldz, 
  float newx, float newz, 
  float* resultx, float* resultz)
{
  unsigned int s = walls.size();
  for (unsigned int i = 0; i < s; i++)
  {
    OnHitWall(pCharacter, const_cast<WallPoly*>(&(walls[i])), oldx, oldz, newx, newz, resultx, resultz);
    newx = *resultx;
    newz = *resultz;
  }
}

//#define BOUNCE_OFF_DEBUG

float Controller::GetReflectionDegs(float incidenceDegs, const WallPoly* pWp)
{
  // Get normalized wall normal in x-z plane only.
  Vec3f normal = pWp->GetNormal();
  Geom2d::Vec2d v1(normal.x, normal.z); // convert wall normal to 2D
  v1.Normalise();

  // Get angle of normal, to compare with angle of incidence.
  float nAngle = atan2(v1.x, v1.y);
  float nDegs = RadToDeg(nAngle);
  if (nDegs < 0)
  {
    nDegs += 360.0f;
  }

  float diff = incidenceDegs - 180.0f - nDegs;
  if (diff < 0)
  {
    diff += 360.0f;
  }

  float result = incidenceDegs;
  result -= 180.0f;
  result -= 2 * diff;

#ifdef BOUNCE_OFF_DEBUG
  std::cout 
   << " NORMAL: " << nDegs 
//   << " YROT: " << yrot
   << " DIFF: " << diff
   << " RESULT: " << result
   << "\n";
#endif
  return result;
}

void Controller::BounceOff(
  FreeMovingGameObject* pObj,
  WallPoly* pWp, 
  float oldx, float oldz, 
  float newx, float newz, 
  float* resultx, float* resultz)
{
#ifdef BOUNCE_OFF_DEBUG
std::cout << "*** BOUNCE OFF WALL - ball " << pObj->GetId() << "\n";
#endif

  // Billiard-ball style collision.
  // We want to reflect back from the wall with the opposite angle of incidence
  //  to the wall normal.

  Orientation o = *(pObj->GetOrientation());

  // POOL
  // Calling Slide() forces the ball to the outside of the wall. This
  // avoids getting balls stuck in walls.
  // But we still need to bounce and slide only off the contact wall - 
  // not other walls which intersect the bounding sphere. TODO

  Slide(pObj, pWp, oldx, oldz, newx, newz, resultx, resultz);

  float result = GetReflectionDegs(o.GetYRot(), pWp);
  o.SetYRot(result);

#ifdef BOUNCE_OFF_DEBUG
std::cout << "BOUNCE: Old OR: " << ToString(*(pObj->GetOrientation())).c_str() << "\n   New OR: " << ToString(o).c_str() << "\n";

float fwdx, fwdz;
pObj->GetForwardVector(&fwdx, &fwdz);
std::cout << "BOUNCE: old fwd vec: " << ToString(fwdx).c_str() << " " << ToString(fwdz).c_str() << "\n";
#endif

  pObj->SetOrientation(o);

#ifdef BOUNCE_OFF_DEBUG
pObj->GetForwardVector(&fwdx, &fwdz);
std::cout << "BOUNCE: NEW fwd vec: " << ToString(fwdx).c_str() << " " << ToString(fwdz).c_str() << "\n";

#endif

}

// Slide an object moving from (oldx, oldz) to (newx, newz) along the Wall
// pointed to by pWp. 
void Controller::Slide(
  FreeMovingGameObject* pCharacter, 
  WallPoly* pWp, 
  float oldx, float oldz, 
  float newx, float newz, 
  float* resultx, float* resultz)
{
  // By default, the result is the same as the new pos.
  *resultx = newx;
  *resultz = newz;

  Vec3f normal = pWp->GetNormal();
  normal.Normalise();

  // Get the distance moved. Move back this far (max).
  // The distance we move back depends on the angle the movement direction
  // makes with the wall. 
  const float dx = newx - oldx;
  const float dz = newz - oldz;

  // TODO find out how this could be zero
  if (dx == 0 && dz == 0)
  {
#ifdef NORM_DEBUG
std::cout << "CONTROLLER: dist zero, can't normalize\n";
#endif
    return;
  }

  float dist = sqrt(dx * dx + dz * dz);
  // Get Dot Product of player direction vector and wall normal. This should vary
  // between -1 (hit wall head on) to 0 (sliding parallel to the wall).
  Geom2d::Vec2d v1(normal.x, normal.z); // convert wall normal to 2D
  Geom2d::Vec2d v2(dx, dz); // player direction 2D vector
  if (!v1.Normalise())
  {
#ifdef NORM_DEBUG
std::cout << "Normalise failed: V1\n";
std::cout << "Object: " << pCharacter->GetId() << " (" << pCharacter->GetTypeName() << ")\n";
Assert(0);
#endif
  }
 
  if (!v2.Normalise())
  {
#ifdef NORM_DEBUG
std::cout << "Normalise failed: V2\n";
std::cout << "Object: " << pCharacter->GetId() << " (" << pCharacter->GetTypeName() << ")\n";
Assert(0);
#endif
  }

  // Get the dot product, but reverse the sign so we get 1 == perpendicular.
  float dotproduct = -(Geom2d::DotProduct(v1, v2));

  if (dotproduct < 0)
  {
#ifdef SLIDE_DEBUG
    std::cout << "SLIDE bad dot product!!!!\n";
#endif
    return;
  }

  // Ensure we move back enough to go round corners.
  if (dotproduct < s_smallestDotProduct)
  {  
    dotproduct = s_smallestDotProduct; 
  }

  // The distance to move away from the wall is proportional to the d.p.
  // So for a head-on collide we move back all the way we moved forward; for
  // a side-on collision we move back proportionately less.
  dist *= dotproduct;

  if (dist < s_smallestReverseDistance)
  {
    dist = s_smallestReverseDistance; 
  }
  if (pCharacter->IsFalling())
  {
    // push further away from wall.
    if (dist < s_smallestFallingReverseDistance)
    {
      dist = s_smallestFallingReverseDistance; 
    }
  }

  newx += normal.x * dist;
  newz += normal.z * dist;
  
  *resultx = newx;
  *resultz = newz;
}

bool Controller::IsIntersectingCeiling(
  FreeMovingGameObject* pCharacter, 
  const BoundingSphere& bsBefore,
  const BoundingSphere& bsAfter )
{
  const HeightServer* pCharacterHeightServer = pCharacter->GetHeightServer();

  bool collided = pCharacterHeightServer->IntersectsCeiling(bsBefore, bsAfter);

  return collided;

}

void Controller::EnforceBoundary(FreeMovingGameObject* pCharacter)
{
}

void Controller::RollDownSlope(FreeMovingGameObject*)
{
}


// Used in HandleWallCollision() below to get rid of walls
// which are below ground height.
struct GroundWallRemover
{
GroundWallRemover(const HeightServer* pHs, float ballH) : m_pHs(pHs), m_ballH(ballH) {}
bool operator() (const WallPoly& wp) 
{ 
  m_ballH += 0.5f; // safety margin so we get height poly if object height is
                   // slightly below ground.
  // This function relies on getting the heights of polys right at their
  // edges. Sometimes we must fudge the (x, z) coord using this constant
  // to get the height.
  static const float JITTER = 0.001f;
  // We need to find the ground height for points which may be at the bottom of
  // large solids. So set a large range to search.
  static const float FIND_H = 100.0f;

  // Test each vertex.
  for (int i = 0; i < 3; i++)
  {
    const Vec3f& v = wp.GetVertex(i);
    float x = v.x;
    float y = v.y;
    float z = v.z;

    // If the height at (x, z) given by the heightserver
    // is less than the height of the wall at that point, then  
    // the wall is not below ground, and should not be deleted,
    // so return false.
    float h = 0;
    // Give a range, to try to get the ground poly when the object is
    // slightly below ground height. This could happen if the height
    // has not been corrected yet.
    // We may fail to get the height because the point-in-poly test fails
    // as it is right on the edge of the poly.
    const HeightPoly* pHp = m_pHs->GetHeightPoly(x, m_ballH, y - FIND_H, z);
    // If we failed to get the height, try jittering the (x, z) coord.
    if (!pHp)
    {
      pHp = m_pHs->GetHeightPoly(x + JITTER, m_ballH, y - FIND_H, z + JITTER); 
    }
    if (!pHp)
    {
      pHp = m_pHs->GetHeightPoly(x - JITTER, m_ballH, y - FIND_H, z + JITTER); 
    }
    if (!pHp)
    {
      pHp = m_pHs->GetHeightPoly(x - JITTER, m_ballH, y - FIND_H, z - JITTER); 
    }
    if (!pHp)
    {
      pHp = m_pHs->GetHeightPoly(x + JITTER, m_ballH, y - FIND_H, z - JITTER); 
    }

    if (!pHp)
    {
      // DO NOT delete this poly, as we can't tell where the ground is.

#ifdef _DEBUG
      std::cout << "Can't get height! x=" << v.x << ", y=" << v.y << ", z=" << v.z << " Ball height=" << m_ballH << "\n";
      std::cout << "Poly: ";
      for (int j = 0; j < 3; j++)
      {
        const Vec3f& v = wp.GetVertex(j);
        std::cout << "(" << v.x << ", " << v.y << ", " << v.z << ") ";
      }
      std::cout << "\n";
#endif
      return false;
    }

    // h is the height of the ground at this (x, z).
    h = pHp->GetY(v.x, v.z); 
    // Increase the ground height slightly. This could smooth out 
    // 'invisible collision' problems.
    // This will not fix the tops of doors because they have a top,
    // and really do stick up out of the ground.
    h += 0.15f; // TODO TEMP TEST
    if (h < y)
    {
      // Ground height is less than wall height - so DO NOT delete this wall!
      return false;
    }
  }
#ifdef _DEBUG
  std::cout << "Removing: ";
  for (int j = 0; j < 3; j++)
  {
    const Vec3f& v = wp.GetVertex(j);
    std::cout << "(" << v.x << ", " << v.y << ", " << v.z << ") ";
  }
  std::cout << " ball Height: " << m_ballH;
  std::cout << "\n";
#endif

  return true;
}
const HeightServer* m_pHs;
// Height of ball - don't look for ground polys higher than this.
float m_ballH;
};


void Controller::HandleWallCollision(
  FreeMovingGameObject* pCharacter, 
  float stepHeight)
{
  RollDownSlope(pCharacter);

  Orientation pO = *(pCharacter->GetOrientation());

  // Save the player coord - but not the orientation.
  float oldx = pO.GetX();
  float oldy = pO.GetY();
  float oldz = pO.GetZ();

  // Get character's bounding sphere prior to updating position
  const BoundingSphere bsBefore = *(pCharacter->GetBoundingSphere());

  // Get new player orientation.
  pCharacter->Recalculate();

  // Make sure the b.sphere is updated.
  Matrix m;
  m.SetIdentity();
  pCharacter->CreateBoundingSphere(m);

  // Object height has been recalculated using vertical vel. in Recalculate() above.
  // Now we want to adjust for hitting the ground, etc;
  // also we have to update the vertical vel if the vertical position has
  // changed!
  UpdateCharacterYPos(pCharacter, stepHeight);


  //pCharacter->RefreshHeightServer(); // done in EngineRunningBase 
  float newx = pO.GetX();
  float newy = pO.GetY();
  float newz = pO.GetZ();

  // Get character's bounding sphere after update
  const BoundingSphere bsAfter = *(pCharacter->GetBoundingSphere());

  // If the sphere hasn't moved, don't bother doing collision testing.
  if (bsBefore == bsAfter)
  {
    return;
  }  

  // If the character hits a ceiling, reset the coords and return.
  // TODO no other action necessary ?
  // TODO Seems to be a problem - player can get head stuck on ceilings.
  // TODO only allow DOWNWARD movement ??

  // TODO 
  // Optimise this once it's working!!!!!
  
  // Now check to make sure player bounding sphere isn't intersecting the
  // scenery - if it is, we have hit a wall. 

  const HeightServer* pCharacterHeightServer = 
    pCharacter->FreeMovingGameObject::GetHeightServer();

  // 2/11/01 Long-overdue new interface which takes into account the 'before' and
  // 'after' positions of the character. This means we can detect a collision
  // even if the character has gone completely through a wall.
  //typedef std::vector<WallPoly> WallVector;
  WallVector walls;
  bool collided = pCharacterHeightServer->Intersects(bsBefore, bsAfter, &walls);

  if (!collided)
  {
    return;
  }

  // Remove repeated wall normals.
  // This is to solve some problems where the player is pushed back by more 
  // than one wall, when visually it's the same wall.
  //unsigned int wallsNonUnique = walls.size();

  std::sort(walls.begin(), walls.end(), PlaneLessThan());
  walls.erase(std::unique(walls.begin(), walls.end()), walls.end());
  //unsigned int wallsUnique = walls.size();

/*
  POOL - don't need to do this.

  // j.c. 10/5/2004 - Pool, walls of blocks are sometimes collided with
  // although they are below ground height.
  // Remove walls which are below ground height.
  float ballH = pCharacter->GetOrientation()->GetY();
  walls.erase(std::remove_if(walls.begin(), walls.end(), 
    GroundWallRemover(pCharacterHeightServer, ballH)), walls.end());
*/

  if (walls.empty())
  {
#ifdef WALLS_DEBUG
    std::cout << "NO WALLS LEFT\n";
#endif
    return;
  }


  // Ceiling flag: set to true if a wall looks like a ceiling.
  bool isCeiling = false;
  for (WallVector::iterator it = walls.begin(); it != walls.end(); ++it)
  {
    if (it->b() < -0.7)
    {
      isCeiling = true;
      break;
    }
  }

#if defined(WALLS_DEBUG)
  if (wallsUnique < wallsNonUnique)
  {
/*
    cout << "UNIQUE walls reduced " <<  wallsNonUnique << " to " << wallsUnique << endl;
*/
  }
#endif

#if defined(HS_DEBUG)
  Engine::Instance()->PushColour(0, 0, 0, 1);
  for (int i = 0; i < (int)walls.size(); i++)
  {
    WallPoly* pWp = &walls[i]; 
    pWp->Draw();
  }
  Engine::Instance()->PopColour();
#endif

  if (isCeiling && newy >= oldy)
  {
/*
    pO->SetX(oldx);
    pO->SetY(oldy);
    pO->SetZ(oldz);
*/
    return; 
  }

  // If there's one wall, move away in the direction of the wall normal.
  if (walls.size() == 1)
  {
#ifdef WALLS_DEBUG
std::cout << "WALL COLLISION: 1 wall\n";
#endif

    WallPoly* pWp = &walls[0];

    OnHitWall(pCharacter, pWp, oldx, oldz, newx, newz, &newx, &newz); 
//// TODO!!!!!!
// Bounce off data is being overwritten after the above call!!!!

    Orientation pO = *(pCharacter->GetOrientation());
    pO.SetX(newx);
    pO.SetZ(newz);
    pCharacter->SetOrientation(pO);

#if defined(_DEBUG)
    // Are we still colliding with something ? We shouldn't be!
    const BoundingSphere bsAfterSlide = *(pCharacter->GetBoundingSphere());
    std::vector<WallPoly> wallsAfter;
    bool collided = pCharacterHeightServer->Intersects(bsBefore, bsAfterSlide, &wallsAfter);
    if (collided)
    {
      std::sort(wallsAfter.begin(), wallsAfter.end(), PlaneLessThan());
      wallsAfter.erase(std::unique(wallsAfter.begin(), wallsAfter.end()), wallsAfter.end());
      //unsigned int wallsUnique = wallsAfter.size();
/*
      cout << "COLLISION: still intersecting " << wallsUnique << " walls!" << endl;
*/
    }
#endif
  }
  else /*if (walls.size() > 1)*/
  {
#ifdef WALLS_DEBUG
std::cout << "WALL COLLISION: " << walls.size() << " walls\n";
#endif

    // Try Sliding along each wall. Then retest for collisions.
    OnHitWalls(pCharacter, walls, oldx, oldz, newx, newz, &newx, &newz); 

    Orientation pO = *(pCharacter->GetOrientation());
    pO.SetX(newx);
    pO.SetZ(newz);
    pCharacter->SetOrientation(pO);

    const BoundingSphere bsAfterSlide = *(pCharacter->GetBoundingSphere());
    std::vector<WallPoly> wallsAfter;
    bool collided = pCharacterHeightServer->Intersects(bsBefore, bsAfterSlide, &wallsAfter);
    if (!collided)
    {
      // Now we aren't touching any walls. Sounds like a good point to stop!
      return;
    }

    // Still colliding with something ?
    pO.SetX(oldx);
    pO.SetZ(oldz);
    pCharacter->SetOrientation(pO);

/*
    cout << "Can't get free of walls???" << endl;
*/
    // Drastic action is needed.
  }
}

void Controller::UpdateCharacterYPos(
  FreeMovingGameObject* pCharacter,
  float stepHeight)
{
  // Get the HeightServer for the Character.
  const HeightServer* pCharHs = pCharacter->GetHeightServer();

  // Get height of static scenery at the player's (x, z) coord.
  float playerx = pCharacter->GetOrientation()->GetX();
  float playery = pCharacter->GetOrientation()->GetY();
  float playerz = pCharacter->GetOrientation()->GetZ();

  // remember height until end of function..
  float oldHeight = playery;

  float height = HeightServer::BadHeight;

  // TODO hack! We add a constant to the player height so we get the new correct height 
  // if the player is moving up a slope.
  // TODO Fix this by using the HeightServer to get the new Y-value when the character
  // (x, z) coord is updated.

  // Get a height range. This centres on the current y-coord, and increases
  // with vertical speed.
  float hrange = fabs(pCharacter->GetVerticalVel());

  // TODO TEMP TEST
  // Range must not be too big or we may pick a totally wrong height.
  hrange *= 0.25f;

  if (hrange < 1.0f) // TODO CONFIG
  {
    hrange = 1.0f;
  }
  float ymin = playery - hrange;
  float ymax = playery + hrange; 
  const HeightPoly* pHp = pCharHs->GetHeightPoly(
    playerx, 
    ymax, 
    ymin, 
    playerz);

  if (pHp)
  {
    height = pHp->GetY(playerx, playerz);
  }

  //Assert(height != HeightServer::BadHeight);

  float playerHeight = pCharacter->GetHeight();

  // *** TODO POOL ***
// TODO Remove this - we do it in BallController
  // This height is the y-coord of the centre of the ball.
  // We want to make the lowest point of the ball the origin so
  // it sits on the table, not halfway through it.
//  float radius = pCharacter->GetBoundingSphere()->GetRadius();
//  playerHeight -= radius;
  // ***

  if (pCharacter->IsFalling())
  {
//std::cout << "CONTROLLER: Obj " << pCharacter->GetId() << ": " << pCharacter->GetTypeName() <<"\n";
//std::cout << "Y POS: height: " << height << " playerHeight " << playerHeight << " Falling: " << pCharacter->IsFalling() << "\n";

    // Player is falling
    // Check if player has hit ground.
    if (playerHeight <= height && pCharacter->IsFalling())
    {
//std::cout << "Falling - 1\n";
      // Hit ground.
      // stops bouncing - //pCharacter->SetFalling(false);
      pCharacter->SetHeight(height); // + radius); // POOL HACK
      pCharacter->Bounce();
      // above call may set falling flag
      if (!pCharacter->IsFalling())
      {
        // Stopped bouncing, so recalc. forward vector.
        pCharacter->RecalcForwardVector();
      }

      // TODO Check height Fallen From to see if we have broken anything..
      float fallHeight = pCharacter->GetHeightFallenFrom();
      if (fallHeight - height > 2.0f) // TODO CONFIG
        // TODO config file - also different degrees ?
      {
        pCharacter->FellFromHeight(fallHeight - height);
        // Prevent bouncing from doing more damage
        pCharacter->SetHeightFallenFrom(height);
      }
    }
    else if (playerHeight <= pCharacter->GetGroundHeight())
    {
//std::cout << "Falling - 2\n";
      // Check if ground height is still valid.
      if (pCharacter->GetGroundHeight() > height)
      {
        pCharacter->SetGroundHeight(height);
      }
      else
      {
        // Hit ground.
        // - bouncing - //pCharacter->SetFalling(false);
        pCharacter->SetHeight(pCharacter->GetGroundHeight());
        pCharacter->Bounce();
        if (!pCharacter->IsFalling())
        {
          // Stopped bouncing, so recalc. forward vector.
          pCharacter->RecalcForwardVector();
        }

        // TODO Check m_heightFallenFrom to see if we have broken anything..
        if (pCharacter->GetHeightFallenFrom() - height > 2.0f) 
          // TODO config file - also different degrees ?
        {
          pCharacter->FellFromHeight(pCharacter->GetHeightFallenFrom() - height);
          // Prevent bouncing from doing more damage
          pCharacter->SetHeightFallenFrom(height);
        }
      }
    }
  }
  // "Nearly falling" - this means the character has a short time to either get 
  // back on something solid, or jump.
  else
  {
    // Player not falling.. but check if is just about to.
    if (fabs(playerHeight - height) < 0.0001f)
    {
//std::cout << "CONTROLLER: Same height, no problemo.\n";
    }
    else if (playerHeight >  height + stepHeight)
    {
//std::cout << "CONTROLLER: IS FALLING\n";
      // Is falling.
      pCharacter->SetFalling(true);
      pCharacter->SetGroundHeight(height);
      // Remember the height character is falling from..
      pCharacter->SetHeightFallenFrom(playerHeight);

    }
    else if (playerHeight > height)
    {
//std::cout << "CONTROLLER: rolling down slope ? \n";
      // Above ground but not falling -> moving down a slope.
      pCharacter->SetHeight(height); // + radius); // POOL HACK

      // TODO make slope slippery - increase player velocity in 
      // downward direction.

      // TODO Move camera height up. This is because the camera will be closer to
      // the ground due to the slope.
    }
    else if (playerHeight < height && playerHeight > (height - stepHeight)) 
    {
//std::cout << "CONTROLLER: FIGHT FALLING!!\n";

      // Just below height. Could be on a slope, or at a small step which we 
      //  can jump up to.
      pCharacter->SetHeight(height); // + radius); // POOL HACK
      pCharacter->SetFalling(false);
    }
    else if (playerHeight < height)
    { 
//std::cout << "CONTROLLER: Fallen below expected height\n";
      // Can't jump up to new height. Return to old position.
      // We should spot this earlier by doing a collision detect between
      // character and solids. Or something.
     
      Orientation o = *(pCharacter->GetOrientation()); 
      o.SetX(playerx);
      o.SetY(playery);
      o.SetZ(playerz);
      pCharacter->SetOrientation(o);
    }
  }

  float newHeight = pCharacter->GetOrientation()->GetY();
  if (pCharacter->GetVerticalVel() == 0)
  {
    // If height has changed, work out what the vertical velocity is.
    if (newHeight != oldHeight)
    {
      float vVel = (newHeight - oldHeight) / Engine::Instance()->GetDeltaTime();
      pCharacter->SetVerticalVel(vVel);
    }
  }

}

}

