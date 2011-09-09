/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: BallController.cpp,v $
Revision 1.1.10.6  2007/03/10 22:25:32  jay
Add calls to ShotLog

Revision 1.1.10.5  2005/09/08 20:00:11  jay
Try to limmit POOL hacks to BallController

Revision 1.1.10.4  2005/05/15 17:09:17  jay
Multi-walls: call OnBounceOffWall for every wall

Revision 1.1.10.3  2005/05/08 08:16:11  jay
Small change in OnHitWalls() to update position after each call to Slide().
This seems to have fixed the problem of the ball getting stuck in a wall.

Revision 1.1.10.2  2005/04/12 20:05:12  jay
Attempt to fix bad reflection angle when ball hits multiple walls.

Revision 1.1.10.1  2005/04/11 21:15:53  jay
Improvements to pool physics.

Revision 1.1  2004/09/08 15:42:44  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "Common.h"
#include "BallController.h"
#include "FreeMovingGameObject.h"
#include "Geom2d.h"
#include "HeightServer.h"
#include "Engine.h"
#include "AngleCompare.h"
#include "PoolBall.h"
#include "ShotLog.h"

namespace Amju
{
void BallController::UpdateObject(FreeMovingGameObject* p)
{
  Assert(p);

  // Move character in current direction, but handle collision with walls.
  // Calls Character's Reclculate() function.
  HandleWallCollision(p, s_stepHeight);
}

void BallController::OnHitWalls(  
  FreeMovingGameObject* pObj,
  const WallVector& walls,
  float oldx, float oldz, 
  float newx, float newz, 
  float* resultx, float* resultz)
{
  Orientation o = *(pObj->GetOrientation());

  // Get total bounce angle
  float bounce = 0;
  unsigned int s = walls.size();
  for (unsigned int i = 0; i < s; i++)
  {
    WallPoly* pWp = const_cast<WallPoly*>(&(walls[i]));

    // Slide on this wall, then add the bounce angle to running total
    Slide(pObj, pWp, oldx, oldz, newx, newz, resultx, resultz);
    newx = *resultx;
    newz = *resultz;
    bounce += GetReflectionDegs(o.GetYRot(), pWp);
    pObj->OnBounceOffWall();
#ifdef BALL_DEBUG
std::cout << " ON HIT WALLS: obj: " << pObj->GetId() << " new vel " << pObj->GetForwardVel() << "\n";
#endif
  }
  bounce /= (float)s; // i.e. get average angle
  o.SetYRot(bounce);
  pObj->SetOrientation(o);
  //pObj->OnBounceOffWall();

#ifdef BALL_DEBUG
std::cout << "ON HIT WALLS: obj: " << pObj->GetId() << " final vel " << pObj->GetForwardVel() << "\n";
#endif

#ifdef POOL_ONLINE
  PoolBall* pBall = dynamic_cast<PoolBall*>(pObj);
  std::string str = "Bounce off wall: Ball ID: ";
  str += ToString(pBall->GetId());
  str += " num walls: ";
  str += ToString(s);
  ShotLog(str);
#endif
}


void BallController::OnHitWall(  
  FreeMovingGameObject* pCharacter,
  WallPoly* pWp, 
  float oldx, float oldz, 
  float newx, float newz, 
  float* resultx, float* resultz)
{
  BounceOff(pCharacter, pWp, oldx, oldz, newx, newz, resultx, resultz);

  pCharacter->OnBounceOffWall();
}

void BallController::RollDownSlope(FreeMovingGameObject* p)
{
#define SLOPES_WORKING
#define OLD_SLOPE_CODE

#ifdef SLOPES_WORKING
  static float staticTurnVel = 
    Engine::Instance()->GetConfigFloat("golf_turn_vel");

  // Must be moving and in contact with the ground.
  if (p->IsFalling() || p->GetForwardVel() < SMALLEST)
  {
    return;
  }

  // If the ground is on a slope, roll: this means that the 
  // component of the ball velocity which is going up/down the slope
  // is affected by gravity.
  Orientation o = *(p->GetOrientation());

  // Get the current ground poly.
  const HeightPoly* pHp = p->GetHeightPoly();
  if (!pHp)
  {
    pHp = p->GetHeightServer()->GetHeightPoly(
      o.GetX(), o.GetY() + 0.9f, o.GetY() - 0.9f, o.GetZ());

    if (!pHp)
    {
      return;
    }
  }

  // Get the normal of the ground poly we are on.
  Vec3f normal = pHp->GetNormal();

  if (fabs(normal.x) < SMALLEST && fabs(normal.z) < SMALLEST)
  {
    // Not sloping.
    return;
  }

  // Get the (x, z) components.
  Geom2d::Vec2d v1(normal.x, normal.z); // convert wall normal to 2D
  float mag = v1.Magnitude();

  // Normalise this (x, z) vector and find its angle.
  v1.Normalise();
  float ar = atan2(v1.x, v1.y);

  // Ground slopes in this direction
  float adegs = RadToDeg(ar);

  // Current (x, z) direction of object travel
  float yrot = o.GetYRot();

  float degDiff = AngleCompare::DegDiff(yrot, adegs);

#ifdef NEW_SLOPE_CODE
  // Get component of velocity in direction of slope
  // NB Is negative if we are going UP the slope, positive if DOWN.
  float vSlope = cos(radDiff);
  // Get component of velocity perpendicular to slope
  float vPerp = sin(radDiff);
  // The perpendicular component is unchanged.
  // The component in the direction of the slope changes due to gravity,
  // proportional to the severity of the slope.

#ifdef _DEBUG
std::cout 
   << "Slope components: inline: " << vSlope 
   << " perp: " << vPerp
   << "\n";

  // TODO Slope severity: we need a number 0..1 to show slope steepness,
  // plus sign for whether we are going up or down it.
std::cout << "Slope severity: " << mag << "\n";

  if (vSlope > 0)
  {
    std::cout << "Going DOWN a slope\n";
  }
  else if (vSlope < 0)
  {
    std::cout << "Going UP a slope\n";
  }
#endif 

  float dt = Engine::Instance()->GetDeltaTime();
  float GRAVITY = p->GetGravity();
  GRAVITY *= 0.001f; // TODO TEMP TEST
  vSlope += dt * GRAVITY * mag;

  // New velocity is old multiplied by the magnitude of vSlope and vPerp.
  float vMult = sqrt(vSlope * vSlope + vPerp * vPerp);
  float vOld = p->GetForwardVel();
  float vNew = vOld * vMult;
#ifdef _DEBUG
  if (vOld < vNew)
  {
    std::cout << "FASTER\n";
  }
  else
  {
    std::cout << "SLOWER\n";
  }
#endif
  p->SetForwardVel(vNew);

  // New direction: vSlope in direction of Slope; vPerp in direction
  // 90degrees to slope.
  float dirNew = adegs - vPerp / vNew * 90.0f;

#ifdef _DEBUG
std::cout 
   << "Old angle: " << o.GetYRot()
   << " New angle: " << dirNew
   << "\n";  
#endif
  o.SetYRot(dirNew);
#endif // NEW_SLOPE_CODE


#ifdef OLD_SLOPE_CODE
  float turnvel = staticTurnVel; 
  turnvel *= mag;
  // Difference we will add to the object direction this frame
  float dt = Engine::Instance()->GetDeltaTime();
  float frameDiff = degDiff * turnvel * dt;
  o.SetYRot(o.GetYRot() + frameDiff);

  p->SetOrientation(o);
#endif // OLD_SLOPE_CODE

#endif // SLOPES_WORKING
}

void BallController::UpdateCharacterYPos(
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
  // This height is the y-coord of the centre of the ball.
  // We want to make the lowest point of the ball the origin so
  // it sits on the table, not halfway through it.
  float radius = pCharacter->GetBoundingSphere()->GetRadius();
  playerHeight -= radius;
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
      pCharacter->SetHeight(height + radius); // POOL HACK
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
      pCharacter->SetHeight(height + radius); // POOL HACK

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
      pCharacter->SetHeight(height + radius); // POOL HACK
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

