/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: FallDownController.cpp,v $
Revision 1.1.2.2  2005/09/11 19:00:48  jay
Get fall-down controller to actually work

Revision 1.1.2.1  2005/09/08 19:59:46  jay
Add simple controller for objects which just fall down.

*/

#include "FallDownController.h"
#include "HeightServer.h"
#include "FreeMovingGameObject.h"
#include "Common.h"
#include "Geometry.h"
#include "Geom2d.h"
#include "Engine.h"
#include "AngleCompare.h"
#include "StringUtils.h"
#include "SchAssert.h"

namespace Amju
{
void FallDownController::UpdateObject(FreeMovingGameObject* p)
{
//  Assert(p);
//  UpdateCharacterYPos(p, s_stepHeight);

//  Controller::UpdateObject(p);
  HandleWallCollision(p, s_stepHeight);

}

void FallDownController::UpdateCharacterYPos(
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
  //float ymin = playery - hrange;
  float ymax = playery + hrange;
  const HeightPoly* pHp = pCharHs->GetHeightPoly(
    playerx,
    ymax,
    //ymin,
    playerz);
  if (pHp)
  {
    height = pHp->GetY(playerx, playerz);
  }

  //Assert(height != HeightServer::BadHeight);

  float playerHeight = pCharacter->GetHeight();

  if (pCharacter->IsFalling())
  {
//std::cout << "CONTROLLER: Obj " << pCharacter->GetId() << ": " 
//          << pCharacter->GetTypeName() <<"\n";
//std::cout << "Y POS: height: " << height << " playerHeight " 
//          << playerHeight << " Falling: " << pCharacter->IsFalling() 
//          << " GroundHeight: " << pCharacter->GetGroundHeight() << "\n";

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
    else
    {
//std::cout << "Falling.. 3\n";
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


